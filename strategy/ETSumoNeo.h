#ifndef ETSUMO_NEO_H
#define ETSUMO_NEO_H

#include "IStrategy.h"
#include "../drive/ExtrusionRunning.h"
#include "../drive/ClimbingRunning.h"
#include "../drive/StraightRunning.h"
#include "../drive/PivotTurn.h"
#include "../drive/LineTrace.h"
#include "../drive/CurveRunning.h"
#include "../detection/LineDetection.h"
#include "../detection/ColorDetection.h"
#include "../detection/ObjectDetection.h"
#include "../detection/RightAngledDetection.h"
#include "../measurement/TimeMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include <vector>

#define SUMO_EXTRUSION_DISTANCE  100 //押し出し走行距離
#define SUMO_EXTRUSION_SPEED     20  //押し出しスピード

namespace strategy{
    class ETSumoNeo : public IStrategy{
    private:
        //ET相撲Neoの走行状態
        enum class StrategyPhase{
            INIT,
            HOSHITORI,
            SET_VALUE,
            BACK,
            TURN_LEFT,
            STRAIGHT,
            LINE_TRACE,
            LINE_TRACE_LITTLE,
            STOP,
            WAIT_1_SEC,
            TURN_LITTLE,
            CLIMB,
            WAIT_2_SEC,
            TURN_TO_SIDE,
            BACK_TO_LINE,
            STRAIGHT_4_CM,
            TURN_TO_DOWN,
            SUMO,
            GET_OF,
            LINE_DETECTION,
            LINE_RETURN
        };

        //相撲全体の状態
        enum class SumoPhase{
            FIRST_EXTRUSION,
            SECOND_EXTRUSION,
            THIRD_EXTRUSION,
            FIRST_TURN,
            SECOND_TURN,
            THIRD_TURN,
            UPPER_STAGE,
            DOWN_STAGE,
            STRAIGHT_3_CM,
            ACROSS_LINE,
            CURVE_TOP,
            CORRECTION
        };

        //特定の力士を押し出す走行状態
        enum class ExtrusionPhase{
            START_LINE_TRACE,
            EXTRUSION,
            FIRST_TURN,
            SECOND_TURN,
            END_LINE_TRACE
        };

        //星取
        enum class Hoshitori{
            RED,        //赤
            BLUE,       //青
            YELLOW,     //黄
            GREEN,      //緑
            NONE        //なし
        };

        //難所攻略手順
        std::vector<StrategyPhase> strategyProcedure_{
            StrategyPhase::INIT,             //車体角度保存
            StrategyPhase::HOSHITORI,        //星取取得
            StrategyPhase::SET_VALUE,        //星取が判明したので値を代入
            StrategyPhase::BACK,             //星取を踏まないようにバック
            StrategyPhase::TURN_LEFT,        //左に旋回
            StrategyPhase::STRAIGHT,         //ラインまで直進
            StrategyPhase::LINE_TRACE,       //土俵を向くまでライントレース
            StrategyPhase::LINE_TRACE_LITTLE,//すこしライントレース
            StrategyPhase::STOP,             //新幹線検知するまで停止
            StrategyPhase::WAIT_1_SEC,       //検知後に待つ
            StrategyPhase::TURN_LITTLE,      //すこし旋回
            StrategyPhase::CLIMB,            //登壇
            StrategyPhase::WAIT_1_SEC,       //登壇後に機体が落ち着くまで待つ
            StrategyPhase::TURN_TO_SIDE,     //横を向く
            StrategyPhase::BACK_TO_LINE,     //中央線までバック
            StrategyPhase::STRAIGHT_4_CM,    //4cm直進
            StrategyPhase::TURN_TO_DOWN,     //下を向くように旋回
            StrategyPhase::SUMO,             //相撲-SumoPhase-
            StrategyPhase::BACK_TO_LINE,     //-SumoPhase終了(降段方向を向いている)-ラインまでバック
            StrategyPhase::STOP,             //新幹線検知するまで停止
            StrategyPhase::WAIT_2_SEC,       //検知後に待つ
            StrategyPhase::GET_OF,           //降段
            StrategyPhase::LINE_DETECTION,
            StrategyPhase::LINE_RETURN       //ライン復帰
        };

        //相撲攻略手順(星取り赤・青)
        std::vector<SumoPhase> sumoProcedureRorB_{
            SumoPhase::DOWN_STAGE,      //直角検知までライントレース
            SumoPhase::STRAIGHT_3_CM,   //3cm直進
            SumoPhase::FIRST_TURN,      //横を向く
            SumoPhase::FIRST_EXTRUSION, //一つ目押し出し
            SumoPhase::CURVE_TOP,       //上段を向く
            SumoPhase::UPPER_STAGE,     //上段に移動
            SumoPhase::ACROSS_LINE,     //ラインを横切る
            SumoPhase::SECOND_TURN,     //横を向く
            SumoPhase::SECOND_EXTRUSION,//二つ目押し出し
            SumoPhase::ACROSS_LINE,     //ラインを横切る
            SumoPhase::THIRD_EXTRUSION, //三つ目押し出し
            SumoPhase::CORRECTION,      //同じ位置になるように移動
            SumoPhase::THIRD_TURN       //降段方向を向く
        };

