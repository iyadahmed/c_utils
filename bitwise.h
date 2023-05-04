#pragma once

#define SET_BIT(value, pos) ((value) |= (1UL << (pos)))
#define CLEAR_BIT(value, pos) ((value) &= (~(1UL << (pos))))
#define GET_BIT(value, pos) ((value) & (1UL << (pos)))