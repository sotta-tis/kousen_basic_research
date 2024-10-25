#pragma once

#include <drogon/HttpController.h>
#include "servoClient.h"

using namespace drogon;

class adminController : public drogon::HttpController<adminController>
{
private:
    std::string DOBOT_HOST="";
    int DOBOT_PORT=-1;

    std::string SERVO_HOST="";
    int SERVO_PORT=-1;

    int standby = 0;
    int close = 0;
    int open = 0;

    double  scale = 0.1;
    double img_initial_x = 0;
    double img_initial_y = 0;
    double img_box_width = 0;
    double img_box_height = 0;

    std::unique_ptr<ServoClient> servoClient;

public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(adminController::get, "/{2}/{1}", Get); // path is /adminController/{arg2}/{arg1}
    // METHOD_ADD(adminController::your_method_name, "/{1}/{2}/list", Get); // path is /adminController/{arg1}/{arg2}/lis
        ADD_METHOD_TO(adminController::getAdminProps,"/admin",Get);
        ADD_METHOD_TO(adminController::getImage,"/admin/image",Get);
        ADD_METHOD_TO(adminController::setHostDetail, "/admin/dobot/set", Get);
        ADD_METHOD_TO(adminController::doDobot, "/admin/dobot/do", Get);
        ADD_METHOD_TO(adminController::goDobot, "/admin/dobot/go", Get);
        ADD_METHOD_TO(adminController::setImageLocation, "/admin/dobot/set/img/location", Get);
        ADD_METHOD_TO(adminController::setGlipperHost, "/admin/glipper/set", Get);
        ADD_METHOD_TO(adminController::setGlipperInitial, "/admin/glipper/initial", Get);
        ADD_METHOD_TO(adminController::setGlipperDo, "/admin/glipper/do/{action}", Get);
        ADD_METHOD_TO(adminController::shutDown, "/admin/kill", Get);
    METHOD_LIST_END

    // システムシャットダウン
    void shutDown(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

    // set dobot host details
    void setHostDetail(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    // dobot do
    void doDobot(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void goDobot(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

    //void setSushiZoneMaxCoordinate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    //void setSushiZoneMinCoordinate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    //void setSushiZoneHeight(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

    //void setReleasePointCoordinate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

    //void setReleasePointCoordinate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

    void setGlipperHost(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void setGlipperInitial(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void setGlipperDo(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,std::string action);

    void getAdminProps(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getImage(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void setImageLocation(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};
