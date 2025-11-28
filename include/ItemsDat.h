#ifndef ITEMSDAT_H
#define ITEMSDAT_H

#include "ExtendBuffer.h"
#include "ItemDefinition.h"
#include <stdint.h>

typedef struct {
    ExtendBuffer buffer;
    const char* key;
    ItemsDatMeta meta;
} ItemsDat;

void itemsdat_init(ItemsDat* dat);
void itemsdat_init_data(ItemsDat* dat, const uint8_t* data, size_t len);
void itemsdat_free(ItemsDat* dat);

int itemsdat_get_write_size(ItemsDat* dat);
char* itemsdat_read_string(ItemsDat* dat, int encoded, int id);
void itemsdat_write_string(ItemsDat* dat, const char* s, int id, int encoded);
void itemsdat_decode(ItemsDat* dat);
void itemsdat_encode(ItemsDat* dat);

#endif
