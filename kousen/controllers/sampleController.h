#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class sampleController : public drogon::HttpController<sampleController>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(sampleController::get, "/{2}/{1}", Get); // path is /sampleController/{arg2}/{arg1}
    // METHOD_ADD(sampleController::your_method_name, "/{1}/{2}/list", Get); // path is /sampleController/{arg1}/{arg2}/list
    ADD_METHOD_TO(sampleController::index, "/", Get); // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    void index(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)const;
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
};
