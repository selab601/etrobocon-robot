#ifndef SELFPOSITIONESTIMATION_
#define SELFPOSITIONESTIMATION_

#include "ev3api.h"
#include "Coordinates.h"
#include "Clock.h"
#include <math.h>

namespace measurement
{


	/* ギア比 */
	#define ENCORDER_GEAR 360 //360*(GEAR2_TOOTH/GEAR1_TOOTH)
	/* ホイール半径 */
	#define WHEEL_RADIUS 40 //mm
	/* トレッド */
	#define TREAD 130  //mm

	class SelfPositionEstimation
	{
	public:
		static SelfPositionEstimation* getInstance();

		/*!
		* @brief自己位置を更新する
		* インスタンスが作られた時点で ループで いつも 使わないときでも 更新しておく ＜かなり重要
		* 引数の値について
		* ETRC2015ではエンコーダ値の符号を反転させて渡してください
		* @return なし
		* @author 塩畑
		*/
		void update(int32_t countL,int32_t countR);
		/*!
		* @brief 設定された原点からの座標を返す
		* @return 座標
		* @author 塩畑
		*/
		Coordinates* getLocation();
		/*!
		* @brief 位置のx座標値を返す
		* @return x座標値
		* @author 塩畑
		*/
		long getLocationX();
		/*!
		* @brief 位置のy座標を返す
		* @return y座標値
		* @author 塩畑
		*/
		long getLocationY();
		/*!
		* @brief 計測開始
		* @return なし
		* @author 塩畑
		*/
		void startMeasure();
		/*!
		* @brief 計測地点を原点としたx座標を返す
		* @return x座標値
		* @author 塩畑
		*/
		long getMeasureX();
		/*!
		* @brief 計測地点を原点としたy座標を返す
		* @return y座標値
		* @author 塩畑
		*/
		long getMeasureY();
		/*!
		* @brief locationからの距離を返す
		* @return 距離
		* @date 2014/07/09
		* @author 塩畑
		*/
		int getDistance();
		/*!
		* @brief 移動距離を返す
		* @return 移動距離
		* @date 2014/07/25
		* @author 塩畑
		*/
		long getMigrationLength();
		/*!
		* @brief 走行体の向いている方向を角度で返す
		* @return 度数法
		* @author 塩畑
		*/
		int getAngle();
		/*!
		* @brief mapクラス初期化
		* Mapクラスの使用 に 必須の関数 ではない
		* 初期化したい位置があるときのみ使用する
		* 属性全て初期化している訳ではない
		* 座標と，方向だけを0にするだけ
		* 速度なども0にしているが，してもしなくても良い
		* 移動距離も初期化します
		* @author 塩畑
		*/
		void initMap();

	private:

		static SelfPositionEstimation* _instance;

		//シングルトンなのでプライベート
		SelfPositionEstimation();

		//場所 コンスタンスもしくはinitが呼び出された位置を原点としています
		Coordinates location;
		//測定点
		Coordinates measurePoint;
		//EV3本体向き 小数点部分
		double angle;
		//増分時間
		uint32_t deltaTime;
		//左モータエンコーダ値バッファ
		int32_t leftBuf;
		//右モータエンコーダ値バッファ
		int32_t rightBuf;
		//本体 速度
		double velocityEV3;
		//本体 回転角速度
		double angularEV3;
		//左 車輪 角速度
		double angularVL;
		//右 車輪 角速度
		double angularVR;
		//時間取得
		ev3api::Clock clock;
		//移動距離
		long migrationLength;
		//小数点計算用
		double ml;
	};
}

#endif