#include "../include/ItemsDat.h"
#include <sstream>
#include <stdexcept>

int ItemsDat::getWriteSize() {
    int size = 130 * static_cast<int>(meta.items.size());
    for (auto &p : meta.items) {
        ItemDefinition &item = p.second;
        auto addStr = [&](const std::string &s){ size += static_cast<int>(s.size()) + 2; };
        addStr(item.name);
        addStr(item.texture);
        addStr(item.extraFile);
        addStr(item.petName);
        addStr(item.petPrefix);
        addStr(item.petSuffix);
        addStr(item.petAbility);
        addStr(item.extraOptions);
        addStr(item.texture2);
        addStr(item.extraOptions2);
        addStr(item.punchOptions);
        addStr(item.chairTexture);
        addStr(item.itemRenderer);
        addStr(item.info);

        size += static_cast<int>(item.extraBytes.size());
        size += static_cast<int>(item.bodyPart.size());
        size += static_cast<int>(item.unknownBytes2.size());
    }
    return size + 4 + 2;
}

std::string ItemsDat::readString(bool encoded, int id) {
    int16_t len = buffer.readI16();
    std::string s;
    s.reserve(len);
    for (int i=0;i<len;i++){
        uint8_t c = buffer.readU8();
        if (encoded) c = c ^ static_cast<uint8_t>(key[(id + i) % key.size()]);
        s.push_back(static_cast<char>(c));
    }
    return s;
}

void ItemsDat::writeString(const std::string &s, int id, bool encoded) {
    buffer.writeI16(static_cast<int16_t>(s.size()));
    for (size_t i=0;i<s.size();i++){
        uint8_t c = static_cast<uint8_t>(s[i]);
        if (encoded) c = c ^ static_cast<uint8_t>(key[(id + i) % key.size()]);
        buffer.writeU8(c);
    }
}

void ItemsDat::decode() {
    buffer.mempos = 0;
    meta.version = buffer.readI16();
    meta.itemCount = buffer.readI32();
    for (int i=0;i<meta.itemCount;i++){
        ItemDefinition item;
        item.id = buffer.readI32();
        item.flags = buffer.readU16();
        item.type = buffer.readU8();
        item.materialType = buffer.readU8();

        item.name = readString(true, item.id);
        item.texture = readString(false, item.id);

        item.textureHash = buffer.readI32();
        item.visualEffectType = buffer.readU8();
        item.cookingTime = buffer.readI32();

        item.textureX = buffer.readU8();
        item.textureY = buffer.readU8();
        item.storageType = buffer.readU8();
        item.isStripeyWallpaper = buffer.readU8();
        item.collisionType = buffer.readU8();
        item.breakHits = buffer.readU8() / 6;

        item.resetStateAfter = buffer.readI32();
        item.bodyPartType = buffer.readU8();
        item.rarity = buffer.readI16();
        item.maxAmount = buffer.readU8();

        item.extraFile = readString(false, item.id);
        item.extraFileHash = buffer.readI32();
        item.audioVolume = buffer.readI32();

        item.petName = readString(false, item.id);
        item.petPrefix = readString(false, item.id);
        item.petSuffix = readString(false, item.id);
        item.petAbility = readString(false, item.id);

        item.seedBase = buffer.readU8();
        item.seedOverlay = buffer.readU8();
        item.treeBase = buffer.readU8();
        item.treeLeaves = buffer.readU8();

        item.seedColor = buffer.readI32();
        item.seedOverlayColor = buffer.readI32();
        item.ingredient = buffer.readI32();
        item.growTime = buffer.readI32();

        item.fxFlags = buffer.readI32();

        item.extraOptions = readString(false, item.id);
        item.texture2 = readString(false, item.id);
        item.extraOptions2 = readString(false, item.id);

        item.unknownInt1 = buffer.readI32();
        item.unknownInt2 = buffer.readI32();
        item.flags2 = buffer.readI32();

        item.extraBytes.resize(60);
        for (int k=0;k<60;k++) item.extraBytes[k] = buffer.readU8();

        item.tileRange = buffer.readI32();
        item.vaultCapacity = buffer.readI32();

        if (meta.version >= 11) {
            item.punchOptions = readString(false, item.id);
            if (meta.version >= 12) {
                item.flags3 = buffer.readI32();
                item.bodyPart.resize(9);
                for (int k=0;k<9;k++) item.bodyPart[k] = buffer.readU8();
            }
            if (meta.version >= 13) item.lightRange = buffer.readI32();
            if (meta.version >= 14) item.unknownInt3 = buffer.readI32();
            if (meta.version >= 15) {
                item.canSit = buffer.readU8();
                item.playerOffsetX = buffer.readI32();
                item.playerOffsetY = buffer.readI32();

                item.chairTextureX = buffer.readI32();
                item.chairTextureY = buffer.readI32();

                item.chairLegOffsetX = buffer.readI32();
                item.chairLegOffsetY = buffer.readI32();

                item.chairTexture = readString(false, item.id);
            }
            if (meta.version >= 16) item.itemRenderer = readString(false, item.id);
            if (meta.version >= 17) item.extraFlags1 = buffer.readI32();
            if (meta.version >= 18) item.itemRendererHash = buffer.readI32();
            if (meta.version >= 19) {
                item.unknownBytes2.resize(9);
                for (int k=0;k<9;k++) item.unknownBytes2[k] = buffer.readU8();
            }
            if (meta.version >= 21) item.unknownShort1 = buffer.readI16();
            if (meta.version >= 22) item.info = readString(false, item.id);
            if (meta.version >= 23) {
                item.recipe.resize(2);
                for (int r=0;r<2;r++) {
                    uint16_t data = buffer.readU16();
                    if (data) item.recipe[r] = data;
                }
            }
        }

        meta.items[item.id] = std::move(item);
    }
}

