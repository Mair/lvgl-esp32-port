#include "driver/i2c_master.h"
#include "touch_ft6236.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "math.h"

#define CTP_SDA GPIO_NUM_12
#define CTP_SCL GPIO_NUM_14
#define FT6236_ADDRESS 0X38

static i2c_master_dev_handle_t touch_handel;

void init_touch(void)
{
    i2c_master_bus_config_t i2c_master_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
        .sda_io_num = CTP_SDA,
        .scl_io_num = CTP_SCL};

    i2c_master_bus_handle_t i2c_master_bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_bus_config, &i2c_master_bus_handle));

    i2c_device_config_t i2c_device_config = {
        .scl_speed_hz = 100000,
        .device_address = FT6236_ADDRESS};

    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_master_bus_handle, &i2c_device_config, &touch_handel));
}

void get_touch_data(touch_t *touch)
{
    const uint8_t x_high_reg = 0x03;
    const uint8_t x_low_reg = 0x04;
    const uint8_t y_high_reg = 0x05;
    const uint8_t y_low_reg = 0x06;

    uint8_t x_high_val = 0;
    uint8_t x_low_val = 0;
    uint8_t y_high_val = 0;
    uint8_t y_low_val = 0;

    i2c_master_transmit_receive(touch_handel, &x_high_reg, 1, &x_high_val, 1, -1);
    i2c_master_transmit_receive(touch_handel, &x_low_reg, 1, &x_low_val, 1, -1);
    i2c_master_transmit_receive(touch_handel, &y_high_reg, 1, &y_high_val, 1, -1);
    i2c_master_transmit_receive(touch_handel, &y_low_reg, 1, &y_low_val, 1, -1);

    bool is_touched = x_high_val & 0b01000000; // 0x40
    uint16_t raw_x = ((x_high_val & 0x0f) << 8) | x_low_val;
    uint16_t raw_y = ((y_high_val & 0x0f) << 8) | y_low_val;

    touch->is_touched = !is_touched;
    touch->y = raw_x;
    touch->x = abs(320 - raw_y);
}