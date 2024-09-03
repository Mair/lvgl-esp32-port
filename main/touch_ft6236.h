#ifndef TOUCH_FT6236_H
#define TOUCH_FT6236_H

#include <stdint.h>

typedef struct touch_t
{
    uint16_t x;
    uint16_t y;
    bool is_touched;
} touch_t;

void init_touch(void);
void get_touch_data(touch_t *touch);

#endif