#include "indexController.h"

void indexController::index(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;

    try {
        jsonResponse["aa"]="aaa";
    }catch(const std::exception& e){
        statusCode=drogon::k500InternalServerError;
    }

    // JSONレスポンスを作成して返す
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}