        //相撲攻略手順(星取り黄・緑)
        std::vector<SumoPhase> sumoProcedureYorG_{
            SumoPhase::DOWN_STAGE,      //直角検知までライントレース
            SumoPhase::STRAIGHT_3_CM,   //3cm直進
            SumoPhase::FIRST_TURN,      //横を向く
            SumoPhase::FIRST_EXTRUSION, //一つ目押し出し
            SumoPhase::ACROSS_LINE,     //ラインを横切る
            SumoPhase::SECOND_EXTRUSION,//二つ目押し出し
            SumoPhase::CURVE_TOP,       //上段を向く
            SumoPhase::UPPER_STAGE,     //上段に移動
            SumoPhase::ACROSS_LINE,     //ラインを横切る
            SumoPhase::SECOND_TURN,     //横を向く
            SumoPhase::THIRD_EXTRUSION, //三つ目押し出し
            SumoPhase::CORRECTION,      //同じ位置になるように移動
            SumoPhase::THIRD_TURN       //降段方向を向く
        };



        //走行
        drive::ExtrusionRunning* extrusionRunning_;
        drive::ClimbingRunning* climbingRunning_;
        drive::StraightRunning* straightRunning_;
        drive::PivotTurn* pivotTurn_;
        drive::CurveRunning* curveRunning_;
        drive::LineTrace* linetrace_;

        //検知
        detection::LineDetection* lineDetection_;
        detection::ColorDetection* colorDetection_;
        detection::ObjectDetection* objectDetection_;
        detection::RightAngledDetection* rightAngledDetection_;

        //計測
        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::TimeMeasurement* timeMeasurement_;
        measurement::BodyAngleMeasurement* bodyAngleMeasurement_;

        //押し出し手順
        ExtrusionPhase extrusionPhase_;

        //取得した星取
        Hoshitori hoshitori_;

        //距離検知の設定などを一度だけ行うためのフラグ
        bool hasExecutedPhase_;

        //難所クリアしたかどうか
        bool strategySuccess_;

        //相撲をクリアしたかどうか
        bool sumoSuccess_;

        //星取の色が判明してから決まる値
        int climbBeforeLittleAngle_;        //登壇前に少し回転する角度
        int climbAfterSideFaceAngle_;       //登壇後に横を向く角度
        bool isRightRotation_;              //ライン検知まで旋回を右周りでやるかどうか
        Hoshitori firstWrestlerColor_;      //一人目の力士の色
        Hoshitori secondWrestlerColor_;     //二人目の力士の色
        Hoshitori thirdWrestlerColor_;      //三人目の力士の色
        drive::LineTraceEdge upperStageEdge_;//上段に移動するライントレースのエッジ
        drive::LineTraceEdge downStageEdge_; //下段に移動するライントレースのエッジ
        int firstTurnAngle_;                //一回目の旋回角度
        int secondTurnAngle_;               //二回目の旋回角度
        int thirdTurnAngle_;                //三回目の旋回角度
        int isRightCurve_;                  //右カーブで上を向くかどうか
        bool isCorrect_;                    //補正によって右側に移動するかどうか

    public:
        //コンストラクタ
        ETSumoNeo();

        /**
         * @brief ET相撲Neoを攻略する
         * @return 攻略完了:true,攻略中:false
         */
        bool capture();

    private:
        /**
         * @brief 難所の手順を実行
         * @param strategyPhase 難所の手順
         * @return 難所終了:true,攻略中:false
         */
        bool executeStrategy(StrategyPhase strategyPhase);

        /**
         * @brief 相撲を攻略する
         * @return 攻略完了:true,攻略中:false
         */
        bool captureSumo();

        /**
         * @brief 相撲の手順を実行
         * @details 星取の色によって振る舞いを変える
         * @param sumoPhase 相撲の手順
         * @return 相撲終了:true,攻略中:false
         */
        bool executeSumo(SumoPhase sumoPhase);

        /**
         * @brief 指定した星取のブロックを押し出す
         * @details 中央線からスタートし、中央線で終了する、
         * 左側(赤・黄)と右側(青・緑)はそれぞれ同じ動作をする
         *
         * @param blockColor 押し出すブロックの色
         * @return 押し出し終了:true,攻略中:false
         */
        bool extrusion(Hoshitori blockColor);

        /**
         * @brief 星取が判明してから決まる値を代入する
         */
        void setValue();

        /**
         * @brief ライン検知するまで旋回する
         *
         * @param isRight 右旋回かどうか
         * @param speed 旋回スピード
         *
         * @return 終了:true,移動中:false
         */
        bool turn(bool isRight,int speed);

        /**
         * @brief 上段に向けてカーブ走行する
         *
         * @param isRight 右カーブかどうか
         * @param speed カーブスピード
         *
         * @return 終了:true,移動中:false
         */
        bool curve(bool isRight,int speed);

        /**
         * @brief ライン復帰しやすいように補正する
         * @details 最後に黄色を押し出した場合に緑の方まで進む
         * @return 終了:true,移動中:false
         */
        bool correction();

        /**
         * @brief 星取検知
         * @details 星取の赤、青、黄、緑のいずれかを検知する、
         * 引数をtureとすることで取得した星取を保存できる
         *
         * @param getHoshitori 星取を保存するかどうか
         * @return 赤、青、黄、緑だった場合:true
         */
        bool hoshitoriDetection(bool saveHoshitori = false);

        /**
         * @brief 距離検知を開始する
         * @details setTarget,startを同時に行い、フラグ管理も行う
         * @param distance 検知する距離
         */
        void startDistanceMeasurement(int distance);

        /**
         * @brief 時間検知を開始する
         * @details setBase,setTargetを同時に行い、フラグ管理も行う
         * @param time 検知する時間
         */
        void startTimeMeasurement(int time);
    };
}

#endif
