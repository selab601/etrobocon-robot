#include "Avoidance.h"

namespace drive{
    //コンストラクタ
    Avoidance::Avoidance(){
        bodyAngle_ = new measurement::BodyAngleMeasurement();
        runToState_ = RunToState::INIT;
    }

    // TODO: 直線に近いときにきちんと台座回避する
    // TODO: ライントレースのエッジをセットする
    // TODO: ブロックを離さないようにする
    bool Avoidance::runTo(int currentMm, int dstMMm, int degree){
        static int x = 300;
        static int y = 300;
        static int turnStartDegree = 0;
        switch (runToState_){
            case RunToState::INIT:
                {
                bodyAngle_->setBaseAngle();
                double rad = degree * M_PI /180;
                x = dstMMm * cos(rad) / 2;
                y = dstMMm * sin(rad) / 2;
                x += currentMm / 2;
                polar_.centerPivot(!hasBlock_);  // ブロックを離さないように
                polar_.setMaxPwm(20);
                runToState_ = RunToState::POLAR;
                ev3_speaker_play_tone(200, 100);
                }
                break;

            case RunToState::POLAR:
                if (polar_.runToXY(x, y)){
                    runToState_ = RunToState::TURN;
                    turnStartDegree = bodyAngle_->getResult();
                    ev3_speaker_play_tone(550, 100);
                }
                break;

            case RunToState::TURN:
                polar_.centerPivot(true);
                if ( polar_.bodyTurn(degree - turnStartDegree, 20) ){
                    runToState_ = RunToState::FINISHED;
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
}
