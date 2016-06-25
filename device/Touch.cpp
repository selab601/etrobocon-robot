#include "Touch.h"

// インスタンス変数初期化
Touch* Touch::_instance = 0;

// シングルトン
Touch* Touch::getInstance(){
    if(_instance == 0){
        _instance = new Touch();
    }
    return _instance;
}

Touch::Touch() :
    // ポートの指定
    TouchSensor(PORT_4)
{}
