#ifndef SONARSENSOR_H_
#define SONARSENSOR_H_

#include "SonarSensor.h"

using namespace ev3api;

class Sonar : ev3api::SonarSensor
{
private:
    static Sonar* _instance;

    // ソナーのインスタンス生成無視のフラグ．(無視するなら true, 無視しないなら false)
    // ソナー使用には専用の周期のタスクが必要
    // 該当タスクが存在しない場合にインスタンスを生成すると不具合が生じる可能性があるため，デフォルト値は false
    static bool IGNORE_INSTANCE_CREATION;

private:
    Sonar();

public:
    /**
     * @brief
     *   シングルトンパターンに基づいて，ソナーの共有インスタンスを取得する．
     *   ただし，considerSonar メソッドによってインスタンス生成を有効化しておく必要がある．
     * @return
     *   インスタンス生成が有効の場合は 共有インスタンス
     *   インスタンス生成が無効の場合は 0
     */
    static Sonar* getInstance();

    /**
     * @brief ソナーのインスタンス生成を無効化
     */
    static void ignoreSonar();

    /**
     * @brief ソナーのインスタンス生成を有効化
     */
    static void considerSonar();
};

#endif
