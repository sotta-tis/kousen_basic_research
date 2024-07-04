#include "adminController.h"

// Add definition of your processing function here

void adminController::adminIndex(const drogon::HttpRequestPtr &req,
                                 std::function<void(const HttpResponsePtr &)>
                                 &&callback) {
    // viewData梱包
    drogon::HttpViewData viewData;
    viewData.insert("DOBOT_HOST",adminController::DOBOT_HOST);
    viewData.insert("DOBOT_PORT",adminController::DOBOT_PORT);

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
