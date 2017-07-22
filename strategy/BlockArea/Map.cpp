#include "Map.h"
#include "math.h"

namespace strategy{

    Map::Map(){
        BlockPlace* daiza1  = new BlockPlace(1,BlockAreaColor::RED,-1350*sqrt(3),0);
        BlockPlace* daiza2  = new BlockPlace(2,BlockAreaColor::BLUE,-900*sqrt(3),0);
        BlockPlace* daiza3  = new BlockPlace(3,BlockAreaColor::YELLOW,-450*sqrt(3),0);
        BlockPlace* daiza4  = new BlockPlace(4,BlockAreaColor::BLUE,0,0);
        BlockPlace* daiza5  = new BlockPlace(5,BlockAreaColor::YELLOW,-2250*sqrt(3)/2,-450*sqrt(3)/2);
        BlockPlace* daiza6  = new BlockPlace(6,BlockAreaColor::GREEN,-1350*sqrt(3)/2,-450/2);
        BlockPlace* daiza7  = new BlockPlace(7,BlockAreaColor::RED,-450*sqrt(3)/2,-450/2);
        BlockPlace* daiza8  = new BlockPlace(8,BlockAreaColor::RED,-900*sqrt(3),-450);
        BlockPlace* daiza9  = new BlockPlace(9,BlockAreaColor::BLUE,-450*sqrt(3),-450);
        BlockPlace* daiza10 = new BlockPlace(10,BlockAreaColor::GREEN,-450*(5*sqrt(3)-1)/2,-450*(sqrt(3)+1)/2);
        BlockPlace* daiza11 = new BlockPlace(11,BlockAreaColor::GREEN,-450*(sqrt(3)-1)/2,-450*(sqrt(3)+1)/2);
        BlockPlace* daiza12 = new BlockPlace(12,BlockAreaColor::BLUE,-450*(4*sqrt(3)+1)/2,-450*(sqrt(3)+2)/2);
        BlockPlace* daiza13 = new BlockPlace(13,BlockAreaColor::YELLOW,-450*(4*sqrt(3)-1)/2,-450*(sqrt(3)+2)/2);
        BlockPlace* daiza14 = new BlockPlace(14,BlockAreaColor::RED,-450*(2*sqrt(3)+1)/2,-450*(sqrt(3)+2)/2);
        BlockPlace* daiza15 = new BlockPlace(15,BlockAreaColor::YELLOW,-450*(2*sqrt(3)-1)/2,-450*(sqrt(3)+2)/2);

        //隣接してる台座の登録
        //left  +
        //right -
        daiza1->next[0]     = daiza2;
        daiza1->next[-30]   = daiza5;
        daiza1->next[-75]   = daiza10;

        daiza2->next[180]   = daiza1;
        daiza2->next[0]     = daiza3;
        daiza2->next[-30]   = daiza6;
        daiza2->next[-150]  = daiza5;

        daiza3->next[180]   = daiza2;
        daiza3->next[0]     = daiza4;
        daiza3->next[-30]   = daiza7;
        daiza3->next[-150]  = daiza6;

        daiza4->next[180]   = daiza3;
        daiza4->next[-105]  = daiza11;
        daiza4->next[-150]  = daiza7;

        daiza5->next[150]   = daiza1;
        daiza5->next[30]    = daiza2;
        daiza5->next[-30]   = daiza8;
        daiza5->next[-120]  = daiza10;

        daiza6->next[150]   = daiza2;
        daiza6->next[30]    = daiza3;
        daiza6->next[-30]   = daiza9;
        daiza6->next[-150]  = daiza8;

        daiza7->next[150]   = daiza3;
        daiza7->next[30]    = daiza4;
        daiza7->next[-60]   = daiza11;
        daiza7->next[-150]  = daiza9;

        daiza8->next[150]   = daiza5;
        daiza8->next[30]    = daiza6;
        daiza8->next[-60]   = daiza13;
        daiza8->next[-120]  = daiza12;

        daiza9->next[150]   = daiza6;
        daiza9->next[30]    = daiza7;
        daiza9->next[-60]   = daiza15;
        daiza9->next[-120]  = daiza14;

        daiza10->next[105]  = daiza1;
        daiza10->next[60]   = daiza5;
        daiza10->next[-30]  = daiza12;

        daiza11->next[120]  = daiza7;
        daiza11->next[75]   = daiza4;
        daiza11->next[-150] = daiza15;

        daiza12->next[150]  = daiza10;
        daiza12->next[60]   = daiza8;
        daiza12->next[0]    = daiza13;

        daiza13->next[180]  = daiza12;
        daiza13->next[120]  = daiza8;
        daiza13->next[0]    = daiza14;

        daiza14->next[180]  = daiza13;
        daiza14->next[60]   = daiza9;
        daiza14->next[0]    = daiza15;

        daiza15->next[180]  = daiza14;
        daiza15->next[120]  = daiza9;
        daiza15->next[30]   = daiza11;


    }

}
