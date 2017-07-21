#include "BlockAreaGame.h"

namespace strategy{
  BlockAreaGame::BlockAreaGame(){
    map_ = new Map();
    map_->addBlockPlace(BlockPlace(1,BlockAreaColor::RED,0,0));
    map_->addBlockPlace(BlockPlace(2,BlockAreaColor::BLUE,0,0));
    map_->addBlockPlace(BlockPlace(3,BlockAreaColor::YELLOW,0,0));
    map_->addBlockPlace(BlockPlace(4,BlockAreaColor::BLUE,0,0));
    map_->addBlockPlace(BlockPlace(5,BlockAreaColor::YELLOW,0,0));
    map_->addBlockPlace(BlockPlace(6,BlockAreaColor::GREEN,0,0));
    map_->addBlockPlace(BlockPlace(7,BlockAreaColor::RED,0,0));
    map_->addBlockPlace(BlockPlace(8,BlockAreaColor::RED,0,0));
    map_->addBlockPlace(BlockPlace(9,BlockAreaColor::BLUE,0,0));
    map_->addBlockPlace(BlockPlace(10,BlockAreaColor::GREEN,0,0));
    map_->addBlockPlace(BlockPlace(11,BlockAreaColor::GREEN,0,0));
    map_->addBlockPlace(BlockPlace(12,BlockAreaColor::BLUE,0,0));
    map_->addBlockPlace(BlockPlace(13,BlockAreaColor::YELLOW,0,0));
    map_->addBlockPlace(BlockPlace(14,BlockAreaColor::RED,0,0));
    map_->addBlockPlace(BlockPlace(15,BlockAreaColor::YELLOW,0,0));


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
