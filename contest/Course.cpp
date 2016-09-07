/**
 * @file Course.cpp
 * @brief コースクラス
 * @aurhor usui kakeru
 */
#include "Course.h"

using namespace strategy;
using namespace measurement;
using namespace std;

namespace contest_pkg{

    /* コンストラクタ */
    Course::Course(SelectedCourse course){
        selfPositionEstimation_ = SelfPositionEstimation::getInstance();
        selfPositionEstimation_->initMap();
        sectionNumber_ = 0;
 
        //それぞれのコースの戦略やライントレースを追加していく
        if(course == SelectedCourse::R_COURSE){
        //ブロック並べ
        sections_.emplace_back(1000,new BlockAreaEntry());
        //sections_.emplace_back(9100,new RCourseStandard());
        }

        if(course == SelectedCourse::L_COURSE){
        sections_.emplace_back(8000,new LCourseStandard());
        //相撲
        // sections_.emplace_back(200,new TestStrategy2());
        }
    }

    /* コース攻略 */
    void Course::capture(){
        if(sections_[sectionNumber_].capture()){
            if(sectionNumber_ + 1 != sections_.size()){
                sectionNumber_++;
            }
        }
    }

}
