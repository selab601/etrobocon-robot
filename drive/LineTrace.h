/**
 * @file LineTrace.h
 * @brief ライントレースクラス
 * @details  初期設定[maxPwm:デフォルト値][edge:RIGHT][pid:デフォルト値][target:デフォルト値]
            setMaxPwm(),setPid(),setTarget(),setEdge()で各種設定を行った後にrun()で走行
 * @author kuno
 */

#ifndef LINE_TRACE_
#define LINE_TRACE_

#include "../device/ColorSensor.h"
#include "../device/Motors.h"
#include "../measurement/DistanceMeasurement.h"
#include "PidController.h"

<<<<<<< HEAD
#define DEFAULT_TARGET      0.5F    /* 明るさセンサの目標値となる値の黒の割合のデフォルト値*/
#define DEFAULT_MAXPWM      40      /* デフォルトのmaxPwm値*/
=======
#define DEFAULT_TARGET      0.7F    /* 明るさセンサの目標値となる値の黒の割合のデフォルト値*/
#define DEFAULT_MAXPWM      80      /* デフォルトのmaxPwm値*/
>>>>>>> 5eb50690fae7112a01f01a6ebecb7a39273bb028

#define LINETRACE_TREAD      1      /*未使用 きちんとした角速度に計算する定数*/
namespace drive{

    enum class LineTraceEdge{
        RIGHT,
        LEFT
    };

    /**
     * @brief ライントレースのPIDパラメータのパターン
     * @details 上にあるものほど反応が悪く、下にあるものほど反応が良い
     */
    enum class LineTracePid{
        //! 反応は悪いけど速い(PWM80とかにするとき)
        VERY_FAST,

        //! 少し反応が良い(PWM80で一応全部のカーブ曲がれる)
        FAST,

        //! 中間 (PWM30-70くらいのとき)
        MID,

        //! 安定してゆっくり走る (PWM30くらいのときかライン復帰したいとき)
        SLOW,

        //! 反応が極端に良く、ガクガクする(ライン復帰したい時だけ使う)
        RETURN,
    };

    class LineTrace{
    private:
        static LineTrace* instance_;
        LineTrace();

        enum class LineTraceEdgeChangePhase{
            ACROSS,
            END
        };

        enum class ChangeEdge2Phase{
            INIT,
            TO_MID_LINE,
            TO_OPPOSITE,
            END
        };

        //! PID制御
        PidController pidController_;

        int targetValue10_ = 0;            // ターゲット値：ターゲット目標値を元に算出される(明るさセンサの値を10倍した時の)光センサの目標値
        bool useHsv_ = false;

        int margin_;

        //ライントレースを行うエッジ
        LineTraceEdge edge_;

        //エッジ切り替えメソッドでの状態
        LineTraceEdgeChangePhase edgeChangeStatus_ = LineTraceEdgeChangePhase::ACROSS;
        //! エッジ切り替えでメソッドで使う
        int maxPwm_ = 0;

        // エッジ切り替え2での状態
        ChangeEdge2Phase changeEdge2Phase_ = ChangeEdge2Phase::INIT;

        //距離検知
        measurement::DistanceMeasurement* distanceMeasurement_;

        // Device
        device::ColorSensor* colorSensor_;
        //! エッジ切り替えメソッドで使う
        device::Motors* motors_;

    public:

        /**
         * @brief インスタンスを取得する
         *
         * @return インスタンスのポインタ
         * @author Nagaoka
         */
        static LineTrace* getInstance();


        /**
         * @brief HSVのV値を使うか輝度値を使うか指定する
         *
         * @param useHsv HSVのV値を使う場合true
         */
        void useHsv(bool useHsv = true);

        /**
         * @brief ライントレースを行う
         * @detail 必ず呼び出す直前にreset()を呼ばなければならない
         *
         * @param maxPwm モータのPWMの最大値
         * @param edge  ライントレースするエッジ(RIGHT,LEFT)
         * @param target ターゲット目標値 ( Black 0.0 < target < 1.0 White) default:0.6
         * @author Nagaoka
         */
        void run(int maxPwm,LineTraceEdge edge,double relativeTarget = DEFAULT_TARGET);

         /**
         * @brief ライントレースを行う
         *         こっちのrun()は事前に全てのset~が呼ばれていることが前提
         * @detail 必ず呼び出す直前にreset()を呼ばなければならない
         * @author kuno
         */
         void run();

         /**
         * @brief ライントレースを行う
         *         こっちのrun()は事前に全てのset~が呼ばれていることが前提
         * @detail 必ず呼び出す直前にreset()を呼ばなければならない。
         *      setPid(0, 0, 0)でrunCurve()を呼んでみてラインと同じカーブになるか実験してください
         * @param deltaRad カーブ曲がるときのdeltaRad, 左に曲がるときが正
         */
         void runCurve(int deltaRad );

        /**
         * @brief ライントレースするエッジをセットする
         * @param edge ライントレースするエッジ(RIGHT/LEFT)
         * @author kuno
         */
         void setEdge(LineTraceEdge edge);

        /**
         * @brief ライントレースする際のPWM値(モーターを回転させる強さ≒ライントレースする速さ)をセットする
         *         片方のタイヤは常にこの強さで走行する
         * @param maxPwm
         * @author kuno
         */
         void setMaxPwm(int maxPwm = DEFAULT_MAXPWM);

        /**
         * @brief PIDパラーメータをセットする
         * @param kp P制御の係数
         * @param ki I制御の係数
         * @param kd D制御の係数
         * @sa run
         */
        void setPid(double kp, double ki, double kd);


        /**
         * @brief パターンを指定してPIDパラメータをセットする
         * @sa LineTracePid
         *
         * @param pidPattern 指定したいパターン
         */
        void setPid(LineTracePid pidPattern);

        /**
         * @brief ターゲット目標値をセットする(≠ターゲット値)
         * @details ターゲット値(白)0.0 < x < 1.0(黒) の値から、ターゲット目標値を計算しセットする
                    x ≦ 0.0 || 1.0 ≦ x の場合 default値(0.6)を計算に使用してターゲット目標値をセット
         * @author Nagaoka
         **/
        void setTarget(double relativeTarget = DEFAULT_TARGET);

        /**
         * @brief PID制御の内部の情報をリセットする
         * @details 積分の値、ひとつ前のセンサの値、時間の情報を初期化する。ライントレースを使うときには必ず使う直前に最初に呼ぶ
         * @author Nagaoka
         **/
        void reset();

        /**
         * @brief ライントレースしながらエッジ切り替えを行う
         * @return エッジ切り替えが終了するとTrueを返す。
         * @author kuno
         **/
        bool changeEdge();


        /**
         * @brief LineTrace::run(), runCurve()を呼びながらエッジ切り替えを行う
         * @details このメソッドは状況に応じてターゲット値をエッジの情報を切り替えているだけなので、LineTrace::run() を呼びながらこのメソッドを呼ぶ
         *
         * @param toMidLineLength ラインの中心方向に移動する間の距離[mm] (300mm 程度推薦)
         * @param toOpposingLength ラインの中心から反対側に移動する間の距離(100mm ~ 300mm 程度推薦)
         *
         * @return エッジ切り替えが終了したらtrue
         */
        bool changeEdge2(int toMidLineLength, int toOpposingLength);

        /**
         * @brief edgeGetter 1:RIGHT 2:LEFT
         * @return edge
         */
        LineTraceEdge getEdge();

    private:

    }; //end of class
};
#endif
