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

        commonData::DOBOT_HOST = adminController::DOBOT_HOST;
        commonData::DOBOT_PORT = adminController::DOBOT_PORT;

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
        commonData::addTask([this]() {
            sockC::moveArmParam(commonData::D_M_x,commonData::D_M_y,commonData::D_M_z,commonData::D_M_r,adminController::DOBOT_HOST,adminController::DOBOT_PORT);
        });
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
        commonData::addTask([this]() {
            sockC::moveArmParamGo(commonData::D_M_x,commonData::D_M_y,commonData::D_M_z,commonData::D_M_r,adminController::DOBOT_HOST,adminController::DOBOT_PORT);
        });
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

        commonData::servoClient = std::make_unique<ServoClient>(req->getParameter("ipAddress"),std::stoi(req->getParameter("port")));

        if(commonData::servoClient->connectToServer()){
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
        commonData::standby = std::stoi(req->getParameter("standby"));
        commonData::close = std::stoi(req->getParameter("close"));
        commonData::open = std::stoi(req->getParameter("open"));
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
            commonData::servoClient->sendAngle(commonData::standby,false);
        }
        if (action == "close") {
            commonData::servoClient->sendAngle(commonData::close,false);
        }
        if (action == "open") {
            commonData::servoClient->sendAngle(commonData::open,false);
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

    jsonResponse["dobot"]["initial"]["location"]["img"]["x"]=commonData::img_initial_x;
    jsonResponse["dobot"]["initial"]["location"]["img"]["y"]=commonData::img_initial_y;
    jsonResponse["dobot"]["initial"]["location"]["img"]["width"]=commonData::img_box_width;
    jsonResponse["dobot"]["initial"]["location"]["img"]["height"]=commonData::img_box_height;

    jsonResponse["location"]["zone"]["max_x"] = commonData::ZONE_MAX_x;
    jsonResponse["location"]["zone"]["max_y"] = commonData::ZONE_MAX_y;
    jsonResponse["location"]["zone"]["min_x"] = commonData::ZONE_MIN_x;
    jsonResponse["location"]["zone"]["min_y"] = commonData::ZONE_MIN_y;
    jsonResponse["location"]["zone"]["z"] = commonData::ZONE_z;
    jsonResponse["location"]["zone"]["r"] = commonData::ZONE_r;

    jsonResponse["location"]["release"]["x"] = commonData::RELEASE_x;
    jsonResponse["location"]["release"]["y"] = commonData::RELEASE_y;
    jsonResponse["location"]["release"]["z"] = commonData::RELEASE_z;
    jsonResponse["location"]["release"]["r"] = commonData::RELEASE_r;

    jsonResponse["location"]["image"]["x"] = commonData::IMAGE_x;
    jsonResponse["location"]["image"]["y"] = commonData::IMAGE_y;
    jsonResponse["location"]["image"]["z"] = commonData::IMAGE_z;
    jsonResponse["location"]["image"]["r"] = commonData::IMAGE_r;

    jsonResponse["glip"]["host"] = adminController::SERVO_HOST;
    jsonResponse["glip"]["port"] = adminController::SERVO_PORT;
    jsonResponse["glip"]["standby"] = commonData::standby;
    jsonResponse["glip"]["close"] = commonData::close;
    jsonResponse["glip"]["open"] = commonData::open;

    jsonResponse["image"]["scale"] = commonData::scale;

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
    commonData::scale = std::stod(req->getParameter("scale"));
    cv::Mat image= commonData::cropImage(commonData::getImageFromCameraOrPath("aa"),320,320,commonData::scale);

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
        commonData::img_initial_x = std::stod(req->getParameter("x"));
        commonData::img_initial_y = std::stod(req->getParameter("y"));
        commonData::img_box_width = std::stod(req->getParameter("width"));
        commonData::img_box_height = std::stod(req->getParameter("height"));

        commonData::addTask([this]() {
            cv::Mat image= commonData::cropImage(commonData::getImageFromCameraOrPath("aa"),320,320,commonData::scale);
            commonData::objectDetection(image,"http://127.0.0.1:8881");
        });

    }catch(const std::exception& e){
        statusCode=drogon::k501NotImplemented;
    }

    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

void adminController::setSushiZoneMaxCoordinate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try{
        commonData::ZONE_MAX_x = std::stoi(req->getParameter("x"));
        commonData::ZONE_MAX_y = std::stoi(req->getParameter("y"));

        commonData::ZONE_X_diff = commonData::ZONE_MAX_x - commonData::ZONE_MIN_x;
        commonData::ZONE_Y_diff = commonData::ZONE_MAX_y - commonData::ZONE_MIN_y;

    }catch (const std::exception& e) {
        statusCode= drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

void adminController::setSushiZoneMinCoordinate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try{
        commonData::ZONE_MIN_x = std::stoi(req->getParameter("x"));
        commonData::ZONE_MIN_y = std::stoi(req->getParameter("y"));

        commonData::ZONE_X_diff = commonData::ZONE_MAX_x - commonData::ZONE_MIN_x;
        commonData::ZONE_Y_diff = commonData::ZONE_MAX_y - commonData::ZONE_MIN_y;

    }catch (const std::exception& e) {
        statusCode= drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

void adminController::setSushiZoneHeight(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try{
        commonData::ZONE_z = std::stoi(req->getParameter("z"));
        commonData::ZONE_r = std::stoi(req->getParameter("r"));

    }catch (const std::exception& e) {
        statusCode= drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}


void adminController::setReleasePointCoordinate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try{
        commonData::RELEASE_x = std::stoi(req->getParameter("x"));
        commonData::RELEASE_y = std::stoi(req->getParameter("y"));
        commonData::RELEASE_z = std::stoi(req->getParameter("z"));
        commonData::RELEASE_r = std::stoi(req->getParameter("r"));

    }catch (const std::exception& e) {
        statusCode= drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

void adminController::setImagePointCoordinate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;
    try{
        commonData::IMAGE_x = std::stoi(req->getParameter("x"));
        commonData::IMAGE_y = std::stoi(req->getParameter("y"));
        commonData::IMAGE_z = std::stoi(req->getParameter("z"));
        commonData::IMAGE_r = std::stoi(req->getParameter("r"));
    }catch (const std::exception& e) {
        statusCode= drogon::k500InternalServerError;
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}

