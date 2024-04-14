#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class test_controller : public drogon::HttpSimpleController<test_controller>
{
  public:
    virtual void asyncHandleHttpRequest(
            const HttpRequestPtr& req,
            std::function<void (const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    PATH_ADD("/",Get);
    PATH_LIST_END
};
