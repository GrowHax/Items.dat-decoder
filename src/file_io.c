#include "../include/file_io.h"
#include <stdio.h>
#include <stdlib.h>

ByteVector readFile(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        ByteVector vec = {NULL, 0};
        return vec; // error
    }
    fseek(f, 0, SEEK_END);
    long lsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    size_t size = (size_t)lsize;
    uint8_t* data = (uint8_t*)malloc(size);
    if (!data) {
        fclose(f);
        ByteVector vec = {NULL, 0};
        return vec;
    }
    size_t read = fread(data, 1, size, f);
    fclose(f);
    if (read != size) {
        free(data);
        ByteVector vec = {NULL, 0};
        return vec;
    }
    ByteVector vec = {data, size};
    return vec;
}

void writeFile(const char* path, const uint8_t* data, size_t size) {
    FILE* f = fopen(path, "wb");
    if (!f) return; // error
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
}

void bytevector_free(ByteVector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
}
