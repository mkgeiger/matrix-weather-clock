#ifndef _FONT_H
#define _FONT_H

#include <c_types.h>

#define FONT_WIDTH  3
#define FONT_HEIGHT 7

extern const uint8_t char_unknown[FONT_HEIGHT][FONT_WIDTH];

// 1-byte UTF-8 codes
extern const uint8_t (*charactermap[])[FONT_WIDTH];

// 2-byte UTF-8 codes
extern const uint8_t char_0xC2A3[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2A7[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2A9[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2AB[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2B0[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2B1[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2B2[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2B3[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2B4[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2B5[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC2BB[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC384[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC396[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC397[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC39C[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC39F[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC3A4[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC3B6[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC3B7[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xC3BC[FONT_HEIGHT][FONT_WIDTH];

// 3-byte UTF-8 codes
extern const uint8_t char_0xE2809C[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xE2809E[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_0xE282AC[FONT_HEIGHT][FONT_WIDTH];

// 4-byte UTF-8 codes
extern const uint8_t char_smile[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_neutral[FONT_HEIGHT][FONT_WIDTH];
extern const uint8_t char_frown[FONT_HEIGHT][FONT_WIDTH];

#endif
