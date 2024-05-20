#include "indexController.h"

void indexController::index(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    auto resp = HttpResponse::newHttpViewResponse("indexMy.csp");
    callback(resp);
}
void indexController::menu(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    auto resp = HttpResponse::newHttpViewResponse("menu.csp");
    callback(resp);
}
void indexController::checkout(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    auto resp = HttpResponse::newHttpViewResponse("checkout.csp");
    callback(resp);
}
void indexController::history(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    auto resp = HttpResponse::newHttpViewResponse("history.csp");
    callback(resp);
}
// Add definition of your processing function here
