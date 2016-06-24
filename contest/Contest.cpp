#include "Contest.h"

#include "../drive/LineTrace.h"

namespace contest {

  /* メンバ */
  Contest* Contest::instance = 0;


  /* コンストラクタ */
  Contest::Contest() {
    su = StartUp::getInstance();
    starter = Starter::getInstance();
    // state = STARTUP;
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
    /* スタートアップ */
    /*
    if (state == STARTUP) {
      if (su->startUp()) {
        state = RUN;
      }
    } else if (state == RUN) {
      */

      /*スタートアップ*/
      if ( su->startUp() ){
        /*選択されたコースのインスタンスを生成する */
        switch(su->getSelectedCourse()){
        case 'L':
        {
            if (!initialized) {
                initialized = true;
                // drive::LineTrace::getInstance()->setPID(1.0, 0.001, 0.000001);
                // drive::LineTrace::getInstance()->setPID(0.0005, 0.00000001, 0.0005);
                //drive::LineTrace::getInstance()->setPID(0.006, 0.0, 0.01);

                // drive::LineTrace::getInstance()->setPID(0.006, 0.0, 0.024); //わりとよかった(20, 0.5
                // drive::LineTrace::getInstance()->setPID(0.024, 0.0, 0.024); // 灰色走れる(20, 0.5)
                // drive::LineTrace::getInstance()->setPID(0.024, 0.0, 0.048); // 灰色走れる, ギザギザだけどRコースの変なカーブいける！(20, 0.5)
                // drive::LineTrace::getInstance()->setPID(0.018, 0.0, 0.048); // 灰色走れる,これでもギザギザだけどRコースの変なカーブいける！(20, 0.5)
                // drive::LineTrace::getInstance()->setPID(0.018, 0.0, 0.06); // もっとギザギザだけど普通に走れる(20, 0.5)

                // drive::LineTrace::getInstance()->setPID(0.0063, 0.0, 0.06); // ブレブレだけど普通に走れる(50, 0.5)
                // drive::LineTrace::getInstance()->setPID(0.0075, 0.0, 0.0024);
                // drive::LineTrace::getInstance()->setPID(0.0144, 0.000072, 0.72);
                // drive::LineTrace::getInstance()->setPID(0.0144, 0.0, 0.72); // めっちゃ安定する！（30, 0.6);
                drive::LineTrace::getInstance()->setPID(0.0216, 0.0, 1.431);
            }
            break;
        }
          case 'R':
              if (!initialized) {
                  drive::LineTrace::getInstance()->setPID();
                  initialized = true;
              }
          break;
        }

        /* スタート受付 */
        if ( starter->start() ){
          switch (su->getSelectedCourse() ){
          case 'L':
            drive::LineTrace::getInstance()->run(100, 0.6);
            break;
          case 'R':
            drive::LineTrace::getInstance()->run(100);
            break;
          }
        }
    }
  }
}
