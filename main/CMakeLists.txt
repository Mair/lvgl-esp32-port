idf_component_register(
SRCS 
    "main.c"
    "tft_display.c"
    "lvgl_src/sample.c"
    "lvgl_src/send_receive_proc_x.c"
    "process_x.c"
    "touch_ft6236.c"
INCLUDE_DIRS 
    "."
    "lvgl_src"
)

idf_build_set_property(COMPILE_OPTIONS "-DLV_CONF_INCLUDE_SIMPLE=1" APPEND )
idf_build_set_property(COMPILE_OPTIONS "-DLV_CONF_PATH=${CMAKE_SOURCE_DIR}/main/lv_conf.h" APPEND )