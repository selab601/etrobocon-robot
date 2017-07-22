#include "Map.h"

namespace strategy{

    Map::Map(){
        BlockPlace* daiza1  = new BlockPlace(1,BlockAreaColor::RED,0,0);
        BlockPlace* daiza2  = new BlockPlace(2,BlockAreaColor::BLUE,0,0);
        BlockPlace* daiza3  = new BlockPlace(3,BlockAreaColor::YELLOW,0,0);
        BlockPlace* daiza4  = new BlockPlace(4,BlockAreaColor::BLUE,0,0);
        BlockPlace* daiza5  = new BlockPlace(5,BlockAreaColor::YELLOW,0,0);
        BlockPlace* daiza6  = new BlockPlace(6,BlockAreaColor::GREEN,0,0);
        BlockPlace* daiza7  = new BlockPlace(7,BlockAreaColor::RED,0,0);
        BlockPlace* daiza8  = new BlockPlace(8,BlockAreaColor::RED,0,0);
        BlockPlace* daiza9  = new BlockPlace(9,BlockAreaColor::BLUE,0,0);
        BlockPlace* daiza10 = new BlockPlace(10,BlockAreaColor::GREEN,0,0);
        BlockPlace* daiza11 = new BlockPlace(11,BlockAreaColor::GREEN,0,0);
        BlockPlace* daiza12 = new BlockPlace(12,BlockAreaColor::BLUE,0,0);
        BlockPlace* daiza13 = new BlockPlace(13,BlockAreaColor::YELLOW,0,0);
        BlockPlace* daiza14 = new BlockPlace(14,BlockAreaColor::RED,0,0);
        BlockPlace* daiza15 = new BlockPlace(15,BlockAreaColor::YELLOW,0,0);

        daiza1.leftAngle0_     = daiza2;
        daiza1.rightAngle30_   = daiza5;
        daiza1.rightAngle75_   = daiza10;

        daiza2.leftAngle180_   = daiza1;
        daiza2.leftAngle0_     = daiza3;
        daiza2 .rightAngle30_  = daiza6;
        daiza2 .rightAngle150_ = daiza5;

        daiza3.leftAngle180_   = daiza2;
        daiza3.leftAngle0_     = daiza4;
        daiza3.rightAngle30_   = daiza7;
        daiza3.rightAngle150_  = daiza6;

        daiza4.leftAngle180_   = daiza3;
        daiza4.rightAngle105_  = daiza11;
        daiza4.rightAngle150_  = daiza7;

        daiza5.leftAngle150_   = daiza1;
        daiza5.leftAngle30_    = daiza2;
        daiza5.rightAngle30_   = daiza8;
        daiza5.rightAngle120_  = daiza10;

        daiza6.leftAngle150_   = daiza2;
        daiza6.leftAngle30_    = daiza3;
        daiza6.rightAngle30_   = daiza9;
        daiza6.rightAngle150_  = daiza8;

        daiza7.leftAngle150_   = daiza3;
        daiza7.leftAngle30_    = daiza4;
        daiza7.rightAngle60_   = daiza11;
        daiza7.rightAngle150_  = daiza9;

        daiza8.leftAngle150_   = daiza5;
        daiza8.leftAngle30_    = daiza6;
        daiza8.rightAngle60_   = daiza13;
        daiza8.rightAngle120_  = daiza12;

        daiza9.leftAngle150_   = daiza6;
        daiza9.leftAngle30_    = daiza7;
        daiza9.rightAngle60_   = daiza15;
        daiza9.rightAngle120_  = daiza14;

        daiza10.leftAngle105_  = daiza1;
        daiza10.leftAngle60_   = daiza5;
        daiza10.rightAngle30_  = daiza12;

        daiza11.leftAngle120_  = daiza7;
        daiza11.leftAngle75_   = daiza4;
        daiza11.rightAngle150_ = daiza15;

        daiza12.leftAngle150_  = daiza10;
        daiza12.leftAngle60_   = daiza8;
        daiza12.leftAngle0_    = daiza13;

        daiza13.leftAngle180_  = daiza12;
        daiza13.leftAngle120_  = daiza8;
        daiza13.leftAngle0_    = daiza14;

        daiza14.leftAngle180_  = daiza13;
        daiza14.leftAngle60_   = daiza9;
        daiza14.leftAngle0_    = daiza15;

        daiza15.leftAngle180_  = daiza14;
        daiza15.leftAngle120_  = daiza9;
        daiza15.leftAngle30_   = daiza11;

        }

        void Map::addBlockPlace(BlockPlace blockPlace){
        places_.emplace_back(blockPlace);
        }

        void Map::addLine(BlockAreaLine line){
        lines_.emplace_back(line);
        }

        BlockPlace Map::getBlockPlaceAt(int blockPlaceId){
        return places_[blockPlaceId];
        }

        }
