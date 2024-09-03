#include "lvgl.h"

void lvgl_sample(void)
{
    lv_obj_create(lv_scr_act());
    lv_obj_t *lbl = lv_label_create(lv_scr_act());
    lv_label_set_text(lbl, "Hello World!");

    lv_obj_t *btn = lv_button_create(lv_scr_act());
    lv_label_set_text(lv_label_create(btn), "click me");
    lv_obj_center(btn);

    lv_obj_t *roj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(roj, 30, 30);
    lv_obj_set_pos(roj, 100, 100);
    lv_obj_set_style_bg_color(roj, lv_color_make(0xff, 0x00, 0x00), 0);

    lv_obj_t *goj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(goj, 30, 30);
    lv_obj_set_pos(goj, 150, 150);
    lv_obj_set_style_bg_color(goj, lv_color_make(0x00, 0xff, 0x00), 0);

    lv_obj_t *bl = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bl, 30, 30);
    lv_obj_set_pos(bl, 200, 200);
    lv_obj_set_style_bg_color(bl, lv_color_make(0x00, 0x00, 0xff), 0);

    lv_log("sample complete\n");
}
