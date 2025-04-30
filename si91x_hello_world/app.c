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

/*******************************************************************************
 * Initialize application.
 ******************************************************************************/

SemaphoreHandle_t spi_rx_sem;

void app_init(void)
{
  // =========== GPIO INIT =============
  gpio_example_init();
  //===================
  printf("Hello World!\r\n");
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


