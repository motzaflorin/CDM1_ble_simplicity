#include "sl_event_handler.h"

#include "system_si91x.h"
#include "sl_si91x_clock_manager.h"
#include "rsi_board.h"
#include "rsi_debug.h"
#include "sl_sleeptimer.h"
#include "sl_si91x_button_instances.h"
#include "sl_si91x_led_instances.h"

void sl_platform_init(void)
{
  SystemCoreClockUpdate();
  sl_si91x_clock_manager_init();
  RSI_Board_Init();
  DEBUGINIT();
}

void sl_driver_init(void)
{
  button_init_instances();
  led_init_instances();
}

void sl_service_init(void)
{
  sl_sleeptimer_init();
}

void sl_stack_init(void)
{
}

void sl_internal_app_init(void)
{
}

void sl_platform_process_action(void)
{
}

void sl_service_process_action(void)
{
}

void sl_stack_process_action(void)
{
}

void sl_internal_app_process_action(void)
{
}

