#include "ui/textedit_ui.h"

// 0x66DAA4
static int dword_66DAA4;

// NOTE: Odd buffer size.
//
// 0x66D9D8
static char byte_66D9D8[204];

// 0x66DAA8
static TextEdit* textedit_ui_current_textedit;

// 0x66DAAC
static int dword_66DAAC;

// 0x66DAB0
static int dword_66DAB0;

// 0x66DAB
static int dword_66DAB4;

// 0x566E20
bool textedit_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    return true;
}

// 0x566E30
void textedit_ui_reset()
{
}

// 0x566E40
void textedit_ui_exit()
{
}

// 0x566E50
void textedit_ui_focus(TextEdit* textedit)
{
    textedit_ui_current_textedit = textedit;

    if (*textedit->buffer != '\0') {
        dword_66DAAC = strlen(textedit->buffer);
        dword_66DAB4 = dword_66DAAC;
    } else {
        dword_66DAAC = 0;
        dword_66DAB4 = 0;
    }

    dword_66DAB0 = 0;

    strcpy(byte_66D9D8, textedit->buffer);
    dword_66DAA4 = true;
}

// 0x566ED0
void textedit_ui_unfocus()
{
    textedit_ui_current_textedit = NULL;
    dword_66DAAC = 0;
    dword_66DAA4 = false;
}

// 0x566EF0
bool textedit_ui_is_focused()
{
    return dword_66DAA4;
}

// 0x566F00
bool textedit_ui_process_message(TigMessage* msg)
{
    if (textedit_ui_current_textedit == NULL) {
        return false;
    }

    if (msg->type == TIG_MESSAGE_KEYBOARD) {
        if (msg->data.keyboard.pressed == 1) {
            switch (msg->data.keyboard.key) {
            case DIK_HOME:
                dword_66DAAC = 0;
                break;
            case DIK_UP:
                if (dword_66DAAC - 40 >= 0) {
                    dword_66DAAC -= 40;
                }
                break;
            case DIK_LEFT:
                if (dword_66DAAC > 0) {
                    dword_66DAAC--;
                }
                break;
            case DIK_RIGHT:
                if (dword_66DAAC < dword_66DAB4) {
                    dword_66DAAC++;
                }
                break;
            case DIK_DOWN:
                if (dword_66DAAC + 40 < dword_66DAB4) {
                    dword_66DAAC += 40;
                }
                break;
            case DIK_INSERT:
                dword_66DAB0 = !dword_66DAB0;
                break;
            case DIK_DELETE:
                memcpy(&(textedit_ui_current_textedit->buffer[dword_66DAAC]),
                    &(textedit_ui_current_textedit->buffer[dword_66DAAC + 1]),
                    dword_66DAB4 - dword_66DAAC);
                textedit_ui_current_textedit->buffer[dword_66DAB4--] = '\0';
                break;
            default:
                return false;
            }

            if (textedit_ui_current_textedit->on_change != NULL) {
                textedit_ui_current_textedit->on_change(textedit_ui_current_textedit);
            }

            return true;
        }
    }

    if (msg->type == TIG_MESSAGE_CHAR) {
        if (msg->data.character.ch == VK_BACK) {
            if (dword_66DAAC > 0) {
                dword_66DAAC--;
                memcpy(&(textedit_ui_current_textedit->buffer[dword_66DAAC]),
                    &(textedit_ui_current_textedit->buffer[dword_66DAAC + 1]),
                    dword_66DAB4 - dword_66DAAC);
                textedit_ui_current_textedit->buffer[dword_66DAB4--] = '\0';
            }

            if (textedit_ui_current_textedit->on_change != NULL) {
                textedit_ui_current_textedit->on_change(textedit_ui_current_textedit);
            }

            return true;
        }

        if (msg->data.character.ch == VK_TAB) {
            if (textedit_ui_current_textedit->on_tab != NULL) {
                textedit_ui_current_textedit->on_tab(textedit_ui_current_textedit);
            }

            return true;
        }

        if (msg->data.character.ch == VK_RETURN) {
            textedit_ui_current_textedit->on_enter(textedit_ui_current_textedit);
            return true;
        }

        if ((msg->data.character.ch >= '\0' && msg->data.character.ch < ' ')
            || dword_66DAAC >= textedit_ui_current_textedit->size - 1
            || !sub_5671E0(msg->data.character.ch)) {
            return false;
        }

        if (!dword_66DAB0) {
            if (dword_66DAB4 > textedit_ui_current_textedit->size - 1) {
                return true;
            }

            dword_66DAB4++;

            memcpy(&(textedit_ui_current_textedit->buffer[dword_66DAAC + 1]),
                &(textedit_ui_current_textedit->buffer[dword_66DAAC]),
                dword_66DAB4 - dword_66DAAC);
        }

        textedit_ui_current_textedit->buffer[dword_66DAAC] = msg->data.character.ch;

        if (++dword_66DAAC > dword_66DAB4) {
            dword_66DAB4 = dword_66DAAC;
            textedit_ui_current_textedit->buffer[dword_66DAAC] = '\0';
        }

        if (textedit_ui_current_textedit->on_change != NULL) {
            textedit_ui_current_textedit->on_change(textedit_ui_current_textedit);
        }

        return true;
    }

    return false;
}

// 0x5671E0
bool sub_5671E0(char ch)
{
    if ((textedit_ui_current_textedit->flags & 1) == 0
        && (textedit_ui_current_textedit->flags & 2) == 0) {
        return true;
    }

    if ((textedit_ui_current_textedit->flags & 1) != 0) {
        switch (ch) {
        case '"':
        case '\'':
        case '*':
        case '/':
        case ':':
        case '<':
        case '>':
        case '?':
        case '\\':
        case '|':
            return false;
        }
    }

    if (ch >= 'A' && ch <= 'Z') {
        return false;
    }

    if (ch >= 'a' && ch <= 'a') {
        return false;
    }

    return true;
}

// 0x5672A0
void textedit_ui_clear()
{
    dword_66DAAC = 0;
    dword_66DAB4 = 0;

    if (textedit_ui_current_textedit != NULL) {
        *textedit_ui_current_textedit->buffer = '\0';
        if (textedit_ui_current_textedit != NULL) {
            textedit_ui_current_textedit->on_change(textedit_ui_current_textedit);
        }
    }
}

// 0x5672D0
void textedit_ui_restore()
{
    if (textedit_ui_current_textedit != NULL) {
        strcpy(textedit_ui_current_textedit->buffer, byte_66D9D8);
        if (textedit_ui_current_textedit != NULL) {
            textedit_ui_current_textedit->on_change(textedit_ui_current_textedit);
        }
    }
}

// 0x567320
void textedit_ui_commit()
{
    if (textedit_ui_current_textedit != NULL) {
        textedit_ui_current_textedit->on_enter(textedit_ui_current_textedit);
    }
}
