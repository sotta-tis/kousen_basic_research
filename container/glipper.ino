#include <WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* server_ip = "192.168.1.100";  // サーバーのIPアドレス
const int server_port = 8080;

WiFiClient client;
Servo myServo;  // サーボモーターオブジェクト

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    // サーボモーターの設定（GPIOピンを指定）
    myServo.attach(18);  // GPIO18ピンにサーボを接続
}

void loop() {
    if (client.connect(server_ip, server_port)) {
        Serial.println("Connected to server");

        while (client.connected()) {
            if (client.available()) {
                String command = client.readStringUntil('\n');
                command.trim();  // 改行を除去

                // コマンドに基づいてサーボを制御
                if (command == "open") {
                    myServo.write(180);  // サーボを180度に
                    Serial.println("Servo opened");
                } else if (command == "close") {
                    myServo.write(0);  // サーボを0度に
                    Serial.println("Servo closed");
                } else {
                    Serial.println("Unknown command");
                }
            }
        }
        client.stop();
        Serial.println("Disconnected from server");
    } else {
        Serial.println("Connection to server failed");
    }

    delay(1000);  // 1秒待機
}
