/*******************************************************************************
 * @file    gpio example.c
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
/**============================================================================
 * @brief : This file contains example application for GPIO example
 * @section Description :
 * This application demonstrates toggling, pin, group interrupts of GPIO pin
  for HP, ULP and UULP instances.
 ============================================================================**/
#include "rsi_debug.h"
#include "sl_si91x_peripheral_gpio.h"
#include "gpio_example.h"

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/
#define PORT0 0 // GPIO Port 0

#define PAD_SELECT_9   9  // GPIO PAD selection number
#define MAX_PAD_SELECT 34 // Maximum GPIO PAD selection number
// [DEV BOARD testing] P24 pin is not soldered correctly and needs a slight finger pressure to make the contact
#define OUTPUT_TRIG_PIN 46 // Pin number 10 port 0 for led
#define OUTPUT_VALUE 1 // GPIO output value

#define MS_DELAY_COUNTER 4600 // Delay count
#define DELAY            1000 // Delay for 1sec
#define FALL_EDGE        8    // Fall edge event


/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
// Enum for enabling GPIO instances functionality
typedef enum {
  M4_GPIO_PIN         = 1, // Initialize GPIO HP instance
  ULP_GPIO_PIN        = 0, // Initialize GPIO ULP instance
  UULP_GPIO_PIN       = 0, // Initialize GPIO UULP instance
  M4_GPIO_PIN_INTR    = 0, // Configure GPIO HP instance pin interrupt
  M4_GPIO_GROUP_INTR  = 0, // Configure GPIO HP instance group interrupt
  M4_GPIO_PORT        = 0, // Set GPIO port output
  ULP_GPIO_PIN_INTR   = 0, // Configure GPIO ULP instance pin interrupt
  ULP_GPIO_GROUP_INTR = 0, // Configure GPIO ULP instance group interrupt
  UULP_GPIO_PIN_INTR  = 0, // Configure GPIO UULP instance pin interrupt //check
} gpio_instance_type_t;

/*******************************************************************************
 *************************** LOCAL VARIABLES   *********************************
 ******************************************************************************/
//static sl_si91x_gpio_group_interrupt_config_t config_grp_int;
uint8_t pad_sel = 1;

/*******************************************************************************
 **********************  Local Function prototypes   ***************************
 ******************************************************************************/
static void delay(uint32_t idelay);
static void sl_gpio_initialization(void);


/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/
/*******************************************************************************
 * GPIO example initialization function. It initializes HP/ULP clock, pin mode,
 * direction and configure pin and group interrupts
 ******************************************************************************/
void gpio_example_init(void)
{
  uint8_t get_pin;

  // Initialize GPIO HP instance
  sl_gpio_initialization();

  // Set single GPIO pin to '1'
  sl_gpio_set_pin_output(PORT0, OUTPUT_TRIG_PIN);
  // Get single GPIO pin value '0' or '1'
  get_pin = sl_gpio_get_pin_output(PORT0, OUTPUT_TRIG_PIN);
  DEBUGOUT("get_pin_output = %d\n", get_pin);
  // Clear GPIO pin value
  sl_gpio_clear_pin_output(PORT0, OUTPUT_TRIG_PIN);

 }
/*******************************************************************************
 * Function will run continuously in while loop and led toggles for every 1sec
 ******************************************************************************/
void gpio_example_process_action(void)
{
  // Initialize GPIO HP instance
  if (M4_GPIO_PIN == SET) {
    sl_gpio_toggle_pin_output(PORT0, OUTPUT_TRIG_PIN); // Toggle GPIO pin
    delay(DELAY);                            // Delay of 1sec
  }
  printf("GPIO toggle\r\n");
}

/*******************************************************************************
 * Delay function for 1ms
 ******************************************************************************/
static void delay(uint32_t idelay)
{
  for (uint32_t x = 0; x < MS_DELAY_COUNTER * idelay; x++) {
    __NOP();
  }
}

/*******************************************************************************
 * GPIO initialization function
 ******************************************************************************/
static void sl_gpio_initialization(void)
{
  DEBUGOUT("\r\n M4_GPIO_PIN test starts \r\n");
  sl_si91x_gpio_enable_clock((sl_si91x_gpio_select_clock_t)M4CLK_GPIO); // Enable GPIO M4_CLK

  // Enable pad selection for GPIO pins
  // PadSelection-9 related GPIOS are pre-configured for Other Functions
  for (pad_sel = 1; pad_sel < MAX_PAD_SELECT; pad_sel++) {
    if (pad_sel != PAD_SELECT_9) {
      sl_si91x_gpio_enable_pad_selection(pad_sel);
    }
  }
  // Enable pad receiver for GPIO pins
  sl_si91x_gpio_enable_pad_receiver(OUTPUT_TRIG_PIN);
  // Set the pin mode for GPIO pins.
  sl_gpio_set_pin_mode(PORT0, OUTPUT_TRIG_PIN, _MODE0, OUTPUT_VALUE);

  // Select the direction of GPIO pin whether Input/ Output
  sl_si91x_gpio_set_pin_direction(PORT0, OUTPUT_TRIG_PIN, (sl_si91x_gpio_direction_t)GPIO_OUTPUT);
}

