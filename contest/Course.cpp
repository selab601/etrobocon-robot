/**
 * @file Course.cpp
 * @brief コースクラス
 * @aurhor usui kakeru
 */
#include "Course.h"

using namespace strategy;
using namespace std;

namespace contest_pkg{

	/* コンストラクタ */
	Course::Course(char selectedCourse){
        selectedCourse_ = selectedCourse;
		sectionNumber_ = 0;

		//それぞれのコースの戦略やライントレースを追加していく
		if(selectedCourse_ == 'R'){
		// sections.emplace_back(100,new TestStrategy());
		// sections.emplace_back(200,new TestStrategy2());
		}

		if(selectedCourse_ == 'L'){
		// sections.emplace_back(100,new TestStrategy());
		// sections.emplace_back(200,new TestStrategy2());
		}
	}

	/* コース攻略 */
	void Course::capture(){
		while(sectionNumber_ != sections_.size()){
			if(sections_[sectionNumber_].capture()){
				sectionNumber_++;
			}
		}
	}

}
