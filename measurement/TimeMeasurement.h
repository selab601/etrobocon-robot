#ifndef _TIME_MEASUREMENT_H
#define _TIME_MEASUREMENT_H

#include "ev3api.h"
#include "Clock.h"

namespace measurement{
    class TimeMeasurement {

    private:
        //! 時間取得
        ev3api::Clock clock;

        //! 基底時間
        uint32_t baseTime_;
        //! 目標時間
        uint32_t targetTime_;


    public:
        /**
         * @brief コンストラクタ
          */
        TimeMeasurement();

        /**
         * @brief setBaseTime,setTargetTimeを同時に実行
         * 単位はミリ秒なので10秒を目標時間にしたかったら「10000」を引数に指定してください
         * フラグ管理をしているので実行前にreset()が必要
         * @target 目標時間(target > 0)
         */
        void start(int target);

        /**
         * @brief 目標時間に達したかどうかを検知する
         * 目標時間は基底時間からの経過時間を指す
         * @return 目標時間に達した(true)，してない(false)
          */
        bool getResult();


        /**
         * @brief 基底時間からの経過時間を計測する
         *
         * @return 経過時間
         */
        uint32_t getRelative();


        /**
         * @brief 目標時間までの残り時間を返す
         *
         * @return 目標時間までの残り時間
         */
        uint32_t getRemaining();

        /**
         * @brief 基底時間をセットする
          */
        void setBaseTime();


        /**
         * @brief 現在時刻以外の基底時間をセットする
         *
         * @param relativeMs 現在時刻を基準とした基底時間[msec] ex. 1秒前: -1000[ms]
         */
        void setBaseTime(uint32_t relativeMs);

        /**
         * @brief 目標時間をセットする
         * 単位はミリ秒なので10秒を目標時間にしたかったら「10000」を引数に指定してください
          */
        void setTargetTime(uint32_t targetTime);

        /**
         * @brief baseTime, TargetTime, フラグをリセット
         */
        void reset();
    };
};

#endif
