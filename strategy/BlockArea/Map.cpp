#include "Map.h"
#include "math.h"

using namespace drive;

namespace strategy{

    Map::Map(){

        //ブロック置き場のデータを登録
        blockPlaces_[0]  = new BlockPlace(0,BlockAreaColor::RED,0,120);
        blockPlaces_[1]  = new BlockPlace(1,BlockAreaColor::YELLOW,45,120);
        blockPlaces_[2]  = new BlockPlace(2,BlockAreaColor::BLUE,90,120);
        blockPlaces_[3]  = new BlockPlace(3,BlockAreaColor::GREEN,135,120);
        blockPlaces_[4]  = new BlockPlace(4,BlockAreaColor::BLUE,0,80);
        blockPlaces_[5]  = new BlockPlace(5,BlockAreaColor::GREEN,45,80);
        blockPlaces_[6]  = new BlockPlace(6,BlockAreaColor::RED,90,80);
        blockPlaces_[7]  = new BlockPlace(7,BlockAreaColor::YELLOW,135,80);
        blockPlaces_[8]  = new BlockPlace(8,BlockAreaColor::RED,0,40);
        blockPlaces_[9]  = new BlockPlace(9,BlockAreaColor::YELLOW,45,40);
        blockPlaces_[10] = new BlockPlace(10,BlockAreaColor::BLUE,90,40);
        blockPlaces_[11] = new BlockPlace(11,BlockAreaColor::GREEN,135,40);
        blockPlaces_[12] = new BlockPlace(12,BlockAreaColor::BLUE,0,0);
        blockPlaces_[13] = new BlockPlace(13,BlockAreaColor::GREEN,45,0);
        blockPlaces_[14] = new BlockPlace(14,BlockAreaColor::RED,90,0);
        blockPlaces_[15] = new BlockPlace(15,BlockAreaColor::YELLOW,135,0);

        //ブロックエリア8に侵入する際のルートをつくるための応急措置,8の左側にあると仮定します
        blockPlaces_[16] = new BlockPlace(16,BlockAreaColor::BLACK,-1,40);

        //隣接してる台座の登録
        //left  +
        //right -
        blockPlaces_[0]->next[0]     = blockPlaces_[1];
        blockPlaces_[0]->next[-90]   = blockPlaces_[4];

        blockPlaces_[1]->next[180]   = blockPlaces_[0];
        blockPlaces_[1]->next[0]     = blockPlaces_[2];
        blockPlaces_[1]->next[-90]   = blockPlaces_[5];

        blockPlaces_[2]->next[180]   = blockPlaces_[1];
        blockPlaces_[2]->next[0]     = blockPlaces_[3];
        blockPlaces_[2]->next[-90]   = blockPlaces_[6];

        blockPlaces_[3]->next[180]   = blockPlaces_[2];
        blockPlaces_[3]->next[-90]   = blockPlaces_[7];

        blockPlaces_[4]->next[90]    = blockPlaces_[0];
        blockPlaces_[4]->next[0]     = blockPlaces_[5];
        blockPlaces_[4]->next[-90]   = blockPlaces_[8];

        blockPlaces_[5]->next[90]    = blockPlaces_[1];
        blockPlaces_[5]->next[180]   = blockPlaces_[4];
        blockPlaces_[5]->next[0]     = blockPlaces_[6];
        blockPlaces_[5]->next[-90]   = blockPlaces_[9];

        blockPlaces_[6]->next[90]    = blockPlaces_[2];
        blockPlaces_[6]->next[180]   = blockPlaces_[5];
        blockPlaces_[6]->next[0]     = blockPlaces_[7];
        blockPlaces_[6]->next[-90]   = blockPlaces_[10];

        blockPlaces_[7]->next[90]    = blockPlaces_[3];
        blockPlaces_[7]->next[180]   = blockPlaces_[6];
        blockPlaces_[7]->next[-90]   = blockPlaces_[11];

        blockPlaces_[8]->next[90]    = blockPlaces_[4];
        blockPlaces_[8]->next[0]     = blockPlaces_[9];
        blockPlaces_[8]->next[-90]   = blockPlaces_[12];

        blockPlaces_[9]->next[90]    = blockPlaces_[5];
        blockPlaces_[9]->next[180]   = blockPlaces_[8];
        blockPlaces_[9]->next[0]     = blockPlaces_[10];
        blockPlaces_[9]->next[-90]   = blockPlaces_[13];

        blockPlaces_[10]->next[90]   = blockPlaces_[6];
        blockPlaces_[10]->next[180]  = blockPlaces_[9];
        blockPlaces_[10]->next[0]    = blockPlaces_[11];
        blockPlaces_[10]->next[-90]  = blockPlaces_[14];

        blockPlaces_[11]->next[90]   = blockPlaces_[7];
        blockPlaces_[11]->next[180]  = blockPlaces_[10];
        blockPlaces_[11]->next[-90]  = blockPlaces_[15];

        blockPlaces_[12]->next[90]   = blockPlaces_[8];
        blockPlaces_[12]->next[0]    = blockPlaces_[13];

        blockPlaces_[13]->next[90]   = blockPlaces_[9];
        blockPlaces_[13]->next[180]  = blockPlaces_[12];
        blockPlaces_[13]->next[0]    = blockPlaces_[14];

        blockPlaces_[14]->next[90]   = blockPlaces_[10];
        blockPlaces_[14]->next[180]  = blockPlaces_[13];
        blockPlaces_[14]->next[0]    = blockPlaces_[15];

        blockPlaces_[15]->next[90]   = blockPlaces_[11];
        blockPlaces_[15]->next[180]  = blockPlaces_[14];

        //応急措置
        blockPlaces_[16]->next[0]  = blockPlaces_[8];



        //ブロックの初期位置取得
        blockCode_ = BlockCode::getInstance();
        blockIs_["BLOCK1"] = blockPlaces_[blockCode_->getIdBlock1()];
        blockIs_["BLOCK2"] = blockPlaces_[blockCode_->getIdBlock2()];
        blockIs_["BLOCK3"] = blockPlaces_[blockCode_->getIdBlock3()];
        blockIs_["BLOCK4"] = blockPlaces_[blockCode_->getIdBlock4()];
        blockIs_["POWER BLOCK"]  = blockPlaces_[blockCode_->getIdPowerBlockCode()];



        //EV3の初期位置は8番置き場（にしたいのですが，応急的に17番目のスポットを8の左に仮定します）
        ev3Is_ = blockPlaces_[16];
        //ev3Is_ = blockPlaces_[8];
        ev3HasBlock_ = false;

        catching_ = new Catching();
        avoidance_ = new Avoidance();

        //デバック用
        //sprintf(message, "%d",blockIs_["BLOCK1"]->getId());
        //communication::BtManager::getInstance()->setMessage(message);
        //communication::BtManager::getInstance()->send();
    }




