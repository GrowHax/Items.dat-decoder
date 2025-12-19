#include "../include/ItemsDat.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void itemsdat_init(ItemsDat* dat) {
    extendbuffer_init(&dat->buffer);
    dat->key = "PBG892FXX982ABC*";
    itemsdatmeta_init(&dat->meta, 0);
}

void itemsdat_init_data(ItemsDat* dat, const uint8_t* data, size_t len) {
    extendbuffer_init_data(&dat->buffer, data, len);
    dat->key = "PBG892FXX982ABC*";
    itemsdatmeta_init(&dat->meta, 0);
}

void itemsdat_free(ItemsDat* dat) {
    extendbuffer_free(&dat->buffer);
    itemsdatmeta_free(&dat->meta);
}

int itemsdat_get_write_size(ItemsDat* dat) {
    int size = 130 * dat->meta.itemCount;
    for (int i = 0; i < dat->meta.itemCount; i++) {
        ItemDefinition* item = &dat->meta.items[i];
        size += strlen(item->name ? item->name : "") + 2;
        size += strlen(item->texture ? item->texture : "") + 2;
        size += strlen(item->extraFile ? item->extraFile : "") + 2;
        size += strlen(item->petName ? item->petName : "") + 2;
        size += strlen(item->petPrefix ? item->petPrefix : "") + 2;
        size += strlen(item->petSuffix ? item->petSuffix : "") + 2;
        size += strlen(item->petAbility ? item->petAbility : "") + 2;
        size += strlen(item->extraOptions ? item->extraOptions : "") + 2;
        size += strlen(item->texture2 ? item->texture2 : "") + 2;
        size += strlen(item->extraOptions2 ? item->extraOptions2 : "") + 2;
        size += strlen(item->punchOptions ? item->punchOptions : "") + 2;
        size += strlen(item->chairTexture ? item->chairTexture : "") + 2;
        size += strlen(item->itemRenderer ? item->itemRenderer : "") + 2;
        size += strlen(item->info ? item->info : "") + 2;
        size += 60; // extraBytes
        size += 9; // bodyPart
        size += 9; // unknownBytes2
    }
    return size + 4 + 2;
}

char* itemsdat_read_string(ItemsDat* dat, int encoded, int id) {
    int16_t len = extendbuffer_readI16(&dat->buffer);
    if (len < 0) len = 0;
    size_t remaining = 0;
    if (dat->buffer.size > dat->buffer.mempos) remaining = dat->buffer.size - dat->buffer.mempos;
    if ((size_t)len > remaining) len = (int16_t)remaining;
    char* s = (char*)malloc((size_t)len + 1);
    if (!s) {
        char* empty = (char*)malloc(1);
        if (!empty) return NULL;
        empty[0] = '\0';
        return empty;
    }
    for (int i = 0; i < len; i++) {
        uint8_t c = extendbuffer_readU8(&dat->buffer);
        if (encoded && dat->key && strlen(dat->key) > 0) c ^= (uint8_t)dat->key[(id + i) % strlen(dat->key)];
        s[i] = (char)c;
    }
    s[len] = 0;
    return s;
}

void itemsdat_write_string(ItemsDat* dat, const char* s, int id, int encoded) {
    int len = strlen(s);
    extendbuffer_writeI16(&dat->buffer, (int16_t)len);
    for (int i = 0; i < len; i++) {
        uint8_t c = (uint8_t)s[i];
        if (encoded) c ^= (uint8_t)dat->key[(id + i) % strlen(dat->key)];
        extendbuffer_writeU8(&dat->buffer, c);
    }
}

