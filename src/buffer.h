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

#define BUFFER_STATUS_OVERLAP 0x01
#define BUFFER_STATUS_WRITING 0x02
#define BUFFER_STATUS_READING 0x04

#define BUFFER_STATUS_IS(buf, _status)    (((buf)->status & (_status)) != 0)
#define BUFFER_STATUS_SET(buf, _status)   {(buf)->status |= (_status);}
#define BUFFER_STATUS_UNSET(buf, _status) {(buf)->status &= ~(_status);}

#define Buffer_IsAvailable(buf) ((buf)->buffer && !BUFFER_STATUS_IS(buf, BUFFER_STATUS_READING|BUFFER_STATUS_WRITING) && ((buf)->r_idx != (buf)->w_idx || BUFFER_STATUS_IS((buf), BUFFER_STATUS_OVERLAP)))
#define Buffer_Flush(buf)       {BUFFER_STATUS_UNSET((buf), BUFFER_STATUS_OVERLAP); (buf)->r_idx = (buf)->w_idx;}

typedef struct {
  uint8_t   status;
  void      *buffer;
  size_t    itemSz;
  uint32_t  bufferLength;
  uint32_t  w_idx;
  uint32_t  r_idx;
} Buffer_t;

void    Buffer_Init(Buffer_t *buf, void *bufferData, size_t itemSz, uint16_t length);
int32_t Buffer_Read(Buffer_t *buf, void *dst, uint16_t itemNum);
int32_t Buffer_Write(Buffer_t *buf, const void *src, uint16_t itemNum);

#endif /* INC_BUFFER_H_ */
