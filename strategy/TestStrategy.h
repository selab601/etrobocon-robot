

#ifndef _TEST_STRATEGY_H
#define _TEST_STRATEGY_H

#include "IStrategy.h"



namespace strategy {
    class TestStrategy : public IStrategy{
    private:
        int test;
    public:
        TestStrategy(); //コンストラクタ

      bool WalkThrough();
    };
};

#endif
