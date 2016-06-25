#include "Sonar.h"

// インスタンス変数初期化
Sonar* Sonar::_instance = 0;

bool Sonar::IGNORE_INSTANCE_CREATION = false;

// シングルトン
Sonar* Sonar::getInstance(){
    if (Sonar::IGNORE_INSTANCE_CREATION) { return 0; }

    if(_instance == 0){
        _instance = new Sonar();
    }
    return _instance;
}

Sonar::Sonar()
    // ポートを指定している
    :sonarSensor(PORT_2)
{
    Sonar::IGNORE_INSTANCE_CREATION = false;
}

void Sonar::ignoreSonar() {
    Sonar::IGNORE_INSTANCE_CREATION = true;
}

void Sonar::considerSonar() {
    Sonar::IGNORE_INSTANCE_CREATION = false;
}

int16_t Sonar::getSonarValue(){
    return sonarSensor.getDistance();
}

bool Sonar::listen(){
    return sonarSensor.listen();
}
