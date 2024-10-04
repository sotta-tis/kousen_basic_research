#include "adminController.h"
#include "socketCommunication.h"
#include "killProcess.h"
#include "commonData.h"
#include "servoClient.h"
#include "json.hpp"

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
        adminController::D_M_x = std::stoi(req->getParameter("x"));
        adminController::D_M_y = std::stoi(req->getParameter("y"));
        adminController::D_M_z = std::stoi(req->getParameter("z"));
        adminController::D_M_r = std::stoi(req->getParameter("r"));

        // DOBOTのアームを任意座標に移動
        sockC::moveArmParam(adminController::D_M_x,adminController::D_M_y,adminController::D_M_z,adminController::D_M_r,adminController::DOBOT_HOST,adminController::DOBOT_PORT);
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
