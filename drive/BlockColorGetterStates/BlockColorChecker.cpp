#include "BlockColorChecker.h"

namespace drive {
    BlockColorChecker::BlockColorChecker() {
        // 状態の実行順序を定義
        // WARNING: FINISH を push しておかないと無限ループする
        states_.push(CHECKER_STATE::RGB);
        states_.push(CHECKER_STATE::FINISH);

        timer_ = measurement::TimeMeasurement();
        colorSensor_ = device::ColorSensor::getInstance();
        colorDetection_ = detection::ColorDetection();
        isInitializedTime_ = false;
        display_ = device::Display::getInstance();
    }

    bool BlockColorChecker::isExecuted(colorid_t* result) {
        if (isInitializedTime_) {
            timer_.setBaseTime();
            timer_.setTargetTime(1000);
            isInitializedTime_ = true;
        }

        // 本当はSTATE パターン使いたかったけど面倒なのでやめた
        // 輝度値なども取得する可能性があるのでフラグ管理ではなく switch で分岐
        switch (states_.front()) {
        case CHECKER_STATE::RGB:
        {
            rgb_raw_t* tmpRGBValue = new rgb_raw_t;
            tmpRGBValue->r = 0;
            tmpRGBValue->g = 0;
            tmpRGBValue->b = 0;
            ev3_color_sensor_get_rgb_raw(EV3_PORT_3, tmpRGBValue);
            pastRValues_.push_back(tmpRGBValue->r);
            pastGValues_.push_back(tmpRGBValue->g);
            pastBValues_.push_back(tmpRGBValue->b);
            break;
        }
        case CHECKER_STATE::FINISH:
        {
            // TODO: 平均値を取る予定だったが，外乱が少しでも入ると狂うので，現在やめてます．別の方法でやるかも
            //uint16_t RValue = calcAverage<uint16_t>(pastRValues_, 0);
            //uint16_t GValue = calcAverage<uint16_t>(pastGValues_, 0);
            //uint16_t BValue = calcAverage<uint16_t>(pastBValues_, 0);
            //rgb_raw_t aveRGB = {RValue, GValue, BValue};

            // ブロック専用の判定
            // ライントレースしながらの色判定ではないので，黒白の判定を緩めに変更
            // ここに書かないで色検知に持って行ったほうが綺麗かも？
            rgb_raw_t rgb;
            device::hsv_raw_t hsv;
            colorSensor_->updateColor(); // RGB, HSVの更新
            hsv.h = colorSensor_->getH();
            hsv.s = colorSensor_->getS();
            hsv.v = colorSensor_->getV();
            int hueValue = hsv.h;
            if (hsv.v <= 30) {
                ev3_speaker_play_tone(200, 1000);
                *result = COLOR_BLACK;
            } else if (hsv.s <= 90) {
                ev3_speaker_play_tone(500, 5000);
                *result = COLOR_WHITE;
            } else if (hueValue >= 0 && hueValue <= 20) {
                ev3_speaker_play_tone(200, 1000);
                *result = COLOR_RED;
            } else if (hueValue >= 30 && hueValue <= 80) {
                ev3_speaker_play_tone(450, 1000);
                *result = COLOR_YELLOW;
            } else if (hueValue >= 100 && hueValue <= 140) {
                ev3_speaker_play_tone(700, 1000);
                *result = COLOR_GREEN;
            } else if (hueValue >= 150 && hueValue <= 220) {
                ev3_speaker_play_tone(950, 1000);
                *result = COLOR_BLUE;
            } else {
                ev3_speaker_play_tone(1000, 5000);
                *result = COLOR_NONE;
            }

            return true;
        }
        }

        // 一定時間毎に状態を切り替える
        if (timer_.getResult() == true) {
            if (states_.size() > 1)
                states_.pop();
            isInitializedTime_ = false;
        }

        return false;
    }

    // ジェネリクスを C++ で使ってみたかっただけ
    template <class T>
    T BlockColorChecker::calcAverage(const std::vector<T> &vec, T start) {
        for(const auto e : vec) {
            start += e;
        }
        return start/vec.size();
    }
}
