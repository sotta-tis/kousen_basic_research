#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class adminController : public drogon::HttpController<adminController>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(adminController::get, "/{2}/{1}", Get); // path is /adminController/{arg2}/{arg1}
    // METHOD_ADD(adminController::your_method_name, "/{1}/{2}/list", Get); // path is /adminController/{arg1}/{arg2}/list
    ADD_METHOD_TO(adminController::adminIndex, "/admin", Get); // path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(adminController::setHostDetail, "/admin/set/{}{}", Get);
    METHOD_LIST_END

    // http response method
    void adminIndex(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

    // set host details
    void setHostDetail(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

};
