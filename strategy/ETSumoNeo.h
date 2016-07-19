#ifndef ETSUMO_NEO_H
#define ETSUMO_NEO_H

#include "IStrategy.h"
#include "../drive/ExtrusionRunning.h"
#include "../drive/ClimbingRunning.h"
#include "../drive/StraightRunning.h"
#include "../drive/PivotTurn.h"
#include "../drive/LineTrace.h"
#include "../detection/LineDetection.h"
#include "../detection/ColorDetection.h"
#include "../detection/ObjectDetection.h"
#include "../measurement/DistanceMeasurement.h"

namespace strategy{
    class ETSumoNeo : public IStrategy{
    private:
        //ET相撲Neoの走行状態
        enum class Statas{
            HOSHITORI,
            TURN,
            STRAIGHT,
            LINE_TRACE,
            STOP,
            CLIMB,
            SUMO,
            GET_OF
        };

        //登壇後に行う相撲の走行ステップ
        enum class SumoStatas{
            a,
            b
        };

        Statas statas_;
        SumoStatas sumoStatas_;

        //走行
        drive::ExtrusionRunning* extrusionRunning_;
        drive::ClimbingRunning* climbingRunning_;
        drive::StraightRunning* straightRunning_;
        drive::PivotTurn* pivotTurn_;
        drive::LineTrace* linetrace_;

        //検知
        detection::LineDetection* lineDetection_;
        detection::ColorDetection* colorDetection_;
        detection::ObjectDetection* objectDetection_;



        //自己位置
        measurement::DistanceMeasurement* distanceMeasurement_;

        //取得した星取
        colorid_t hoshitori_;

    public:
        //コンストラクタ
        ETSumoNeo();

        /**
         * @brief ET相撲Neoを攻略する
         * @return 攻略完了：true,攻略中：false
         */
        bool capture();

        bool performSumo();

    };
}





#endif
