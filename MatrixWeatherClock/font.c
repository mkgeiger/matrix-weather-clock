#include "font.h"

const uint8_t char_blank[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // blank
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0} 
};

const uint8_t char_unknown[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // unknown
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1}
};

const uint8_t char_33[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // !
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_34[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // "
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_35[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // #
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_36[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // $
    {0, 1, 0},
    {1, 1, 1},
    {1, 1, 0},
    {0, 0, 1},
    {1, 1, 1},
    {0, 1, 0}
};

const uint8_t char_37[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // %
    {1, 0, 1},
    {0, 0, 1},
    {0, 1, 0},
    {1, 0, 0},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_38[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // &
    {0, 1, 0},
    {1, 0, 1},
    {0, 1, 0},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 1}
};

const uint8_t char_39[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_40[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // (
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 1},
    {0, 0, 0}
};

const uint8_t char_41[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // )
    {1, 0, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {1, 0, 0},
    {0, 0, 0}
};

const uint8_t char_42[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // *
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 0},
    {1, 0, 1},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_43[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // +
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_44[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // ,
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
    {0, 1, 0}
};

const uint8_t char_45[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // -
    {0, 0, 0},
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_46[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // .
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_47[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // /
    {0, 0, 1},
    {0, 0, 1},
    {0, 1, 0},
    {1, 0, 0},
    {1, 0, 0},
    {0, 0, 0}
};

const uint8_t char_48[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 0
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_49[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 1
    {0, 1, 0},
    {1, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_50[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 2
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_51[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 3
    {1, 1, 1},
    {0, 0, 1},
    {0, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_52[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 4
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 0}
};

const uint8_t char_53[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 5
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_54[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 6
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_55[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 7
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 0}
};

const uint8_t char_56[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 8
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_57[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 9
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_58[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // :
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_59[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // ;
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
    {0, 1, 0}
};

const uint8_t char_60[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // <
    {0, 0, 1},
    {0, 1, 0},
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {0, 0, 0}
};

const uint8_t char_61[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // =
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_62[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // >
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {0, 1, 0},
    {1, 0, 0},
    {0, 0, 0}
};

const uint8_t char_63[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // ?
    {1, 1, 0},
    {0, 0, 1},
    {0, 1, 0},
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_64[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // @
    {0, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 0},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_65[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // A
    {0, 1, 0},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_66[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // B
    {1, 1, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 1},
    {1, 1, 0},
    {0, 0, 0}
};

const uint8_t char_67[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // C
    {0, 1, 0},
    {1, 0, 1},
    {1, 0, 0},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_68[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // D
    {1, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 0},
    {0, 0, 0}
};

const uint8_t char_69[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // E
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 0},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_70[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // F
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 0},
    {1, 0, 0},
    {1, 0, 0},
    {0, 0, 0}
};

const uint8_t char_71[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // G
    {0, 1, 1},
    {1, 0, 0},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_72[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // H
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_73[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // I
    {1, 1, 1},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_74[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // J
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_75[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // K
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_76[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // L
    {1, 0, 0},
    {1, 0, 0},
    {1, 0, 0},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_77[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // M
    {1, 0, 1},
    {1, 1, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_78[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // N
    {0, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 0},
    {0, 0, 0}
};

const uint8_t char_79[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // O
    {0, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_80[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // P
    {1, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0}
};

const uint8_t char_81[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // Q
    {0, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 1}
};

const uint8_t char_82[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // R
    {1, 1, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_83[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // S
    {0, 1, 1},
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 0},
    {0, 0, 0}
};

const uint8_t char_84[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // T
    {1, 1, 1},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_85[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // U
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_86[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // V
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_87[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // W
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 1, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_88[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // X
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_89[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // Y
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_90[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // Z
    {1, 1, 1},
    {0, 0, 1},
    {0, 1, 0},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_91[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // [
    {0, 1, 1},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_92[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '\'
    {1, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 0}
};

const uint8_t char_93[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // ]
    {1, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 0},
    {0, 0, 0}
};

const uint8_t char_94[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // ^
    {0, 1, 0},
    {1, 0, 1},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_95[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // _
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {1, 1, 1}
};

const uint8_t char_96[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // `
    {0, 1, 0},
    {0, 0, 1},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_97[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // a
    {0, 0, 0},
    {0, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_98[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // b
    {1, 0, 0},
    {1, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 0},
    {0, 0, 0},
};

const uint8_t char_99[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // c
    {0, 0, 0},
    {0, 1, 1},
    {1, 0, 0},
    {1, 0, 0},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_100[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // d
    {0, 0, 1},
    {0, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_101[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // e
    {0, 0, 0},
    {0, 1, 0},
    {1, 0, 1},
    {1, 1, 0},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_102[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // f
    {0, 0, 1},
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_103[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // g
    {0, 0, 0},
    {0, 1, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 1},
    {1, 1, 0}
};

const uint8_t char_104[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // h
    {1, 0, 0},
    {1, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_105[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // i
    {0, 1, 0},
    {0, 0, 0},
    {1, 1, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_106[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // j
    {0, 0, 1},
    {0, 0, 0},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {1, 1, 0}
};

const uint8_t char_107[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // k
    {1, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_108[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // l
    {1, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_109[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // m
    {0, 0, 0},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_110[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // n
    {0, 0, 0},
    {1, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_111[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // o
    {0, 0, 0},
    {0, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_112[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // p
    {0, 0, 0},
    {1, 1, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 0},
    {1, 0, 0}
};

const uint8_t char_113[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // q
    {0, 0, 0},
    {0, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 1}
};

const uint8_t char_114[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // r
    {0, 0, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 0},
    {1, 0, 0},
    {0, 0, 0}
};

const uint8_t char_115[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // s
    {0, 0, 0},
    {0, 1, 1},
    {1, 1, 0},
    {0, 0, 1},
    {1, 1, 0},
    {0, 0, 0}
};

const uint8_t char_116[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // t
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 1},
    {0, 0, 0}
};

const uint8_t char_117[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // u
    {0, 0, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_118[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // v
    {0, 0, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_119[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // w
    {0, 0, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_120[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // x
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 0},
    {0, 1, 0},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_121[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // y
    {0, 0, 0},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 1},
    {1, 1, 0}
};

const uint8_t char_122[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // z
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 1},
    {0, 1, 0},
    {1, 0, 0},
    {1, 1, 1}
};

const uint8_t char_123[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // {
    {0, 0, 1},
    {0, 1, 0},
    {1, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 1}
};

const uint8_t char_124[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // |
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_125[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // }
    {1, 0, 0},
    {0, 1, 0},
    {0, 1, 1},
    {0, 1, 0},
    {0, 1, 0},
    {1, 0, 0}
};

const uint8_t char_126[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // ~
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xC2A3[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '£'
    {0, 1, 1},
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_0xC2A7[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '§'
    {0, 1, 1},
    {1, 1, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
    {0, 0, 0}
};

const uint8_t char_0xC2A9[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '©'
    {0, 1, 0},
    {1, 0, 1},
    {1, 0, 0},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_0xC2AB[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '«'
    {0, 0, 0},
    {0, 1, 0},
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xC2B0[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '°'
    {0, 1, 0},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xC2B1[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '±'
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_0xC2B2[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '²'
    {1, 1, 0},
    {0, 1, 0},
    {0, 1, 1},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xC2B3[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '³'
    {1, 1, 1},
    {0, 1, 1},
    {1, 1, 1},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xC2B4[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '´'
    {0, 0, 1},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xC2B5[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 'µ'
    {0, 0, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 0}
};

const uint8_t char_0xC2BB[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '»'
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {0, 1, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xC384[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 'Ä'
    {1, 0, 1},
    {0, 1, 0},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {0, 0, 0}
};

const uint8_t char_0xC396[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 'Ö'
    {1, 0, 1},
    {0, 1, 0},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_0xC397[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '×'
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 0},
    {1, 0, 1},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xC39C[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 'Ü'
    {1, 0, 1},
    {0, 0, 0},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 0}
};

const uint8_t char_0xC39F[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 'ß'
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 0, 0}
};

const uint8_t char_0xC3A4[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 'ä'
    {1, 0, 1},
    {0, 0, 0},
    {0, 1, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_0xC3B6[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 'ö'
    {1, 0, 1},
    {0, 0, 0},
    {0, 1, 0},
    {1, 0, 1},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_0xC3B7[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '÷'
    {0, 1, 0},
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0}
};

const uint8_t char_0xC3BC[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // 'ü'
    {1, 0, 1},
    {0, 0, 0},
    {1, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {0, 0, 0}
};

const uint8_t char_0xE2809C[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '“'
    {1, 0, 1},
    {1, 0, 1},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const uint8_t char_0xE2809E[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '„'
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {1, 0, 1},
    {1, 0, 1}
};

const uint8_t char_0xE282AC[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '€'
    {0, 1, 1},
    {1, 0, 0},
    {1, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 1, 1}
};

const uint8_t char_smile[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '🙂'
    {0, 2, 0},
    {3, 2, 3},
    {2, 2, 2},
    {3, 2, 3},
    {2, 3, 2},
    {0, 2, 0}
};

const uint8_t char_neutral[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '😐'
    {0, 2, 0},
    {3, 2, 3},
    {2, 2, 2},
    {3, 3, 3},
    {2, 2, 2},
    {0, 2, 0}
};

const uint8_t char_frown[FONT_HEIGHT][FONT_WIDTH] = {
    {0, 0, 0}, // '🙁'
    {0, 2, 0},
    {3, 2, 3},
    {2, 2, 2},
    {2, 3, 2},
    {3, 2, 3},
    {0, 2, 0}
};

const uint8_t (*charactermap[])[FONT_WIDTH] = {
    char_blank,    //   0
    char_unknown,  //   1
    char_unknown,  //   2
    char_unknown,  //   3
    char_unknown,  //   4
    char_unknown,  //   5
    char_unknown,  //   6
    char_unknown,  //   7
    char_unknown,  //   8
    char_blank,    //   9
    char_blank,    //  10
    char_unknown,  //  11
    char_unknown,  //  12
    char_blank,    //  13
    char_unknown,  //  14
    char_blank,    //  15
    char_unknown,  //  16
    char_unknown,  //  17
    char_unknown,  //  18
    char_unknown,  //  19
    char_unknown,  //  20
    char_unknown,  //  21
    char_unknown,  //  22
    char_unknown,  //  23
    char_unknown,  //  24
    char_unknown,  //  25
    char_unknown,  //  26
    char_unknown,  //  27
    char_unknown,  //  28
    char_unknown,  //  29
    char_unknown,  //  30
    char_unknown,  //  31
    char_blank,    //  32
    char_33,
    char_34,
    char_35,
    char_36,
    char_37,
    char_38,
    char_39,
    char_40,
    char_41,
    char_42,
    char_43,
    char_44,
    char_45,
    char_46,
    char_47,
    char_48,
    char_49,
    char_50,
    char_51,
    char_52,
    char_53,
    char_54,
    char_55,
    char_56,
    char_57,
    char_58,
    char_59,
    char_60,
    char_61,
    char_62,
    char_63,
    char_64,
    char_65,
    char_66,
    char_67,
    char_68,
    char_69,
    char_70,
    char_71,
    char_72,
    char_73,
    char_74,
    char_75,
    char_76,
    char_77,
    char_78,
    char_79,
    char_80,
    char_81,
    char_82,
    char_83,
    char_84,
    char_85,
    char_86,
    char_87,
    char_88,
    char_89,
    char_90,
    char_91,
    char_92,
    char_93,
    char_94,
    char_95,
    char_96,
    char_97,
    char_98,
    char_99,
    char_100,
    char_101,
    char_102,
    char_103,
    char_104,
    char_105,
    char_106,
    char_107,
    char_108,
    char_109,
    char_110,
    char_111,
    char_112,
    char_113,
    char_114,
    char_115,
    char_116,
    char_117,
    char_118,
    char_119,
    char_120,
    char_121,
    char_122,
    char_123,
    char_124,
    char_125,
    char_126,
    char_unknown  // 127
};
