#include "../include/ItemDefinition.h"
#include <string.h>
#include <stdio.h>

void itemdefinition_init(ItemDefinition* item) {
    memset(item, 0, sizeof(ItemDefinition));
    item->name = NULL;
    item->texture = NULL;
    item->extraFile = NULL;
    item->petName = NULL;
    item->petPrefix = NULL;
    item->petSuffix = NULL;
    item->petAbility = NULL;
    item->extraOptions = NULL;
    item->texture2 = NULL;
    item->extraOptions2 = NULL;
    item->punchOptions = NULL;
    item->chairTexture = NULL;
    item->itemRenderer = NULL;
    item->info = NULL;
}

void itemdefinition_free(ItemDefinition* item) {
    free(item->name);
    free(item->texture);
    free(item->extraFile);
    free(item->petName);
    free(item->petPrefix);
    free(item->petSuffix);
    free(item->petAbility);
    free(item->extraOptions);
    free(item->texture2);
    free(item->extraOptions2);
    free(item->punchOptions);
    free(item->chairTexture);
    free(item->itemRenderer);
    free(item->info);
    itemdefinition_init(item);
}

void itemsdatmeta_init(ItemsDatMeta* meta, int32_t count) {
    meta->items = (ItemDefinition*)malloc(count * sizeof(ItemDefinition));
    if (!meta->items) {
        printf("malloc failed for items\n");
        exit(1);
    }
    meta->itemCount = count;
    meta->version = 0;
    for (int i = 0; i < count; i++) {
        itemdefinition_init(&meta->items[i]);
    }
}

void itemsdatmeta_free(ItemsDatMeta* meta) {
    for (int i = 0; i < meta->itemCount; i++) {
        itemdefinition_free(&meta->items[i]);
    }
    free(meta->items);
    meta->items = NULL;
    meta->itemCount = 0;
    meta->version = 0;
}
