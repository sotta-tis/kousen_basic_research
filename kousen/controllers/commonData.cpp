#include "commonData.h"

namespace commonData{
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

// グローバル変数の定義
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondVar;
    bool stopTaskRunner = false;

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
}
