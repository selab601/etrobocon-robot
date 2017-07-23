#include "Map.h"
#include "math.h"

namespace strategy{

    Map::Map(){
        blockPlaces_[1]  = new BlockPlace(1,BlockAreaColor::RED,-1350*sqrt(3),0);
        blockPlaces_[2]  = new BlockPlace(2,BlockAreaColor::BLUE,-900*sqrt(3),0);
        blockPlaces_[3]  = new BlockPlace(3,BlockAreaColor::YELLOW,-450*sqrt(3),0);
        blockPlaces_[4]  = new BlockPlace(4,BlockAreaColor::BLUE,0,0);
        blockPlaces_[5]  = new BlockPlace(5,BlockAreaColor::YELLOW,-2250*sqrt(3)/2,-450*sqrt(3)/2);
        blockPlaces_[6]  = new BlockPlace(6,BlockAreaColor::GREEN,-1350*sqrt(3)/2,-450/2);
        blockPlaces_[7]  = new BlockPlace(7,BlockAreaColor::RED,-450*sqrt(3)/2,-450/2);
        blockPlaces_[8]  = new BlockPlace(8,BlockAreaColor::RED,-900*sqrt(3),-450);
        blockPlaces_[9]  = new BlockPlace(9,BlockAreaColor::BLUE,-450*sqrt(3),-450);
        blockPlaces_[10] = new BlockPlace(10,BlockAreaColor::GREEN,-450*(5*sqrt(3)-1)/2,-450*(sqrt(3)+1)/2);
        blockPlaces_[11] = new BlockPlace(11,BlockAreaColor::GREEN,-450*(sqrt(3)-1)/2,-450*(sqrt(3)+1)/2);
        blockPlaces_[12] = new BlockPlace(12,BlockAreaColor::BLUE,-450*(4*sqrt(3)+1)/2,-450*(sqrt(3)+2)/2);
        blockPlaces_[13] = new BlockPlace(13,BlockAreaColor::YELLOW,-450*(4*sqrt(3)-1)/2,-450*(sqrt(3)+2)/2);
        blockPlaces_[14] = new BlockPlace(14,BlockAreaColor::RED,-450*(2*sqrt(3)+1)/2,-450*(sqrt(3)+2)/2);
        blockPlaces_[15] = new BlockPlace(15,BlockAreaColor::YELLOW,-450*(2*sqrt(3)-1)/2,-450*(sqrt(3)+2)/2);

        //隣接してる台座の登録
        //left  +
        //right -
        blockPlaces_[1]->next[0]     = blockPlaces_[2];
        blockPlaces_[1]->next[-30]   = blockPlaces_[5];
        blockPlaces_[1]->next[-75]   = blockPlaces_[10];

        blockPlaces_[2]->next[180]   = blockPlaces_[1];
        blockPlaces_[2]->next[0]     = blockPlaces_[3];
        blockPlaces_[2]->next[-30]   = blockPlaces_[6];
        blockPlaces_[2]->next[-150]  = blockPlaces_[5];

        blockPlaces_[3]->next[180]   = blockPlaces_[2];
        blockPlaces_[3]->next[0]     = blockPlaces_[4];
        blockPlaces_[3]->next[-30]   = blockPlaces_[7];
        blockPlaces_[3]->next[-150]  = blockPlaces_[6];

        blockPlaces_[4]->next[180]   = blockPlaces_[3];
        blockPlaces_[4]->next[-105]  = blockPlaces_[11];
        blockPlaces_[4]->next[-150]  = blockPlaces_[7];

        blockPlaces_[5]->next[150]   = blockPlaces_[1];
        blockPlaces_[5]->next[30]    = blockPlaces_[2];
        blockPlaces_[5]->next[-30]   = blockPlaces_[8];
        blockPlaces_[5]->next[-120]  = blockPlaces_[10];

        blockPlaces_[6]->next[150]   = blockPlaces_[2];
        blockPlaces_[6]->next[30]    = blockPlaces_[3];
        blockPlaces_[6]->next[-30]   = blockPlaces_[9];
        blockPlaces_[6]->next[-150]  = blockPlaces_[8];

        blockPlaces_[7]->next[150]   = blockPlaces_[3];
        blockPlaces_[7]->next[30]    = blockPlaces_[4];
        blockPlaces_[7]->next[-60]   = blockPlaces_[11];
        blockPlaces_[7]->next[-150]  = blockPlaces_[9];

        blockPlaces_[8]->next[150]   = blockPlaces_[5];
        blockPlaces_[8]->next[30]    = blockPlaces_[6];
        blockPlaces_[8]->next[-60]   = blockPlaces_[13];
        blockPlaces_[8]->next[-120]  = blockPlaces_[12];

        blockPlaces_[9]->next[150]   = blockPlaces_[6];
        blockPlaces_[9]->next[30]    = blockPlaces_[7];
        blockPlaces_[9]->next[-60]   = blockPlaces_[15];
        blockPlaces_[9]->next[-120]  = blockPlaces_[14];

        blockPlaces_[10]->next[105]  = blockPlaces_[1];
        blockPlaces_[10]->next[60]   = blockPlaces_[5];
        blockPlaces_[10]->next[-30]  = blockPlaces_[12];

        blockPlaces_[11]->next[120]  = blockPlaces_[7];
        blockPlaces_[11]->next[75]   = blockPlaces_[4];
        blockPlaces_[11]->next[-150] = blockPlaces_[15];

        blockPlaces_[12]->next[150]  = blockPlaces_[10];
        blockPlaces_[12]->next[60]   = blockPlaces_[8];
        blockPlaces_[12]->next[0]    = blockPlaces_[13];

        blockPlaces_[13]->next[180]  = blockPlaces_[12];
        blockPlaces_[13]->next[120]  = blockPlaces_[8];
        blockPlaces_[13]->next[0]    = blockPlaces_[14];

        blockPlaces_[14]->next[180]  = blockPlaces_[13];
        blockPlaces_[14]->next[60]   = blockPlaces_[9];
        blockPlaces_[14]->next[0]    = blockPlaces_[15];

        blockPlaces_[15]->next[180]  = blockPlaces_[14];
        blockPlaces_[15]->next[120]  = blockPlaces_[9];
        blockPlaces_[15]->next[30]   = blockPlaces_[11];

        //ブロックの初期位置取得
        blockCode_ = BlockCode::getInstance();
        blockIs_["RED"]    = blockPlaces_[blockCode_->getIdRed()];
        blockIs_["BLUE"]   = blockPlaces_[blockCode_->getIdBlue()];
        blockIs_["GREEN"]  = blockPlaces_[blockCode_->getIdGreen()];
        blockIs_["YELLOW"] = blockPlaces_[blockCode_->getIdYellow()];
        blockIs_["BLACK"]  = blockPlaces_[blockCode_->getIdBlack()];



        //EV3の初期位置は8番置き場
        currentLocation_ = blockPlaces_[10];
        routeBlockPlace_.push_back(currentLocation_);
        routeDigree_.push_back(-120);
        routeHasBlock_.push_back(0);
    }

