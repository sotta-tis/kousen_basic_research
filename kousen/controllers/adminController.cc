#include "adminController.h"
#include "adminController.h"

// Add definition of your processing function here

void adminController::adminIndex(const drogon::HttpRequestPtr &req,
                                 std::function<void(const HttpResponsePtr &)>
                                 &&callback) {
    auto resp = HttpResponse::newHttpViewResponse("admin.csp");
    callback(resp);
}

void adminController::setHostDetail(const drogon::HttpRequestPtr &req,
                                    std::function<void(const HttpResponsePtr &)>
                                    &&callback) {

}