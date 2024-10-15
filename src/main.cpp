#include <Arduino.h>
#include <lvgl.h>
#include "gfx4desp32_gen4_ESP32_43T.h"

gfx4desp32_gen4_ESP32_43T gfx = gfx4desp32_gen4_ESP32_43T();


//Based on code from https://forum.4dsystems.com.au/node/80632

//#define LCD_DOUBLE_BUF

static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_color_t *disp_draw_buf1;
static lv_color_t *disp_draw_buf2;
static unsigned long last_ms;
static lv_display_t *my_disp;

void init_ui(void);

uint32_t my_tick_cb(void)
{
  return (uint32_t)millis();
}

/* Display flushing */
void my_disp_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
	uint32_t numPixels = lv_area_get_size(area);
  gfx.SetGRAM(area->x1, area->y1, area->x2, area->y2);
  gfx.pushColors((uint16_t *)color_p, numPixels);
  lv_display_flush_ready(disp);
}

void my_touchpad_read(lv_indev_t * indev, lv_indev_data_t * data)
{
  if (gfx.touch_Update())
  {
    int touchStatus = gfx.touch_GetPen();
    if (touchStatus == 1)
    {
      data->state = LV_INDEV_STATE_PR;
      data->point.x = gfx.touch_GetX();
      data->point.y = gfx.touch_GetY();
    }
    else if (touchStatus == 2) data->state = LV_INDEV_STATE_REL;
  }
  else data->state = LV_INDEV_STATE_REL;
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("LVGL Template");

  pinMode(GEN4_RGB_PIN_NUM_BK_LIGHT, OUTPUT);
  digitalWrite(GEN4_RGB_PIN_NUM_BK_LIGHT, HIGH);

  gfx.begin(16);
  gfx.Cls();
  gfx.Orientation(PORTRAIT);
  gfx.touch_Set(TOUCH_ENABLE);
  gfx.BacklightOn(true);

  lv_init();
  lv_tick_set_cb(my_tick_cb);
  
  screenWidth = gfx.getWidth();
  screenHeight = gfx.getHeight();

  /* Initialize the display */
  my_disp = lv_display_create(screenWidth, screenHeight);
  uint32_t bufsize = screenWidth * 128 * lv_color_format_get_size(lv_display_get_color_format(NULL));
  
  disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(bufsize, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf1) Serial.println("LVGL disp_draw_buf1 allocate failed!");
  else
  {
#ifdef LCD_DOUBLE_BUF
    disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(bufsize, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (!disp_draw_buf2) Serial.println("LVGL disp_draw_buf2 allocate failed!");
    else
    {
#endif

      lv_display_set_flush_cb(my_disp, my_disp_flush_cb);
      lv_display_set_buffers(my_disp, disp_draw_buf1, disp_draw_buf2, bufsize, LV_DISP_RENDER_MODE_PARTIAL);  //LV_DISP_RENDER_MODE_PARTIAL

      /* Initialize the input device driver */
      static lv_indev_t * indev = lv_indev_create();
      lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
      lv_indev_set_read_cb(indev, my_touchpad_read);

      init_ui();

      Serial.println("Setup done");

#ifdef LCD_DOUBLE_BUF
    }
#endif

  }
  last_ms = millis();
}

void loop() {
  delay(5);
  lv_timer_handler();
  }

//Adapted from https://docs.lvgl.io/master/examples.html
void init_ui()
  {
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

    /*Create a white label, set its text and align it to the center*/
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  }