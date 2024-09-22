#include <stdio.h>
#include "tft_display.h"
#include "lvgl.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_timer.h"
#include "lvgl_src/sample.h"
#include "touch_ft6236.h"
#include "send_receive_proc_x.h"
#include "process_x.h"

#define LCD_H_RES 320
#define LCD_V_RES 240
#define LVGL_TICK_PERIOD_MS 2

static void on_pointer(lv_indev_t *indev, lv_indev_data_t *data)
{
    touch_t touch;
    get_touch_data(&touch);
    data->point.x = touch.x;
    data->point.y = touch.y;
    data->state = touch.is_touched ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void my_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map)
{
    esp_lcd_panel_handle_t *panel_handle_ptr = (esp_lcd_panel_handle_t *)lv_display_get_user_data(display);
    esp_lcd_panel_handle_t panel_handle = *panel_handle_ptr;

    lv_draw_sw_rgb565_swap(px_map, lv_area_get_size(area));
    esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map);
    lv_disp_flush_ready(display);
}

static void lvgl_tick(void *arg)
{
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

void app_main(void)
{
    process_x();

    init_touch();

    esp_lcd_panel_handle_t *panel_handle = init_display();

    lv_init();
    lv_display_t *lv_display = lv_display_create(LCD_H_RES, LCD_V_RES);

    uint32_t buf_size = LCD_H_RES * LCD_V_RES * 2 / 10;
    lv_color_t *buf1 = heap_caps_malloc(buf_size, MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(buf_size, MALLOC_CAP_DMA);
    assert(buf2);

    lv_display_set_buffers(lv_display, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_resolution(lv_display, LCD_H_RES, LCD_V_RES);
    lv_display_set_user_data(lv_display, panel_handle);
    lv_display_set_flush_cb(lv_display, my_flush_cb);

    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .name = "lvgl_tick",
        .callback = lvgl_tick};
    esp_timer_handle_t lvgl_tick_timer = NULL;
    esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
    esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000);

    lv_indev_t *pointer = lv_indev_create();
    lv_indev_set_type(pointer, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(pointer, on_pointer);

    lv_obj_t *icon = lv_label_create(lv_screen_active());
    lv_label_set_text(icon, LV_SYMBOL_UP);
    lv_obj_set_style_text_color(icon, lv_palette_main(LV_PALETTE_PINK), 0);

    lv_indev_set_cursor(pointer, icon);

    // lvgl_sample();
    process_x_ui();

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_timer_handler();
    }
}
