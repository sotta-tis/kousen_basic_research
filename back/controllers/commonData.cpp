#include "commonData.h"
#include <drogon/HttpClient.h>
#include <drogon/drogon.h>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <string>
#include "json.hpp"
#include <map>
#include <json/json.h>
#include "socketCommunication.h"

using json = nlohmann::json;

namespace commonData{
    double  scale = 0.1;

    std::unique_ptr<ServoClient> servoClient;

     int standby = 0;
     int close = 0;
     int open = 0;

     int dishStandby = 0;
     int dishClose = 0;
     int dishOpen = 0;

    std::string DOBOT_HOST="--";
    int DOBOT_PORT=0;

    int D_M_x =0;
    int D_M_y =0;
    int D_M_z =0;
    int D_M_r =0;

    int ZONE_MAX_x =0;
    int ZONE_MAX_y =0;
    int ZONE_MIN_x =0;
    int ZONE_MIN_y =0;
    int ZONE_X_diff = 0;
    int ZONE_Y_diff = 0;
    int ZONE_r =0;
    int ZONE_z =0;

    int RELEASE_x =0;
    int RELEASE_y =0;
    int RELEASE_z =0;
    int RELEASE_r =0;

    int IMAGE_x =0;
    int IMAGE_y =0;
    int IMAGE_z =0;
    int IMAGE_r =0;

    int INITIAL_x =0;
    int INITIAL_y =0;
    int INITIAL_z =0;
    int INITIAL_r =0;

    int DISH_x = 0;
    int DISH_y = 0;
    int DISH_z = 0;
    int DISH_r = 0;

    int RELEASE_DISH_x = 0;
    int RELEASE_DISH_y = 0;
    int RELEASE_DISH_z = 0;
    int RELEASE_DISH_r = 0;

    double img_initial_x = 0;
    double img_initial_y = 0;
    double img_box_width = 0;
    double img_box_height = 0;

    // グローバル変数の定義
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondVar;
    bool stopTaskRunner = false;
    std::set<drogon::WebSocketConnectionPtr> clients;
    std::mutex clientsMutex;

    std::map<int,int> sushiLabel={
            {1, 0},
            {2,0},
            {3,0},
            {4,0},
            {5,0},
            {6,0}
    };
    std::vector<std::pair<int,std::vector<double>>> sushiBoxes;
    int sushiCount;

    void sentMessageToWebsockets(const std::string &message)
    {
        std::lock_guard<std::mutex> guard(commonData::clientsMutex);
        for (const auto &client : commonData::clients)
        {
            if (client->connected())
            {
                client->send(message);
            }
        }
    }

