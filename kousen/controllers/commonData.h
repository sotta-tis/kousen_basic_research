#pragma once

#include <drogon/WebSocketConnection.h>
#include <set>
#include <mutex>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>

namespace commonData{
    extern std::set<drogon::WebSocketConnectionPtr> clients;  // clientsセットを外部からアクセス可能に
    extern std::mutex clientsMutex;

    void sentMessageToWebsockets(const std::string &message);

    // グローバル変数としてタスクキューや条件変数を定義
    extern std::queue<std::function<void()>> taskQueue;
    extern std::mutex queueMutex;
    extern std::condition_variable queueCondVar;
    extern bool stopTaskRunner;

// タスクを追加する関数
    void addTask(std::function<void()> task);

// タスクを処理する関数（別スレッドで実行）
    void startTaskRunner();

// タスク処理ループ
    void taskRunner();
}