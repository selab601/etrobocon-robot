    #ifndef BLOCKCODE_
    #define BLOCKCODE_

    #include "math.h"
    #include "stdio.h"
    #include <array> 

    namespace contest_pkg {
        /**
         * 
         */
        class BlockCode{
        private:

            static BlockCode* instance_;
            BlockCode();

            int red_;
            int blue_;
            int yellow_;
            int green_;
            int black_;
            
            //それぞれの色の配置が黒ブロックでのどこに相当するかのリスト
            int redToBlalck_[11]   = {2,3,4,5,6,9,10,11,12,13,15};
            int blueToBlack_[11]   = {1,3,5,6,7,8,10,11,13,14,15};
            int yellowToBlack_[11] = {1,2,4,6,7,8,9,10,11,12,14};
            
            /**
             * @brief 色によって位置番号が違うので黒での位置番号に統一
             * @param  redCode 赤ブロックの位置番号
             * @return         黒ブロックでの位置番号
             */
            int encodeRed(int redCode);
            int encodeBlue(int blueCode);
            int encodeYellow(int yellowCode);

    public:
        static BlockCode* getInstance();

        /**
         * @brief ブロック運びエリア用の初期位置コードからブロックの配置を計算する
         * @param initPositionCode [初期位置コード]
         */
        void setCode(int initPositionCode);

        int getIdRed();
        int getIdBlue();
        int getIdYellow();
        int getIdGreen();
        int getIdBlack();
    };

}
#endif