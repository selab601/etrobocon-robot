#include "Avoidance.h"

namespace drive{
    //コンストラクタ
    Avoidance::Avoidance(){
        bodyAngle_ = new measurement::BodyAngleMeasurement();
        colorSensor_ = device::ColorSensor::getInstance();
        lineTrace_ = LineTrace::getInstance();
        runToState_ = RunToState::INIT;
    }

    // TODO: ブロックを離さないようにする
    bool Avoidance::runTo(int currentMm, int dstMMm, int degree){
        static int x = 0;
        static int y = 0;
        static int avoidX = 0;
        static int avoidY = 0;
        static int turnDegree10 = 0;
        static int avoidedDegree10 = 0;
        static int fixedDegree = 0;
        static int turnSpeed = 20;

        switch (runToState_){
            case RunToState::INIT:
                {
                // -180 〜 180° にする
                fixedDegree = degree;
                while (fixedDegree < 0){
                    fixedDegree += 360;
                }
                fixedDegree += 180;
                fixedDegree %= 360;
                fixedDegree -= 180;

                bodyAngle_->setBaseAngle();
                double rad = fixedDegree * M_PI /180;
                x = dstMMm * cos(rad) / 2;
                y = dstMMm * sin(rad) / 2;
                x += currentMm / 2;
                avoidX = currentMm / 2;
                avoidY = fixedDegree <= 0? -160 : 160;
                if (lineTrace_->getEdge() == LineTraceEdge::RIGHT){
                    y       += 10; // 右エッジの時、Y方向を1cm増やす
                    avoidY  += 10;
                }
                else{
                    y       -= 10; // 左エッジの時、Y方向を1cm減らす
                    avoidY  -= 10;
                }

                polar_.centerPivot(!hasBlock_);  // ブロックを離さないように
                polar_.setMaxPwm(20);
                edgeCount_ = 0;
                atWhite_ = true;
                turnSpeed = 20;
                // 真後ろに行く時
                if (abs(fixedDegree) > 170){
                    runToState_ = RunToState::PIVOT_TURN;
                }
                // 避けないとぶつかる時
                else if (abs(fixedDegree) < 55){
                    runToState_ = RunToState::AVOID;
                }
                else if (400 >currentMm || 400 > dstMMm){
                    avoidY = avoidY * 3 /2; // ぶつかりやすいから多めに避ける
                    turnSpeed = 12;
                    runToState_ = RunToState::AVOID;
                }
                // 目的地にまっすぐ行くだけで良い時
                else {
                    runToState_ = RunToState::TO_DST;
                }
                ev3_speaker_play_tone(200, 100);
                }
                break;

            case RunToState::AVOID:
                {
                    polar_.centerPivot(true); // 精度を上げる
                    if (polar_.runToXY(avoidX, avoidY)){
                        avoidedDegree10 = bodyAngle_->getRelative10();
                        x -= avoidX;
                        y -= avoidY;
                        runToState_ = RunToState::AVOID_TURN;
                    }
                }
                break;

            case RunToState::AVOID_TURN:
                polar_.centerPivot(true);
                if ( polar_.bodyTurn(-avoidedDegree10, 20) ){
                    runToState_ = RunToState::TO_DST;
                    ev3_speaker_play_tone(800, 100);
                }
                break;

            case RunToState::TO_DST:
                if (polar_.runToXY(x, y)){
                    runToState_ = RunToState::TURN;
                    turnDegree10 = fixedDegree*10 - bodyAngle_->getRelative10();
                    ev3_speaker_play_tone(550, 100);
                }
                if (polar_.getLeftMm() < 150){
                    incrementEdge(); // 最後の15センチの間でラインを探す
                }
                break;

            case RunToState::TURN:
                polar_.centerPivot(true);
                if ( polar_.bodyTurn(turnDegree10, turnSpeed) ){
                    runToState_ = RunToState::CALCULATE_EDGE;
                    ev3_speaker_play_tone(800, 100);
                }
                decrementEdge();
                break;

            case RunToState::CALCULATE_EDGE:
                if (0 < turnDegree10) {  // ラインを超えたら右エッジ
                    if (2 <= edgeCount_) {
                        lineTrace_->setEdge(LineTraceEdge::RIGHT);
                        ev3_led_set_color (LED_RED);
                    }
                    else {
                        lineTrace_->setEdge(LineTraceEdge::LEFT);
                        ev3_led_set_color (LED_GREEN);
                    }
                }
                else { // ラインを超えたら左エッジ
                    if (2 <= edgeCount_) {
                        lineTrace_->setEdge(LineTraceEdge::LEFT);
                        ev3_led_set_color (LED_GREEN);
                    }
                    else {
                        lineTrace_->setEdge(LineTraceEdge::RIGHT);
                        ev3_led_set_color (LED_RED);
                    }
                }
                if (2 < edgeCount_ || 0 > edgeCount_){
                    ev3_speaker_play_tone(1000, 500); // エラーを知らせる
                }
                runToState_ = RunToState::FINISHED;
                break;

            case RunToState::PIVOT_TURN:
                polar_.centerPivot(true);
                if ( polar_.bodyTurn(1800, 20) ){
                    runToState_ = RunToState::FINISHED;
                    // エッジを逆にする
                    if (lineTrace_->getEdge() == LineTraceEdge::LEFT){
                        lineTrace_->setEdge(LineTraceEdge::RIGHT);
                    }
                    else{
                        lineTrace_->setEdge(LineTraceEdge::LEFT);
                    }
                    ev3_speaker_play_tone(800, 100);
                }
                break;

            case RunToState::FINISHED:
                runToState_ = RunToState::INIT;
                return true;

        }
        return false;
    }

    void Avoidance::hasBlock(bool hasBlock){
        hasBlock_ = hasBlock;
    }

    bool Avoidance::isWhite(){
        int threshold = colorSensor_->getBlackCalibratedValue();
        threshold += (colorSensor_->getWhiteCalibratedValue() - colorSensor_->getBlackCalibratedValue()) / 5;
        return colorSensor_->getBrightness() >= threshold;
    }

    void Avoidance::incrementEdge(){
        if (atWhite_){
            if (!isWhite()){
                edgeCount_++;
                atWhite_ = false;
                ev3_led_set_color (LED_RED);
                ev3_speaker_play_tone(800, 10);
            }
        }
        else{
            if (isWhite()){
                edgeCount_++;
                atWhite_ = true;
                ev3_led_set_color (LED_GREEN);
                ev3_speaker_play_tone(400, 10);
            }
        }
    }
    void Avoidance::decrementEdge(){
        if (atWhite_){
            if (!isWhite()){
                edgeCount_++;
                atWhite_ = false;
                ev3_led_set_color (LED_RED);
            }
        }
        else{
            if (isWhite()){
                edgeCount_++;
                atWhite_ = true;
                ev3_led_set_color (LED_GREEN);
            }
        }
    }
}
