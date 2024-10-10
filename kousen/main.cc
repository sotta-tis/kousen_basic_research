#include <drogon/drogon.h>
#include <chrono>

#include"controllers/killProcess.h"
#include "controllers/commonData.h"

int main() {
    commonData::startTaskRunner();
    std::cout<<killPro::pid<<std::endl;
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 8055);
    //Load config file
    drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app()
            .enableSession(100000)
            .run();
    return 0;
}
