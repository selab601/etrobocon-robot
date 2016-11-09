#include "Shippofurifuri.h"

using namespace drive;
using namespace device;

namespace strategy{
    Shippofurifuri::Shippofurifuri(){
        curveRunning_           = new CurveRunning();
        strategySuccess_        = false;
    }

    //攻略する
    bool Shippofurifuri::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            //難所攻略手順を1つずつ実行する
            if(executePhase(phaseProcedure_[procedureNumber])){
                procedureNumber++;
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
        }
        //最後まで終わったら
        if(procedureNumber == phaseProcedure_.size()){
            strategySuccess_ = true;
        }
        return strategySuccess_;
    }

    //攻略手順
    bool Shippofurifuri::executePhase(Phase phase){
        switch(phase){
        case Phase::FURIFURI:
            curveRunning_->run(15, -15);         // くるくる
            Shippo::getInstance()->furifuri();  // ふりふり

            if (isClicked()){
                return true;
            }
            if (device::SonarSensor::getInstance()->getDistance() <= 5){
                return true;
            }
            return false;

            // 尻尾を上に向けて止める
        case Phase::STOP_FURIFURI:
            curveRunning_->run(0, 0);
            return Shippo::getInstance()->pleased();

        default: return false;
        }
    }

    bool Shippofurifuri::isClicked(){
        static bool hasPressed = false;
        if (device::TouchSensor::getInstance()->isPressed()){
            hasPressed = true;
        }
        else if (hasPressed){
            hasPressed = false;
            // クリックされたら音を出す
            ev3_speaker_play_tone ( 500, 100);
            return true;
        }
        return false;
    }

}
