#include <WiFi.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const int serverPort = 80;

WiFiServer server(serverPort);

void setup() {
    Serial.begin(115200);

    // Wi-Fiに接続
    WiFi.begin(ssid, password);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to Wi-Fi");

    // サーバー開始
    server.begin();
    Serial.printf("Server started on port %d\n", serverPort);
}

void loop() {
    WiFiClient client = server.available();  // クライアント接続を待機

    if (client) {
        Serial.println("Client connected");
        while (client.connected()) {
            if (client.available()) {
                String data = client.readStringUntil('\n');  // データ受信
                Serial.println("Received: " + data);
                client.print("Echo: " + data);  // 受信したデータをクライアントに返す
            }
        }
        client.stop();
        Serial.println("Client disconnected");
    }
}
