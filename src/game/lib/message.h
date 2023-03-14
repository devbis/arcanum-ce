#ifndef ARCANUM_GAME_LIB_MESSAGE_H_
#define ARCANUM_GAME_LIB_MESSAGE_H_

typedef struct MessageListItem {
    int num;
    char* text;
};

bool message_find(int message_handle, MessageListItem* message_list_item);

#endif /* ARCANUM_GAME_LIB_MESSAGE_H_ */
