/*
 * buffer.c
 *
 *  Created on: Dec 10, 2021
 *      Author: janoko
 */

#include "buffer.h"


void Buffer_Init(Buffer_t *buf, uint8_t *bufferData, size_t size)
{
  buf->buffer = bufferData;
  buf->size = size;
  buf->r_idx = 0;
  buf->w_idx = 0;
  buf->isOverlap = 0;
}


int32_t Buffer_Read(Buffer_t *buf, uint8_t *dstBuf, size_t length)
{
  uint16_t readLen = 0;

  if (buf->buffer == NULL || dstBuf == NULL) return -1;

  while (length--) {
    if (buf->r_idx == buf->w_idx && !buf->isOverlap) break;
    *dstBuf = *(buf->buffer+buf->r_idx);
    buf->isOverlap = 0;
    buf->r_idx++;
    if (buf->r_idx == buf->size) buf->r_idx = 0;
    dstBuf++;
    readLen++;
  }

  return readLen;
}


int32_t Buffer_Write(Buffer_t *buf, uint8_t *srcBuf, size_t length)
{
  uint16_t writeLen = 0;

  if (buf->buffer == NULL || srcBuf == NULL) return -1;

  while (length--) {
    *(buf->buffer+buf->w_idx) = *srcBuf;

    writeLen++;
    srcBuf++;
    buf->w_idx++;
    if (buf->w_idx == buf->size) buf->w_idx = 0;
    if (buf->isOverlap == 1) buf->r_idx = buf->w_idx;
    if (buf->w_idx == buf->r_idx) {
      buf->isOverlap = 1;
    }
  }

  return writeLen;
}