    void Map::makeRoute(){

        /*
        //ブロックの目的地設定
        blockDestination_["BLOCK1"] = blockPlaces_[5];
        blockDestination_["BLOCK2"] = blockPlaces_[6];
        blockDestination_["BLOCK3"] = blockPlaces_[9];
        blockDestination_["BLOCK4"] = blockPlaces_[10];

        
        //ブロックの目的地に近い場所
        blockDisplace_["RED"]    = blockPlaces_[15];
        blockDisplace_["BLUE"]   = blockPlaces_[7];
        blockDisplace_["GREEN"]  = blockPlaces_[3];
        blockDisplace_["YELLOW"] = blockPlaces_[12];
        

        //黒ブロックのいい感じの置き場を確認
        selectBlackDestination();
        

        //10番ブロック置き場にブロックがあれば必ず始めに運ぶ
        if(checkBlock(blockPlaces_[10])){
            makeDodgeAvoidancePath();
        }
        

        makePath(blockPlaces_[11]);

        while(!checkFinish()){

            
            //５角形が埋まってたら５角形の置き場からずらす
            if(checkPentagon()){
                makeDisplaceBlockPath();
            }
            

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

        //ブロック並べエリアから退出するためのルート算出
        makePath(blockPlaces_[7]);
        */
        makePath(blockPlaces_[11]);
        //終了に行動パターンを変更
        routeMovePattern_[routeMovePattern_.size()] = MovePattern::END;
    }




    bool Map::checkFinish(){
        for(auto itr = blockIs_.begin(); itr !=blockIs_.end();++itr ){
             if(blockIs_[itr->first] != blockDestination_[itr->first]){return false;}
        }
        return true;
    }

