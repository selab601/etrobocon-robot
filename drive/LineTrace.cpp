/**
 * @file LineTrace.cpp
 * @brief 右・左エッジでのライントレースとエッジの切り替えを行う(予定)
 * @author kuno
 */

#include "LineTrace.h"

using namespace measurement;
using namespace device;
namespace drive{

    LineTrace* LineTrace::instance_ = 0;
    LineTrace::LineTrace()
    {
        motors_ = Motors::getInstance();
        colorSensor_ = ColorSensor::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        reset();

        setMaxPwm();
        setEdge(LineTraceEdge::RIGHT);
        setPid(LineTracePid::MID);
        setTarget();
    }

    LineTrace* LineTrace::getInstance(){
        if(instance_ == NULL)
        instance_ = new LineTrace();
        return instance_;
    }

    void LineTrace::useHsv(bool useHsv){
        useHsv_ = useHsv;
    }

    void LineTrace::run(int maxPwm,LineTraceEdge edge ,double relativeTarget){
        setTarget(relativeTarget);
        setEdge(edge);
        setMaxPwm(maxPwm);
        run();
    }

    void LineTrace::run(){
        runCurve(0);
    }

    void LineTrace::runCurve(int deltaRad){
        int controllValue = targetValue10_ - colorSensor_->getRelativeBrightness(useHsv_)*10;
        // 生の輝度値を使っていたときのPIDパラメータを使うため応急処置
        controllValue *= 0.7;
        if(edge_ == LineTraceEdge::RIGHT){
            controllValue *= -1;
        }
        pidController_.calculatePwm(1000*pidController_.calculatePid(controllValue) + deltaRad);
        pidController_.run();
    }

    void LineTrace::setMaxPwm(int maxPwm){
        pidController_.setMaxPwm(maxPwm);
        maxPwm_ = maxPwm;
    }

    void LineTrace::setEdge(LineTraceEdge edge){
        edge_ = edge;
    }

    LineTraceEdge LineTrace::getEdge(){
        return edge_;
    }

    void LineTrace::setPid(double kp, double ki, double kd){
        pidController_.setPid(kp, ki, kd);
    }

    void LineTrace::setPid(LineTracePid pidPattern){
        switch (pidPattern){
            case LineTracePid::VERY_FAST:  //! 反応は悪いけど速い(PWM80くらいにするとき)
                setPid(0.0005, 0, 0.1556);
                break;
            case LineTracePid::FAST:       //! 少し反応を良くした(PWM80で一応全部のカーブ曲がれる)
                setPid(0.0006, 0, 0.16);
                break;
            case LineTracePid::MID:        //! 中間 ブレないで安定して走る (PWM30-60くらいのとき)
                setPid(0.0012, 0,0.1);
                break;
            case LineTracePid::SLOW:       //! 安定してゆっくり走る (PWM30-40くらいのとき) 以前のMID
                //setPid(0.0216, 0, 1.32192);
                setPid(0.012, 0, 0.8874);
                break;
            case LineTracePid::RETURN:     //! 反応が極端に良く、ガクガクする(ライン復帰したい時だけ使う)
                setPid(0.02, 0, 2);
                break;
        }
    }


    void LineTrace::setTarget(double relativeTarget){
        if(relativeTarget <= 0.0 || 1.0 <= relativeTarget){
            targetValue10_ = 1000 * DEFAULT_TARGET;
        }
        else{
            targetValue10_ = 1000 * relativeTarget;
        }
    }

    void LineTrace::reset(){
        pidController_.reset();

        // エッジ切り替えの状態のリセット
        edgeChangeStatus_ = LineTraceEdgeChangePhase::ACROSS;
        changeEdge2Phase_ = ChangeEdge2Phase::INIT;
    }

    bool LineTrace::changeEdge(){

        switch(edgeChangeStatus_){

            case LineTraceEdgeChangePhase::ACROSS:
                //エッジの移動
                if(edge_==LineTraceEdge::RIGHT){
                    //現在右エッジなので左エッジに移動
                    motors_->setPWM(device::MOTOR_LEFT, maxPwm_*0.95);
                    motors_->setPWM(device::MOTOR_RIGHT, maxPwm_);
                }
                else {
                    //現在左エッジなので右エッジに移動
                    motors_->setPWM(device::MOTOR_LEFT, maxPwm_);
                    motors_->setPWM(device::MOTOR_RIGHT, maxPwm_*0.95);
                }

                //光の値から今いる場所を確認
                margin_= colorSensor_->getBrightness()*10 - colorSensor_->getBlackCalibratedValue();

                //黒のキャリブ値との差が一定以下(15以下)になったら設定エッジの変更
                if(margin_ < 15){
                    //ev3_speaker_play_tone(500,100);
                    if(edge_==LineTraceEdge::RIGHT){
                        //右エッジ→左エッジ
                        setEdge(LineTraceEdge::LEFT);
                        //切り替えのための傾きを直す
                        //motors_->setPWM(device::MOTOR_LEFT, maxPwm_);
                        //motors_->setPWM(device::MOTOR_RIGHT, maxPwm_*0.8);
                    }
                    else{
                        //左エッジ→右エッジ
                        setEdge(LineTraceEdge::RIGHT);
                        //切り替えのための傾きを直す
                        //motors_->setPWM(device::MOTOR_LEFT, maxPwm_*0.8);
                        //motors_->setPWM(device::MOTOR_RIGHT, maxPwm_);
                    }
                    edgeChangeStatus_ = LineTraceEdgeChangePhase::END;
                }
                return false;


            case LineTraceEdgeChangePhase::END:
                //エッジ切り替え完了
                //フェイズのリセット
                edgeChangeStatus_ = LineTraceEdgeChangePhase::ACROSS;
                return true;

            default:
                return false;
        }
    }

    bool LineTrace::changeEdge2(int toMidLineLength, int toOpposingLength){
        static int preTargetValue = 0;
        static DistanceMeasurement distanceMeasurement = DistanceMeasurement();
        switch(changeEdge2Phase_){
            case ChangeEdge2Phase::INIT:
                preTargetValue = targetValue10_;

                changeEdge2Phase_ = ChangeEdge2Phase::TO_MID_LINE;
                break;

                // 線の中心方向に移動する
            case ChangeEdge2Phase::TO_MID_LINE:
                distanceMeasurement.start(toMidLineLength);
                setTarget(0.15);
                if (distanceMeasurement.getResult()){
                    distanceMeasurement.reset();
                    changeEdge2Phase_ = ChangeEdge2Phase::TO_OPPOSITE;

                    // エッジを逆にする
                    LineTraceEdge nextEdge = LineTraceEdge::LEFT == edge_? LineTraceEdge::RIGHT : LineTraceEdge::LEFT;
                    setEdge(nextEdge);

                    // ライントレース情報の初期化(reset())
                    pidController_.reset();
                }
                break;

                // 線の反対側に移動する
            case ChangeEdge2Phase::TO_OPPOSITE:
                distanceMeasurement.start(toOpposingLength);
                setTarget(0.55);
                if (distanceMeasurement.getResult()){
                    changeEdge2Phase_ = ChangeEdge2Phase::END;
                }
                break;

                // ターゲット値を戻して終了
            case ChangeEdge2Phase::END:
                this->targetValue10_ = preTargetValue; // ターゲット値を戻す
                changeEdge2Phase_ = ChangeEdge2Phase::INIT; // 初期状態に戻す
                return true;
        }
        return false;
    }

};
