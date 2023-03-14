#ifndef ARCANUM_GAME_LIB_MESSAGE_H_
#define ARCANUM_GAME_LIB_MESSAGE_H_

typedef struct MessageListItem {
    int num;
    char* text;
};

bool message_load(const char* path, int* msg_file);
void sub_4D43A0(int msg_file, MessageListItem* message_list_item);
bool message_find(int message_handle, MessageListItem* message_list_item);
bool message_unload(int msg_file);

#endif /* ARCANUM_GAME_LIB_MESSAGE_H_ */