    bool Map::checkPentagon(){
        //１つでも５角形以外の場所にブロックがあればfalse
        bool checker = false;
        for(auto is = blockIs_.begin(); is !=blockIs_.end();++is ){
            for(auto dest = blockDestination_.begin(); dest !=blockDestination_.end();++dest ){
                if(blockIs_[is->first]->getId() == blockDestination_[dest->first]->getId() ){checker = true;}
            }
            if(checker){checker=false;}
            else{return false;}
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

        int minDistance = 100000;//10000は適当 ev3から次に運ぶブロックまでの距離

        for(auto itr = blockIs_.begin(); itr != blockIs_.end();++itr ){
                //【条件】 ev3の現在地から一番近い && 目的地に着いてない && 目的地に他のブロックが置いてない
                if( (minDistance > ev3Is_->getDistance(blockIs_[itr->first]) ) && (blockIs_[itr->first] != blockDestination_[itr->first]) && !(checkBlock(blockDestination_[itr->first])) ){
                    minDistance           = ev3Is_->getDistance(blockIs_[itr->first]);  //一番近いブロックの距離に更新
                    nextCarryBlockColor_  = itr->first; //次に運ぶブロックの色
                }
        }

    }

    void Map::selectDisplaceBlock(){

        int minDistance = 100000;//10000は適当 ev3から次に運ぶブロックまでの距離

        for(auto itr = blockIs_.begin(); itr != blockIs_.end();++itr ){
                //【条件】 ev3の現在地から一番近い && 目的地に着いてない
                if( (minDistance > ev3Is_->getDistance(blockIs_[itr->first]) ) && (blockIs_[itr->first] != blockDestination_[itr->first])  ){
                    minDistance           = ev3Is_->getDistance(blockIs_[itr->first]);  //一番近いブロックの距離に更新
                    nextCarryBlockColor_  = itr->first; //次に運ぶブロックの色
                }
        }

    }

    void Map::selectBlackDestination(){
        //8番置き場までより,14番置き場までの方が近かったらそっちに置くように修正
        if(blockIs_["BLACK"]->getDistance(blockPlaces_[8]) > blockIs_["BLACK"]->getDistance(blockPlaces_[14]) ){
            blockDestination_["BLACK"] = blockPlaces_[14];
            blockDestination_["RED"] = blockPlaces_[8];
            blockDisplace_["RED"]    = blockPlaces_[5];
            blockDisplace_["BLACK"]  = blockPlaces_[15];
        }

    }


    void Map::makePath(BlockPlace* goal){

        int goalDegree;//ev3の現在地(ブロック置き場)からゴール(ブロック置き場)までの角度
        BlockPlace* candidatePlace = ev3Is_;//次の置き場候補
        while(candidatePlace->getId() != goal->getId()){

            //1個前と同じ置き場だったらとぱす(応急処置)
            //if( (!routeBlockPlace_.empty()) && routeBlockPlace_.back()->getId() == candidatePlace->getId() ){
                //とばす
            //}
            //else{
                routeBlockPlace_.push_back(candidatePlace);//pathに追加
                sprintf(message, "goaldegree:%d",ev3Is_->getDegree(goal));
                communication::BtManager::getInstance()->setMessage(message);
                communication::BtManager::getInstance()->send();
                if(checkBlock(candidatePlace)){ //ブロックがあったら避ける
                    routeMovePattern_.push_back(MovePattern::AVOID);
                }
                else{//ブロックがなかったら避けない
                    if(ev3HasBlock_){routeMovePattern_.push_back(MovePattern::CATCH);}
                    else{routeMovePattern_.push_back(MovePattern::PASS);}
                }
            //}
            ev3Is_ = candidatePlace;//位置更新
            goalDegree = ev3Is_->getDegree(goal);//角度更新
            candidatePlace = ev3Is_->getNextPlace(goalDegree);//次の置き場を聞く
        }
        routeBlockPlace_.push_back(candidatePlace);//pathに追加 ゴール
        //sprintf(message, "%d->",candidatePlace->getId());
        //communication::BtManager::getInstance()->setMessage(message);
        //communication::BtManager::getInstance()->send();

        if(ev3HasBlock_){routeMovePattern_.push_back(MovePattern::PUT);}//目的地に着いたのでブロックを置く
        else{routeMovePattern_.push_back(MovePattern::CATCH);}//目的地に着いたのでブロックを取る
        ev3Is_ = candidatePlace;//位置更新
    }

    void Map::makeDisplaceBlockPath(){

        //有効移動してないブロックを１つ選ぶ
        selectDisplaceBlock();

        //ブロックの位置まで移動()
        makePath(blockIs_[nextCarryBlockColor_]);
        ev3HasBlock_ = true;

        //目的地まで移動()
        makePath(blockDisplace_[nextCarryBlockColor_]);
        ev3HasBlock_ = false;

        //運んだのでブロックの位置を更新
        blockIs_[nextCarryBlockColor_] =  blockDisplace_[nextCarryBlockColor_];
    }

    void Map::makeDodgeAvoidancePath(){
        //運ぶブロックの色を取得
        selectDisplaceBlock();
        //10番から運び始めることを設定
        makePath(blockIs_[nextCarryBlockColor_]);
        ev3HasBlock_ = true;

        //目的地まで移動 → 運んだのでブロックの位置を更新
        //優先度.1 目的地に邪魔なブロックがなければ運ぶ
        //優先度.2 目的地のそば
        //優先度.3 とりあえず近い12番置き場
        //優先度.4 とりあえず近い5番置き場
        //優先度.5 とりあえず近い2番置き場
        if(!checkBlock(blockDestination_[nextCarryBlockColor_]) ){
            makePath(blockDestination_[nextCarryBlockColor_]);
            blockIs_[nextCarryBlockColor_] = blockDestination_[nextCarryBlockColor_];
        }
        else if(!checkBlock(blockDisplace_[nextCarryBlockColor_]) ){
            makePath(blockDisplace_[nextCarryBlockColor_]);
            blockIs_[nextCarryBlockColor_] = blockDisplace_[nextCarryBlockColor_];
        }
        else if(!checkBlock(blockPlaces_[12]) ){
            makePath(blockPlaces_[12]);
            blockIs_[nextCarryBlockColor_] = blockPlaces_[12];
        }
        else if(!checkBlock(blockPlaces_[5]) ){
            makePath(blockPlaces_[5]);
            blockIs_[nextCarryBlockColor_] = blockPlaces_[5];
        }
        else{
            makePath(blockPlaces_[2]);
            blockIs_[nextCarryBlockColor_] = blockPlaces_[2];
        }
        ev3HasBlock_ = false;

    }


    bool Map::runPath(){
        static unsigned int patternNumber = 0;//実行中のpathのNo. routeBlockPlace_ と routeMovePattern_ は対応してるのでこれで一括管理
        int preDistance  = 0;
        int nextDistance = 0;
        int degreeForRun = 0;
        calculated_  = false;
        //pathを順に見てく
        if(patternNumber < routeMovePattern_.size()-1){

            //角度と距離の計算(4mごとに計算しないようにFlag管理)
            if(!calculated_){
                //patternNumber = 0 のときはCATCH エリア進入時には前の台座は存在しないので定数で角度を計算
                if(patternNumber == 0){
                degreeForRun = routeBlockPlace_[patternNumber]->getDegree(routeBlockPlace_[patternNumber+1]) - ev3DegreeAtEntry_;
                }
                else{
                degreeForRun = routeBlockPlace_[patternNumber]->getDegree(routeBlockPlace_[patternNumber+1]) - routeBlockPlace_[patternNumber-1]->getDegree(routeBlockPlace_[patternNumber]);
                preDistance  = routeBlockPlace_[patternNumber-1]->getDistance(routeBlockPlace_[patternNumber]);
                }
                nextDistance = routeBlockPlace_[patternNumber]->getDistance(routeBlockPlace_[patternNumber+1]);

                //degreeForRun の結果が-270度とか出すのでdrive::Catchingがエラー出さないように調整
                while(degreeForRun<0){
                degreeForRun += 360;
                }
                degreeForRun += 180;
                degreeForRun %= 360;
                degreeForRun -= 180;
                calculated_ = true;
            }

            //その置き場での行動パターンを確認
            switch(routeMovePattern_[patternNumber]){
                case MovePattern::CATCH:
                            //計算した角度でcatching(ブロックを持っている)
                            ev3HasBlock_ = true;
                            catching_->hasBlock(ev3HasBlock_);
                            if(catching_->run(nextDistance,degreeForRun)){
                                patternNumber++;
                                calculated_ = false;
                            }
                            break;
                case MovePattern::PASS:
                            //計算した角度でcatching(ブロックを持ってない)
                            catching_->hasBlock(ev3HasBlock_);
                            if(catching_->run(nextDistance,degreeForRun)){
                                patternNumber++;
                                calculated_ = false;
                            }
                            break;
                case MovePattern::AVOID:
                            //計算した角度でavoid
                            avoidance_->hasBlock(ev3HasBlock_);
                            if(avoidance_->runTo(preDistance,nextDistance,degreeForRun)){
                                patternNumber++;
                                calculated_ = false;
                            }
                            break;
                case MovePattern::PUT:
                            //PUTの時はAVOIDもしないと次の置き場に移動できてないのでputProcess_でPUT動作を管理
                            ev3HasBlock_ = false;
                            switch(putProcess_){
                                case PutProcess::PUT:
                                        if(catching_->putBlock(preDistance)){
                                            putProcess_ = PutProcess::AVOID; //次のputProcess_へ
                                        }
                                        break;
                                case PutProcess::AVOID:
                                        if(avoidance_->runTo(preDistance,nextDistance,degreeForRun)){
                                             putProcess_ = PutProcess::END; //次のputProcessへ
                                        }
                                     break;
                                case PutProcess::END:
                                        putProcess_ = PutProcess::PUT;
                                        patternNumber++;
                                        calculated_ = false;
                                     break;
                            }
                            break;
                case MovePattern::END:
                            //最後は何もしない
                            break;
            }//end switch
            return false;
        }//end if
        else{return true;}//end runPath()

    }
}
