/**
 * @file Course.cpp
 * @brief コースクラス
 * @aurhor usui kakeru
 */
#include "Course.h"
#include "strategy/AIRead.h"
#include "strategy/AIAnswer.h"
#include "../strategy/Shippofurifuri.h"

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
        sections_.emplace_back(new RCourseStandard());
        //sections_.emplace_back(new BlockAreaGame());
        sections_.emplace_back(new BlockAreaSimple());
        sections_.emplace_back(new ParkingR());
        // ゴール後にしっぽふりふり
        // タッチセンサ押したら上向けて止める
        sections_.emplace_back(new Shippofurifuri());
        }

        if(course == SelectedCourse::L_COURSE){
        // TODO: ショートカットか普通のか選ぶ (どっちかコメントアウト消す)
        sections_.push_back(new AIAnswer());
        // sections_.emplace_back(new LCourseStandard());  // ラインにそって走る
        // sections_.emplace_back(new AIRead()); //AIアンサー実装したらコメント消してください
        // sections_.emplace_back(new ParkingL());
        // ゴール後にしっぽふりふり
        // タッチセンサ押したら上向けて止める
        //sections_.emplace_back(new Shippofurifuri());
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
