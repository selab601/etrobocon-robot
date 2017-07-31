#include "BlockAreaGame.h"

namespace strategy{
  BlockAreaGame::BlockAreaGame(){
    map_ = new Map();
    map_->makeRoute1();//ルート計算した
 }

  bool BlockAreaGame::capture(){
    return map_->runPath();
  }
};
