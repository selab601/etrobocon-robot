#include "Destination.h"

using Coordinate = drive::BlockAreaCoordinate;

namespace drive{
    Destination::Destination(int x, int y, Destination::Direction EV3Position){
        stageCoordinate_ = BlockAreaCoordinate(x, y);
        EV3Position_ = EV3Position;
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
            if(EV3Position_ == Direction::UP || EV3Position_ == Direction::DOWN)
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

    Destination::Direction Destination::getDirection(Coordinate from, Coordinate to) {
        Coordinate diff = to - from;
        if(diff.getX()==0)
        {
            if(diff.getY()>0)
            {
                return Direction::DOWN;
            }
            else if(diff.getY()<0)
            {
                return Direction::UP;
            }
            else
            {
                return Direction::NONE;
            }
        }
        else if(diff.getY()==0)
        {
            if(diff.getX()>0)
            {
                return Direction::RIGHT;
            }
            else if(diff.getX()<0)
            {
                return Direction::LEFT;
            }
            else
            {
                return Direction::NONE;
            }
        }
        else
        {
            //ここには来ない
            return Direction::NONE;
        }
    }

    Destination::Position Destination::getPosition(Direction from, Direction to) {
        if (from == to) {
            return Position::EQUAL;
        }

        if ((from == Direction::UP && to == Direction::DOWN) ||
            (from == Direction::DOWN && to == Direction::UP) ||
            (from == Direction::RIGHT && to == Direction::LEFT) ||
            (from == Direction::LEFT && to == Direction::RIGHT)) {
            return Position::REVERSE;
        }

        if ((from == Direction::UP && to == Direction::RIGHT) ||
            (from == Direction::RIGHT && to == Direction::DOWN) ||
            (from == Direction::DOWN && to == Direction::LEFT) ||
            (from == Direction::LEFT && to == Direction::UP)) {
            return Position::LEFT;
        }

        if ((from == Direction::UP && to == Direction::LEFT) ||
            (from == Direction::LEFT && to == Direction::DOWN) ||
            (from == Direction::DOWN && to == Direction::RIGHT) ||
            (from == Direction::RIGHT && to == Direction::UP)) {
            return Position::RIGHT;
        }

        return Position::NONE;
    }

    bool Destination::runTo(int x,int y){
        Coordinate destination = Coordinate(x, y);
        Coordinate diff = (destination - stageCoordinate_);
        if(diff.getX() == 0 && diff.getY() == 0)
        {
            return true;
        }
        Coordinate nextStageCoordinate = getNextStageCoordinate(destination);
        Direction nextStageDirection = getDirection(stageCoordinate_, nextStageCoordinate);
        // 次どちらの座標に向かって進むのかで場合分け
        Position position = getPosition(EV3Position_ , nextStageDirection);
        bool isFinished = false;
        switch (position) {
            case Position::EQUAL:
                isFinished = pivotTurn_.turn(180);
                break;
            case Position::REVERSE:
                if((nextStageDirection == Direction::UP && stageCoordinate_.getX() == 1) ||
                    (nextStageDirection == Direction::DOWN && stageCoordinate_.getX() == 4) ||
                    (nextStageDirection == Direction::RIGHT && stageCoordinate_.getY() == 1) ||
                    (nextStageDirection == Direction::LEFT && stageCoordinate_.getY() == 4))
                {
                    isFinished = avoidance_.startAvoidance(DirectionKind::STRAIGHT_RIGHT);
                }
                else
                {
                    isFinished = avoidance_.startAvoidance(DirectionKind::STRAIGHT_LEFT);
                }
            case Position::RIGHT:
                isFinished = avoidance_.startAvoidance(DirectionKind::RIGHT);
                break;
            case Position::LEFT:
                isFinished = avoidance_.startAvoidance(DirectionKind::LEFT);
                break;
            default:
                break;
        }

        if (isFinished) {
            stageCoordinate_ = nextStageCoordinate;
            switch (position) {
                case Position::EQUAL:
                    switch(EV3Position_){
                        case Direction::UP:
                            EV3Position_ = Direction::DOWN;
                            break;
                        case Direction::DOWN:
                            EV3Position_ = Direction::UP;
                            break;
                        case Direction::RIGHT:
                            EV3Position_ = Direction::LEFT;
                            break;
                        case Direction::LEFT:
                            EV3Position_ = Direction::RIGHT;
                            break;
                        default:
                            break;
                    }
                case Position::RIGHT:
                    switch(EV3Position_){
                        case Direction::UP:
                            EV3Position_ = Direction::RIGHT;
                            break;
                        case Direction::DOWN:
                            EV3Position_ = Direction::LEFT;
                            break;
                        case Direction::RIGHT:
                            EV3Position_ = Direction::DOWN;
                            break;
                        case Direction::LEFT:
                            EV3Position_ = Direction::UP;
                            break;
                        default:
                            break;
                    }
                case Position::LEFT:
                    switch(EV3Position_){
                        case Direction::UP:
                            EV3Position_ = Direction::LEFT;
                            break;
                        case Direction::DOWN:
                            EV3Position_ = Direction::RIGHT;
                            break;
                        case Direction::RIGHT:
                            EV3Position_ = Direction::UP;
                            break;
                        case Direction::LEFT:
                            EV3Position_ = Direction::DOWN;
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        return false;
    }

    const char* Destination::getStr(Direction direction) {
        switch (direction) {
        case Direction::RIGHT:
            return "RIGHT";
        case Direction::LEFT:
            return "LEFT";
        case Direction::UP:
            return "UP";
        case Direction::DOWN:
            return "DOWN";
        case Direction::NONE:
            return "NONE";
        default:
            return "UNDEFINED";
        }
    }
    const char* Destination::getStr(Position position) {
        switch (position) {
        case Position::EQUAL:
            return "EQUAL";
        case Position::REVERSE:
            return "REVERSE";
        case Position::RIGHT:
            return "RIGHT";
        case Position::LEFT:
            return "LEFT";
        case Position::NONE:
            return "NONE";
        default:
            return "UNDEFINED";
        }
    }
}
