#include "commonData.h"

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
}
