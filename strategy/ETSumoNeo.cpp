#include "ETSumoNeo.h"

using namespace drive;
using namespace detection;
namespace strategy{
    ETSumoNeo::ETSumoNeo(int a){
        linetrace_ = LineTrace::getInstance();
        straightRunning_ = new StraightRunning();
        climbingRunning_ = new ClimbingRunning();
        extrusionRunning_ = new ExtrusionRunning();
        lineDetection_ = new LineDetection();
        a_ = a;
    }

    bool ETSumoNeo::capture(){
        straightRunning_->run(a_);
        return true;
    }
}
