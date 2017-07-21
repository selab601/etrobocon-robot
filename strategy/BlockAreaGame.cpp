#include "BlockAreaGame.h"

namespace strategy{
  BlockAreaGame::BlockAreaGame(){
    map_ = new Map();
    map_->addBlockPlace(BlockPlace(1,BlockAreaColor::RED));
    map_->addBlockPlace(BlockPlace(2,BlockAreaColor::BLUE));
  }

  bool BlockAreaGame::capture(){
    //工事中

    //Todo
    //初期配置もらう
    //ルート計算する(ここでIIteratorでmapを渡す
    //ルート通りにmoveする
    //全部おわったら↓true
    return true;
  }
};
