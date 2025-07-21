/*
 * message_queue.h
 *

 */

#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_


#include <stdint.h>
#include <stdbool.h>

#define MAX_FRAME_SIZE       67    // Max size of a complete SPI frame
#define MESSAGE_QUEUE_SIZE   8     // Number of complete frames in buffer

typedef struct {
    uint8_t data[MAX_FRAME_SIZE];
    uint8_t length;
} message_t;

void message_queue_init(void);
bool message_queue_push(const uint8_t *data, uint8_t len);
bool message_queue_pop(uint8_t *out_data, uint8_t *out_len);
bool message_queue_available(void);

#endif /* MESSAGE_QUEUE_H_ */
