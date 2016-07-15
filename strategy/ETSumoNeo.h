#ifndef ETSUMO_NEO_H
#define ETSUMO_NEO_H

#include "IStrategy.h"
#include "../drive/ExtrusionRunning.h"
#include "../drive/ClimbingRunning.h"
#include "../drive/StraightRunning.h"
#include "../drive/LineTrace.h"
#include "../detection/LineDetection.h"

namespace strategy{
    class ETSumoNeo : public IStrategy{
    private:
        enum class Step{
            A,
            B
        };

        drive::ExtrusionRunning* extrusionRunning_;
        drive::ClimbingRunning* climbingRunning_;
        drive::StraightRunning* straightRunning_;
        drive::LineTrace* linetrace_;
        detection::LineDetection* lineDetection_;
        int a_;
    public:
        //コンストラクタ
        ETSumoNeo(int a);

        /**
         * @brief ET相撲Neoを攻略する
         * @return 攻略完了：true,攻略中：false
         */
        bool capture();
    };
}





#endif