void itemsdat_decode(ItemsDat* dat) {
    dat->buffer.mempos = 0;
    int16_t version = extendbuffer_readI16(&dat->buffer);
    int32_t count = extendbuffer_readI32(&dat->buffer);
    fprintf(stderr, "itemsdat_decode: version=%d, itemCount=%d\n", version, count);
    if (count == NULL) {
        fprintf(stderr, "Invalid itemCount (%d) - aborting decode\n", count);
        return;
    }

    itemsdatmeta_free(&dat->meta);
    itemsdatmeta_init(&dat->meta, count);
    dat->meta.version = version;
    for (int i = 0; i < dat->meta.itemCount; i++) {
        ItemDefinition* item = &dat->meta.items[i];
        item->id = extendbuffer_readI32(&dat->buffer);
        item->flags = extendbuffer_readU16(&dat->buffer);
        item->type = extendbuffer_readU8(&dat->buffer);
        item->materialType = extendbuffer_readU8(&dat->buffer);

        item->name = itemsdat_read_string(dat, 1, item->id);
        item->texture = itemsdat_read_string(dat, 0, item->id);

        item->textureHash = extendbuffer_readI32(&dat->buffer);
        item->visualEffectType = extendbuffer_readU8(&dat->buffer);
        item->cookingTime = extendbuffer_readI32(&dat->buffer);

        item->textureX = extendbuffer_readU8(&dat->buffer);
        item->textureY = extendbuffer_readU8(&dat->buffer);
        item->storageType = extendbuffer_readU8(&dat->buffer);
        item->isStripeyWallpaper = extendbuffer_readU8(&dat->buffer);
        item->collisionType = extendbuffer_readU8(&dat->buffer);
        item->breakHits = extendbuffer_readU8(&dat->buffer) / 6;

        item->resetStateAfter = extendbuffer_readI32(&dat->buffer);
        item->bodyPartType = extendbuffer_readU8(&dat->buffer);
        item->rarity = extendbuffer_readI16(&dat->buffer);
        item->maxAmount = extendbuffer_readU8(&dat->buffer);

        item->extraFile = itemsdat_read_string(dat, 0, item->id);
        item->extraFileHash = extendbuffer_readI32(&dat->buffer);
        item->audioVolume = extendbuffer_readI32(&dat->buffer);

        item->petName = itemsdat_read_string(dat, 0, item->id);
        item->petPrefix = itemsdat_read_string(dat, 0, item->id);
        item->petSuffix = itemsdat_read_string(dat, 0, item->id);
        item->petAbility = itemsdat_read_string(dat, 0, item->id);

        item->seedBase = extendbuffer_readU8(&dat->buffer);
        item->seedOverlay = extendbuffer_readU8(&dat->buffer);
        item->treeBase = extendbuffer_readU8(&dat->buffer);
        item->treeLeaves = extendbuffer_readU8(&dat->buffer);

        item->seedColor = extendbuffer_readI32(&dat->buffer);
        item->seedOverlayColor = extendbuffer_readI32(&dat->buffer);
        item->ingredient = extendbuffer_readI32(&dat->buffer);
        item->growTime = extendbuffer_readI32(&dat->buffer);

        item->fxFlags = extendbuffer_readI32(&dat->buffer);

        item->extraOptions = itemsdat_read_string(dat, 0, item->id);
        item->texture2 = itemsdat_read_string(dat, 0, item->id);
        item->extraOptions2 = itemsdat_read_string(dat, 0, item->id);

        item->unknownInt1 = extendbuffer_readI32(&dat->buffer);
        item->unknownInt2 = extendbuffer_readI32(&dat->buffer);
        item->flags2 = extendbuffer_readI32(&dat->buffer);

        for (int k = 0; k < 60; k++) item->extraBytes[k] = extendbuffer_readU8(&dat->buffer);

        item->tileRange = extendbuffer_readI32(&dat->buffer);
        item->vaultCapacity = extendbuffer_readI32(&dat->buffer);

        if (dat->meta.version >= 11) {
            item->punchOptions = itemsdat_read_string(dat, 0, item->id);
            if (dat->meta.version >= 12) {
                item->flags3 = extendbuffer_readI32(&dat->buffer);
                for (int k = 0; k < 9; k++) item->bodyPart[k] = extendbuffer_readU8(&dat->buffer);
            }
            if (dat->meta.version >= 13) item->lightRange = extendbuffer_readI32(&dat->buffer);
            if (dat->meta.version >= 14) item->unknownInt3 = extendbuffer_readI32(&dat->buffer);
            if (dat->meta.version >= 15) {
                item->canSit = extendbuffer_readU8(&dat->buffer);
                item->playerOffsetX = extendbuffer_readI32(&dat->buffer);
                item->playerOffsetY = extendbuffer_readI32(&dat->buffer);

                item->chairTextureX = extendbuffer_readI32(&dat->buffer);
                item->chairTextureY = extendbuffer_readI32(&dat->buffer);

                item->chairLegOffsetX = extendbuffer_readI32(&dat->buffer);
                item->chairLegOffsetY = extendbuffer_readI32(&dat->buffer);

                item->chairTexture = itemsdat_read_string(dat, 0, item->id);
            }
            if (dat->meta.version >= 16) item->itemRenderer = itemsdat_read_string(dat, 0, item->id);
            if (dat->meta.version >= 17) item->extraFlags1 = extendbuffer_readI32(&dat->buffer);
            if (dat->meta.version >= 18) item->itemRendererHash = extendbuffer_readI32(&dat->buffer);
            if (dat->meta.version >= 19) {
                for (int k = 0; k < 9; k++) item->unknownBytes2[k] = extendbuffer_readU8(&dat->buffer);
            }
            if (dat->meta.version >= 21) item->unknownShort1 = extendbuffer_readI16(&dat->buffer);
            if (dat->meta.version >= 22) item->info = itemsdat_read_string(dat, 0, item->id);
            if (dat->meta.version >= 23) {
                item->recipe[0] = extendbuffer_readU16(&dat->buffer);
                item->recipe[1] = extendbuffer_readU16(&dat->buffer);
            }
            if (dat->meta.version >= 24) item->unknownBytes3 = extendbuffer_readU8(&dat->buffer);
        }
    }
}

