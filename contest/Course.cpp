/**
 * @file Course.cpp
 * @brief コースクラス
 * @aurhor 薄井　翔
 */
#include "Course.h"

namespace contest{

	/* コンストラクタ */
	Course::Course(char selectCourse){
    sectionNumber = 0;


    if(selectCourse == 'R'){
      sections.emplace_back(100,new TestStrategy());
    }

    if(selectCourse == 'L'){
      sections.emplace_back(100,new TestStrategy());
    }
	}


  void Course::captureCourse(){
    while(sectionNumber != sections.size()){
      if(sections[sectionNumber].captureSection()){
        sectionNumber++;
      }
    }
  }

}
