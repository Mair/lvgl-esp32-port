#include "driver/spi_master.h"
#include "esp_lcd_ili9341.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/gpio.h"

#define LCD_SCK_SCLK GPIO_NUM_18
#define LCD_SDI_MOSI GPIO_NUM_19
#define LCD_SDO_MISO GPIO_NUM_21
#define LCD_RS_DC GPIO_NUM_5
#define LCD_RST GPIO_NUM_3
#define LCD_CS GPIO_NUM_4
#define LCD_LED GPIO_NUM_2

#define LCD_PIXEL_CLOCK_HZ (20 * 1000 * 1000)
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8

esp_lcd_panel_handle_t *init_display(void)
{
    gpio_set_direction(LCD_LED, GPIO_MODE_OUTPUT);
    gpio_set_level(LCD_LED, 0);

    spi_bus_config_t bus_config = {
        .mosi_io_num = LCD_SDI_MOSI,
        .miso_io_num = LCD_SDO_MISO,
        .sclk_io_num = LCD_SCK_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_handle_t io_handel = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = LCD_CS,
        .dc_gpio_num = LCD_RS_DC,
        .spi_mode = 0,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &io_handel));

    static esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handel, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    // correct for our screen
    esp_lcd_panel_invert_color(panel_handle, true);
    esp_lcd_panel_swap_xy(panel_handle, true);
    esp_lcd_panel_mirror(panel_handle, true, true);

    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    gpio_set_level(LCD_LED, 1);
    return &panel_handle;
}

// // no rot
// esp_lcd_panel_swap_xy(panel_handle, false);
// esp_lcd_panel_mirror(panel_handle, true, false);

// // rot 90
// esp_lcd_panel_swap_xy(panel_handle, true);
// esp_lcd_panel_mirror(panel_handle, true, true);

// // rot 180
// esp_lcd_panel_swap_xy(panel_handle, false);
// esp_lcd_panel_mirror(panel_handle, false, true);

// // rot 270
// esp_lcd_panel_swap_xy(panel_handle, true);
// esp_lcd_panel_mirror(panel_handle, false, false);