void itemsdat_encode(ItemsDat* dat) {
    dat->buffer.mempos = 0;
    int size = itemsdat_get_write_size(dat);
    extendbuffer_resize(&dat->buffer, size);
    extendbuffer_writeI16(&dat->buffer, dat->meta.version);
    extendbuffer_writeI32(&dat->buffer, dat->meta.itemCount);

    for (int i = 0; i < dat->meta.itemCount; i++) {
        ItemDefinition* item = &dat->meta.items[i];
        extendbuffer_writeI32(&dat->buffer, item->id);
        extendbuffer_writeU16(&dat->buffer, item->flags);
        extendbuffer_writeU8(&dat->buffer, item->type);
        extendbuffer_writeU8(&dat->buffer, item->materialType);

        itemsdat_write_string(dat, item->name ? item->name : "", item->id, 1);
        itemsdat_write_string(dat, item->texture ? item->texture : "", item->id, 0);

        extendbuffer_writeI32(&dat->buffer, item->textureHash);
        extendbuffer_writeU8(&dat->buffer, item->visualEffectType);
        extendbuffer_writeI32(&dat->buffer, item->cookingTime);

        extendbuffer_writeU8(&dat->buffer, item->textureX);
        extendbuffer_writeU8(&dat->buffer, item->textureY);
        extendbuffer_writeU8(&dat->buffer, item->storageType);
        extendbuffer_writeU8(&dat->buffer, item->isStripeyWallpaper);
        extendbuffer_writeU8(&dat->buffer, item->collisionType);
        extendbuffer_writeU8(&dat->buffer, item->breakHits * 6);

        extendbuffer_writeI32(&dat->buffer, item->resetStateAfter);
        extendbuffer_writeU8(&dat->buffer, item->bodyPartType);
        extendbuffer_writeI16(&dat->buffer, item->rarity);
        extendbuffer_writeU8(&dat->buffer, item->maxAmount);

        itemsdat_write_string(dat, item->extraFile ? item->extraFile : "", item->id, 0);
        extendbuffer_writeI32(&dat->buffer, item->extraFileHash);
        extendbuffer_writeI32(&dat->buffer, item->audioVolume);

        itemsdat_write_string(dat, item->petName ? item->petName : "", item->id, 0);
        itemsdat_write_string(dat, item->petPrefix ? item->petPrefix : "", item->id, 0);
        itemsdat_write_string(dat, item->petSuffix ? item->petSuffix : "", item->id, 0);
        itemsdat_write_string(dat, item->petAbility ? item->petAbility : "", item->id, 0);

        extendbuffer_writeU8(&dat->buffer, item->seedBase);
        extendbuffer_writeU8(&dat->buffer, item->seedOverlay);
        extendbuffer_writeU8(&dat->buffer, item->treeBase);
        extendbuffer_writeU8(&dat->buffer, item->treeLeaves);

        extendbuffer_writeI32(&dat->buffer, item->seedColor);
        extendbuffer_writeI32(&dat->buffer, item->seedOverlayColor);
        extendbuffer_writeI32(&dat->buffer, item->ingredient);
        extendbuffer_writeI32(&dat->buffer, item->growTime);

        extendbuffer_writeI32(&dat->buffer, item->fxFlags);

        itemsdat_write_string(dat, item->extraOptions ? item->extraOptions : "", item->id, 0);
        itemsdat_write_string(dat, item->texture2 ? item->texture2 : "", item->id, 0);
        itemsdat_write_string(dat, item->extraOptions2 ? item->extraOptions2 : "", item->id, 0);

        extendbuffer_writeI32(&dat->buffer, item->unknownInt1);
        extendbuffer_writeI32(&dat->buffer, item->unknownInt2);
        extendbuffer_writeI32(&dat->buffer, item->flags2);

        for (int k = 0; k < 60; k++) extendbuffer_writeU8(&dat->buffer, item->extraBytes[k]);

        extendbuffer_writeI32(&dat->buffer, item->tileRange);
        extendbuffer_writeI32(&dat->buffer, item->vaultCapacity);

        if (dat->meta.version >= 11) {
            itemsdat_write_string(dat, item->punchOptions ? item->punchOptions : "", item->id, 0);
            if (dat->meta.version >= 12) {
                extendbuffer_writeI32(&dat->buffer, item->flags3);
                for (int k = 0; k < 9; k++) extendbuffer_writeU8(&dat->buffer, item->bodyPart[k]);
            }
            if (dat->meta.version >= 13) extendbuffer_writeI32(&dat->buffer, item->lightRange);
            if (dat->meta.version >= 14) extendbuffer_writeI32(&dat->buffer, item->unknownInt3);
            if (dat->meta.version >= 15) {
                extendbuffer_writeU8(&dat->buffer, item->canSit);
                extendbuffer_writeI32(&dat->buffer, item->playerOffsetX);
                extendbuffer_writeI32(&dat->buffer, item->playerOffsetY);

                extendbuffer_writeI32(&dat->buffer, item->chairTextureX);
                extendbuffer_writeI32(&dat->buffer, item->chairTextureY);

                extendbuffer_writeI32(&dat->buffer, item->chairLegOffsetX);
                extendbuffer_writeI32(&dat->buffer, item->chairLegOffsetY);

                itemsdat_write_string(dat, item->chairTexture ? item->chairTexture : "", item->id, 0);
            }
            if (dat->meta.version >= 16) itemsdat_write_string(dat, item->itemRenderer ? item->itemRenderer : "", item->id, 0);
            if (dat->meta.version >= 17) extendbuffer_writeI32(&dat->buffer, item->extraFlags1);
            if (dat->meta.version >= 18) extendbuffer_writeI32(&dat->buffer, item->itemRendererHash);
            if (dat->meta.version >= 19) for (int k = 0; k < 9; k++) extendbuffer_writeU8(&dat->buffer, item->unknownBytes2[k]);
            if (dat->meta.version >= 21) extendbuffer_writeI16(&dat->buffer, item->unknownShort1);
            if (dat->meta.version >= 22) itemsdat_write_string(dat, item->info ? item->info : "", item->id, 0);
            if (dat->meta.version >= 23) {
                extendbuffer_writeU16(&dat->buffer, item->recipe[0]);
                extendbuffer_writeU16(&dat->buffer, item->recipe[1]);
            }
        }
    }
}
