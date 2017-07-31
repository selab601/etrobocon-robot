#ifndef BODYANGLE_MEASUREMENT_
#define BODYANGLE_MEASUREMENT_

#include "SelfPositionEstimation.h"
#include "ev3api.h"

namespace measurement{
    /**
     * 車体角度検知クラス
     */
    class BodyAngleMeasurement{
    private:
        //! 基底角度
        int baseAngle_;

        //! 自己位置推定
        SelfPositionEstimation* selfPositionEstimation_;

    public:
        /**
         * @brief コンストラククタ
         */
        BodyAngleMeasurement();

        /**
         * @brief 現在の角度を基底角度として設定する
         */
        void setBaseAngle();

        /**
         * @brief 基底角度からの現在の角度を返す
         */
        int getResult();

        /**
         * @brief 基底角度からの現在の角度を返す
         */

        /**
         * @brief 基底角度からの現在の角度を返す(10倍)
         *
         * @return 角度の10倍[0.1Deg]
         */
        int getRelative10();

    };
};

#endif
