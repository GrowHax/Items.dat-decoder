#ifndef ITEM_DEFINITION_H
#define ITEM_DEFINITION_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int32_t id;
    uint16_t flags;
    uint8_t type;
    uint8_t materialType;

    char* name;
    char* texture;
    int32_t textureHash;
    uint8_t visualEffectType;
    int32_t cookingTime;

    uint8_t textureX;
    uint8_t textureY;
    uint8_t storageType;
    uint8_t isStripeyWallpaper;
    uint8_t collisionType;
    uint8_t breakHits;

    int32_t resetStateAfter;
    uint8_t bodyPartType;
    int16_t rarity;
    uint8_t maxAmount;

    char* extraFile;
    int32_t extraFileHash;
    int32_t audioVolume;

    char* petName;
    char* petPrefix;
    char* petSuffix;
    char* petAbility;

    uint8_t seedBase;
    uint8_t seedOverlay;
    uint8_t treeBase;
    uint8_t treeLeaves;
    int32_t seedColor;
    int32_t seedOverlayColor;
    int32_t ingredient;
    int32_t growTime;

    int32_t fxFlags;

    char* extraOptions;
    char* texture2;
    char* extraOptions2;

    int32_t unknownInt1;
    int32_t unknownInt2;
    int32_t flags2;

    uint8_t extraBytes[60];

    int32_t tileRange;
    int32_t vaultCapacity;

    // version >= 11
    char* punchOptions;
    int32_t flags3;
    uint8_t bodyPart[9];
    int32_t lightRange;
    int32_t unknownInt3;
    uint8_t canSit;
    int32_t playerOffsetX;
    int32_t playerOffsetY;
    int32_t chairTextureX;
    int32_t chairTextureY;
    int32_t chairLegOffsetX;
    int32_t chairLegOffsetY;
    char* chairTexture;
    char* itemRenderer;
    int32_t extraFlags1;
    int32_t itemRendererHash;
    uint8_t unknownBytes2[9];
    int16_t unknownShort1;
    char* info;
    uint16_t recipe[2];
    uint8_t unknownBytes3;
} ItemDefinition;

typedef struct {
    ItemDefinition* items;
    int32_t itemCount;
    int16_t version;
} ItemsDatMeta;

void itemdefinition_init(ItemDefinition* item);
void itemdefinition_free(ItemDefinition* item);
void itemsdatmeta_init(ItemsDatMeta* meta, int32_t count);
void itemsdatmeta_free(ItemsDatMeta* meta);

#endif
