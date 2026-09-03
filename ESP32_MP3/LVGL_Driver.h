#pragma once

#include <lvgl.h>
#include "lv_conf.h"
#include <esp_heap_caps.h>
#include "Display_ST7701.h"
#include "Touch_CST820.h"

#define LCD_WIDTH     ESP_PANEL_LCD_WIDTH
#define LCD_HEIGHT    ESP_PANEL_LCD_HEIGHT
#define LVGL_BUF_LEN  (ESP_PANEL_LCD_WIDTH * ESP_PANEL_LCD_HEIGHT / 5)

#define EXAMPLE_LVGL_TICK_PERIOD_MS  2

// LVGL v9 uses lv_display_t and lv_indev_t instead of the old _drv_t structs
extern lv_display_t * disp;
extern lv_indev_t * indev;

#ifdef __cplusplus
extern "C" {
#endif

void Lvgl_print(const char * buf);

// Updated v9 signatures
void Lvgl_Display_LCD(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);
void Lvgl_Touchpad_Read(lv_indev_t * indev, lv_indev_data_t * data);
void example_increase_lvgl_tick(void *arg);

void Lvgl_Init(void);
void Lvgl_Loop(void);

#ifdef __cplusplus
}
#endif