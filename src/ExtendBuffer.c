#include "../include/ExtendBuffer.h"
#include <stdlib.h>
#include <string.h>

void extendbuffer_init(ExtendBuffer* buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
    buf->mempos = 0;
}

void extendbuffer_init_size(ExtendBuffer* buf, size_t size) {
    buf->data = (uint8_t*)malloc(size);
    if (!buf->data) abort(); // or handle error
    buf->size = size;
    buf->capacity = size;
    buf->mempos = 0;
}

void extendbuffer_init_data(ExtendBuffer* buf, const uint8_t* d, size_t len) {
    buf->data = (uint8_t*)malloc(len);
    if (!buf->data) abort();
    memcpy(buf->data, d, len);
    buf->size = len;
    buf->capacity = len;
    buf->mempos = 0;
}

void extendbuffer_free(ExtendBuffer* buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
    buf->mempos = 0;
}

void extendbuffer_resize(ExtendBuffer* buf, size_t new_size) {
    if (new_size > buf->capacity) {
        size_t new_cap = buf->capacity == 0 ? 1 : buf->capacity * 2;
        while (new_cap < new_size) new_cap *= 2;
        uint8_t* new_data = (uint8_t*)realloc(buf->data, new_cap);
        if (!new_data) abort();
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    buf->size = new_size;
}

uint8_t extendbuffer_readU8(ExtendBuffer* buf) {
    if (buf->mempos >= buf->size) {
        return 0; // out of range, return 0
    }
    return buf->data[buf->mempos++];
}

int8_t extendbuffer_readI8(ExtendBuffer* buf) {
    return (int8_t)extendbuffer_readU8(buf);
}

uint16_t extendbuffer_readU16(ExtendBuffer* buf) {
    uint8_t b1 = extendbuffer_readU8(buf);
    uint8_t b2 = extendbuffer_readU8(buf);
    return b1 | (b2 << 8);
}

int16_t extendbuffer_readI16(ExtendBuffer* buf) {
    return (int16_t)extendbuffer_readU16(buf);
}

uint32_t extendbuffer_readU32(ExtendBuffer* buf) {
    uint8_t b1 = extendbuffer_readU8(buf);
    uint8_t b2 = extendbuffer_readU8(buf);
    uint8_t b3 = extendbuffer_readU8(buf);
    uint8_t b4 = extendbuffer_readU8(buf);
    return b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);
}

int32_t extendbuffer_readI32(ExtendBuffer* buf) {
    return (int32_t)extendbuffer_readU32(buf);
}

void extendbuffer_writeU8(ExtendBuffer* buf, uint8_t v) {
    if (buf->mempos >= buf->size) {
        extendbuffer_resize(buf, buf->mempos + 1);
    }
    buf->data[buf->mempos++] = v;
}

void extendbuffer_writeI8(ExtendBuffer* buf, int8_t v) {
    extendbuffer_writeU8(buf, (uint8_t)v);
}

void extendbuffer_writeU16(ExtendBuffer* buf, uint16_t v) {
    if (buf->mempos + 1 >= buf->size) {
        extendbuffer_resize(buf, buf->mempos + 2);
    }
    buf->data[buf->mempos++] = v & 0xFF;
    buf->data[buf->mempos++] = (v >> 8) & 0xFF;
}

void extendbuffer_writeI16(ExtendBuffer* buf, int16_t v) {
    extendbuffer_writeU16(buf, (uint16_t)v);
}

void extendbuffer_writeU32(ExtendBuffer* buf, uint32_t v) {
    if (buf->mempos + 3 >= buf->size) {
        extendbuffer_resize(buf, buf->mempos + 4);
    }
    buf->data[buf->mempos++] = v & 0xFF;
    buf->data[buf->mempos++] = (v >> 8) & 0xFF;
    buf->data[buf->mempos++] = (v >> 16) & 0xFF;
    buf->data[buf->mempos++] = (v >> 24) & 0xFF;
}

void extendbuffer_writeI32(ExtendBuffer* buf, int32_t v) {
    extendbuffer_writeU32(buf, (uint32_t)v);
}