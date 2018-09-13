#ifndef FORCING_OUT_RUNNING_H_
#define FORCING_OUT_RUNNING_H_

#include "../drive/CurveRunning.h"
#include "../drive/PivotTurn.h"
#include "../device/Arm.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "PolarRunning.h"
#include <vector>
#define DEFAULT_FORCING_DEGREE 50

namespace drive{
    class ForcingOutRunning{

    private:
        //寄り切り走行の状態
        enum class Phase{
            SET_ANGLE,
            CURVE,
            ARM_UP,
            ARM_DOWN,
            BACK,
        };

        //走行手順
        std::vector<Phase> phaseProcedure_{
            Phase::SET_ANGLE,//車体角度保存
            Phase::CURVE,    //信地旋回
            Phase::ARM_UP,  //追加　BACK時ブロックを巻き込まないようにする
            //Phase::ARM_UP,   //アーム上げる
            //Phase::ARM_DOWN, //アーム戻す
            Phase::SET_ANGLE,//車体角度保存
            Phase::BACK,      //開始地点に戻る
            Phase::ARM_DOWN,//追加
        };

        drive::CurveRunning* curveRunning_;
        drive::PivotTurn* pivotTurn_;
        device::Arm* arm_;
        measurement::BodyAngleMeasurement* bodyAngleMeasurement_;

        PolarRunning polar_;

        //成功したかどうか
        bool isSuccess_;
        //右に曲がるかどうか
        bool isRight_;
        //現在の手順番号
        unsigned int procedureNumber_;
        //走行スピード
        int speed_;

        //! 信地旋回で押し出す角度
        int degree_ = DEFAULT_FORCING_DEGREE;


    public:
        //コンストラクタ
        ForcingOutRunning();

        /**
         * @brief 寄り切り走行を実行
         *
         * @param speed 走行スピード
         * @param isRight 右に寄り切り:true,左に寄り切り:false
         *
         * @return 実行完了:true,実行中:false
         */
        bool run(int speed ,bool isRight);

        /**
         * @brief スピードやフラグをリセット
         * @details 複数回使う場合はreset()を挟んでください
         */
        void reset();

        void setDegree(int degree = DEFAULT_FORCING_DEGREE);

    private:
        /**
         * @brief 走行手順を実行
         *
         * @param phase 走行手順
         * @return 実行完了:true,実行中:false
         */
        bool executePhase(Phase phase);
    };

}

#endif
