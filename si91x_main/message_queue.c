/*
 * message_queue.c
 *

 */
#include "message_queue.h"
#include <string.h>
#include <stdio.h>

static message_t msg_queue[MESSAGE_QUEUE_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

void message_queue_init(void)
{
    head = 0;
    tail = 0;
}

bool message_queue_available(void)
{
    return (head != tail);
}

bool message_queue_push(const uint8_t *data, uint8_t len)
{
    if (len > MAX_FRAME_SIZE) return false;

    uint8_t next_head = (head + 1) % MESSAGE_QUEUE_SIZE;
    if (next_head == tail) {
        // Queue full
        return false;
    }

    memcpy(msg_queue[head].data, data, len);
    msg_queue[head].length = len;
    head = next_head;
    return true;
}

bool message_queue_pop(uint8_t *subscriber_index, uint8_t *out_data, uint8_t *out_len)
{
//  printf("Sub_index is %d, and head is %d\r\n",*subscriber_index, head);
  if (*subscriber_index == head) {
          // Subscriber is up-to-date
          return false;
      }

    memcpy(out_data, msg_queue[*subscriber_index].data, msg_queue[*subscriber_index].length);
    *out_len = msg_queue[*subscriber_index].length;

    *subscriber_index = (*subscriber_index + 1) % MESSAGE_QUEUE_SIZE;
    return true;
}

