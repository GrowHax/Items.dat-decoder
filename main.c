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

void print_json_string_field(FILE* outf, const char* key, const char* str) {
    char escaped[1024];
    escape_json_string(str ? str : "", escaped, sizeof(escaped));
    fprintf(outf, "      \"%s\": \"%s\",\n", key, escaped);
}

void print_json_int_field(FILE* outf, const char* key, int value) {
    fprintf(outf, "      \"%s\": %d,\n", key, value);
}

void print_json_uint_field(FILE* outf, const char* key, unsigned value) {
    fprintf(outf, "      \"%s\": %u,\n", key, value);
}

void print_json_byte_array_field(FILE* outf, const char* key, const unsigned char* arr, size_t len) {
    fprintf(outf, "      \"%s\": [", key);
    for (size_t k = 0; k < len; k++) {
        if (k) fprintf(outf, ", ");
        fprintf(outf, "%u", arr[k]);
    }
    fprintf(outf, "],\n");
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

    /* printf("After decode\n"); */
    printf("https://github.com/GrowHax/items.dat-decoder\n");
    printf("Decoded items.dat\n");
    printf("Version: %d\n", items.meta.version);
    printf("Item count (meta): %d\n", items.meta.itemCount);
    printf("Items read: %d\n", items.meta.itemCount);
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
                fprintf(stderr, "cry JSON output: %s\n", jsonOutPath);
                itemsdat_free(&items);
                return 2;
            }
        }

        fprintf(outf, "{\n");
        fprintf(outf, "  \"version\": %d,\n", items.meta.version);
        fprintf(outf, "  \"item_count\": %d,\n", items.meta.itemCount);
        fprintf(outf, "  \"items\": [\n");
        int firstItem = 1;

        for (int i = 0; i < items.meta.itemCount; i++) {
            if (!firstItem) fprintf(outf, ",\n");
            firstItem = 0;
            ItemDefinition* it = &items.meta.items[i];
            fprintf(outf, "    {\n");
            print_json_int_field(outf, "id", it->id);
            print_json_string_field(outf, "name", it->name);
            print_json_string_field(outf, "texture", it->texture);
            print_json_uint_field(outf, "flags", it->flags);
            print_json_uint_field(outf, "type", (unsigned)it->type);
            print_json_uint_field(outf, "material_type", (unsigned)it->materialType);
            print_json_int_field(outf, "texture_hash", it->textureHash);
            print_json_uint_field(outf, "visual_effect_type", (unsigned)it->visualEffectType);
            print_json_int_field(outf, "cooking_time", it->cookingTime);
            print_json_uint_field(outf, "texture_x", (unsigned)it->textureX);
            print_json_uint_field(outf, "texture_y", (unsigned)it->textureY);
            print_json_uint_field(outf, "storage_type", (unsigned)it->storageType);
            print_json_uint_field(outf, "is_stripey_wallpaper", (unsigned)it->isStripeyWallpaper);
            print_json_uint_field(outf, "collision_type", (unsigned)it->collisionType);
            print_json_uint_field(outf, "break_hits", (unsigned)it->breakHits);
            print_json_int_field(outf, "reset_state_after", it->resetStateAfter);
            print_json_uint_field(outf, "body_part_type", (unsigned)it->bodyPartType);
            print_json_int_field(outf, "rarity", it->rarity);
            print_json_uint_field(outf, "max_amount", (unsigned)it->maxAmount);
            print_json_string_field(outf, "extra_file", it->extraFile);
            print_json_int_field(outf, "extra_file_hash", it->extraFileHash);
            print_json_int_field(outf, "audio_volume", it->audioVolume);
            print_json_string_field(outf, "pet_name", it->petName);
            print_json_string_field(outf, "pet_prefix", it->petPrefix);
            print_json_string_field(outf, "pet_suffix", it->petSuffix);
            print_json_string_field(outf, "pet_ability", it->petAbility);
            print_json_uint_field(outf, "seed_base", (unsigned)it->seedBase);
            print_json_uint_field(outf, "seed_overlay", (unsigned)it->seedOverlay);
            print_json_uint_field(outf, "tree_base", (unsigned)it->treeBase);
            print_json_uint_field(outf, "tree_leaves", (unsigned)it->treeLeaves);
            print_json_int_field(outf, "seed_color", it->seedColor);
            print_json_int_field(outf, "seed_overlay_color", it->seedOverlayColor);
            print_json_int_field(outf, "ingredient", it->ingredient);
            print_json_int_field(outf, "grow_time", it->growTime);
            print_json_int_field(outf, "fx_flags", it->fxFlags);
            print_json_string_field(outf, "extra_options", it->extraOptions);
            print_json_string_field(outf, "texture2", it->texture2);
            print_json_string_field(outf, "extra_options2", it->extraOptions2);
            print_json_int_field(outf, "unknown_int1", it->unknownInt1);
            print_json_int_field(outf, "unknown_int2", it->unknownInt2);
            print_json_int_field(outf, "flags2", it->flags2);
            print_json_byte_array_field(outf, "extra_bytes", it->extraBytes, 60);
            print_json_int_field(outf, "tile_range", it->tileRange);
            print_json_int_field(outf, "vault_capacity", it->vaultCapacity);
            print_json_string_field(outf, "punch_options", it->punchOptions);
            print_json_int_field(outf, "flags3", it->flags3);
            print_json_byte_array_field(outf, "body_part", it->bodyPart, 9);
            print_json_int_field(outf, "light_range", it->lightRange);
            print_json_int_field(outf, "unknown_int3", it->unknownInt3);
            print_json_uint_field(outf, "can_sit", (unsigned)it->canSit);
            print_json_int_field(outf, "player_offset_x", it->playerOffsetX);
            print_json_int_field(outf, "player_offset_y", it->playerOffsetY);
            print_json_int_field(outf, "chair_texture_x", it->chairTextureX);
            print_json_int_field(outf, "chair_texture_y", it->chairTextureY);
            print_json_int_field(outf, "chair_leg_offset_x", it->chairLegOffsetX);
            print_json_int_field(outf, "chair_leg_offset_y", it->chairLegOffsetY);
            print_json_string_field(outf, "chair_texture", it->chairTexture);
            print_json_string_field(outf, "item_renderer", it->itemRenderer);
            print_json_int_field(outf, "extra_flags1", it->extraFlags1);
            print_json_int_field(outf, "item_renderer_hash", it->itemRendererHash);
            print_json_byte_array_field(outf, "unknown_bytes2", it->unknownBytes2, 9);
            print_json_int_field(outf, "unknown_short1", it->unknownShort1);
            print_json_string_field(outf, "info", it->info);
            fprintf(outf, "      \"recipe\": [%u, %u]\n", (unsigned)it->recipe[0], (unsigned)it->recipe[1]);
            fprintf(outf, "    }");
        }
        fprintf(outf, "\n  ]\n}\n");

        if (outf != stdout) fclose(outf);
        printf("Wrote JSON to %s\n", jsonOutPath ? jsonOutPath : "stdout");
    }

    itemsdat_free(&items);
    return 0;
}
