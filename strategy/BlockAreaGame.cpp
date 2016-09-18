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
    sholdMoveBlock_ = true;
    destination_ = drive::Destination::getInstance();
    blockColorGetter_ = drive::BlockColorGetter();

    block_x[0] = 1; block_y[0] = 1; //ブロック1
    block_x[1] = 1; block_y[1] = 2; //ブロック3
    block_x[2] = 2; block_y[2] = 2; //ブロック2
    block_x[3] = 3; block_y[3] = 2; //ブロック4

    block_exist[block_x[0]-1][block_y[0]-1]=1;
    block_exist[block_x[1]-1][block_y[1]-1]=1;
    block_exist[block_x[2]-1][block_y[2]-1]=1;
    block_exist[block_x[3]-1][block_y[3]-1]=1;

    priorInformation_ = std::vector<drive::BlockAreaCoordinate>();
    priorInformation_.emplace_back(drive::BlockAreaCoordinate(block_x[0] , block_y[0]));
    priorInformation_.emplace_back(drive::BlockAreaCoordinate(block_x[1] , block_y[1]));
    priorInformation_.emplace_back(drive::BlockAreaCoordinate(block_x[2] , block_y[2]));
    priorInformation_.emplace_back(drive::BlockAreaCoordinate(block_x[3] , block_y[3]));

    redStage_ = std::vector<drive::BlockAreaCoordinate>();
    redStage_.emplace_back(drive::BlockAreaCoordinate(1 ,3 ));
    redStage_.emplace_back(drive::BlockAreaCoordinate(1 ,4 ));
    redStage_.emplace_back(drive::BlockAreaCoordinate(2 ,3 ));
    redStage_.emplace_back(drive::BlockAreaCoordinate(2 ,4 ));

    yellowStage_ = std::vector<drive::BlockAreaCoordinate>();
    yellowStage_.emplace_back(drive::BlockAreaCoordinate(1 ,1 ));
    yellowStage_.emplace_back(drive::BlockAreaCoordinate(1 ,2 ));
    yellowStage_.emplace_back(drive::BlockAreaCoordinate(2 ,1 ));
    yellowStage_.emplace_back(drive::BlockAreaCoordinate(2 ,2 ));

    greenStage_ = std::vector<drive::BlockAreaCoordinate>();
    greenStage_.emplace_back(drive::BlockAreaCoordinate(3 ,1 ));
    greenStage_.emplace_back(drive::BlockAreaCoordinate(3 ,2 ));
    greenStage_.emplace_back(drive::BlockAreaCoordinate(4 ,1 ));
    greenStage_.emplace_back(drive::BlockAreaCoordinate(4 ,2 ));

    blueStage_ = std::vector<drive::BlockAreaCoordinate>();
    blueStage_.emplace_back(drive::BlockAreaCoordinate(3 ,3 ));
    blueStage_.emplace_back(drive::BlockAreaCoordinate(3 ,4 ));
    blueStage_.emplace_back(drive::BlockAreaCoordinate(4 ,3 ));
    blueStage_.emplace_back(drive::BlockAreaCoordinate(4 ,4 ));

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
        if(x == currentCoordinate.getX() && y == currentCoordinate.getY()){
            return true;
        }
        catching_.catchBlock(x,y);
        return false;
    }
    else{
        return destination_->runTo(x,y);
    }
  }

  bool BlockAreaGame::nearStage(std::vector<drive::BlockAreaCoordinate> coordinate , int method){
    drive::BlockAreaCoordinate currentCoordinate = destination_->currentCoordinate_;
    int tmp;
    int diff = 100;
    for(int i = 0 ; i < 4 ; i++){
        if(block_exist[coordinate[i].getX() - 1][coordinate[i].getY() - 1] == method){
            tmp = abs(currentCoordinate.getX()- (coordinate[i].getX())) + abs(currentCoordinate.getY() - coordinate[i].getY());
            if(diff > tmp){
                diff = tmp;
                destination_x = coordinate[i].getX();
                destination_y = coordinate[i].getY();
            }
        }
    }
    return true;
  }

  bool BlockAreaGame::capture(){
    switch(Status_){
      case Status::STANDBY:
      destination_->setCurrentLocation(1,2,direction_); // 侵入後ゲーム開始位置で更新
        if(block_exist[0][1] == 0){
            if(block_exist[0][0] == 1){
                if(catching_.catchBlock(1,1)){
                Status_ = Status::DECISION;
                }
            }
            else if(catching_.catchBlock(2,2)){
                Status_ = Status::DECISION;
            }
        }
        else{
            Status_ = Status::DECISION;
        }
        break;

      case Status::DECISION:
        //3個運び終わるまで
        if(carried == 3 || confirmed >= 4){
          Status_ = Status::TO_EXIT_COORDINATE;
        }else{
          Status_ = Status::TO_DESTINATION;
        }
        break;

      case Status::TO_DESTINATION:
        if(!isFinishedNearStage_){
            isFinishedNearStage_ = nearStage(priorInformation_ , 1);
        }
        else{
            if(runTo(destination_x,destination_y)){
                drive::BlockAreaCoordinate currentCoordinate = destination_->currentCoordinate_;
                block_exist[currentCoordinate.getX() - 1][currentCoordinate.getY() - 1] = 0;
                isFinishedNearStage_ = false;
                Status_ = Status::BLOCK_COLOR_GET;
            }
        }
        break;

      case Status::BLOCK_COLOR_GET:
        //linetrace_->run(20,drive::LineTraceEdge::LEFT,0.6);
        if(blockColorGetter_.isExecuted(result)){
          //取得したブロックの色に応じて目的地決定
          if(result->blockColor == COLOR_BLACK) {           // 黒ブロック
            block_exist[destination_x - 1][destination_y - 1] = 2;
            confirmed++;
            blockColorGetter_ = drive::BlockColorGetter();
            sholdMoveBlock_ = false;
          } else if(getColorOfStage(drive::BlockAreaCoordinate(destination_x, destination_y)) == result->blockColor) {
            //確認したブロック数，運んだブロック数，ブロックの有無の更新
            block_exist[destination_x - 1][destination_y - 1] = 2;
            confirmed++;
            carried++;
            blockColorGetter_ = drive::BlockColorGetter();
            sholdMoveBlock_ = false;
          } else if(result->blockColor == COLOR_RED){         //赤ブロック
            //目的地台座上にブロックがなければ，目的地とする
            nearStage(redStage_ , 0);
            sholdMoveBlock_ = true;
          }else if(result->blockColor == COLOR_YELLOW){       //黄色ブロック
            nearStage(yellowStage_ , 0);
            sholdMoveBlock_ = true;
          }else if(result->blockColor == COLOR_GREEN){         //緑ブロック
            nearStage(greenStage_ , 0);
            sholdMoveBlock_ = true;
          }else if(result->blockColor == COLOR_BLUE){          //青ブロック
            nearStage(blueStage_ , 0);
            sholdMoveBlock_ = true;
          }else{ //NONE とかの場合
            blockColorGetter_ = drive::BlockColorGetter();
            break;
          }

          distanceMeasurement_.setTargetDistance(100);
          distanceMeasurement_.startMeasurement();
          Status_ = Status::BACK_TO_LINE;
        }
        break;

    case Status::BACK_TO_LINE:
    {
        drive::BlockAreaCoordinate currentCoordinate = destination_->currentCoordinate_;
        drive::BlockAreaCoordinate destination = drive::BlockAreaCoordinate(destination_x, destination_y);
        drive::BlockAreaCoordinate nextCoordinate = destination_->getNextStageCoordinate(destination);
        drive::Destination::Direction direction = destination_->getDirection(currentCoordinate, nextCoordinate);
        drive::Destination::Position position = destination_->getPosition(destination_->EV3Position_, direction);
        // 後ろに移動する場合(EQUAL)以外は戻る
        // ブロックを動かさない場合も戻る
        if(position == drive::Destination::Position::EQUAL
           || sholdMoveBlock_ == false){
          straightRunning_.run(-20);
          if (!distanceMeasurement_.getResult()) {
              break;
          }
        }

        // 戻り終えたら/戻らなくて良い場合は
        if (sholdMoveBlock_) {
            Status_ = Status::CATCH;
        } else {
            Status_ = Status::DECISION;
        }

        break;
    }
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
          block_exist[destination_x - 1][destination_y - 1] = 2;
          confirmed++;
          carried++;
          blockColorGetter_ = drive::BlockColorGetter();
          Status_ = Status::DECISION;
        }
        break;

      case Status::TO_EXIT_COORDINATE:
        if (runTo(4,4)) {
            Status_ = Status::DONE;
        }
        break;

      case Status::DONE:
        motor_->setWheelPWM(0,0);
        return true;
    }
    return false;
  }

    colorid_t BlockAreaGame::getColorOfStage(drive::BlockAreaCoordinate coordinate) {
        for (auto cor : redStage_) {
            if (coordinate == cor) { return COLOR_RED; }
        }
        for (auto cor : yellowStage_) {
            if (coordinate == cor) { return COLOR_YELLOW; }
        }
        for (auto cor : blueStage_) {
            if (coordinate == cor) { return COLOR_BLUE; }
        }
        for (auto cor : greenStage_) {
            if (coordinate == cor) { return COLOR_GREEN; }
        }
        return COLOR_NONE;
    }
};
