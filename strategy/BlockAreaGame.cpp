#include "BlockAreaGame.h"

namespace strategy{
  BlockAreaGame::BlockAreaGame(){
    map_ = new Map();
    map_->makeRoute1();//ルート計算した
    map_->getrouteBlockPlace();//ルート取得


 }

  bool BlockAreaGame::capture(){
    //工事中

    //Todo
    //ルート通りにmoveする
    //全部おわったら↓true
    return true;
  }
};
