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
#define Buffer_Flush(buf)       {(buf)->isOverlap = 0; (buf)->r_idx = (buf)->w_idx;}

typedef struct {
  void      *buffer;
  size_t    itemSz;
  uint32_t  bufferLength;
  uint32_t  w_idx;
  uint32_t  r_idx;
  uint8_t   isOverlap;
} Buffer_t;

void    Buffer_Init(Buffer_t *buf, void *bufferData, size_t itemSz, uint16_t length);
int32_t Buffer_Read(Buffer_t *buf, void *dst, uint16_t itemNum);
int32_t Buffer_Write(Buffer_t *buf, const void *src, uint16_t itemNum);

#endif /* INC_BUFFER_H_ */
