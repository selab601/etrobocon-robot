#include "BlockAreaGame.h"


namespace strategy{
  BlockAreaGame::BlockAreaGame(){
    linetrace_ = drive::LineTrace::getInstance();
    distanceMeasurement_ = measurement::DistanceMeasurement();
    motor_ = device::Motors::getInstance();
    pivotTurn_ = drive::PivotTurn();
    straightRunning_ = drive::StraightRunning();
    lineDetection_ = detection::LineDetection();
    curveRunning_ = drive::CurveRunning();
    bodyAngleMeasurement_ = measurement::BodyAngleMeasurement();
    colorDetection_ = detection::ColorDetection();

    catching_ = drive::Catching();
    destination_ = drive::Destination::getInstance();
    blockColorGetter_ = drive::BlockColorGetter();

    block_x[0] = 2; block_y[0] = 2; //ブロック1
    block_x[1] = 3; block_y[2] = 2; //ブロック3
    block_x[2] = 3; block_y[1] = 1; //ブロック2
    block_x[3] = 4; block_y[3] = 1; //ブロック4

    block_exist[1][1]=1;
    block_exist[2][1]=1;
    block_exist[2][0]=1;
    block_exist[3][0]=1;

    Status_ = Status::STANDBY;
    direction_ = drive::Destination::Direction::DOWN; //進入時の台座に対する相対位置
    result = new drive::colorset_t;
  }
  bool BlockAreaGame::runTo(int x,int y){
    drive::BlockAreaCoordinate currentCoordinate = destination_->currentCoordinate_;
    drive::BlockAreaCoordinate destination = drive::BlockAreaCoordinate(x,y);
    drive::BlockAreaCoordinate nextCoordinate = destination_->getNextStageCoordinate(destination);
    drive::Destination::Direction direction = destination_->getDirection(currentCoordinate, nextCoordinate);
    drive::Destination::Position position = destination_->getPosition(destination_->EV3Position_, direction);
    if((position == drive::Destination::Position::REVERSE) && (block_exist[currentCoordinate.getX()-1][currentCoordinate.getY()-1] == 0)){
        catching_.catchBlock(x,y);
        return false;
    }
    else{
        return destination_->runTo(x,y);
    }
  }

  bool BlockAreaGame::capture(){
    switch(Status_){
      case Status::STANDBY:
        destination_->setCurrentLocation(1,2,direction_); // 侵入後ゲーム開始位置で更新
        Status_ = Status::DECISION;
        break;

      case Status::DECISION:
        //3個運び終わるまで
        if(carried == 3 || confirmed >= 4){
          Status_ = Status::DONE;
        }else{
          Status_ = Status::TO_DESTINATION;
        }
        break;

      case Status::TO_DESTINATION:
        //ブロックの確認した数に応じて目的地決定
        if(runTo(block_x[confirmed],block_y[confirmed])){
          Status_ = Status::BLOCK_COLOR_GET;
        }
        break;

      case Status::BLOCK_COLOR_GET:
        //linetrace_->run(20,drive::LineTraceEdge::LEFT,0.6);
        if(blockColorGetter_.isExecuted(result)){
          //取得したブロックの色に応じて目的地決定
          if(result->blockColor == COLOR_RED){                //赤ブロック
            //目的地台座上にブロックがなければ，目的地とする
            //(2,3)優先
            if(block_exist[1][2] != 1){
              destination_x=2;
              destination_y=3;
            }else if(block_exist[0][2] != 1){
              destination_x=1;
              destination_y=3;
            }else if(block_exist[1][3] != 1){
              destination_x=2;
              destination_y=4;
            }else if(block_exist[0][3] != 1){
              destination_x=1;
              destination_y=4;
            }
          }else if(result->blockColor == COLOR_YELLOW){       //黄色ブロック
            //(2,2)優先
            if(block_exist[1][1] != 1){
              destination_x=2;
              destination_y=2;
            }else if(block_exist[0][1] != 1){
              destination_x=1;
              destination_y=2;
            }else if(block_exist[1][0] != 1){
              destination_x=2;
              destination_y=1;
            }else if(block_exist[0][0] != 1){
              destination_x=1;
              destination_y=1;
            }
          }else if(result->blockColor == COLOR_GREEN){         //緑ブロック
            //(3,2)優先
            if(block_exist[2][1] != 1){
              destination_x=3;
              destination_y=2;
            }else if(block_exist[3][1] != 1){
              destination_x=4;
              destination_y=2;
            }else if(block_exist[2][0] != 1){
              destination_x=3;
              destination_y=1;
            }else if(block_exist[3][0] != 1){
              destination_x=4;
              destination_y=1;
            }
          }else if(result->blockColor == COLOR_BLUE){          //青ブロック
            //(3,3)優先
            if(block_exist[2][2] != 1){
              destination_x=3;
              destination_y=3;
            }else if(block_exist[3][2] != 1){
              destination_x=4;
              destination_y=3;
            }else if(block_exist[2][3] != 1){
              destination_x=3;
              destination_y=4;
            }else if(block_exist[3][3] != 1){
              destination_x=4;
              destination_y=4;
            }
          }else if(result->blockColor == COLOR_BLACK){         //黒ブロック
            confirmed++;
            blockColorGetter_ = drive::BlockColorGetter();
            Status_ = Status::DECISION;
            break;
          }else{ //NONE とかの場合
            blockColorGetter_ = drive::BlockColorGetter();
            break;
          }
          Status_ = Status::CATCH;
        }
        break;

      case Status::CATCH:
        if(catching_.catchBlock(destination_x,destination_y)){
          Status_ = Status::TO_INSTALLATION;
        }
        break;

      case Status::TO_INSTALLATION:
        if(runTo(destination_x,destination_y)){
          Status_ = Status::INSTALLATION;
        }
        break;

      case Status::INSTALLATION:
        if(catching_.putBlock()){
          //確認したブロック数，運んだブロック数，ブロックの有無の更新
          block_exist[block_x[confirmed] - 1][block_y[confirmed] - 1] = 0;
          block_exist[destination_x - 1][destination_y - 1] = 1;
          confirmed++;
          carried++;
          blockColorGetter_ = drive::BlockColorGetter();
          Status_ = Status::DECISION;
        }
        break;

      case Status::DONE:
        motor_->setWheelPWM(0,0);
        return true;
    }
    return false;
  }
};
