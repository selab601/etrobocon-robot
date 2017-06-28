#include "Coordinates.h"


namespace measurement
{
    Coordinates::Coordinates() :
            x_(0),
            y_(0),
            dx_(0),
            dy_(0) {}

    Coordinates::Coordinates(long x, long y) :
            x_(x),
            y_(y),
            dx_(0),
            dy_(0) {}

    void Coordinates::set_x(long x){
        this->x_ = x;
    }

    void Coordinates::set_y(long y){
        this->y_ = y;
    }

    void Coordinates::set_xy(long x,long y){
        this->x_ = x;
        this->y_ = y;
    }

    //引数の名前は，dxではなくxの方が良い
    void Coordinates::set_x(double dx){
        x_ = (int)dx;
        this->dx_ = dx_ - (int)dx;
    }

    void Coordinates::set_y(double dy){
        y_ = (int)dy;
        this->dy_ = dy_ - (int)dy;
    }

    void Coordinates::set_xy(double dx,double dy){
        x_ = (int)dx;
        this->dx_ = dx_ - (int)dx;
        y_ = (int)dy;
        this->dy_ = dy_ - (int)dy;
    }

    long Coordinates::get_x(){
        return x_;
    }

    long Coordinates::get_y(){
        return y_;
    }

    void Coordinates::add_xy(long x,long y){
        this->x_ += x;
        this->y_ += y;
    }

    void Coordinates::add_xy(double X,double Y){
        dx_ += X;
        dy_ += Y;
        x_+= (int)dx_;
        y_+= (int)dy_;
        dx_ = dx_ - (int)dx_;
        dy_ = dy_ - (int)dy_;
    }

    void Coordinates::init(){
        x_=0;y_=0;dx_=0;dy_=0;
    }
}
