/**
 * @file Contest.cpp
 * @brief 競技クラス
 * @author usui kakeru
 */

#include "Contest.h"


namespace contest_pkg {

    /* メンバ */
    Contest* Contest::instance_ = 0;


    /* コンストラクタ */
    Contest::Contest() {
        startUp_ = StartUp::getInstance();
    }

    /* インスタンス取得 */
    Contest* Contest::getInstance() {
        if (instance_ == 0) {
            instance_ = new Contest();
        }
        return instance_;
    }

    /* 走行体制御 */
    void Contest::perform() {
        static bool initialized = false;

        /*スタートアップ*/
        if ( startUp_->isFinished() ){
            /*選択されたコースのインスタンスを生成する */
            switch(startUp_->getSelectedCourse()){
            case 'L':
                if (!initialized) {
                    courseL_ = new Course(SelectedCourse::L_COURSE);
                    initialized = true;
                }
                break;

            case 'R':
                if (!initialized) {
                    courseR_ = new Course(SelectedCourse::R_COURSE);
                    initialized = true;
                }
                break;
            }

            /* スタート受付 */
            if ( startUp_->acceptStart() ){
                switch (startUp_->getSelectedCourse() ){
                    case 'L':
                    courseL_->capture();
                    break;
                    case 'R':
                    courseR_->capture();
                    break;
                }
            }
        }
    }
}
