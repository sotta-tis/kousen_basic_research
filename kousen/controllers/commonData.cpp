#include "commonData.h"
#include <drogon/HttpClient.h>
#include <drogon/drogon.h>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <json/json.h>

namespace commonData{
    // グローバル変数の定義
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondVar;
    bool stopTaskRunner = false;
    std::set<drogon::WebSocketConnectionPtr> clients;
    std::mutex clientsMutex;

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

    // サーバーに画像を送信して結果を受け取る関数
    void sendImageToServer(const cv::Mat& image, const std::string& server_url) {
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
        client->sendRequest(req, [temp_image_path](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
            if (result == drogon::ReqResult::Ok && response) {
                std::cout << "Response from server: " << response->body() << std::endl;
            } else {
                std::cerr << "Failed to get response from server!" << std::endl;
            }
        });
    }


    // JSONレスポンスを解析してバウンディングボックスを描画する関数
    void drawBoundingBoxes(cv::Mat& image, const std::string& jsonResponse) {
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::string errs;

        // JSONパース
        std::istringstream s(jsonResponse);
        if (!Json::parseFromStream(builder, s, &root, &errs)) {
            std::cerr << "Failed to parse JSON: " << errs << std::endl;
            return;
        }

        // 推論結果のバウンディングボックスを描画
        for (const auto& result : root) {
            auto box = result["box"];
            std::cout<<box[0].asFloat() * image.rows;
            float y_min = box[0].asFloat() * image.rows;
            float x_min = box[1].asFloat() * image.cols;
            float y_max = box[2].asFloat() * image.rows;
            float x_max = box[3].asFloat() * image.cols;

            // バウンディングボックスを描画
            cv::rectangle(image, cv::Point(x_min, y_min), cv::Point(x_max, y_max), cv::Scalar(0, 255, 0), 2);

            // クラス名とスコアを描画
            std::string label = result["class_name"].asString() + " " + std::to_string(result["score"].asFloat());
            int baseLine;
            cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            cv::rectangle(image, cv::Point(x_min, y_min - labelSize.height), cv::Point(x_min + labelSize.width, y_min + baseLine),
                          cv::Scalar(255, 255, 255), cv::FILLED);
            cv::putText(image, label, cv::Point(x_min, y_min), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
        }
    }
}
