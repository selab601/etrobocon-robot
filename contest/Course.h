/**
 * @file Course.h
 * @brief コースクラス
 * @author usui kakeru
 */

#ifndef COURSE_H_
#define COURSE_H_

#include "Section.h"
#include <vector>



//ライントレースや戦略をincludeする
//#include "../strategy/aaa.h"

namespace contest_pkg{

	class Course{
		private:

			//選択されたコース
			char selectedCourse;

			//現在攻略している戦略
			unsigned int sectionNumber;

			//区間(Section)型の動的配列
			vector<Section> sections;


		public:
			/**
			 * @brief コンストラクタ
			 * @param selectedCourse 選択されたコース
			 */
			Course(char selectedCourse);


			/**
			 * @brief コースを攻略する
			 * @details 区間を切り替えながら攻略していく
			 */
			void captureCourse();

	};
}
#endif