    bool Map::setBlockOrder(){
        return true;
        //


        // //全部運べなくなったらfalse
        // //なんか運べたらtrue
        // if(flag_red    == false &&
        //    flag_blue   == false &&
        //    flag_green  == false &&
        //    flag_yellow == false &&
        //    flag_black  == false){return false;}
        // else{return true;}
    }



    void Map::makeRoute1(){

        //ブロックの目的地設定
        blockDestination_["RED"]    = blockPlaces_[14];
        blockDestination_["BLUE"]   = blockPlaces_[9];
        blockDestination_["GREEN"]  = blockPlaces_[6];
        blockDestination_["YELLOW"] = blockPlaces_[13];
        blockDestination_["BLACK"]  = blockPlaces_[8];


        BlockPlace* nextCarryBlockIs = NULL; //次に運ぶブロックの場所
        BlockPlace* nextCarryDestination = NULL; //次に運ぶブロックの目的地？
        int minDistance = 100000;//10000は適当　　次に運ぶブロックまでの距離

        int nextDigree;//目的地までの角度


        while(!checkFinish()){

        //マシンの位置から一番近いブロックを１つ選んで
        //ブロックの位置と目的地を確認
        //目的地に運び済みのブロックは運ばない
        for(auto itr = blockIs_.begin(); itr !=blockIs_.end();++itr ){
                if(minDistance > currentLocation_->getDistance(itr->second) && itr->second != blockDestination_[itr->first]){

                    nextCarryBlockIs = itr->second;
                    nextCarryDestination = blockDestination_[itr->first];
                }
        }

        //ブロックの位置まで移動()
        //ブロックの位置までの角度を求める
        nextDigree = currentLocation_->getDigree(nextCarryBlockIs);

        //ブロックの場所に到達するまで
        while(currentLocation_->getNextPlace(nextDigree) != nextCarryBlockIs){
            routeBlockPlace_.push_back(currentLocation_->getNextPlace(nextDigree));//ルートとして登録
            routeDigree_.push_back(currentLocation_->getDigree(currentLocation_->getNextPlace(nextDigree)) );
            routeHasBlock_.push_back(0);
            currentLocation_ = currentLocation_->getNextPlace(nextDigree);//現在地を更新
        }
        //到達したらそこを登録
        routeBlockPlace_.push_back(currentLocation_->getNextPlace(nextDigree));//ルートとして登録
        routeDigree_.push_back(currentLocation_->getDigree(currentLocation_->getNextPlace(nextDigree)) );
        routeHasBlock_.push_back(1);
        currentLocation_ = currentLocation_->getNextPlace(nextDigree);//現在地を更新


       //目的地まで移動()
       //目的地までの角度
        nextDigree = currentLocation_->getDigree(nextCarryDestination);

        //ブロックの目的地に到達するまで
        while(currentLocation_->getNextPlace(nextDigree) != nextCarryDestination){
            routeBlockPlace_.push_back(currentLocation_->getNextPlace(nextDigree));//ルートとして登録
            routeDigree_.push_back(currentLocation_->getDigree(currentLocation_->getNextPlace(nextDigree)) );
            routeHasBlock_.push_back(1);
            currentLocation_ = currentLocation_->getNextPlace(nextDigree);//現在地を更新
        }
        //到達したらそこを登録
        routeBlockPlace_.push_back(currentLocation_->getNextPlace(nextDigree));//ルートとして登録
        routeDigree_.push_back(currentLocation_->getDigree(currentLocation_->getNextPlace(nextDigree)) );
        routeHasBlock_.push_back(0);
        currentLocation_ = currentLocation_->getNextPlace(nextDigree);//現在地を更新

    }
    }




    bool Map::checkFinish(){
        for(auto itr = blockIs_.begin(); itr !=blockIs_.end();++itr ){
             if(itr->second != blockDestination_[itr->first]){return false;}
        }
        return true;
    }





}
