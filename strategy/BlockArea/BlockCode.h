    #ifndef BLOCKCODE_
    #define BLOCKCODE_

    #include "math.h"
    #include "stdio.h"
    #include <array>

    namespace strategy {
        /**
         * 2018年のブロック並べエリアでは初期位置コードによって
         * 赤、青、黄、緑の4ブロックの位置がわかるのでそれを計算し、保存するクラス
         * 場所はわかるが色の識別がないため，別途webカメラによる取得が必要
         */
        class BlockCode{
        private:

            static BlockCode* instance_;
            BlockCode();

            int block1_;
            int block2_;
            int block3_;
            int block4_;
            int powerBlockCode_;
            bool isPowerSpot_[16] = {false};

            //使わないかも,パワーブロックコードから判定,2018の規約参照
            int powerBlock1_;
            int powerBlock2_;

    public:
        static BlockCode* getInstance();

        /**
         * @brief ブロック運びエリア用の初期位置コードからブロックの配置を計算する
         * @param initPositionCode [初期位置コード]
         */
        void setCode(int initPositionCode);

        int getIdBlock1();
        int getIdBlock2();
        int getIdBlock3();
        int getIdBlock4();
        int getIdPowerBlockCode();

        /**
         * @brief 引数のスポットがパワースポットか返す
         * true == パワースポット
         */
        bool isPowerSpot(int placeNum);

    };

}
#endif
