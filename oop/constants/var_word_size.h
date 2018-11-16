#ifndef __VAR_WORD_SIZE__
#define __VAR_WORD_SIZE__

#include "arch_const.h"

#if WORD_SIZE == 16
typedef int16_t word_t;
#else
typedef int32_t word_t;
#endif

#endif