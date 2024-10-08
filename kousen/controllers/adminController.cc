#include "adminController.h"
#include "socketCommunication.h"
#include "killProcess.h"
#include "loginUtility.h"
#include "commonData.h"
#include "servoClient.h"

// Add definition of your processing function here
// admin画面
void adminController::admin(const HttpRequestPtr &req,
                            std::function<void(const HttpResponsePtr &)> &&callback) const
{
    callback(adminController::loginFilter(req,adminController::adminIndex()));
}

drogon::HttpResponsePtr adminController::loginFilter(const HttpRequestPtr &req,drogon::HttpResponsePtr requiredRes)const{
    bool loginState = false;
    // login失敗時のビューデータ作成
    auto viewData = drogon::HttpViewData();
    viewData.insert("message", std::string(""));
    drogon::HttpResponsePtr response =drogon::HttpResponse::newHttpViewResponse("login.csp", viewData);

    // セッションからログイン済みか判断
    drogon::SessionPtr sessionHolder = req->getSession();
    if(sessionHolder->find("loginState")) loginState = sessionHolder->getOptional<bool>("loginState").value_or(false);

    // ログイン済みであればそのままadmin画面を表示
    if (loginState) response = requiredRes;

    return response;
}

// 管理画面のレスポンスを作成
drogon::HttpResponsePtr adminController::adminIndex()const {
    // viewData梱包
    drogon::HttpViewData viewData;
    viewData.insert("PID",killPro::pid);
    viewData.insert("DOBOT_HOST",adminController::DOBOT_HOST);
    viewData.insert("DOBOT_PORT",adminController::DOBOT_PORT);
    viewData.insert("x",adminController::D_M_x);
    viewData.insert("y",adminController::D_M_y);
    viewData.insert("z",adminController::D_M_z);
    viewData.insert("r",adminController::D_M_r);

    viewData.insert("SERVO_HOST",adminController::SERVO_HOST);
    viewData.insert("SERVO_PORT",adminController::SERVO_PORT);
    viewData.insert("STANDBY",adminController::standby);
    viewData.insert("CLOSE",adminController::close);
    viewData.insert("OPEN",adminController::open);

    // コールバックレスポンス作成
    return HttpResponse::newHttpViewResponse("admin.csp",viewData);
}

void adminController::shutDown(const drogon::HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback) {
    // "/admin/kill"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        sockC::quit(adminController::DOBOT_HOST,adminController::DOBOT_PORT);
        killPro::killProcess();
    }
    callback(response);
}

void adminController::setHostDetail(const drogon::HttpRequestPtr &req,
                                    std::function<void(const HttpResponsePtr &)> &&callback) {
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        // Dobotのipアドレスとportを更新
        adminController::DOBOT_HOST = req->getParameter("ipAddress");
        adminController::DOBOT_PORT = std::stoi(req->getParameter("port"));

        // 通信
        sockC::setting(adminController::DOBOT_HOST,adminController::DOBOT_PORT);
    }

    callback(response);
}

void adminController::doDobot(const drogon::HttpRequestPtr &req,
                              std::function<void(const HttpResponsePtr &)> &&callback) {
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        adminController::D_M_x = std::stoi(req->getParameter("x"));
        adminController::D_M_y = std::stoi(req->getParameter("y"));
        adminController::D_M_z = std::stoi(req->getParameter("z"));
        adminController::D_M_r = std::stoi(req->getParameter("r"));

        // DOBOTのアームを任意座標に移動
        sockC::moveArmParam(adminController::D_M_x,adminController::D_M_y,adminController::D_M_z,adminController::D_M_r,adminController::DOBOT_HOST,adminController::DOBOT_PORT);
    }

    callback(response);
}

// 新規ユーザー登録画面
void adminController::newuser(const HttpRequestPtr &req,
                              std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto response = drogon::HttpResponse::newHttpViewResponse("userAdd.csp");
    callback(response);
}

