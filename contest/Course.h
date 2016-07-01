/**
 * @file Course.h
 * @brief コースクラス
 * @author 薄井　翔
 */

#ifndef COURSE_H_
#define COURSE_H_

#include "Section.h"
#include <vector>



//ライントレースや難所などの戦略をincludeする
#include "../strategy/TestStrategy.h"
//#include "../strategy/aaa.h"

using namespace strategy;
using namespace std;

namespace contest{

	class Course{
		private:

			//選択されたコース
			char selectCourse;

			unsigned int sectionNumber;

			//区間(Section)型の動的配列
			std::vector<Section> sections;


		public:
			/**
			 * @brief コンストラクタ
			 * @param course 攻略するコース
			 */
			Course(char selectCourse);


			/**
			 * @brief コースを攻略する
			 * @details 区間を切り替えながら攻略していく
			 */
			void captureCourse();

	};
}
#endif
