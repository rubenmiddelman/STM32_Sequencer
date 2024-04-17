#include "LEDs.h"

uint8_t LED_Data[MAX_LEDS][4];
uint8_t LED_Mod[MAX_LEDS][4];
uint16_t pwmData[(24 * MAX_LEDS) + 50];
int datasentflag = 1;

void Set_LED(uint8_t LED_Number, uint8_t red, uint8_t green, uint8_t blue) {
  LED_Data[LED_Number][0] = LED_Number;
  LED_Data[LED_Number][1] = green;
  LED_Data[LED_Number][2] = red;
  LED_Data[LED_Number][3] = blue;
}

#define PI 3.14159265

void Set_LEDS_Off() {
  for (int i = 0; i < MAX_LEDS; i++) {
    for (int j = 0; j < 4; j++) {
      if (j == 0) {
        LED_Data[i][0] = i;
      }
      LED_Data[i][j] = 0;
    }
  }
}

void Set_LEDs_Note_List(uint8_t *notes_On_Or_Off) {
  for (int i = 0; i < 8; i++) {
    if (notes_On_Or_Off[i]) {
      Set_LED(i, 0, 255, 0);
    } else {
      Set_LED(i, 255, 0, 0);
    }
  }
}

void Set_Brightness(int brightness) // 0-45
{
#if USE_BRIGHTNESS

  if (brightness > 45)
    brightness = 45;
  for (int i = 0; i < MAX_LEDS; i++) {
    LED_Mod[i][0] = LED_Data[i][0];
    for (int j = 1; j < 4; j++) {
      float angle = 90 - brightness; // in degrees
      angle = angle * PI / 180;      // in rad
      LED_Mod[i][j] = (LED_Data[i][j]) / (tan(angle));
    }
  }

#endif
}

void WS2812_Send(void) {
  uint32_t indx = 0;
  uint32_t color;

  for (int i = 0; i < MAX_LEDS; i++) {
#if USE_BRIGHTNESS
    color = ((LED_Mod[i][1] << 16) | (LED_Mod[i][2] << 8) | (LED_Mod[i][3]));
#else
    color = ((LED_Data[i][1] << 16) | (LED_Data[i][2] << 8) | (LED_Data[i][3]));
#endif

    for (int i = 23; i >= 0; i--) {
      if (color & (1 << i)) {
        pwmData[indx] = 60; // 2/3 of 90
      }

      else
        pwmData[indx] = 30; // 1/3 of 90

      indx++;
    }
  }

  for (int i = 0; i < 50; i++) {
    pwmData[indx] = 0;
    indx++;
  }

  HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);
  HAL_Delay(1);
  while (!datasentflag) {
  };
  datasentflag = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
  datasentflag = 1;
}
