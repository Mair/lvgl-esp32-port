#include "lvgl.h"
#include "process_x.h"

void proc_x_timer_update(lv_timer_t *timer)
{
    lv_obj_t *value_from_proc_x = (lv_obj_t *)lv_timer_get_user_data(timer);
    receive_payload_t receive_payload = {};
    receive_from_process_x(&receive_payload);
    lv_label_set_text_fmt(value_from_proc_x, "%d", receive_payload.value);
}

void process_x_ui(void)
{
    lv_obj_t *value_from_proc_x = lv_label_create(lv_screen_active());
    lv_obj_align(value_from_proc_x, LV_ALIGN_TOP_MID, 0, 10);
    lv_label_set_text(value_from_proc_x, "loading...");
    lv_timer_create(proc_x_timer_update, 100, value_from_proc_x);
}