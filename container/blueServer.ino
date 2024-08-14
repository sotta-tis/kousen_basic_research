#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// サービスとキャラクタリスティックのUUIDを定義
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"  // サービスUUID（サービスを一意に識別）
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"  // キャラクタリスティックUUID（キャラクタリスティックを一意に識別）

BLECharacteristic *pCharacteristic;  // キャラクタリスティックへのポインタを宣言

void setup() {
    Serial.begin(115200);

    // BLEデバイスを初期化し、名前を設定
    BLEDevice::init("ESP32_BLE_Server");
    BLEServer *pServer = BLEDevice::createServer();  // BLEサーバーを作成

    // サービスを作成し、サーバーに追加
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // キャラクタリスティックを作成し、そのプロパティを設定
    pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ   |  // 読み取り可能
            BLECharacteristic::PROPERTY_WRITE  |  // 書き込み可能
            BLECharacteristic::PROPERTY_NOTIFY   // 通知可能
    );

    // キャラクタリスティックに通知機能を追加（必要に応じてクライアントに変更を通知）
    pCharacteristic->addDescriptor(new BLE2902());

    // キャラクタリスティックへの書き込みに対するコールバックを設定
    pCharacteristic->setCallbacks(new MyCallbacks());

    // サービスを開始
    pService->start();

    // アドバタイズ（BLEデバイスが周囲のデバイスに自身を通知する機能）を開始
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();

    Serial.println("BLE server is up and running.");  // サーバーが起動したことをシリアルモニタに出力
}

// キャラクタリスティックへの書き込みが発生した際の処理を定義するクラス
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string receivedValue = pCharacteristic->getValue();  // キャラクタリスティックに書き込まれた値を取得

        if (receivedValue.length() > 0) {
            Serial.println("Received Value: " + String(receivedValue.c_str()));  // 受信した値をシリアルモニタに表示

            // 受信したコマンドに応じて対応する関数を実行し、結果を取得
            String result;
            if (receivedValue == "COMMAND1") {
                result = executeCommand1();  // "COMMAND1"を受け取った場合の処理
            } else if (receivedValue == "COMMAND2") {
                result = executeCommand2();  // "COMMAND2"を受け取った場合の処理
            } else {
                result = "Unknown command";  // 未知のコマンドの場合の処理
            }

            // 実行結果をキャラクタリスティックに設定し、クライアントに通知
            pCharacteristic->setValue(result.c_str());
            pCharacteristic->notify();  // クライアントに実行結果を通知
        }
    }
};

// "COMMAND1"に対応する関数
String executeCommand1() {
    // ここにCOMMAND1に対する具体的な処理を記述
    Serial.println("Executing Command 1...");
    return "Command 1 Executed";  // 処理結果を返す
}

// "COMMAND2"に対応する関数
String executeCommand2() {
    // ここにCOMMAND2に対する具体的な処理を記述
    Serial.println("Executing Command 2...");
    return "Command 2 Executed";  // 処理結果を返す
}

void loop() {
    // メインループ内では特に処理を行わないが、必要に応じて他のタスクを追加可能
}
