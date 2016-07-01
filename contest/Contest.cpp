#include "Contest.h"


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
            }
            break;
        }
          case 'R':
              if (!initialized) {
                  initialized = true;
              }
          break;
        }

        /* スタート受付 */
        if ( starter->start() ){
          switch (su->getSelectedCourse() ){
          case 'L':
            break;
          case 'R':
            break;
          }
        }
    }
  }
}
