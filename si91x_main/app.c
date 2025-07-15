/***************************************************************************/ /**
 * @file app.c
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "app.h"
#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "spi_app_layer.h"
#include "gpio_example.h"
#include "cmsis_os2.h" // needed for threads
#include "BLE_services.h"

/*******************************************************************************
 * Initialize application.
 ******************************************************************************/

SemaphoreHandle_t spi_rx_sem;
extern void ble_heart_rate_gatt_server(void *argument);

void app_init(void)
{
  const osThreadAttr_t thread_attributes = {
    .name       = "application_thread",
    .attr_bits  = 0,
    .cb_mem     = 0,
    .cb_size    = 0,
    .stack_mem  = 0,
    .stack_size = 3072,
    .priority   = osPriorityNormal,
    .tz_module  = 0,
    .reserved   = 0,
  };
  // =========== BLE INIT & ADVERTISE =============
  osThreadNew((osThreadFunc_t)ble_heart_rate_gatt_server, NULL, &thread_attributes);

  // =========== GPIO INIT =============
  gpio_example_init();
  //===================
  printf("Hello World!\r\n");
  // TASK notification for wake line

  // Semaphore and task created for SPI 0 latency
  spi_rx_sem = xSemaphoreCreateBinary();
  if (!spi_rx_sem) {
      printf("Failed to create SPI RX semaphore\r\n");
      while (1);
  }

  xTaskCreate(spi_rx_task, "SPI_RX", SPI_RX_TASK_STACK_SIZE, NULL, SPI_RX_TASK_PRIORITY, NULL);


  init_spi_slave();  // Start SPI and arm first RX
}

/*******************************************************************************
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}


