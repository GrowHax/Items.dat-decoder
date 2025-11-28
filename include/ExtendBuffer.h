#ifndef EXTEND_BUFFER_H
#define EXTEND_BUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t mempos;
} ExtendBuffer;

void extendbuffer_init(ExtendBuffer* buf);
void extendbuffer_init_size(ExtendBuffer* buf, size_t size);
void extendbuffer_init_data(ExtendBuffer* buf, const uint8_t* d, size_t len);
void extendbuffer_free(ExtendBuffer* buf);
void extendbuffer_resize(ExtendBuffer* buf, size_t new_size);

uint8_t extendbuffer_readU8(ExtendBuffer* buf);
int8_t extendbuffer_readI8(ExtendBuffer* buf);
uint16_t extendbuffer_readU16(ExtendBuffer* buf);
int16_t extendbuffer_readI16(ExtendBuffer* buf);
uint32_t extendbuffer_readU32(ExtendBuffer* buf);
int32_t extendbuffer_readI32(ExtendBuffer* buf);

void extendbuffer_writeU8(ExtendBuffer* buf, uint8_t v);
void extendbuffer_writeI8(ExtendBuffer* buf, int8_t v);
void extendbuffer_writeU16(ExtendBuffer* buf, uint16_t v);
void extendbuffer_writeI16(ExtendBuffer* buf, int16_t v);
void extendbuffer_writeU32(ExtendBuffer* buf, uint32_t v);
void extendbuffer_writeI32(ExtendBuffer* buf, int32_t v);

#endif
