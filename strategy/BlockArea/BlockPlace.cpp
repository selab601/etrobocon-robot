#include "BlockPlace.h"

namespace strategy{
    BlockPlace::BlockPlace(int id,BlockAreaColor color,int x,int y){
        id_    = id;
        color_ = color; //今年は使わない説
        x_     = x;
        y_     = y;
    }

    int BlockPlace::getDistance(BlockPlace* blockPlace){
        int dx = blockPlace->x_ - this->x_;
        int dy = blockPlace->y_ - this->y_;
        return sqrt(pow(dx,2)+pow(dy,2));
    }

    int BlockPlace::getDegree(BlockPlace* blockPlace){
        int dx = blockPlace->x_ - this->x_;
        int dy = blockPlace->y_ - this->y_;
        return atan2(dy,dx)*180/M_PI;
    }

    BlockPlace* BlockPlace::getNextPlace(int angle){
        std::unordered_map<int,BlockPlace*> diff;
        int min = 180;

        for(auto itr = next.begin();itr != next.end();++itr){
            diff[abs(angle-itr->first)] = next[itr->first];
            if(min > abs(angle-itr->first) ){min = abs(angle-itr->first); }
        }
        return diff[min];
    }

    int BlockPlace::getId(){return id_;}
}
