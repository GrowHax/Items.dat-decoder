#include "include/ItemsDat.h"
#include "include/file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int compare_items(const void* a, const void* b) {
    const ItemDefinition* ia = (const ItemDefinition*)a;
    const ItemDefinition* ib = (const ItemDefinition*)b;
    return (ia->id > ib->id) - (ia->id < ib->id);
}

void escape_json_string(const char* input, char* output, size_t output_size) {
    size_t j = 0;
    for (size_t i = 0; input[i] && j < output_size - 2; i++) {
        char c = input[i];
        switch (c) {
            case '\\': if (j < output_size - 2) { output[j++] = '\\'; output[j++] = '\\'; } break;
            case '\"': if (j < output_size - 2) { output[j++] = '\\'; output[j++] = '\"'; } break;
            case '\n': if (j < output_size - 2) { output[j++] = '\\'; output[j++] = 'n'; } break;
            case '\r': if (j < output_size - 2) { output[j++] = '\\'; output[j++] = 'r'; } break;
            case '\t': if (j < output_size - 2) { output[j++] = '\\'; output[j++] = 't'; } break;
            default: output[j++] = c; break;
        }
    }
    output[j] = 0;
}

int main(int argc, char** argv) {
    ItemsDat items;
    itemsdat_init(&items);
    const char* inPath = "items.dat";
    int ai = 1;
    if (argc > 1) {
        char* first = argv[1];
        if (first[0] != '-') {
            inPath = first;
            ai = 2;
        } else {
            ai = 1;
        }
    }

    ByteVector data = readFile(inPath);
    printf("Data size: %zu\n", data.size);
    if (data.data == NULL) {
        printf("File is empty or not found: %s\n", inPath);
        return 1;
    }

    itemsdat_init_data(&items, data.data, data.size);
    bytevector_free(&data);

    itemsdat_decode(&items);

    printf("After decode\n");
    printf("Decoded items.dat\n");
    printf("Version: %d\n", items.meta.version);
    printf("Item count (meta): %d\n", items.meta.itemCount);
    printf("Items read: %d\n", items.meta.itemCount);

    // Sort items by id
    qsort(items.meta.items, items.meta.itemCount, sizeof(ItemDefinition), compare_items);

    int wantJson = 0;
    const char* jsonOutPath = NULL;
    for (int aj = ai; aj < argc; ++aj) {
        char* a = argv[aj];
        if (strcmp(a, "--json") == 0) {
            wantJson = 1;
            if (aj + 1 < argc) {
                char* maybe = argv[aj+1];
                if (maybe[0] != '-') {
                    jsonOutPath = maybe;
                    ++aj;
                } else {
                    jsonOutPath = "items.json";
                }
            } else {
                jsonOutPath = "items.json";
            }
        } else if (strcmp(a, "--json-stdout") == 0) {
            wantJson = 1;
            jsonOutPath = "-";
        }
    }

    if (wantJson) {
        FILE* outf = stdout;
        if (jsonOutPath && strcmp(jsonOutPath, "-") != 0) {
            outf = fopen(jsonOutPath, "w");
            if (!outf) {
                fprintf(stderr, "Failed to open JSON output: %s\n", jsonOutPath);
                itemsdat_free(&items);
                return 2;
            }
        }

        fprintf(outf, "{\n");
        fprintf(outf, "  \"version\": %d,\n", items.meta.version);
        fprintf(outf, "  \"itemCount\": %d,\n", items.meta.itemCount);
        fprintf(outf, "  \"items\": [\n");
        int firstItem = 1;

        for (int i = 0; i < items.meta.itemCount; i++) {
            if (!firstItem) fprintf(outf, ",\n");
            firstItem = 0;
            ItemDefinition* it = &items.meta.items[i];
            fprintf(outf, "    {\n");
            fprintf(outf, "      \"id\": %d,\n", it->id);

            char escaped[1024];
            escape_json_string(it->name ? it->name : "", escaped, sizeof(escaped));
            fprintf(outf, "      \"name\": \"%s\",\n", escaped);

            escape_json_string(it->texture ? it->texture : "", escaped, sizeof(escaped));
            fprintf(outf, "      \"texture\": \"%s\",\n", escaped);

            fprintf(outf, "      \"flags\": %u,\n", it->flags);
            fprintf(outf, "      \"type\": %u,\n", (unsigned)it->type);
            fprintf(outf, "      \"materialType\": %u\n", (unsigned)it->materialType);
            fprintf(outf, "    }");
        }
        fprintf(outf, "\n  ]\n}\n");

        if (outf != stdout) fclose(outf);
        printf("Wrote JSON to %s\n", jsonOutPath ? jsonOutPath : "stdout");
    } else {
        itemsdat_encode(&items);
        writeFile("items_out.dat", items.buffer.data, items.buffer.size);
        printf("Wrote items_out.dat (round-trip) size=%zu\n", items.buffer.size);
    }

    itemsdat_free(&items);
    return 0;
}
