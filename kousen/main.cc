#include <drogon/drogon.h>
#include <chrono>

#include"controllers/killProcess.h"

int main() {
    std::cout<<killPro::pid<<std::endl;
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 80);
    //Load config file
    drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app()
            .enableSession(1)
            .run();
    return 0;
}
