#include "RightAngledDetection.h"
#include "stdio.h"


namespace detection{
    RightAngledDetection::RightAngledDetection(){
        selfPos = measurement::SelfPositionEstimation::getInstance();
        color = device::ColorSensor::getInstance();
        counter = 0;

        for (int i = 0; i  < RAD_DATA_SIZE; i++){
            brightnessHistory[i] = 0;
            distanceHistory[i] = 0;
        }

    }
  bool RightAngledDetection::isDetected(){
    return isDetected(CHANGE_RATE);
  }

  bool RightAngledDetection::isDetected(float minChnageRate){
        bool isDetected = false;

        if (counter < RAD_DATA_SIZE)
            counter++;

        /* 情報を更新する */
        for (int i = RAD_DATA_SIZE - 1;  0 < i ;  i--){
            brightnessHistory[i] = brightnessHistory[i - 1];
            distanceHistory[i] = distanceHistory[i - 1];
        }
        brightnessHistory[0] = color->getBrightness();
        distanceHistory[0] = localization->get_migrationLength();


        char str[128] = "";
        sprintf (str, "count: %d, brigh: %d, dst: %ld\r\n", counter, brightnessHistory[0], distanceHistory[0]);
        sendMessage(str);


       for (int start = 1;  start < counter;  start++){
           int8_t  brightnessChanges = brightnessHistory[0] - brightnessHistory[start];
            brightnessChanges = brightnessChanges < 0? -brightnessChanges: brightnessChanges;

           long distanceChanges = distanceHistory[0] - distanceHistory[start];
           if (distanceChanges <= 3) continue;

           float changeRate = (float)brightnessChanges / (float)distanceChanges;
           if  ( changeRate >= minChnageRate ){
                isDetected = true;
                // sprintf (str, "start: %d, c_br: %d, c_dst: %ld\r\nbr:%d, dst:%ld \r\n[detected]\r\n", start, brightnessChanges, distanceChanges, brightnessHistory[0], distanceHistory[0]);
                // sendMessage(str);

                break;
           }
       }
       return isDetected;
    }
}
