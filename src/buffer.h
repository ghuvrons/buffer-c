/*
 * buffer.h
 *
 *  Created on: Dec 10, 2021
 *      Author: janoko
 */

#ifndef INC_BUFFER_H_
#define INC_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

#define Buffer_IsAvailable(buf) ((buf)->buffer && !((buf)->r_idx == (buf)->w_idx && !(buf)->isOverlap))

typedef struct {
  uint8_t   *buffer;
  size_t    size;
  uint16_t  w_idx;
  uint16_t  r_idx;
  uint8_t   isOverlap;
} Buffer_t;

void    Buffer_Init(Buffer_t *buf, uint8_t *bufferData, size_t size);
int32_t Buffer_Read(Buffer_t *buf, uint8_t *data, size_t length);
int32_t Buffer_Write(Buffer_t *buf, uint8_t *data, size_t length);

#endif /* INC_BUFFER_H_ */
