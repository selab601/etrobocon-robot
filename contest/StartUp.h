#ifndef START_UP_
#define START_UP_

#include "../device/ColorSensor.h"
#include "../device/TouchSensor.h"
#include "../device/Display.h"
#include "../device/GyroSensor.h"
#include "../drive/StraightRunning.h"
#include "../measurement/TimeMeasurement.h"
#include "../device/Arm.h"
#include "../device/Shippo.h"

namespace contest_pkg{
    class StartUp{
        public:
            /**
             * @brief インスタンスを取得する．
             * @details シングルトンパターンに基づいて，共有のインスタンスを取得する．
             * @return スタートアップクラスのインスタンス
             */
            static StartUp* getInstance();

            /**
             * @brief コース選択とオートキャリブレーションの終了を確認
             * @details オートキャリブレーションのやり方: 黒い線を超えるまで15cm以内の、白いところに置いてタッチセンサを押す
             * @return 終了したらtrue
             *
             */
            bool isFinished();
            /**
             * @brief 選択されたコースを取得する
             * @details Lコース：L, Rコース：R
             *
             * @return 'L', または 'R'
             */
            char getSelectedCourse();

            /**
             * @brief スタートを受け付ける
             * @details タッチセンサが押されるまでまつ
             * @return タッチセンサが押されたらtrue
             */
            bool acceptStart();

        private:

            static StartUp* instance_;	// インスタンス

            //コンストラクタ
            StartUp();

            char selectedCourse_ = 0;	// 選択されたコース (Lコース； 'L', Rコース：'R')

            double currentPwm_= 0;        // 現在のPWM値（加速度つける時に使う）
            int currentTimeMs_ = 0;        // 現在の時刻（加速度つける時に使う）

            // 走行体情報
            device::ColorSensor* brightnessInfo_;
            device::TouchSensor* touch_;
            device::Display* display_;
            device::GyroSensor* gyroSensor_;

            int whiteValue_ = 0;
            int blackValue_ = 0;


            // オートキャリブレーションの状態
            enum class AutoCalibrationState{
                INIT,
                WAIT,
                ADJUST_ARM,
                FORWARD,
                STOP,
                BACK,
                SHOW_RESULT,
                STOP_FURIFURI,
                FINISHED,
            } autoCalibrationState_ = AutoCalibrationState::INIT;

            bool selectCourse();	// コースを選択する

            /**
             * @brief 自動でキャリブレーションする
             *
             * @return キャリブレーションが終了したらtrue
             */
            bool calibrateAutomatically();

            /**
             * @brief タッチセンサがクリックされた
             *
             * @return タッチセンサがクリックされた時true
             */
            bool isClicked();


            /**
             * @brief 加速度をつけてスピードを変える
             *
             * @param currentPwm 現在ののPWM
             * @param targetPwm 目的のPWM
             * @param acceleration 加速度[pwm/s]
             *
             * @return 目的のPWMになった時true
             */
            bool changeSpeed(int currentPwm, int targetPwm, int acceleration);

            /**
             * @brief 加速していき、減速して止まる
             *
             * @param maxPwm 最大のPWM
             * @param acceleration 加速度[pwm/s]
             *
             * @return 終了した時true
             */
            bool runAndStop(int maxPwm, int acceleration);


            /**
             * @brief キャリブレーション値を探す
             * @details 最小値を黒の値、最大値を白の値とし、blackValue_, whiteValue_にセットする
             */
            void findCalibratedValue();

    };
}
#endif
