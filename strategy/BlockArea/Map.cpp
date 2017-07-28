#include "Map.h"
#include "math.h"
//#include "../../communication/BtManager.h" //デバック用

using namespace drive;

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



        //EV3の初期位置は10番置き場
        ev3Is_ = blockPlaces_[10];
        //routeBlockPlace_.push_back(ev3Is_);
        ev3HasBlock_ = false;

        catching_ = new Catching();
        avoidance_ = new Avoidance();

        //デバック用
        // char message[50];
        // sprintf(message, "%d",blockIs_["RED"]->getId());
        // communication::BtManager::getInstance()->setMessage(message);
        // communication::BtManager::getInstance()->send();
    }




    void Map::makeRoute1(){

        //とりあえず通るブロック置き場の羅列ができるようにする
        //getrouteBlockPlace() で確認

        //ブロックの目的地設定
        blockDestination_["RED"]    = blockPlaces_[14];
        blockDestination_["BLUE"]   = blockPlaces_[9];
        blockDestination_["GREEN"]  = blockPlaces_[6];
        blockDestination_["YELLOW"] = blockPlaces_[13];
        blockDestination_["BLACK"]  = blockPlaces_[8];

        while(!checkFinish()){

            //次に運ぶブロックを選択
            selectCarryBlock();

            //ブロックの位置まで移動()
            makePath(blockIs_[nextCarryBlockColor_]);
            ev3HasBlock_ = true;

            //目的地まで移動()
            makePath(blockDestination_[nextCarryBlockColor_]);
            ev3HasBlock_ = false;

            //運んだのでブロックの位置を更新
            blockIs_[nextCarryBlockColor_] =  blockDestination_[nextCarryBlockColor_];

        }
    }




    bool Map::checkFinish(){
        for(auto itr = blockIs_.begin(); itr !=blockIs_.end();++itr ){
             if(blockIs_[itr->first] != blockDestination_[itr->first]){return false;}
        }
        return true;
    }

    bool Map::checkBlock(BlockPlace* checkPlace){
        for(auto itr = blockIs_.begin(); itr !=blockIs_.end();++itr ){
           if(checkPlace->getId() == blockIs_[itr->first]->getId()){return true;}
        }
        return false;
    }

    void Map::selectCarryBlock(){

        int minDistance = 100000;//10000は適当　　次に運ぶブロックまでの距離
        //マシンの位置から一番近いブロックを１つ選んで
        //ブロックの位置と目的地を確認
        //目的地に運び済みのブロックは運ばない
        for(auto itr = blockIs_.begin(); itr != blockIs_.end();++itr ){
                if( (minDistance > ev3Is_->getDistance(blockIs_[itr->first]) ) && (blockIs_[itr->first] != blockDestination_[itr->first]) ){
                    minDistance           = ev3Is_->getDistance(blockIs_[itr->first]);  //一番近いブロックの距離に更新
                    nextCarryBlockColor_  = itr->first; //次に運ぶブロックの色
                }
        }


    }



    void Map::makePath(BlockPlace* goal){
        int goalDegree;//ev3の現在地(ブロック置き場)からゴール(ブロック置き場)までの角度
        BlockPlace* candidatePlace = ev3Is_;//次の置き場候補
        while(candidatePlace->getId() != goal->getId()){
            routeBlockPlace_.push_back(candidatePlace);//pathに追加
            if(checkBlock(candidatePlace)){routeMovePattern_.push_back(MovePattern::AVOID);}//ブロックがあったら避ける
            else{routeMovePattern_.push_back(MovePattern::CATCH);}//ブロックがなかったら避けない
            ev3Is_ = candidatePlace;//位置更新
            goalDegree = ev3Is_->getDegree(goal);//角度更新
            candidatePlace = ev3Is_->getNextPlace(goalDegree);//次の置き場を聞く
        }
        routeBlockPlace_.push_back(candidatePlace);//pathに追加 ゴール
        if(ev3HasBlock_){routeMovePattern_.push_back(MovePattern::PUT);}//目的地に着いたのでブロックを置く
        else{routeMovePattern_.push_back(MovePattern::CATCH);}//目的地に着いたのでブロックを置く
        ev3Is_ = candidatePlace;//位置更新

    }


    bool Map::runPath(){
        static unsigned int patternNumber = 0;//実行中のpathのNo. routeBlockPlace_ と routeMovePattern_ は対応してるのでこれで一括管理
        int degreeForCatching;
        //pathを順に見てく
        if(patternNumber < routeMovePattern_.size()){

            //その置き場での行動パターンを確認
            switch(routeMovePattern_[patternNumber]){
                case MovePattern::CATCH:
                            //patternNumber = 0 のときはCATCH　エリア進入時には前の台座は存在しないので定数で角度を計算
                            if(patternNumber == 0){degreeForCatching = routeBlockPlace_[patternNumber]->getDegree(routeBlockPlace_[patternNumber+1]) - ev3DegreeAtEntry_;}
                            else{degreeForCatching = routeBlockPlace_[patternNumber]->getDegree(routeBlockPlace_[patternNumber+1]) - routeBlockPlace_[patternNumber-1]->getDegree(routeBlockPlace_[patternNumber]);}

                            if(catching_->run(routeBlockPlace_[patternNumber]->getDistance(routeBlockPlace_[patternNumber+1]),degreeForCatching)){
                                patternNumber++;
                            }
                            break;
                case MovePattern::AVOID:
                            if(avoidance_->runTo( routeBlockPlace_[patternNumber-1]->getDistance(routeBlockPlace_[patternNumber]),
                                                 routeBlockPlace_[patternNumber]->getDistance(routeBlockPlace_[patternNumber+1]),
                                                 routeBlockPlace_[patternNumber]->getDegree(routeBlockPlace_[patternNumber+1]) - routeBlockPlace_[patternNumber-1]->getDegree(routeBlockPlace_[patternNumber]) ))
                            {
                                patternNumber++;
                            }
                            break;
                case MovePattern::PUT:
                            //PUTの時はAVOIDもしないと次の置き場に移動できてない　のでputProcess_でPUT動作を管理
                            switch(putProcess_){
                                case PutProcess::PUT:
                                        if(catching_->putBlock() ){
                                            putProcess_ = PutProcess::AVOID; //次のputProcess_へ
                                        }
                                        break;
                                case PutProcess::AVOID:
                                        if(avoidance_->runTo(routeBlockPlace_[patternNumber-1]->getDistance(routeBlockPlace_[patternNumber]),
                                                             routeBlockPlace_[patternNumber]->getDistance(routeBlockPlace_[patternNumber+1]),
                                                             routeBlockPlace_[patternNumber]->getDegree(routeBlockPlace_[patternNumber+1]) - routeBlockPlace_[patternNumber-1]->getDegree(routeBlockPlace_[patternNumber]) ))
                                        {
                                             putProcess_ = PutProcess::END; //次のputProcessへ
                                        }
                                     break;
                                case PutProcess::END:
                                     break;
                            }

                            if(putProcess_ == PutProcess::END){
                                putProcess_ = PutProcess::PUT;
                                patternNumber++;

                            }
                            break;

            }//end switch


            return false;
        }//end if
        else{return true;}

    }




}
