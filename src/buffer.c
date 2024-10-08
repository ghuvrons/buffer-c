/*
 * buffer.c
 *
 *  Created on: Dec 10, 2021
 *      Author: janoko
 */

#include "buffer.h"


void Buffer_Init(Buffer_t *buf, void *bufferData, size_t itemSz, uint16_t length)
{
  buf->status = 0;
  buf->buffer = bufferData;
  buf->itemSz = itemSz;
  buf->bufferLength = (uint32_t)itemSz * (uint32_t)length;
  buf->r_idx = 0;
  buf->w_idx = 0;
}


int32_t Buffer_Read(Buffer_t *buf, void *dst, uint16_t itemNum)
{
  uint32_t readLen = 0;
  uint8_t *dstBytes = dst;
  size_t i;

  if (buf == NULL || buf->buffer == NULL || dstBytes == NULL) return -1;

  BUFFER_STATUS_SET(buf, BUFFER_STATUS_READING);

  while (itemNum--) {
    for (i = 0; i < buf->itemSz; i++) {
      if (buf->r_idx == buf->w_idx && !BUFFER_STATUS_IS(buf, BUFFER_STATUS_OVERLAP)) {
        goto finish;
      }
      *dstBytes = *(((uint8_t*)buf->buffer)+buf->r_idx);

      dstBytes++;
      buf->r_idx++;
      if (buf->r_idx == buf->bufferLength) buf->r_idx = 0;
    }
    BUFFER_STATUS_UNSET(buf, BUFFER_STATUS_OVERLAP);
    readLen++;
  }

finish:
  BUFFER_STATUS_UNSET(buf, BUFFER_STATUS_READING);
  return readLen;
}


int32_t Buffer_Write(Buffer_t *buf, const void *src, uint16_t itemNum)
{
  uint16_t writeLen = 0;
  const uint8_t *srcBytes = src;
  size_t i;

  if (buf == NULL || buf->buffer == NULL || srcBytes == NULL) return -1;

  BUFFER_STATUS_SET(buf, BUFFER_STATUS_WRITING);

  while (itemNum--) {
    for (i = 0; i < buf->itemSz; i++) {
      *(((uint8_t*)buf->buffer)+buf->w_idx) = *srcBytes;

      srcBytes++;
      buf->w_idx++;
      if (buf->w_idx == buf->bufferLength)  buf->w_idx = 0;
      if (buf->w_idx == buf->r_idx)         BUFFER_STATUS_SET(buf, BUFFER_STATUS_OVERLAP);
    }
    if (BUFFER_STATUS_IS(buf, BUFFER_STATUS_OVERLAP) && buf->w_idx != buf->r_idx) {
      // shift r_idx for one item size (skip one item)
      for (i = 0; i < buf->itemSz; i++) {
        buf->r_idx++;
        if (buf->r_idx == buf->bufferLength) buf->r_idx = 0;
      }
      if (buf->w_idx != buf->r_idx) BUFFER_STATUS_UNSET(buf, BUFFER_STATUS_OVERLAP);
    }

    writeLen++;
  }

  BUFFER_STATUS_UNSET(buf, BUFFER_STATUS_WRITING);
  return writeLen;
}
