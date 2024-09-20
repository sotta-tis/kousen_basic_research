#include "servoControl.h"
#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>

ServoControl::ServoControl(int servoPin) : _servoPin(servoPin), _currentAngle(0) {
    wiringPiSetupGpio();  // GPIOの初期化
    softPwmCreate(_servoPin, 0, 200);  // サーボピンのPWM作成
}

void ServoControl::open() {
    setPosition(180);  // サーボを180度に移動（開く動作）
    std::cout << "Servo opened" << std::endl;
}

void ServoControl::close() {
    setPosition(0);  // サーボを0度に移動（閉じる動作）
    std::cout << "Servo closed" << std::endl;
}

void ServoControl::setPosition(int angle) {
    _currentAngle = angle;
    int pulseWidth = (int)((angle / 180.0) * 200);  // 角度をPWM値に変換
    softPwmWrite(_servoPin, pulseWidth);
    delay(500);  // サーボが動く時間の遅延
}
