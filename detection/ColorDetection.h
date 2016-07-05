#ifndef _COLOR_DETECTION_H
#define _COLOR_DETECTION_H

#include "../device/ColorSensor.h"
#include "../device/Display.h"
#include <ev3api.h>

namespace detection{
  class ColorDetection{
  private:
    int colorNumber_;                      //! 現在検知してる色
    int brightnessNumber_;                 //! 現在の輝度
    char message_[100];                    //! ディスプレイ出力用

    device::ColorSensor* colorSensor_;
    device::Display* display_;

  public:
    ColorDetection();

  /**
   * @brief 検知した色を返す
   * @param light 現在の光センサ値
   * @param edge  現在のエッジ
   * @return ターン値
   */
     colorid_t getResult(); // namespace つけたほうがいいかも
  };
}
#endif
