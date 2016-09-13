/**
 * @file LineTrace.cpp
 * @brief 右・左エッジでのライントレースとエッジの切り替えを行う(予定)
 * @author kuno
 */

#include "LineTrace.h"
#include "../device/Display.h"

namespace drive{

    LineTrace* LineTrace::instance_ = 0;
    LineTrace::LineTrace()
    {
        motors_ = device::Motors::getInstance();
        colorSensor_ = device::ColorSensor::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        clock_ = Clock();
        reset();
        blackValue_ = 10 * colorSensor_->getBlackCalibratedValue();
        whiteValue_ = 10 * colorSensor_->getWhiteCalibratedValue();
        setMaxPwm();
        setEdge(LineTraceEdge::RIGHT);
        setPid();
        setTarget();
    }

    LineTrace* LineTrace::getInstance(){
        if(instance_ == NULL)
        instance_ = new LineTrace();
        return instance_;
    }

    void LineTrace::run(int maxPwm,LineTraceEdge edge ,double relativeTarget){
        setTarget(relativeTarget);
        calculatePwm(maxPwm, (int)(calculatePid(colorSensor_->getBrightness(), clock_.now()) * (double)1000) ,edge);
    }

    void LineTrace::run(){
        calculatePwm(maxPwm_, (int)(calculatePid(colorSensor_->getBrightness(), clock_.now()) * (double)1000) ,edge_);
    }

    void LineTrace::setMaxPwm(int maxPwm){
        maxPwm_ = maxPwm;
    }

    void LineTrace::setEdge(LineTraceEdge edge){
        edge_ = edge;
    }

    void LineTrace::setPid(double kp, double ki, double kd){
        kp_ = kp;
        ki_ = ki;
        kd_ = kd;
    }

    double LineTrace::getRateByDeltaRad(int deltaRad){
        return 1000.0F / (double)(LINETRACE_TREAD * deltaRad + 1000);
    }

    void LineTrace::calculatePwm(int maxPwm, int deltaRad ,LineTraceEdge edge){

        int lPwm;
        int rPwm;

        if(edge == LineTraceEdge::RIGHT){
            if (deltaRad < 0 ){
                deltaRad *= -1;
                lPwm = maxPwm;
                rPwm = getRateByDeltaRad(deltaRad) * (double)maxPwm;
            }
            else{
                rPwm = maxPwm;
                lPwm = getRateByDeltaRad(deltaRad) * (double)maxPwm;
            }
        }
        else{/*Edge == LEFT*/
            if (deltaRad < 0 ){
                deltaRad *= -1;
                rPwm = maxPwm;
                lPwm = getRateByDeltaRad(deltaRad) * (double)maxPwm;
            }
            else{
                lPwm = maxPwm;
                rPwm = getRateByDeltaRad(deltaRad) * (double)maxPwm;
            }
        }
        motors_->setPWM(device::MOTOR_LEFT, lPwm);
        motors_->setPWM(device::MOTOR_RIGHT, rPwm);
    }

    double LineTrace::calculatePid(int brightness, int timeMs){
        diff_[1] = diff_[0];
        timeMs_[1] = timeMs_[0];

        diff_[0] = brightness*10 - targetValue_;
        timeMs_[0] = timeMs;

        int timeDiff = timeMs_[1] - timeMs_[0];

        // 積分の計算
        integrated_ += timeDiff * (diff_[1] + diff_[0]) / 2;

        // I、D制御の情報が揃っていない時、P制御の値を返す
        double turn;
        if (usePid_ == false){
            turn = kp_ * (double)diff_[0];
            usePid_ = true;
        }
        else{
            turn = kp_ * (double)diff_[0] +
                   ki_ * (double)integrated_ +
                   kd_ * (double)(diff_[1] - diff_[0]) / (double)timeDiff;
        }
        return turn;
    }

    void LineTrace::setTarget(double relativeTarget){
        if(relativeTarget <= 0.0 || 1.0 <= relativeTarget){
            targetValue_ = blackValue_ + (whiteValue_ - blackValue_) * DEFAULT_TARGET;
        }
        else{
            targetValue_ = blackValue_ + (whiteValue_ - blackValue_) * relativeTarget;
        }
    }

    void LineTrace::reset(){
        usePid_  = false;
        integrated_ = 0;
        diff_[1] = diff_[0] = 0;
        timeMs_[1] = timeMs_[0] = clock_.now();
    }

    bool LineTrace::changeEdge(){

        switch(edgeChangeStatus_){

            case LineTraceEdgeChangePhase::INIT:
                //反対のエッジに移動するための距離を設定
                distanceMeasurement_->setTargetDistance(50);
                distanceMeasurement_->startMeasurement();
                //次のフェイズに
                edgeChangeStatus_ = LineTraceEdgeChangePhase::ACROSS;
                return false;

            case LineTraceEdgeChangePhase::ACROSS:
                if(edge_==LineTraceEdge::RIGHT){
                    //現在右エッジなので左エッジに移動
                    motors_->setPWM(device::MOTOR_LEFT, DEFAULT_MAXPWM*0.7);
                    motors_->setPWM(device::MOTOR_RIGHT, DEFAULT_MAXPWM);
                }
                else {
                    //現在左エッジなので右エッジに移動
                    motors_->setPWM(device::MOTOR_LEFT, DEFAULT_MAXPWM);
                    motors_->setPWM(device::MOTOR_RIGHT, DEFAULT_MAXPWM*0.7);
                }
                //一定距離走ったらエッジを切り替え
                if(distanceMeasurement_->getResult()){
                    if(edge_==LineTraceEdge::RIGHT){
                        //右エッジ→左エッジ
                        setEdge(LineTraceEdge::LEFT);
                    }
                    else{
                        //左エッジ→右エッジ
                        setEdge(LineTraceEdge::RIGHT);
                    }
                    //次のフェイズの準備
                    distanceMeasurement_->setTargetDistance(50);
                    distanceMeasurement_->startMeasurement();
                    //次のフェイズに
                    edgeChangeStatus_ = LineTraceEdgeChangePhase::ADJUST;
                }
                return false;

            case LineTraceEdgeChangePhase::ADJUST:
                //修正用にちょっとライントレース
                run(30,edge_,0.6);
                if(distanceMeasurement_->getResult()){
                    //次のフェイズに
                    edgeChangeStatus_ = LineTraceEdgeChangePhase::END;
                }
                return false;

            case LineTraceEdgeChangePhase::END:
                //エッジ切り替え完了
                //フェイズのリセット
                edgeChangeStatus_ = LineTraceEdgeChangePhase::INIT;
                return true;

            default:
                return false;
        }
    }
};
