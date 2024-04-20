/**
 * @ Author: Ruben Middelman
 * @ Create Time: 2024-04-16 21:31:01
 * @ Modified by: Ruben Middelman
 * @ Modified time: 2024-04-20 03:17:19
 * @ Description: Driver for WS2812b LED's, code found partly online
 */
#include "LEDs.h"

// seeing that we are not using brightness right now we dont really need pi but
// it is nice if we want to set the birghtness later

#define PI 3.14159265

uint8_t LED_Data[MAX_LEDS][4];
uint8_t LED_Mod[MAX_LEDS][4];
uint16_t pwmData[(24 * MAX_LEDS) + 50];
int datasentflag = 1;

/**
 * @brief Set LEDS function, Sets LED
 *
 * TODO: error handling for when LED outisde of boundry is set
 * @param LED_Number
 * @param red
 * @param green
 * @param blue
 */
void Set_LED(uint8_t LED_Number, uint8_t red, uint8_t green, uint8_t blue) {
  LED_Data[LED_Number][0] = LED_Number;
  LED_Data[LED_Number][1] = green;
  LED_Data[LED_Number][2] = red;
  LED_Data[LED_Number][3] = blue;
}

/**
 * @brief Turns off LED's
 *
 */
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

/**
 * @brief Sets the LEDs tot he note list (on or off)
 *
 * TODO: might be nice to let users select color of on or off LED's
 *
 * @param notes_On_Or_Off
 */
void Set_LEDs_Note_List(uint8_t *notes_On_Or_Off) {
  for (int i = 0; i < 8; i++) {
    if (notes_On_Or_Off[i]) {
      Set_LED(i, 0, 255, 0);
    } else {
      Set_LED(i, 255, 0, 0);
    }
  }
}

/**
 * @brief Sets brightness for LED's
 *
 * @param brightness value between 0 and 45 (half of 90 for clock niceness)
 */
void Set_Brightness(int brightness) {
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

/**
 * @brief Sends the set LED data to the LED's via PWM via timer 1
 * also uses DMA to free up CPU
 *
 */
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

/**
 * @brief timer PWM Callback to set flag high
 *
 * @param htim
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
  datasentflag = 1;
}
