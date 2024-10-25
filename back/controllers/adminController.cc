#include <iostream>
#include <string>
#include "adminController.h"
#include "socketCommunication.h"
#include "killProcess.h"
#include "commonData.h"
#include "servoClient.h"
#include "json.hpp"
#include <opencv2/opencv.hpp>

void adminController::shutDown(const drogon::HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback) {
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try {
        sockC::quit(adminController::DOBOT_HOST,adminController::DOBOT_PORT);
    } catch (const std::exception& e) {
        statusCode = drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
    // システムシャットダウン
    killPro::killProcess();
}

void adminController::setHostDetail(const drogon::HttpRequestPtr &req,
                                    std::function<void(const HttpResponsePtr &)> &&callback) {
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try {
        // Dobotのipアドレスとportを更新
        adminController::DOBOT_HOST = req->getParameter("ipAddress");
        adminController::DOBOT_PORT = std::stoi(req->getParameter("port"));

        // 通信
        sockC::setting(adminController::DOBOT_HOST,adminController::DOBOT_PORT);

    } catch (const std::exception& e) {
        statusCode = drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

void adminController::doDobot(const drogon::HttpRequestPtr &req,
                              std::function<void(const HttpResponsePtr &)> &&callback) {
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try{
        commonData::D_M_x = std::stoi(req->getParameter("x"));
        commonData::D_M_y = std::stoi(req->getParameter("y"));
        commonData::D_M_z = std::stoi(req->getParameter("z"));
        commonData::D_M_r = std::stoi(req->getParameter("r"));

        // DOBOTのアームを任意座標に移動
        sockC::moveArmParam(commonData::D_M_x,commonData::D_M_y,commonData::D_M_z,commonData::D_M_r,adminController::DOBOT_HOST,adminController::DOBOT_PORT);
    }catch (const std::exception& e) {
        statusCode= drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

void adminController::goDobot(const drogon::HttpRequestPtr &req,
                              std::function<void(const HttpResponsePtr &)> &&callback) {
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try{
        commonData::D_M_x = std::stoi(req->getParameter("x"));
        commonData::D_M_y = std::stoi(req->getParameter("y"));
        commonData::D_M_z = std::stoi(req->getParameter("z"));
        commonData::D_M_r = std::stoi(req->getParameter("r"));

        // DOBOTのアームを任意座標に移動
        sockC::moveArmParamGo(commonData::D_M_x,commonData::D_M_y,commonData::D_M_z,commonData::D_M_r,adminController::DOBOT_HOST,adminController::DOBOT_PORT);
    }catch (const std::exception& e) {
        statusCode= drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

void adminController::setGlipperHost(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;

    try{
        adminController::SERVO_HOST = req->getParameter("ipAddress");
        adminController::SERVO_PORT = std::stoi(req->getParameter("port"));

        servoClient = std::make_unique<ServoClient>(req->getParameter("ipAddress"),std::stoi(req->getParameter("port")));

        if(servoClient->connectToServer()){
            std::cout<<"servo connection!!"<<std::endl;
        }

    }catch (const std::exception& e){
        statusCode= k500InternalServerError;
    }

    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
};

void adminController::setGlipperInitial(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;

    try {
        adminController::standby = std::stoi(req->getParameter("standby"));
        adminController::close = std::stoi(req->getParameter("close"));
        adminController::open = std::stoi(req->getParameter("open"));
    }catch(const std::exception& e){
        statusCode=drogon::k500InternalServerError;
    }

    // JSONレスポンスを作成して返す
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
};

void adminController::setGlipperDo(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,std::string action){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try {
        if (action == "standby") {
            servoClient->sendAngle(adminController::standby);
        }
        if (action == "close") {
            servoClient->sendAngle(adminController::close);
        }
        if (action == "open") {
            servoClient->sendAngle(adminController::open);
        }
    }catch (const std::exception& e){
        statusCode = drogon::k500InternalServerError;
    }

    // JSONレスポンスを作成して返す
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
};

void adminController::getAdminProps(const drogon::HttpRequestPtr &req,
                                    std::function<void(const HttpResponsePtr &)> &&callback) {
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;

    jsonResponse["dobot"]["host"]=adminController::DOBOT_HOST;
    jsonResponse["dobot"]["port"]=adminController::DOBOT_PORT;
    jsonResponse["dobot"]["x"] = commonData::D_M_x;
    jsonResponse["dobot"]["y"] = commonData::D_M_y;
    jsonResponse["dobot"]["z"] = commonData::D_M_z;
    jsonResponse["dobot"]["r"] = commonData::D_M_r;

    jsonResponse["dobot"]["initial"]["location"]["img"]["x"]=adminController::img_initial_x;
    jsonResponse["dobot"]["initial"]["location"]["img"]["y"]=adminController::img_initial_y;
    jsonResponse["dobot"]["initial"]["location"]["img"]["width"]=adminController::img_box_width;
    jsonResponse["dobot"]["initial"]["location"]["img"]["height"]=adminController::img_box_height;

    jsonResponse["glip"]["host"] = adminController::SERVO_HOST;
    jsonResponse["glip"]["port"] = adminController::SERVO_PORT;
    jsonResponse["glip"]["standby"] = adminController::standby;
    jsonResponse["glip"]["close"] = adminController::close;
    jsonResponse["glip"]["open"] = adminController::open;

    jsonResponse["image"]["scale"] = adminController::scale;

    jsonResponse["sushi"]["count"] = commonData::sushiCount;
    for (const auto& pair : commonData::sushiLabel){
        std::cout<<pair.first<<','<<pair.second<<std::endl;
        jsonResponse["sushi"]["menu"][std::to_string(pair.first)] = pair.second;
    }

    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}


void adminController::getImage(const drogon::HttpRequestPtr &req,
                                    std::function<void(const HttpResponsePtr &)> &&callback){
    adminController::scale = std::stod(req->getParameter("scale"));
    cv::Mat image= commonData::cropImage(commonData::getImageFromCameraOrPath("aa"),320,320,adminController::scale);

    // 画像をJPEG形式でエンコード
    std::vector<uchar> buffer;
    if (!cv::imencode(".jpg",image, buffer)) {
        // エンコード失敗時のエラーレスポンス
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k500InternalServerError);
        resp->setBody("Failed to encode image");
        callback(resp);
        return;
    }

    // エンコードされた画像データをレスポンスとして返す
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_IMAGE_JPG);
    resp->setBody(std::string(reinterpret_cast<const char*>(buffer.data()), buffer.size()));
    // キャッシュ制御の設定（オプション）
    resp->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    resp->addHeader("Pragma", "no-cache");
    resp->addHeader("Expires", "0");
    callback(resp);
}

void adminController::setImageLocation(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;

    try{
        adminController::img_initial_x = std::stod(req->getParameter("x"));
        adminController::img_initial_y = std::stod(req->getParameter("y"));
        adminController::img_box_width = std::stod(req->getParameter("width"));
        adminController::img_box_height = std::stod(req->getParameter("height"));

        commonData::addTask([this]() {
            cv::Mat image= commonData::cropImage(commonData::getImageFromCameraOrPath("aa"),320,320,adminController::scale);
            commonData::objectDetection(image,"http://127.0.0.1:8881");
        });

    }catch(const std::exception& e){
        statusCode=drogon::k501NotImplemented;
    }

    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

