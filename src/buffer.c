/*
 * buffer.c
 *
 *  Created on: Dec 10, 2021
 *      Author: janoko
 */

#include "buffer.h"


void Buffer_Init(Buffer_t *buf, void *bufferData, size_t itemSz, uint16_t length)
{
  buf->buffer = bufferData;
  buf->itemSz = itemSz;
  buf->bufferLength = (uint32_t)itemSz * (uint32_t)length;
  buf->r_idx = 0;
  buf->w_idx = 0;
  buf->isOverlap = 0;
}


int32_t Buffer_Read(Buffer_t *buf, void *dst, uint16_t itemNum)
{
  uint32_t readLen = 0;
  uint32_t length = (uint32_t)itemNum * (uint32_t)buf->itemSz;
  uint8_t *dstBytes = dst;

  if (buf == NULL || buf->buffer == NULL || dstBytes == NULL) return -1;

  while (length--) {
    if (buf->r_idx == buf->w_idx && !buf->isOverlap) break;
    *dstBytes = *(((uint8_t*)buf->buffer)+buf->r_idx);
    buf->isOverlap = 0;
    buf->r_idx++;
    if (buf->r_idx == buf->bufferLength) buf->r_idx = 0;
    dst++;
    readLen++;
  }

  return readLen;
}


int32_t Buffer_Write(Buffer_t *buf, void *src, uint16_t itemNum)
{
  uint16_t writeLen = 0;
  uint32_t length = (uint32_t)itemNum * (uint32_t)buf->itemSz;
  uint8_t *srcBytes = src;

  if (buf == NULL || buf->buffer == NULL || srcBytes == NULL) return -1;

  while (length--) {
    *(((uint8_t*)buf->buffer)+buf->w_idx) = *srcBytes;

    writeLen++;
    srcBytes++;
    buf->w_idx++;
    if (buf->w_idx == buf->bufferLength) buf->w_idx = 0;
    if (buf->isOverlap == 1) buf->r_idx = buf->w_idx;
    if (buf->w_idx == buf->r_idx) {
      buf->isOverlap = 1;
    }
  }

  return writeLen;
}
