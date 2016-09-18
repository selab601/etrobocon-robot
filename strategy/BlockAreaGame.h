/*
 * @brief エリア内でのゲーム課題用戦略
 */

#ifndef _BLOCK_AREA_GAME_H
#define _BLOCK_AREA_GAME_H

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "measurement/DistanceMeasurement.h"
#include "device/Motors.h"
#include "drive/PivotTurn.h"
#include "drive/StraightRunning.h"
#include "detection/LineDetection.h"
#include "drive/CurveRunning.h"
#include "measurement/BodyAngleMeasurement.h"
#include "detection/ColorDetection.h"
#include <cstdlib>

#include "drive/Destination.h"
#include "drive/Catching.h"
#include "drive/BlockColorGetter.h"
#include "drive/BlockColorGetterStates/IBlockColorGetterState.h"
#include "../drive/BlockAreaCoordinate.h"
#include <vector>
//#include "ev3api.h"

namespace strategy{
	class BlockAreaGame : public IStrategy{
		private:
			enum class Status
			{
				STANDBY,
				DECISION,
				TO_DESTINATION,
				BLOCK_COLOR_GET,
				CATCH,
				TO_INSTALLATION,
                TO_EXIT_COORDINATE,
				INSTALLATION,
				DONE
				};
			Status Status_;

			drive::LineTrace* linetrace_;
			measurement::DistanceMeasurement distanceMeasurement_;
			device::Motors* motor_;
			drive::PivotTurn pivotTurn_;
			drive::StraightRunning straightRunning_;
			detection::LineDetection lineDetection_;
			drive::CurveRunning curveRunning_;
			measurement::BodyAngleMeasurement bodyAngleMeasurement_;
			detection::ColorDetection colorDetection_;

			drive::Catching catching_;
			drive::Destination* destination_;
			drive::BlockColorGetter blockColorGetter_;

			drive::colorset_t* result;

			drive::Destination::Direction direction_;

			//事前情報管理
			//黄色：(1,1) 青(4,4)
			//色がわかるものは　どう扱うべきか…
			int block_x[4];
			int block_y[4];

			//ブロックの有無を管理
			int block_exist[4][4]={{0}};

			int carried = 0; //運んだブロックの数
			int confirmed = 0; //確認したブロック数　黒ブロックを確認した際の
            bool isFinishedNearStage_ = false;

			//目的地保存用
			int destination_x; int destination_y;

            bool runTo(int x , int y);

            std::vector<drive::BlockAreaCoordinate> priorInformation_;
            std::vector<drive::BlockAreaCoordinate> redStage_;
            std::vector<drive::BlockAreaCoordinate> yellowStage_;
            std::vector<drive::BlockAreaCoordinate> greenStage_;
            std::vector<drive::BlockAreaCoordinate> blueStage_;

            /**
             * @brief 引数に与えられたものの一番近い座標を次の目的地座標にセットする
             **/
            bool nearStage(std::vector<drive::BlockAreaCoordinate> coordinate , int method);

        colorid_t getColorOfStage(drive::BlockAreaCoordinate coordinate);

		public:
			//コンストラクタ
			BlockAreaGame();

			bool capture();

	};
}

#endif
