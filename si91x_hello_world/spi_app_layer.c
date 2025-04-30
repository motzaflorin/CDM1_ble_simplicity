#include "app.h"
#include <stdio.h>

#include "sl_si91x_ssi.h"
#include "spi_app_layer.h"
#include "sl_si91x_ssi_primary_common_config.h"
#include "sl_si91x_ssi_primary_config.h"
//#include "task.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "sl_sleeptimer.h"
#include "spi_sync.h"

static   void my_ssi_event_cb(uint32_t event);

//uint8_t data_in[10] = {0xaa};
uint8_t data_out[10] = { 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc };

sl_ssi_handle_t ssi_handle = 0;
volatile uint16_t ring_head, ring_tail = 0;
uint8_t ring_buf[RING_BUF_SIZE];
#define MAX_PAYLOAD_SIZE 64
uint8_t data_in[MAX_PAYLOAD_SIZE + 3];  // SPI input buffer MAX_PAYLOAD_SIZE + header + length + cehcksum
#define HEADER 0xB6

typedef enum {
    SPI_STATE_IDLE,
    SPI_STATE_TX_IN_PROGRESS,
    SPI_STATE_RX_IN_PROGRESS,
    SPI_STATE_TX_DONE,
    SPI_STATE_RX_DONE,
    SPI_STATE_ERROR
} spi_comm_state_t;
volatile spi_comm_state_t spi_state = SPI_STATE_IDLE;


typedef struct __attribute__((packed)) { // avoid paading alignment
    uint8_t header;         // header B6
    uint8_t payload_length;
    uint8_t payload[MAX_PAYLOAD_SIZE];
    uint8_t checksum;
} spi_frame_t;

typedef enum {
    WAIT_HEADER,
    WAIT_LENGTH,
    WAIT_PAYLOAD,
    WAIT_CHECKSUM
//    FRAME_COMPLETE
} spi_rx_state_t;
#define MAX_PAYLOAD_SIZE 64
spi_rx_state_t rx_state = WAIT_HEADER;
spi_frame_t rx_frame;
uint8_t rx_payload_counter = 0;

typedef struct {
    spi_rx_state_t state;
    spi_frame_t frame;
    uint8_t payload_counter;
} spi_comm_context_t;

spi_comm_context_t spi_ctx;


// ring buffer set
void ring_buffer_put(uint8_t byte) {
    ring_buf[ring_head] = byte;
    ring_head = (ring_head + 1) % RING_BUF_SIZE;
}

bool ring_buffer_get(uint8_t *byte) {
    if (ring_head == ring_tail)
        return false;
    *byte = ring_buf[ring_tail];
    ring_tail = (ring_tail + 1) % RING_BUF_SIZE;
    return true;
}

// callback handling
 static void my_ssi_event_cb(uint32_t event)
 {
     if (event & ARM_SPI_EVENT_TRANSFER_COMPLETE) {
         printf("SPI transfer done\r\n");

         // Push received bytes into the ring buffer

         for (uint8_t i = 0; i < sizeof(data_in); i++) {
             ring_buffer_put(data_in[i]);
//             printf("data received is %0x\r\n",data_in[i]);
         }

         BaseType_t xHigherPriorityTaskWoken = pdFALSE;
         xSemaphoreGiveFromISR(spi_rx_sem, &xHigherPriorityTaskWoken);
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

         // Rearm SPI
         read_spi();
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
   spi_ctx.state = WAIT_HEADER;
   spi_ctx.payload_counter = 0;
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

void send_spi()
{
  sl_status_t status;
//  memset(data_out, 0x33, sizeof(data_out));
  status = sl_si91x_ssi_send_data(ssi_handle, (void *) data_out, sizeof(data_out));
  if (status != SL_STATUS_OK) {
            printf("SEND Failed \%ld\r\n", status);
        }
}

int validate_checksum(spi_frame_t *frame)
{
  spi_frame_t temp_frame = *frame;
  uint8_t checksum = temp_frame.header ^ temp_frame.payload_length;
  for (uint8_t i = 0; i < temp_frame.payload_length; i++) {
      checksum ^= temp_frame.payload[i];
  }
  if (temp_frame.checksum == checksum){
      return 1;
  } else {
      return 0;
  }

}
void process_frame(spi_frame_t *frame)
{
  uint8_t data_buff[MAX_PAYLOAD_SIZE] = {0};
  memcpy(data_buff, &(frame->payload), frame->payload_length);
  for (uint8_t i = 0; i < frame->payload_length; i++){
      printf("Data received is %0x\r\n",data_buff[i]);
  }
}
void spi_rx_handler(uint8_t rx_byte)
{
    switch (spi_ctx.state) {
        case WAIT_HEADER:
            if (rx_byte == HEADER) {
                spi_ctx.frame.header = rx_byte;
                spi_ctx.state = WAIT_LENGTH;
            }
            break;

        case WAIT_LENGTH:
            spi_ctx.frame.payload_length = rx_byte;
            spi_ctx.payload_counter = 0;
            spi_ctx.state = WAIT_PAYLOAD;
            printf("LENGTH received\r\n");
            break;

        case WAIT_PAYLOAD:
            spi_ctx.frame.payload[spi_ctx.payload_counter++] = rx_byte;
            if (spi_ctx.payload_counter >= MAX_PAYLOAD_SIZE) {//spi_ctx.frame.payload_length) { MAX_payload and length could differ
                spi_ctx.state = WAIT_CHECKSUM;
            }
            break;

        case WAIT_CHECKSUM:
            spi_ctx.frame.checksum = rx_byte;
            if (validate_checksum(&(spi_ctx.frame))) {
                process_frame(&(spi_ctx.frame));
            }
            // Either way, reset
            spi_ctx.state = WAIT_HEADER;
            break;

//        case FRAME_COMPLETE:
//            // Should never stay here; always reset
//            spi_ctx.state = WAIT_HEADER;
//            break;
    }
}

void spi_rx_task()
{
    uint8_t byte;
    while (1) {
        // Wait for semaphore from ISR
        if (xSemaphoreTake(spi_rx_sem, portMAX_DELAY) == pdTRUE) {
            // Drain all received bytes
            while (ring_buffer_get(&byte)) {
                spi_rx_handler(byte);  // your state machine
//                printf("Function for handling  the state\r\n");
            }
        }
    }
}



