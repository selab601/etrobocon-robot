#include "LCourseShortcut.h"

using namespace drive;
using namespace measurement;
using namespace device;

namespace strategy{
    LCourseShortcut::LCourseShortcut(){
        linetrace_ = LineTrace::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        bodyAngleMeasurement_ = BodyAngleMeasurement();

        strategySuccess_ = false;
        hasExecutedPhase_ = false;
        isLineTraceReset_ = false;
    }

    bool LCourseShortcut::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            if(executePhase(phaseProcedure_[procedureNumber])){
                procedureNumber++;
                hasExecutedPhase_ = false;
                ev3_speaker_set_volume(6);
                ev3_speaker_play_tone(500,100);
            }
        }
        if(procedureNumber == phaseProcedure_.size()){
            strategySuccess_ = true;
        }
        return strategySuccess_;
    }

    bool LCourseShortcut::executePhase(Phase phase){
        switch(phase){
            case Phase::INIT:
                linetrace_->setEdge(LineTraceEdge::LEFT);
                linetrace_->setPid(LineTracePid::VERY_FAST);
                return true;

            case Phase::LINETRACE_STRAIGHT:
                distanceMeasurement_->start(2150);
                linetrace_->setPid(LineTracePid::VERY_FAST);
                linetrace_->setMaxPwm(100);
                linetrace_->run();

                if (distanceMeasurement_->getResult()){
                    distanceMeasurement_->reset();
                    return true;
                }
                return false;

            case Phase::LINETRACE_RIGHT:
                distanceMeasurement_->start(1600 + 100);
                linetrace_->setPid(LineTracePid::FAST);
                linetrace_->setMaxPwm(80);
                linetrace_->runCurve(-800);

                if (distanceMeasurement_->getResult()){
                    distanceMeasurement_->reset();
                    return true;
                }
                return false;

            case Phase::LINETRACE_LEFT:
                distanceMeasurement_->start(1170 - 50);
                linetrace_->setPid(LineTracePid::VERY_FAST);
                linetrace_->setMaxPwm(80);
                linetrace_->run();

                if (distanceMeasurement_->getResult()){
                    distanceMeasurement_->reset();
                    // 一時的にまっすぐになってるとこの真ん中あたりにいるはず
                    bodyAngleMeasurement_.setBaseAngle();
                    return true;
                }
                return false;

            case Phase::LINETRACE_TO_GATE2:
                distanceMeasurement_->start(1100);
                linetrace_->setPid(LineTracePid::FAST);
                linetrace_->setMaxPwm(80);
                linetrace_->run();

                if (distanceMeasurement_->getResult()){
                    distanceMeasurement_->reset();
                    return true;
                }
                return false;

            case Phase::POLAR_TO_GOAL_FROM_GATE2:
                polar_.setMaxPwm(80);
                polar_.centerPivot(false);
                if (polar_.runTo(3350, 1107 - bodyAngleMeasurement_.getRelative10(), 0)){
                    return true;
                }
                return false;

            case Phase::TO_LINE:
                polar_.setMaxPwm(40);
                polar_.centerPivot(true);
                if (polar_.runTo(600, 450 - bodyAngleMeasurement_.getRelative10())){
                    return true;
                }
                if (isBlack()){
                    polar_.reset();
                    return true;
                }
                return false;

            case Phase::LINE_RETURN:
                distanceMeasurement_->start(150);
                linetrace_->setPid(LineTracePid::RETURN);
                linetrace_->setEdge(LineTraceEdge::RIGHT);
                linetrace_->setMaxPwm(30);
                linetrace_->run();

                if (distanceMeasurement_->getResult()){
                    distanceMeasurement_->reset();
                    return true;
                }
                return false;

            case Phase::LINETRACE_TO_BLOCKAREA:
                distanceMeasurement_->start(700);
                linetrace_->setPid(LineTracePid::SLOW);
                linetrace_->setMaxPwm(40);
                linetrace_->run();

                if (distanceMeasurement_->getResult()){
                    distanceMeasurement_->reset();
                    return true;
                }
                return false;

            case Phase::STOP:
                linetrace_->setMaxPwm(0);
                linetrace_->runCurve(800);
                return false;

            default: return false;
        }

    }

    bool LCourseShortcut::isBlack(){
        ColorSensor* colorSensor = ColorSensor::getInstance();
        int black = colorSensor->getBlackCalibratedValue();
        int white = colorSensor->getWhiteCalibratedValue();
        int threshold = black + (white - black) / 3;
        return colorSensor->getBrightness() < threshold;
    }
};
