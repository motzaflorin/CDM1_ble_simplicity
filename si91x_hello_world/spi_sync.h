/*
 * spi_sync.h
 *
 *  Created on: Apr 29, 2025
 *      Author: florin.mota
 */

#ifndef SPI_SYNC_H_
#define SPI_SYNC_H_

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t spi_rx_sem;

#endif /* SPI_SYNC_H_ */
