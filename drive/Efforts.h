#ifndef EFFORTS_H_
#define EFFORTS_H_

#include "../drive/LineTrace.h"
#include "../drive/PivotTurn.h"
#include "../drive/CurveRunning.h"
#include "../drive/PushingOutRunning.h"
#include "../drive/ForcingOutRunning.h"
#include "../drive/BlockColorGetter.h"
#include "../drive/StraightRunning.h"
#include "../detection/LineDetection.h"
#include "../detection/ColorDetection.h"
#include "../detection/RightAngledDetection.h"
#include "../measurement/DistanceMeasurement.h"
#include <vector>

namespace drive{
    class Efforts{
    private:
        //取組状態
        enum class Phase{
            INIT,
            LINETRACE_TO_DAIZA,
            BLOCK_COLOR_GET,
            KIMARITE,
            PIVORT_TURN,
            TURN_TO_LINE,
            LINETRACE_RIGHT_ANGLED,
            BACK,
        };
        // 手順
        std::vector<Phase> phaseProcedure_{
            Phase::INIT,                    //取組位置に応じた初期化
            Phase::LINETRACE_TO_DAIZA,      //台座に向かう
            Phase::BLOCK_COLOR_GET,         //ブロック色取得
            Phase::LINETRACE_TO_DAIZA,      //台座から離れてるので再調整
            Phase::KIMARITE,                //取組
            Phase::BACK,                    //1と2のときだけバック
            Phase::PIVORT_TURN,             //旋回
            Phase::TURN_TO_LINE,            //ライン復帰
            Phase::LINETRACE_RIGHT_ANGLED   //中央線に帰る
        };

        drive::LineTrace* lineTrace_;
        drive::PivotTurn* pivotTurn_;
        drive::CurveRunning* curveRunning_;
        drive::BlockColorGetter blockColorGetter_;
        drive::PushingOutRunning* pushingOutRunning_;
        drive::ForcingOutRunning* forcingOutRunning_;
        drive::StraightRunning* straightRunning_;
        detection::LineDetection* lineDetection_;
        detection::ColorDetection* colorDetection_;
        detection::RightAngledDetection* rightAngledDetection_;
        measurement::DistanceMeasurement* distanceMeasurement_;

        //成功したかどうか
        bool isSuccess_;

        //手順番号
        unsigned int procedureNumber_ ;

        //台座とブロックの色を保存
        drive::colorset_t* result_;

        //取組を行う力士の位置
        int positionNumber_;

        //取組に向かうライントレースのエッジ
        drive::LineTraceEdge startEdge_;
        //取組後に帰るライントレースのエッジ
        drive::LineTraceEdge endEdge_;
        //寄り切り走行の向き
        bool isRightForcingOut_;
        //決まり手後に旋回する角度
        int turnAngle_;
        //ライン復帰のカーブ方向
        bool isRightCurve_;

    public:
        //コンストラクタ
        Efforts();

        /**
         * @brief 取組を実行
         *
         * @param positionNumber 取組を行う力士の位置
         *  登壇前の走行体の位置から見て
         *  左手前:1,右手前:2,左奥:3,右奥4
         * @return 実行完了:true,実行中:false
         */
        bool run(int positionNumber);

        /**
         * @brief 初期化
         * @details 複数使う場合はreset()を挟んでください
         */
        void reset();

    private:
        /**
         * @brief 手順を実行
         *
         * @param phase 手順
         * @return 実行完了:true,実行中:false
         */
        bool executePhase(Phase phase);
    };
}

#endif
