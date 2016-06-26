#ifndef SONARSENSOR_H_
#define SONARSENSOR_H_

#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/SonarSensor.h"

namespace device
{
    class SonarSensor : public ev3api::SonarSensor
    {
    private:
        static SonarSensor* _instance;

        // ソナーのインスタンス生成無視のフラグ．(無視するなら true, 無視しないなら false)
        static bool IGNORE_INSTANCE_CREATION;

    private:
        SonarSensor();

    public:
        /**
         * @brief
         *   シングルトンパターンに基づいて，ソナーの共有インスタンスを取得する．
         *   ただし，considerSonar メソッドによってインスタンス生成を有効化しておく必要がある．
         *   ソナー使用には専用の周期のタスクが必要である．
         *   該当タスクが存在しない場合にインスタンスを生成すると不具合が生じる可能性があるため，デフォルトでは無効化されている．
         * @return
         *   インスタンス生成が有効の場合は 共有インスタンス
         *   インスタンス生成が無効の場合は 0
         */
        static SonarSensor* getInstance();

        /**
         * @brief ソナーのインスタンス生成を無効化
         */
        static void ignoreSonar();

        /**
         * @brief ソナーのインスタンス生成を有効化
         */
        static void considerSonar();
    };
}

#endif
