#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

class ServoControl {
public:
    ServoControl(int servoPin);  // コンストラクタ
    void open();  // サーボを開く
    void close(); // サーボを閉じる
    void setPosition(int angle); // 特定の角度にサーボを移動

private:
    int _servoPin;  // サーボピン番号
    int _currentAngle;  // 現在の角度
};

#endif // SERVOCONTROL_H
