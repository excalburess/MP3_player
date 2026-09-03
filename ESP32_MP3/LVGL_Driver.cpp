#include "LVGL_Driver.h"

// LVGL v9 Global Handles
lv_display_t * disp = NULL;
lv_indev_t * indev = NULL;

void* buf1 = NULL;
void* buf2 = NULL;

/* Serial debugging */
void Lvgl_print(const char * buf)
{
    // Serial.printf(buf);
    // Serial.flush();
}

/* Display flushing
   Displays LVGL content on the LCD
   This function implements associating LVGL data to the LCD screen
*/
void Lvgl_Display_LCD(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    // In v9, px_map is already a raw byte pointer, so we pass it directly
    LCD_addWindow(area->x1, area->y1, area->x2, area->y2, px_map);
    lv_display_flush_ready(disp);
}

/* Read the touchpad */
void Lvgl_Touchpad_Read(lv_indev_t * indev, lv_indev_data_t * data)
{
    Touch_Read_Data();
    if (touch_data.points != 0x00) {
        data->point.x = touch_data.x;
        data->point.y = touch_data.y;
        data->state = LV_INDEV_STATE_PR;
        printf("LVGL : X=%u Y=%u points=%d\r\n", touch_data.x, touch_data.y, touch_data.points);
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    
    // CST820 Gesture clearing (if used)
    if (touch_data.gesture != NONE ) {    
    }
    
    touch_data.x = 0;
    touch_data.y = 0;
    touch_data.points = 0;
    touch_data.gesture = NONE;
}

void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

void Lvgl_Init(void)
{
    lv_init();
    
    // Retrieve RGB frame buffers allocated by ESP-IDF LCD driver
    // (Ensure panel_handle is declared in your Display_ST7701.h)
    esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, &buf1, &buf2);                                         
    
    // 1. Create the display in v9
    disp = lv_display_create(LCD_HEIGHT, LCD_WIDTH);
    
    // 2. Set buffers (v9 requires buffer size in BYTES, not pixel count)
    uint32_t buf_size_bytes = LCD_WIDTH * LCD_HEIGHT * sizeof(lv_color_t);
    lv_display_set_buffers(disp, buf1, buf2, buf_size_bytes, LV_DISPLAY_RENDER_MODE_FULL);
    
    // 3. Register flush callback
    lv_display_set_flush_cb(disp, Lvgl_Display_LCD);

    // 4. Create and initialize the input device (Touchpad)
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, Lvgl_Touchpad_Read);

    /* Create simple label to test display */
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello Arduino and LVGL v9!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // Setup LVGL tick timer
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &example_increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
    esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);
}

void Lvgl_Loop(void)
{
    lv_timer_handler(); /* let the GUI do its work */
    // vTaskDelay(pdMS_TO_TICKS(5)); // Uncomment if yielding is needed for RTOS watchdog
}