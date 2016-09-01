#include "Destination.h"

using Coordinate = drive::BlockAreaCoordinate;

namespace drive{
    Destination::Destination(int x, int y, Destination::EV3Position position){
        stageCoordinate_ = BlockAreaCoordinate(x, y);
        EV3Position_ = position;
        avoidance_ = Avoidance();
        straightRunning_ = StraightRunning();
        pivotTurn_ = PivotTurn();
    }

    BlockAreaCoordinate Destination::getNextStageCoordinate(Coordinate destination){
        Coordinate result = stageCoordinate_ - destination;

        //移動先の座標がx軸方向にある
        if(abs(result.getX())>abs(result.getY()))
        {
            return horizonal(result.getX());
        }
        //移動先の座標がy軸方向にある
        else if(abs(result.getX())<abs(result.getY()))
        {
            return vertical(result.getY());
        }
        //移動先の座標が斜めにある
        else
        {
            if(EV3Position_ == EV3Position::UP || EV3Position_ == EV3Position::DOWN)
            {
                return horizonal(result.getX());
            }
            else
            {
                return vertical(result.getY());
            }
        }

        // ここにはこないはずだけどコンパイルエラーが出る
        return Coordinate(-1,-1);
    }

    Coordinate Destination::horizonal(int diffX) {
        if(diffX>0)
        {
          return Coordinate(stageCoordinate_.getX() - 1,stageCoordinate_.getY());
        }
        else
        {
          return Coordinate(stageCoordinate_.getX() + 1,stageCoordinate_.getY());
        }
    }

    Coordinate Destination::vertical(int diffY) {
        if(diffY>0)
            {
                return Coordinate(stageCoordinate_.getX(),stageCoordinate_.getY() - 1);
            }
            else
            {
                return Coordinate(stageCoordinate_.getX(),stageCoordinate_.getY() + 1);
            }
    }

    bool Destination::runTo(int x,int y){
        //未実装
        return false;
    }
}
