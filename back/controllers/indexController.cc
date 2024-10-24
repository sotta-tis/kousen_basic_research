#include "indexController.h"
#include "commonData.h"

void indexController::index(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    drogon::HttpStatusCode statusCode=drogon::k200OK;
    Json::Value jsonResponse;

    try {
        jsonResponse["sushi"]["count"] = commonData::sushiCount;
        for (const auto& pair : commonData::sushiLabel){
            std::cout<<pair.first<<','<<pair.second<<std::endl;
            jsonResponse["sushi"]["menu"][std::to_string(pair.first)] = pair.second;
        }
        statusCode=drogon::k200OK;
    }catch(const std::exception& e){
        statusCode=drogon::k500InternalServerError;
    }

    // JSONレスポンスを作成して返す
    auto resp = HttpResponse::newHttpJsonResponse(jsonResponse);
    resp->setStatusCode(statusCode);
    callback(resp);
}