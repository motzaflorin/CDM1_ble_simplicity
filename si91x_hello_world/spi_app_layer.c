#include "app.h"
#include <stdio.h>

#include "sl_si91x_ssi.h"
#include "spi_app_layer.h"
#include "sl_si91x_ssi_primary_common_config.h"
#include "sl_si91x_ssi_primary_config.h"


static   void my_ssi_event_cb(uint32_t event);

void init_spi_slave(void) {
  printf("Initialiaze SPI from SSI\n");
  sl_status_t status;
  sl_ssi_handle_t ssi_handle = 0;
  const char *data_out = "Some data";
  char data_in[10];

  // Configure as SPI master first
  status = sl_si91x_ssi_init(SL_SSI_MASTER_ACTIVE, &ssi_handle);
  if (status != SL_STATUS_OK) {
    printf("Init Failed\n");
  }

  // set power and config
  status = sl_si91x_ssi_set_configuration(
      ssi_handle,
    &ssi_primary_configuration,  // structure from sl_ssi_instances.h
    SSI_SLAVE_0                 // as of now use 0
  );
  if (status != SL_STATUS_OK) {
      printf("Set Config Failed\n");
  }


  sl_si91x_ssi_register_event_callback(ssi_handle, (sl_ssi_signal_event_t) my_ssi_event_cb);

// TRANSFER DATA
  RSI_SPI_SetSlaveSelectNumber(0);  // Use the correct CS number for your board
  status =  sl_si91x_ssi_transfer_data(ssi_handle,
                                         (const void*)data_out,
                                         (void *) data_in,
                                         strlen(data_out));
  if (status != SL_STATUS_OK) {
         printf("Transfer Failed \%ld\n", status);
     }

//  printf("Data to send: %s\r\n",data_out);
  printf("\n\n");
  printf("Data received: %s\n\r",data_in);




// SEND DATA
// sl_si91x_ssi_send_data(ssi_handle, (const void*)data_out, strlen(data_out));
// if (status != SL_STATUS_OK) {
//         printf("SEND Failed \%ld\n", status);
//     }
// else {
//       printf("Data sent: %s\r\n",data_out);
//   }

}

// callback register
 static void my_ssi_event_cb(uint32_t event)
  {
    if (event & ARM_SPI_EVENT_TRANSFER_COMPLETE) {
//      uint32_t transfer_done = true;
      printf("transfer done\n");
    }

    if (event & ARM_SPI_EVENT_DATA_LOST) {
      // Handle data lost error
    }

    if (event & ARM_SPI_EVENT_MODE_FAULT) {
      // Handle mode fault
    }
  }
