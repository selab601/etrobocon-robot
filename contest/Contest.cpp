/**
 * @file Contest.cpp
 * @brief 競技クラス
 * @author usui kakeru
 */

#include "Contest.h"


namespace contest_pkg {

	/* メンバ */
	Contest* Contest::instance = 0;


	/* コンストラクタ */
	Contest::Contest() {
		startUp = StartUp::getInstance();
	}

	/* インスタンス取得 */
	Contest* Contest::getInstance() {
		if (instance == 0) {
			instance = new Contest();
		}
		return instance;
	}

	/* 走行体制御 */
	void Contest::perform() {
		static bool initialized = false;

		/*スタートアップ*/
		if ( startUp->isFinished() ){
			/*選択されたコースのインスタンスを生成する */
			switch(startUp->getSelectedCourse()){
			case 'L':
			{
				if (!initialized) {
					courseL = new Course('L');
					initialized = true;
				}
				break;
			}

			case 'R':
			{
				if (!initialized) {
					courseR = new Course('R');
					initialized = true;
				}
				break;
			}
		}

			/* スタート受付 */
			if ( startUp->acceptStart() ){
				switch (startUp->getSelectedCourse() ){
					case 'L':
					courseL->capture();
					break;
					case 'R':
					courseR->capture();
					break;
				}
			}
		}
	}
}
