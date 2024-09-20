#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* server_ip = "192.168.11.3";  // サーバーのIPアドレス
const int server_port = 8080;

WiFiClient client;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
}

void loop() {
    if (!client.connected()) {
        if (client.connect(server_ip, server_port)) {
            Serial.println("Connected to server");

            // サーボを開くコマンドを送信
            client.println("open");
            delay(3000);  // 3秒待機

            // サーボを閉じるコマンドを送信
            client.println("close");
            delay(3000);  // 3秒待機
        } else {
            Serial.println("Connection to server failed");
        }
    }
}
