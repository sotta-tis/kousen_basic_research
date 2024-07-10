#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class adminController : public drogon::HttpController<adminController>
{
private:
    std::string DOBOT_HOST="---";
    int DOBOT_PORT=-1;
    int D_M_x =0;
    int D_M_y =0;
    int D_M_z =0;
    int D_M_r =0;

public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(adminController::get, "/{2}/{1}", Get); // path is /adminController/{arg2}/{arg1}
    // METHOD_ADD(adminController::your_method_name, "/{1}/{2}/list", Get); // path is /adminController/{arg1}/{arg2}/list
    ADD_METHOD_TO(adminController::login, "/login", Post);
    ADD_METHOD_TO(adminController::admin, "/admin", Get); // path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(adminController::setHostDetail, "/admin/dobot/set", Get);
    ADD_METHOD_TO(adminController::doDobot, "/admin/dobot/do", Get);
    ADD_METHOD_TO(adminController::shutDown, "/admin/kill", Get);
    ADD_METHOD_TO(adminController::newuser, "/newuser", Get);
    ADD_METHOD_TO(adminController::submit, "/submit", Post);
    METHOD_LIST_END

    // システムシャットダウン
    void shutDown(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

    // http response method
    drogon::HttpResponsePtr adminIndex()const;
    void admin(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback) const;

    // set dobot host details
    void setHostDetail(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    // dobot do
    void doDobot(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);


    void newuser(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) const;

    void submit(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) const;

    void login(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback) const;

    std::string getStoredDigest(std::string userID) const;
};
