#include "Coordinates.h"


namespace measurement
{
	Coordinates::Coordinates() :
			x(0),
			y(0),
			dx(0),
			dy(0) {}

	Coordinates::Coordinates(long x, long y) :
			x(x),
			y(y),
			dx(0),
			dy(0) {}

	void Coordinates::set_x(long x){
		this->x = x;
	}

	void Coordinates::set_y(long y){
		this->y = y;
	}

	void Coordinates::set_xy(long x,long y){
		this->x = x;
		this->y = y;
	}

	//引数の名前は，dxではなくxの方が良い
	void Coordinates::set_x(double dx){
		x = (int)dx;
		this->dx = dx - (int)dx;
	}

	void Coordinates::set_y(double dy){
		y = (int)dy;
		this->dy = dy - (int)dy;
	}

	void Coordinates::set_xy(double dx,double dy){
		x = (int)dx;
		this->dx = dx - (int)dx;
		y = (int)dy;
		this->dy = dy - (int)dy;
	}

	long Coordinates::get_x(){
		return x;
	}

	long Coordinates::get_y(){
		return y;
	}

	void Coordinates::add_xy(long x,long y){
		this->x += x;
		this->y += y;
	}

	void Coordinates::add_xy(double X,double Y){
		dx += X;
		dy += Y;
		x += (int)dx;
		y += (int)dy;
		dx = dx - (int)dx;
		dy = dy - (int)dy;
	}

	void Coordinates::init(){
		x=0;y=0;dx=0;dy=0;
	}
}
