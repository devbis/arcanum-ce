#ifndef ARCANUM_GAME_SCHEMATIC_H_
#define ARCANUM_GAME_SCHEMATIC_H_

typedef struct SchematicInfo {
    int name;
    int description;
    int art_num;
    int item1[3];
    int item2[3];
    int prod[3];
    int qty;
} SchematicInfo;

static_assert(sizeof(SchematicInfo) == 0x34, "wrong size");

#endif /* ARCANUM_GAME_SCHEMATIC_H_ */
