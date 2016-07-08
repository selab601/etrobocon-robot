#include "SonarSensor.h"

namespace device
{
    // インスタンス変数初期化
    SonarSensor* SonarSensor::instance_ = 0;

    bool SonarSensor::IGNORE_INSTANCE_CREATION = false;

    // シングルトン
    SonarSensor* SonarSensor::getInstance(){
        if (SonarSensor::IGNORE_INSTANCE_CREATION) { return 0; }

        if(instance_ == 0){
            instance_ = new SonarSensor();
        }
        return instance_;
    }

    SonarSensor::SonarSensor() :
        // ポートを指定している
        ev3api::SonarSensor(PORT_2)
    {
        SonarSensor::IGNORE_INSTANCE_CREATION = false;
    }

    void SonarSensor::ignoreSonar() {
        SonarSensor::IGNORE_INSTANCE_CREATION = true;
    }

    void SonarSensor::considerSonar() {
        SonarSensor::IGNORE_INSTANCE_CREATION = false;
    }
}
