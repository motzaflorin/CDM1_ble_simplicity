/*
 * spi_app_layer.h
 *
 *  Created on: Apr 22, 2025
 *      Author: florin.mota
 */

#ifndef SPI_APP_LAYER_H_
#define SPI_APP_LAYER_H_

#include "sl_si91x_ssi.h"
extern sl_ssi_control_config_t ssi_primary_configuration;

void init_spi_slave(void);

#endif /* SPI_APP_LAYER_H_ */
