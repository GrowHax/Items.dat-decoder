#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t* data;
    size_t size;
} ByteVector;

ByteVector readFile(const char* path);
void writeFile(const char* path, const uint8_t* data, size_t size);
void bytevector_free(ByteVector* vec);

#endif
