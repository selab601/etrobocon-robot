/**
 * @file Coordinates.h
 * @brief x,yの座標を表すクラス，整数部分はlong型,小数部分はdouble型の変数で保存する.double型は小数部分しか持たない．
 * @author 塩畑
 */

#ifndef COORDINATES_H_
#define COORDINATES_H_

namespace measurement
{
	class Coordinates {
	public:
		Coordinates();
		Coordinates(long,long);
		void set_x(long);
		void set_y(long);
		void set_xy(long,long);
		void set_x(double);
		void set_y(double);
		void set_xy(double,double);
		long get_x();
		long get_y();
		/**
		 * @brief 与えられた引数を,x,yのそれぞれに加算する
		 */
		void add_xy(long,long);
		void add_xy(double,double);
		/**
		 * @brief x,yを0で初期化する
		 */
		void init();
	private:
		//座標の整数部分
		long x_;
		long y_;
		//座標の小数部分
		double dx_;
		double dy_;
	};
}

#endif /* COORDINATES_H_ */
