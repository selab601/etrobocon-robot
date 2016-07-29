#ifndef IBLOCK_COLOR_CHECK_H_
#define IBLOCK_COLOR_CHECK_H_

#include "ev3api.h"

namespace drive {
    typedef struct colorset {
        colorid_t blockColor;
        colorid_t tableColor;
    } colorset_t;

    class IBlockColorCheckState {
    public:
        virtual bool isExecuted(colorset_t* result) = 0;
    };
}

#endif
