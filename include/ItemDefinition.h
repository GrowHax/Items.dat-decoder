#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

struct ItemDefinition {
    int32_t id = 0;
    uint16_t flags = 0;
    uint8_t type = 0;
    uint8_t materialType = 0;

    std::string name;
    std::string texture;
    int32_t textureHash = 0;
    uint8_t visualEffectType = 0;
    int32_t cookingTime = 0;

    uint8_t textureX = 0;
    uint8_t textureY = 0;
    uint8_t storageType = 0;
    uint8_t isStripeyWallpaper = 0;
    uint8_t collisionType = 0;
    uint8_t breakHits = 0;

    int32_t resetStateAfter = 0;
    uint8_t bodyPartType = 0;
    int16_t rarity = 0;
    uint8_t maxAmount = 0;

    std::string extraFile;
    int32_t extraFileHash = 0;
    int32_t audioVolume = 0;

    std::string petName, petPrefix, petSuffix, petAbility;

    uint8_t seedBase = 0, seedOverlay = 0, treeBase = 0, treeLeaves = 0;
    int32_t seedColor = 0, seedOverlayColor = 0, ingredient = 0, growTime = 0;

    int32_t fxFlags = 0;

    std::string extraOptions, texture2, extraOptions2;

    int32_t unknownInt1 = 0, unknownInt2 = 0, flags2 = 0;

    std::vector<uint8_t> extraBytes; // 60 bytes

    int32_t tileRange = 0;
    int32_t vaultCapacity = 0;

    // version >= 11
    std::string punchOptions;
    int32_t flags3 = 0;
    std::vector<uint8_t> bodyPart; // 9 bytes
    int32_t lightRange = 0;
    int32_t unknownInt3 = 0;
    uint8_t canSit = 0;
    int32_t playerOffsetX = 0, playerOffsetY = 0;
    int32_t chairTextureX = 0, chairTextureY = 0;
    int32_t chairLegOffsetX = 0, chairLegOffsetY = 0;
    std::string chairTexture;
    std::string itemRenderer;
    int32_t extraFlags1 = 0;
    int32_t itemRendererHash = 0;
    std::vector<uint8_t> unknownBytes2; // 9 bytes
    int16_t unknownShort1 = 0;
    std::string info;
    std::vector<uint16_t> recipe; // 2 entries
};

struct ItemsDatMeta {
    std::unordered_map<int32_t, ItemDefinition> items;
    int32_t itemCount = 0;
    int16_t version = 0;
};
