#include "lvgl.h"
#include "process_x.h"

void proc_x_timer_update(lv_timer_t *timer)
{
    lv_obj_t *value_from_proc_x = (lv_obj_t *)lv_timer_get_user_data(timer);
    receive_payload_t receive_payload = {};
    receive_from_process_x(&receive_payload);
    lv_label_set_text_fmt(value_from_proc_x, "%d", receive_payload.value);
}

void on_slide_change(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int val = lv_slider_get_value(slider);
    send_payload_t send_payload = {.value = val};
    send_to_process_x(&send_payload);
}

void process_x_ui(void)
{
    // lv_obj_t *value_from_proc_x = lv_label_create(lv_screen_active());
    // lv_obj_align(value_from_proc_x, LV_ALIGN_TOP_MID, 0, 10);
    // lv_label_set_text(value_from_proc_x, "loading...");
    // lv_timer_create(proc_x_timer_update, 100, value_from_proc_x);

    // lv_obj_t *slider = lv_slider_create(lv_screen_active());
    // lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 40);
    // lv_obj_add_event_cb(slider, on_slide_change, LV_EVENT_VALUE_CHANGED, NULL);

    static lv_point_precise_t points[] = {{100, 0}, {0, 100}, {200, 100}, {100, 0}};

    static lv_style_t line_style;
    lv_style_init(&line_style);
    lv_style_set_line_width(&line_style, 12);
    lv_style_set_line_color(&line_style, lv_palette_main(LV_PALETTE_INDIGO));
    lv_style_set_line_rounded(&line_style, true);

    lv_obj_t *line = lv_line_create(lv_screen_active());
    lv_line_set_points(line, points, 4);
    lv_obj_add_style(line, &line_style, 0);
    lv_obj_center(line);
}