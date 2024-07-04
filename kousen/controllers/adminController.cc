#include "adminController.h"

// Add definition of your processing function here

void adminController::adminIndex(const drogon::HttpRequestPtr &req,
                                 std::function<void(const HttpResponsePtr &)>
                                 &&callback) {
    // viewData梱包
    drogon::HttpViewData viewData;
    viewData.insert("DOBOT_HOST",adminController::DOBOT_HOST);
    viewData.insert("DOBOT_PORT",adminController::DOBOT_PORT);
    viewData.insert("x",adminController::D_M_x);
    viewData.insert("y",adminController::D_M_y);
    viewData.insert("z",adminController::D_M_z);
    viewData.insert("r",adminController::D_M_r);

    // コールバック
    auto resp = HttpResponse::newHttpViewResponse("admin.csp",viewData);
    callback(resp);
}

void adminController::setHostDetail(const drogon::HttpRequestPtr &req,
                                    std::function<void(const HttpResponsePtr &)> &&callback) {
    // Dobotのipアドレスとportを更新
    adminController::DOBOT_HOST = req->getParameter("ipAddress");
    adminController::DOBOT_PORT = req->getParameter("port");

    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");
    callback(resp);
}

void adminController::doDobot(const drogon::HttpRequestPtr &req,
                              std::function<void(const HttpResponsePtr &)> &&callback) {
    adminController::D_M_x = req->getParameter("x");
    adminController::D_M_y = req->getParameter("y");
    adminController::D_M_z = req->getParameter("z");
    adminController::D_M_r = req->getParameter("r");

    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");
    callback(resp);
}