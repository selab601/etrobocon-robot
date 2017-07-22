#ifndef MAP_H_
#define MAP_H_

#include "BlockPlace.h"
#include "BlockAreaLine.h"
#include <vector>
#include <unordered_map>

class TestIterator;

namespace strategy{

    class Map{
    private:
        std::vector<int> route_;//ルート
        std::unordered_map<std::string,int> blocks_;//ブロックの現在地

    public:

        //コンストラクタ
        Map();

        /*ルート作る*/

    };

}

#endif
