#include "test_controller.h"

void test_controller::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    drogon::HttpViewData viewData;
    // HttpViewData へのデータの格納は、key - valueのペアで行います。
    // ここでは name というキーワードに対して、リクエストで渡ってきた request_name 引数の値を取得し設定しています。
    viewData.insert(
            "name",
            req->getParameter("request_name")
    );

    //"HelloView.csp"から構築されたviewへの呼び出しを作成し、コールバックに登録します。
    callback(
            drogon::HttpResponse::newHttpViewResponse("HelloView.csp", viewData)
    );
}
