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

using json = nlohmann::json;

namespace commonData{
    int D_M_x =0;
    int D_M_y =0;
    int D_M_z =0;
    int D_M_r =0;

    int ZONE_MAX_x =0;
    int ZONE_MAX_y =0;
    int ZONE_MIN_x =0;
    int ZONE_MIN_y =0;
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
    std::vector<std::pair<int,std::vector<float>>> sushiBoxes;
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
                std::vector<std::vector<float>> boundingBoxesLocation = jsonRes["detection_boxes"];
                std::vector<double> scores = jsonRes["detection_scores"];

                std::vector<std::pair<int,std::vector<float>>> boxes;
                std::map<int,int> label;
                for(int i=0;i<scores.size();i++){
                    if(scores[i]>=0.5) {
                        label[classLabels[i]]+=1;
                        boxes.push_back(std::make_pair(classLabels[i],boundingBoxesLocation[i]));
                    }
                }

                commonData::sushiBoxes = boxes;
                commonData::sushiLabel = label;
                commonData::sushiCount = boxes.size();
                commonData::drawBoundingBoxesAndSave(image,boxes,"sample.jpg");
            }
        });
    }


    // JSONレスポンスを解析してバウンディングボックスを描画する関数
    void drawBoundingBoxesAndSave(const cv::Mat& image, const std::vector<std::pair<int,std::vector<float>>>& boxes, const std::string& savePath) {
        // 各バウンディングボックスを描画
        for (const auto& boxx : boxes) {
            std::vector<float> box = boxx.second;
            // バウンディングボックスの座標 (左上x, 左上y, 右下x, 右下y)
            int x1 = static_cast<int>(box[1] * image.cols);
            int y1 = static_cast<int>(box[0] * image.rows);
            int x2 = static_cast<int>(box[3] * image.cols);
            int y2 = static_cast<int>(box[2] * image.rows);

            std::cout<<box[1]<<','<<box[0]<<','<<box[2]<<','<<box[3]<<','<<std::endl;

            // 矩形（バウンディングボックス）を描画する
            cv::rectangle(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);
        }

        // 画像を保存
        cv::imwrite(savePath, image);

        std::cout << "Image saved to: " << savePath << std::endl;
    }
}
