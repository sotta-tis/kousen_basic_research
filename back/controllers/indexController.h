#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class indexController : public drogon::HttpController<indexController>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(indexController::get, "/{2}/{1}", Get); // path is /indexController/{arg2}/{arg1}
     ADD_METHOD_TO(indexController::index, "/", Get);
     ADD_METHOD_TO(indexController::order, "/order/{label}", Get);
    // ADD_METHOD_TO(indexController::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void index(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)const;
    void order(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback,int label)const;
};
