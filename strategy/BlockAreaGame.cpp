#include "BlockAreaGame.h"
#include "BlockArea/BlockPlace.h"

namespace strategy{
  BlockAreaGame::BlockAreaGame(){
    //itertor_ = map.iterator();
    map_ = new Map();
    //map_->addBlockPlace(new BlockPlace(1,BlockPlace::RED));
    //map_->addBlockPlace(new BlockPlace(2,BlockPlace::BLUE));
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
