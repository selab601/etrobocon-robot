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
    sectionNumber = 0;

    //それぞれのコースの戦略やライントレースを追加していく
    if(selectedCourse == 'R'){
      // sections.emplace_back(100,new TestStrategy());
      // sections.emplace_back(200,new TestStrategy2());
    }

    if(selectedCourse == 'L'){
      // sections.emplace_back(100,new TestStrategy());
      // sections.emplace_back(200,new TestStrategy2());
    }
	}

  /* コース攻略 */
  void Course::captureCourse(){
    while(sectionNumber != sections.size()){
      if(sections[sectionNumber].captureSection()){
        sectionNumber++;
      }
    }
  }

}
