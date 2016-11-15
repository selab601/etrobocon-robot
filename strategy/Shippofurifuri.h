#ifndef SHIPPO_FURIFURI_H
#define SHIPPO_FURIFURI_H

#include "IStrategy.h"
#include "../device/TouchSensor.h"
#include "../drive/CurveRunning.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "../device/Shippo.h"
#include "../device/SonarSensor.h"
#include <vector>

namespace strategy{
    class Shippofurifuri : public IStrategy{
    private:
        enum class Phase{
            FURIFURI,
            STOP_FURIFURI,
        };

        //ふりふり攻略手順
        std::vector<Phase> phaseProcedure_{
            Phase::FURIFURI,
            Phase::STOP_FURIFURI,
        };

        drive::CurveRunning* curveRunning_;

        //難所クリアしたかどうか
        bool strategySuccess_;


    public:
        //コンストラクタ
        Shippofurifuri();

        /**
         * @brief 懸賞運びを攻略する
         * @return 攻略完了:true,攻略中:false
         */
        bool capture();

    private:
        /**
         * @brief 懸賞運びの手順を実行
         *
         * @param phase 手順
         * @return 終了:true,攻略中:false
         */
        bool executePhase(Phase phase);

        /**
         * @brief タッチセンサがクリックされた
         *
         * @return タッチセンサがクリックされた時true
         */
        bool isClicked();

    };
}

#endif
