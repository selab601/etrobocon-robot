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

    //選択されたコース
    enum class SelectedCourse{
        R_COURSE,
        L_COURSE
    };

	class Course{
		private:

			//現在攻略している戦略
			unsigned int sectionNumber_;

			//区間(Section)型の動的配列
			std::vector<Section> sections_;


		public:
			/**
			 * @brief コンストラクタ
			 * @param course 選択されたコース
			 */
			Course(SelectedCourse course);


			/**
			 * @brief コースを攻略する
			 * @details 区間を切り替えながら攻略していく
			 */
			void capture();

	};
}
#endif
