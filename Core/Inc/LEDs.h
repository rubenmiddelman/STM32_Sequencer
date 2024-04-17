
#include "main.h"
#include "math.h"

#define MAX_LEDS 8
#define USE_BRIGHTNESS 0
extern TIM_HandleTypeDef htim1;

void Set_LED(uint8_t LED_Number, uint8_t red, uint8_t green, uint8_t blue);
void Set_LEDS_Off();
void Set_Brightness(int brightness);
void Set_LEDs_Note_List(uint8_t *notes_On_Or_Off);
void WS2812_Send();