    // タスクを追加する関数の実装
    void addTask(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(task);  // タスクをキューに追加
        }
        queueCondVar.notify_one();  // タスクが追加されたことを通知
    }

    // タスクを処理する関数（スレッドを開始する）
    void startTaskRunner() {
        std::thread(taskRunner).detach();  // 別スレッドで実行
    }

    // タスク処理ループ（while文）
    void taskRunner() {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCondVar.wait(lock, [] { return !taskQueue.empty() || stopTaskRunner; });

                // タスク処理を停止する条件
                if (stopTaskRunner && taskQueue.empty()) {
                    break;
                }

                // キューからタスクを取り出す
                task = taskQueue.front();
                taskQueue.pop();
            }

            // タスクを実行
            if (task) {
                task();
            }
        }
    }

    cv::Mat cropImage(const cv::Mat& inputImage, int cropWidth, int cropHeight, double scale) {
        // 横縦比を保ったままリサイズする
        cv::Mat resizedImage;
        cv::resize(inputImage, resizedImage, cv::Size(), scale, scale);

        // リサイズ後の画像サイズを取得
        int width = resizedImage.cols;
        int height = resizedImage.rows;

        // トリミングする領域の左上の座標を計算
        int x = (width - cropWidth) / 2;
        int y = (height - cropHeight) / 2;

        // 領域を画像サイズに収めるためのチェック
        x = std::max(0, x);
        y = std::max(0, y);
        int cropWidthAdjusted = std::min(cropWidth, width - x);
        int cropHeightAdjusted = std::min(cropHeight, height - y);

        // トリミングする領域を設定
        cv::Rect roi(x, y, cropWidthAdjusted, cropHeightAdjusted);

        // トリミング
        return resizedImage(roi);
    }

    cv::Mat getImageFromCameraOrPath(const std::string& fallbackImagePath) {
        cv::Mat frame;

        // カメラデバイスのオープン（デフォルトのカメラはID 0）
        cv::VideoCapture cap(0);
        if (cap.isOpened()) {
            // カメラを開いた後に1秒待機
            std::this_thread::sleep_for(std::chrono::seconds(2));

            // フレームをキャプチャ
            cap >> frame;
            if (!frame.empty()) {
                std::cout << "カメラから画像を取得しました。" << std::endl;
                return frame;
            }
        }
        std::cerr << "カメラが開けないか、フレームを取得できませんでした。" << std::endl;

        // 指定したパスから画像を読み込む
        frame = cv::imread(fallbackImagePath, cv::IMREAD_COLOR);
        if (frame.empty()) {
            std::cerr << "指定されたパスから画像を読み込めませんでした: " << fallbackImagePath << std::endl;
        } else {
            std::cout << "指定されたパスから画像を取得しました。" << std::endl;
        }

        return frame;
    }

    // 画像からオブジェクト検出する
    void objectDetection(const cv::Mat& image, const std::string& server_url) {
        // 画像を一時的にファイルとして保存
        std::string temp_image_path = "./temp_image.jpg";
        cv::imwrite(temp_image_path, image);

        // DrogonでHTTPリクエストを作成
        auto client = drogon::HttpClient::newHttpClient(server_url);

        // アップロードするファイルのリストを作成
        std::vector<drogon::UploadFile> files;
        drogon::UploadFile file(temp_image_path,temp_image_path,"file",drogon::ContentType::CT_IMAGE_JPG);
        files.push_back(file);

        // マルチパートフォームデータを作成
        auto req = drogon::HttpRequest::newFileUploadRequest(std::move(files));
        req->setPath("/upload");
        req->setMethod(drogon::Post);

        // リクエストを非同期に送信し、結果を処理
        client->sendRequest(req, [image,temp_image_path](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
            if (result == drogon::ReqResult::Ok && response){
                auto jsonRes = json::parse(response->body());
                int countObjects = jsonRes["num_detections"];
                std::vector<int> classLabels = jsonRes["detection_classes"];
                std::vector<std::vector<double>> boundingBoxesLocation = jsonRes["detection_boxes"];
                std::vector<double> scores = jsonRes["detection_scores"];

                std::vector<std::pair<int,std::vector<double>>> boxes;
                std::map<int,int> label;
                for(int i=0;i<scores.size();i++){
                    if(scores[i]>=0.5) {
                        label[classLabels[i]]+=1;
                        std::vector<double> buffBox;
                        buffBox.push_back(static_cast<double>(boundingBoxesLocation[i][1] * image.cols));
                        buffBox.push_back(static_cast<double>(boundingBoxesLocation[i][0] * image.rows));
                        buffBox.push_back(static_cast<double>(boundingBoxesLocation[i][3] * image.cols));
                        buffBox.push_back(static_cast<double>(boundingBoxesLocation[i][2] * image.rows));

                        boxes.push_back(std::make_pair(classLabels[i],buffBox));
                    }
                }

                commonData::sushiBoxes = boxes;
                commonData::sushiLabel = label;
                commonData::sushiCount = boxes.size();
                commonData::drawBoundingBoxesAndSave(image,boxes,"../../front/public/sample.jpg");
            }
        });
    }


    // JSONレスポンスを解析してバウンディングボックスを描画する関数
    void drawBoundingBoxesAndSave(const cv::Mat& image, const std::vector<std::pair<int,std::vector<double>>>& boxes, const std::string& savePath) {
        // 各バウンディングボックスを描画
        for (const auto& boxx : boxes) {
            std::vector<double> box = boxx.second;
            // バウンディングボックスの座標 (左上x, 左上y, 右下x, 右下y)
            double x1 = box[0];
            double y1 = box[1];
            double x2 = box[2];
            double y2 = box[3];

            std::cout<<x1<<','<<y1<<','<<x2<<','<<y2<<','<<std::endl;

            // 矩形（バウンディングボックス）を描画する
            cv::rectangle(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);
        }

        cv::rectangle(image, cv::Point(commonData::img_initial_x, commonData::img_initial_y), cv::Point(commonData::img_initial_x + commonData::img_box_width, commonData::img_initial_y + commonData::img_box_height), cv::Scalar(0, 255, 0), 2);
        // 画像を保存
        cv::imwrite(savePath, image);

        std::cout << "Image saved to: " << savePath << std::endl;
    }

    void order(int label){
        std::vector<double> initialVector;
        initialVector.push_back(img_box_width / std::sqrt((img_box_height*img_box_height) + img_box_width*img_box_width));
        initialVector.push_back(img_box_height / std::sqrt((img_box_height*img_box_height) + img_box_width*img_box_width));

        for(int i=0;i<sushiBoxes.size();i++){
            if(sushiBoxes[i].first == label){
                std::vector<double> srcVector = sushiBoxes[i].second;
                std::vector<double> trgVector;
                double diffX = srcVector[2] - srcVector[0];
                double diffY = srcVector[3] - srcVector[1];
                double trgOnImgX = srcVector[0] + (diffX/2) - img_initial_x ;
                double trgOnImgY = srcVector[1] + (diffY/2) - img_initial_y ;
                trgVector.push_back(diffX / std::sqrt( (diffX*diffX) + (diffY*diffY) ));
                trgVector.push_back(diffY / std::sqrt( (diffX*diffX) + (diffY*diffY) ));

                double tiltX = trgOnImgX / initialVector[0];
                double tiltY = trgOnImgY / initialVector[1];

                double zoneVecX = ZONE_X_diff / std::sqrt( (ZONE_X_diff*ZONE_X_diff) + (ZONE_Y_diff*ZONE_Y_diff) );
                double zoneVecY = ZONE_Y_diff / std::sqrt( (ZONE_X_diff*ZONE_X_diff) + (ZONE_Y_diff*ZONE_Y_diff) );

                double trgCoordinateX = (double)ZONE_MIN_x + (zoneVecX * tiltX / img_box_width * abs(ZONE_X_diff));
                double trgCoordinateY = (double)ZONE_MIN_y + (zoneVecY * tiltY / img_box_height * abs(ZONE_Y_diff));

                // DOBOTのアームを任意座標に移動
                commonData::addTask([trgCoordinateX,trgCoordinateY,i,label]() {
                    try{
                        commonData::servoClient->sendAngle(commonData::standby,false);
                        sockC::moveArmParam(trgCoordinateX,trgCoordinateY,commonData::ZONE_z,commonData::ZONE_r,commonData::DOBOT_HOST,commonData::DOBOT_PORT);
                        commonData::servoClient->sendAngle(commonData::close,false);
                        sockC::moveArmParam(RELEASE_x,RELEASE_y,RELEASE_z,RELEASE_r,commonData::DOBOT_HOST,commonData::DOBOT_PORT);
                        commonData::servoClient->sendAngle(commonData::open,false);
                        commonData::servoClient->sendAngle(commonData::dishStandby,true);
                        sockC::moveArmParam(DISH_x,DISH_y,DISH_z,DISH_r,commonData::DOBOT_HOST,commonData::DOBOT_PORT);
                        commonData::servoClient->sendAngle(commonData::dishClose,true);
                        sockC::moveArmParam(RELEASE_DISH_x,RELEASE_DISH_y,RELEASE_DISH_z,RELEASE_DISH_r,commonData::DOBOT_HOST,commonData::DOBOT_PORT);
                        commonData::servoClient->sendAngle(commonData::dishOpen,true);

                        sushiBoxes.erase(sushiBoxes.begin() + i);
                        sushiLabel={
                                {1, 0},
                                {2,0},
                                {3,0},
                                {4,0},
                                {5,0},
                                {6,0}
                        };
                        sushiCount = 0;
                        sockC::moveArmParam(INITIAL_x,INITIAL_y,INITIAL_z,INITIAL_r,commonData::DOBOT_HOST,commonData::DOBOT_PORT);
                        cv::Mat image= commonData::cropImage(commonData::getImageFromCameraOrPath("aa"),320,320,commonData::scale);
                        commonData::objectDetection(image,"http://127.0.0.1:8881");
                    }catch (std::exception& e){
                        std::cout<<"error:occurred"<<std::endl;
                    }

                });
            }
        }
    }

}
/*
 *          int x1 = static_cast<int>(box[1] * image.cols);
            int y1 = static_cast<int>(box[0] * image.rows);
            int x2 = static_cast<int>(box[3] * image.cols);
            int y2 = static_cast<int>(box[2] * image.rows);
 */