#include <WiFi.h>
#include <ESP32Servo.h>

// WiFi 設定
const char* ssid = "yourSSID";  // WiFi名
const char* password = "yourPassword";  // WiFiパスワード

// サーバー設定
WiFiServer server(80);

// サーボモーター設定
Servo myservo;
Servo dishServo;
int servoPin = 18;  // サーボモーター制御ピン
int dishServoPin = 19;

bool isDish = false;

void setup() {
    Serial.begin(115200);
    myservo.attach(servoPin);
    dishServo.attch(dishServoPin);

    // WiFi接続
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    // サーバー開始
    server.begin();
}

void loop() {
    WiFiClient client = server.available();  // クライアントの接続を待つ

    if (client) {
        Serial.println("New Client.");
        String currentLine = "";  // 受信データ

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                if (c == '\n') {
                    // データ受信完了
                    if (currentLine.length() > 0) {
                        int angle = currentLine.toInt();  // 受信データを整数に変換
                        if (angle >= 0 && angle <= 180) {
                            if(isDish){
                                dishServo.write(angle);  // サーボモーター角度制御
                                client.println("Angle set to " + String(angle));
                                isDish = false;
                            }else{
                                myservo.write(angle);  // サーボモーター角度制御
                                client.println("Angle set to " + String(angle));
                            }
                        } else {
                            client.println("Invalid angle.");
                        }
                    }
                    currentLine = "";
                } else if (c != '\r') {
                    if(c == 'd'){
                        isDish = true;
                    }else{
                        currentLine += c;  // 受信データを蓄積
                    }
                }
            }
        }
        client.stop();
        Serial.println("Client Disconnected.");
    }
}
