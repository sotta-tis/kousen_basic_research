#include "adminController.h"
#include "socketCommunication.h"
#include "killProcess.h"
#include "commonData.h"
#include "servoClient.h"

void adminController::shutDown(const drogon::HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback) {
    // "/admin/kill"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        sockC::quit(adminController::DOBOT_HOST,adminController::DOBOT_PORT);
        killPro::killProcess();
    }
    callback(response);
}

void adminController::setHostDetail(const drogon::HttpRequestPtr &req,
                                    std::function<void(const HttpResponsePtr &)> &&callback) {
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        // Dobotのipアドレスとportを更新
        adminController::DOBOT_HOST = req->getParameter("ipAddress");
        adminController::DOBOT_PORT = std::stoi(req->getParameter("port"));

        // 通信
        sockC::setting(adminController::DOBOT_HOST,adminController::DOBOT_PORT);
    }

    callback(response);
}

void adminController::doDobot(const drogon::HttpRequestPtr &req,
                              std::function<void(const HttpResponsePtr &)> &&callback) {
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        adminController::D_M_x = std::stoi(req->getParameter("x"));
        adminController::D_M_y = std::stoi(req->getParameter("y"));
        adminController::D_M_z = std::stoi(req->getParameter("z"));
        adminController::D_M_r = std::stoi(req->getParameter("r"));

        // DOBOTのアームを任意座標に移動
        sockC::moveArmParam(adminController::D_M_x,adminController::D_M_y,adminController::D_M_z,adminController::D_M_r,adminController::DOBOT_HOST,adminController::DOBOT_PORT);
    }

    callback(response);
}

std::string adminController::getStoredDigest(std::string userID) const
{
    std::string resultString;
    drogon::orm::DbClientPtr dbp = drogon::app().getDbClient();
    auto result = dbp->execSqlSync(
            "SELECT digest FROM public.passwords WHERE \"userID\" = $1",
            userID
    );

    if(result.size() > 0)
    {
        resultString = result[0]["digest"].as<std::string>();
    }

    return resultString;
}


void adminController::setGlipperHost(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        adminController::SERVO_HOST = req->getParameter("ipAddress");
        adminController::SERVO_PORT = std::stoi(req->getParameter("port"));

        servoClient = std::make_unique<ServoClient>(req->getParameter("ipAddress"),std::stoi(req->getParameter("port")));

        if(servoClient->connectToServer()){
            std::cout<<"servo connection!!"<<std::endl;
        }
    }

    callback(response);
};

void adminController::setGlipperInitial(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        adminController::standby = std::stoi(req->getParameter("standby"));
        adminController::close = std::stoi(req->getParameter("close"));
        adminController::open = std::stoi(req->getParameter("open"));
    }

    callback(response);
};
void adminController::setGlipperDo(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,std::string action){
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        if(action=="standby"){
            servoClient->sendAngle(adminController::standby);
        }if(action=="close"){
            servoClient->sendAngle(adminController::close);
        }if(action=="open"){
            servoClient->sendAngle(adminController::open);
        }
    }

    callback(response);
};
