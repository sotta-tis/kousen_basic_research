#include "indexController.h"
#include <sys/types.h>
#include <unistd.h>
#include <csignal>

// 自分のプロセスIDを取得
int sig = 9;
pid_t pid=getpid();

void indexController::index(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    auto resp = HttpResponse::newHttpViewResponse("indexMy.csp");
    callback(resp);
}
void indexController::menu(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    auto resp = HttpResponse::newHttpViewResponse("menu.csp");
    kill(pid,sig);
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
