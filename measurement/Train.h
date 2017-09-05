#ifndef _TRAIN_H
#define _TRAIN_H


#include "TimeMeasurement.h"

namespace measurement{
// 新幹線が内側を1周するときの時間[msec]
// 電池ない時: 9702 くらい
// 電池ほぼ満タンの時: 7729 くらい
// 試走会2 7610 くらい
// 東京地区大会 遊佐くんが測ってくれたやつの平均: 8373
#define IN_CYCLE_TIME   7462

// 新幹線が外側を1周するときの時間[msec]
// 電池ない時: 13566 くらい
// 電池ほぼ満タンの時: 10825 くらい
// 試走会2 1080 くらい
// 東京地区大会 遊佐くんが測ってくれたやつの平均: 11696
#define OUT_CYCLE_TIME  10355
#define TRAIN_LENGTH    2500    // 新幹線の長さ(時間)[msec]

// 1週ごとに増やす新幹線の長さ[msec]
#define EXTEND_PER_CYCLE 2500   // 電池がほぼ満タンのときでも対応できる

// 新幹線の位置をもっと先にいることにする時: +
// 新幹線のいちをもっと後ろに居ることにする時: -
// ex. 1秒分先に居ることにする時: 1000[msec]
#define ADJUST_LENGTH   -3000       // 新幹線位置の調整(時間)[msec]

    class Train {

    public:

        /**
         * @brief インスタンスを取得する
         * @details 新幹線は1つしかないのでシングルトンとする
         *
         * @return インスタンス
         */
        static Train* getInstance();

        /**
         * @brief 新幹線が走り出した(競技開始した)ことを知らせる
         */
        void start();

        /**
         * @brief 入り口に新幹線がいることを知らせる(誤差を修正する)
         * @details 入り口でソナーセンサで検知した時にこのメソッドを呼ぶと位置を修正する
         *          新幹線が外側と内側どちらを走っているかは、近い方を選択する
         */
        void setEntrance();
        void setCenter();


        /**
         * @brief 入り口に新幹線がいるとき true (falseのときに行く)
         *
         * @return 新幹線がいるときtrue
         */
        bool atEntrance();
        /**
         * @brief 真ん中の線路に新幹線がいるとき true (falseのときに行く)
         *
         * @return 新幹線がいるときtrue
         */
        bool atCenter();
        /**
         * @brief 出口に新幹線がいるとき true (falseのときに行く)
         *
         * @return 新幹線がいるときtrue
         */
        bool atExit();


    private:
        static Train * instance_;

        TimeMeasurement timeMeasurement_;

        //! 新幹線が内側と外側どっちを走っているか
        bool isOut_ = true;

        /**
         * @brief コンストラクタ
          */
        Train();


        /**
         * @brief 1周期ごとの時間を返す
         *
         * @return 1周期ごとの時間
         */
        int32_t getCycleTime();

        int32_t getCycleTime(int32_t time);


        /**
         * @brief 新幹線が通過する時間を返す
         * @details 時間の経過によるズレを考慮し、時間に比例して長くする
         *
         * @return 新幹線が通過する時間[ms]
         */
        int getPassTime();

        /**
         * @brief 最後に見たときから新幹線が何周したか返す
         *
         * @return 何周したかの10倍[0.1周]
         */
        int getCycleCount10();
    };
};

#endif
