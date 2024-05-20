#include "sampleController.h"

// Add definition of your processing function here

void sampleController::index(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)const{
    auto resp = HttpResponse::newHttpViewResponse("sample.csp");
    callback(resp);
}