void ItemsDat::encode() {
    buffer.mempos = 0;
    int size = getWriteSize();
    buffer = ExtendBuffer(size);
    buffer.writeI16(meta.version);
    buffer.writeI32(static_cast<int32_t>(meta.items.size()));

    for (auto &p : meta.items) {
        ItemDefinition &item = p.second;
        buffer.writeI32(item.id);
        buffer.writeU16(item.flags);
        buffer.writeU8(item.type);
        buffer.writeU8(item.materialType);

        writeString(item.name, item.id, true);
        writeString(item.texture, item.id, false);

        buffer.writeI32(item.textureHash);
        buffer.writeU8(item.visualEffectType);
        buffer.writeI32(item.cookingTime);

        buffer.writeU8(item.textureX);
        buffer.writeU8(item.textureY);
        buffer.writeU8(item.storageType);
        buffer.writeU8(item.isStripeyWallpaper);
        buffer.writeU8(item.collisionType);
        buffer.writeU8(item.breakHits * 6);

        buffer.writeI32(item.resetStateAfter);
        buffer.writeU8(item.bodyPartType);
        buffer.writeI16(item.rarity);
        buffer.writeU8(item.maxAmount);

        writeString(item.extraFile, item.id, false);
        buffer.writeI32(item.extraFileHash);
        buffer.writeI32(item.audioVolume);

        writeString(item.petName, item.id, false);
        writeString(item.petPrefix, item.id, false);
        writeString(item.petSuffix, item.id, false);
        writeString(item.petAbility, item.id, false);

        buffer.writeU8(item.seedBase);
        buffer.writeU8(item.seedOverlay);
        buffer.writeU8(item.treeBase);
        buffer.writeU8(item.treeLeaves);

        buffer.writeI32(item.seedColor);
        buffer.writeI32(item.seedOverlayColor);
        buffer.writeI32(item.ingredient);
        buffer.writeI32(item.growTime);

        buffer.writeI32(item.fxFlags);

        writeString(item.extraOptions, item.id, false);
        writeString(item.texture2, item.id, false);
        writeString(item.extraOptions2, item.id, false);

        buffer.writeI32(item.unknownInt1);
        buffer.writeI32(item.unknownInt2);
        buffer.writeI32(item.flags2);

        for (auto b : item.extraBytes) buffer.writeU8(b);

        buffer.writeI32(item.tileRange);
        buffer.writeI32(item.vaultCapacity);

        if (meta.version >= 11) {
            writeString(item.punchOptions, item.id, false);
            if (meta.version >= 12) {
                buffer.writeI32(item.flags3);
                for (auto b : item.bodyPart) buffer.writeU8(b);
            }
            if (meta.version >= 13) buffer.writeI32(item.lightRange);
            if (meta.version >= 14) buffer.writeI32(item.unknownInt3);
            if (meta.version >= 15) {
                buffer.writeU8(item.canSit);
                buffer.writeI32(item.playerOffsetX);
                buffer.writeI32(item.playerOffsetY);

                buffer.writeI32(item.chairTextureX);
                buffer.writeI32(item.chairTextureY);

                buffer.writeI32(item.chairLegOffsetX);
                buffer.writeI32(item.chairLegOffsetY);

                writeString(item.chairTexture, item.id, false);
            }
            if (meta.version >= 16) writeString(item.itemRenderer, item.id, false);
            if (meta.version >= 17) buffer.writeI32(item.extraFlags1);
            if (meta.version >= 18) buffer.writeI32(item.itemRendererHash);
            if (meta.version >= 19) for (auto b : item.unknownBytes2) buffer.writeU8(b);
            if (meta.version >= 21) buffer.writeI16(item.unknownShort1);
            if (meta.version >= 22) writeString(item.info, item.id, false);
            if (meta.version >= 23) {
                buffer.writeU16(item.recipe.size()>0?item.recipe[0]:0);
                buffer.writeU16(item.recipe.size()>1?item.recipe[1]:0);
            }
        }
    }
}
