#include "app.h"
#include <stdio.h>

#include "sl_si91x_ssi.h"
#include "spi_app_layer.h"
#include "sl_si91x_ssi_primary_common_config.h"
#include "sl_si91x_ssi_primary_config.h"



#include "sl_sleeptimer.h"

static   void my_ssi_event_cb(uint32_t event);
void arm_spi_slave(void);


uint8_t data_in[10] = {0xaa};
uint8_t data_out[10] = {0xcc};
sl_ssi_handle_t ssi_handle = 0;
volatile bool transfer_done = false;


// callback register
 static void my_ssi_event_cb(uint32_t event)
  {
    if (event & ARM_SPI_EVENT_TRANSFER_COMPLETE) {
      printf("transfer done\r\n");
      for(uint8_t i = 0; i < sizeof(data_in) / sizeof(data_in[0]); i++)
        {
          printf("Data received: %x\r\n",data_in[i]);
          data_in[i] = 0;
        }
      transfer_done = true;

      read_spi();
//      send_spi();
    }

    if (event & ARM_SPI_EVENT_DATA_LOST) {
      // Handle data lost error
    }

    if (event & ARM_SPI_EVENT_MODE_FAULT) {
      // Handle mode fault
    }
  }


 // slave mode

 void init_spi_slave(void)
 {
   printf("Initialiaze SPI SLAVE from SSI\r\n");
   sl_status_t status;

//   const char *data_out = "Some data";

//   uint32_t length

   // Configure as SPI SLAVE
   status = sl_si91x_ssi_init(SL_SSI_SLAVE_ACTIVE, &ssi_handle);
   if (status != SL_STATUS_OK) {
     printf("Init Failed\n");
   }

   // set power and config
   status = sl_si91x_ssi_set_configuration(
       ssi_handle,
     &ssi_secondary_configuration,  // structure from sl_ssi_instances.h
     SSI_SLAVE_0                 // as of now use 0
   );
   if (status != SL_STATUS_OK) {
       printf("Set Config Failed\r\\n");
   }


   sl_si91x_ssi_register_event_callback(ssi_handle, (sl_ssi_signal_event_t) my_ssi_event_cb);

////  TRANSFER DATA
   RSI_SPI_SetSlaveSelectNumber(0);  // Use the correct CS number - by default it uses some multislave config option

   // Initial slave arm to receive data before master sends anything
//   arm_spi_slave();  // prepare for next transaction
   read_spi();
//   send_spi();

 }
void read_spi()
{
  sl_status_t status;
  status = sl_si91x_ssi_receive_data(ssi_handle, (void *) data_in, sizeof(data_in));
  if (status != SL_STATUS_OK) {
            printf("RECEIVE Failed \%ld\r\n", status);
        }
}

void arm_spi_slave()
{
  sl_status_t status;
  // Arm the slave for full-duplex transfer
  status = sl_si91x_ssi_transfer_data(ssi_handle, data_out, data_in, sizeof(data_in));
  if (status != SL_STATUS_OK) {
      printf("TRANSFER arm failed: %ld\r\n", status);
  }
}
void send_spi()
{
  sl_status_t status;
  status = sl_si91x_ssi_send_data(ssi_handle, (void *) data_out, sizeof(data_out));
  if (status != SL_STATUS_OK) {
            printf("SEND Failed \%ld\r\n", status);
        }
}
