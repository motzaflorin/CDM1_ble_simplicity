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
extern sl_ssi_control_config_t ssi_secondary_configuration;

void init_spi_master(void);
void init_spi_slave(void);
void read_spi(void);
void send_spi(void);
void arm_spi_slave(void);



#endif /* SPI_APP_LAYER_H_ */