// ユーザー情報の保存
void adminController::submit(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string id = req->getParameter("loginID");
    std::string pass = req->getParameter("passWord");
    std::string validation = req->getParameter("validation");
    drogon::HttpResponsePtr response = drogon::HttpResponse::newHttpViewResponse("userAdd.csp");

    if(pass == validation)
    {
        std::array<u_char, SHA512_DIGEST_LENGTH> passHash = {0};

        // パスワードの暗号化ハッシュを作成します。
        std::string digest =
                createDigestArray<char>(
                        pass.c_str(),
                        static_cast<size_t>( pass.length() ),
                        passHash
                );
        drogon::orm::DbClientPtr dbp =
                drogon::app().getDbClient();

        // ファイルにせよ、DataBaseにせよ、保存するときは平文ではなく暗号化ハッシュを保存します。
        // こうすることで、保存先が露呈して盗み見られた際にも、
        // パスワードそのものが流出することを防ぐことができます。
        auto result = dbp->execSqlSync(
                "INSERT INTO passwords VALUES ($1,$2);",
                id,
                digest
        );

        if( result.affectedRows() != 0)
        {
            auto viewData = drogon::HttpViewData();
            viewData.insert("message", std::string(""));
            response =
                    drogon::HttpResponse::newHttpViewResponse("login.csp", viewData);
        }
    }
    callback( response );
}


std::string adminController::getStoredDigest(std::string userID) const
{
    std::string resultString;
    drogon::orm::DbClientPtr dbp = drogon::app().getDbClient();
    auto result = dbp->execSqlSync(
            "SELECT digest FROM public.passwords WHERE \"userID\" = $1",
            userID
    );

    if(result.size() > 0)
    {
        resultString = result[0]["digest"].as<std::string>();
    }

    return resultString;
}

// login処理
void adminController::login(const HttpRequestPtr &req,
                            std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto viewData = drogon::HttpViewData();

    // エラービュー作成
    viewData.insert("message", std::string("ログイン情報が不正です。"));
    drogon::HttpResponsePtr response = HttpResponse::newHttpViewResponse("login.csp", viewData);

    // ビューから情報取得
    std::string preserve = req->getParameter("preserve");
    std::string id = req->getParameter("loginID");
    std::string pass = req->getParameter("passWord");

    // hash用
    std::array<u_char, 64UL> inputHash;
    std::array<u_char, 64UL> storedHash;

    std::string passDigest =createDigestArray<char>( pass.c_str(), static_cast<size_t>( pass.length() ), inputHash );

    std::string storedDigest =getStoredDigest(id);

    // login情報に問題がなければセッションの登録をして管理者画面に移動
    if( passDigest ==  storedDigest )
    {
        if (preserve != "")
        {
            req->session()->insert("loginState", true);
        }
        else
        {
            req->session()->erase("loginState");
        }
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k302Found);
        resp -> addHeader("Location", "/admin");
        response = resp;
    }

    callback(response);
}

void adminController::setGlipperHost(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        adminController::SERVO_HOST = req->getParameter("ipAddress");
        adminController::SERVO_PORT = std::stoi(req->getParameter("port"));

        servoClient = std::make_unique<ServoClient>(req->getParameter("ipAddress"),std::stoi(req->getParameter("port")));

        if(servoClient->connectToServer()){
            std::cout<<"servo connection!!"<<std::endl;
        }
    }

    callback(response);
};

void adminController::setGlipperInitial(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        adminController::standby = std::stoi(req->getParameter("standby"));
        adminController::close = std::stoi(req->getParameter("close"));
        adminController::open = std::stoi(req->getParameter("open"));
    }

    callback(response);
};
void adminController::setGlipperDo(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,std::string action){
    // "/admin"にリダイレクト
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    resp -> addHeader("Location", "/admin");

    auto response = adminController::loginFilter(req,resp);

    if(response == resp){
        if(action=="standby"){
            servoClient->sendAngle(adminController::standby);
        }if(action=="close"){
            servoClient->sendAngle(adminController::close);
        }if(action=="open"){
            servoClient->sendAngle(adminController::open);
        }
    }

    callback(response);
};
