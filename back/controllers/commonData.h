#pragma once

#include <drogon/WebSocketConnection.h>
#include <set>
#include <mutex>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <opencv2/opencv.hpp>
#include <map>
#include <string>
#include "servoClient.h"


namespace commonData{
    extern double  scale;

    extern std::unique_ptr<ServoClient> servoClient;

    extern int standby;
    extern int close ;
    extern int open ;
    extern int dishStandby;
    extern int dishClose ;
    extern int dishOpen ;

    extern std::string DOBOT_HOST;
    extern int DOBOT_PORT;

    extern int D_M_x;
    extern int D_M_y;
    extern int D_M_z;
    extern int D_M_r;

    extern int ZONE_MAX_x;
    extern int ZONE_MAX_y;
    extern int ZONE_MIN_x;
    extern int ZONE_MIN_y;
    extern int ZONE_X_diff;
    extern int ZONE_Y_diff;
    extern int ZONE_z;
    extern int ZONE_r;

    extern int RELEASE_x;
    extern int RELEASE_y;
    extern int RELEASE_z;
    extern int RELEASE_r;

    extern int IMAGE_x;
    extern int IMAGE_y;
    extern int IMAGE_z;
    extern int IMAGE_r;

    extern double img_initial_x;
    extern double img_initial_y;
    extern double img_box_width;
    extern double img_box_height;

    extern std::set<drogon::WebSocketConnectionPtr> clients;  // clientsセットを外部からアクセス可能に
    extern std::mutex clientsMutex;

    void sentMessageToWebsockets(const std::string &message);

    // グローバル変数としてタスクキューや条件変数を定義
    extern std::queue<std::function<void()>> taskQueue;
    extern std::mutex queueMutex;
    extern std::condition_variable queueCondVar;
    extern bool stopTaskRunner;

    extern std::map<int,int> sushiLabel;
    extern std::vector<std::pair<int,std::vector<double>>> sushiBoxes;
    extern int sushiCount;

    cv::Mat cropImage(const cv::Mat& inputImage, int cropWidth, int cropHeight, double scale);


    // カメラから画像を取得し、失敗した場合は指定されたパスから画像を取得する関数
    cv::Mat getImageFromCameraOrPath(const std::string& fallbackImagePath);

    // タスクを追加する関数
    void addTask(std::function<void()> task);

    // タスクを処理する関数（別スレッドで実行）
    void startTaskRunner();

    // タスク処理ループ
    void taskRunner();

    void objectDetection(const cv::Mat& image, const std::string& server_url);
    void drawBoundingBoxesAndSave(const cv::Mat& image, const std::vector<std::pair<int,std::vector<double>>>& boxes, const std::string& savePath);
    void order(int label);
}