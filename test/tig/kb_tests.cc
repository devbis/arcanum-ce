#include "tig/kb.h"

#include <gtest/gtest.h>

#include "tig/core.h"
#include "tig/dxinput.h"
#include "tig/memory.h"
#include "tig/message.h"
#include "tig/video.h"

// NOTE: Order should match one in `core.c`.
static constexpr struct {
    const char* name;
    int(*init)(TigInitializeInfo*);
    void(*exit)();
} kDeps[] = {
    { "memory", tig_memory_init, tig_memory_exit },
    { "video", tig_video_init, tig_video_exit },
    { "dxinput", tig_dxinput_init, tig_dxinput_exit },
    { "kb", tig_kb_init, tig_kb_exit },
    { "message", tig_message_init, tig_message_exit },
};

static constexpr int kDepsSize = sizeof(kDeps) / sizeof(kDeps[0]);

class TigKeyboardTest : public testing::Test {
protected:
    static bool tig_initialized_;

    static void SetUpTestSuite()
    {
        if (tig_initialized_) {
            return;
        }

        TigInitializeInfo init_info = { 0 };
        init_info.flags = TIG_INITIALIZE_WINDOWED;
        init_info.instance = GetModuleHandle(NULL);
        init_info.width = 800;
        init_info.height = 600;
        init_info.bpp = 16;
        init_info.default_window_proc = DefWindowProcA;

        for (int index = 0; index < kDepsSize; ++index) {
            ASSERT_EQ(kDeps[index].init(&init_info), TIG_OK) << "failed to init '" << kDeps[index].name << "'";
        }

        tig_initialized_ = true;
    }

    static void TearDownTestSuite()
    {
        if (!tig_initialized_) {
            return;
        }

        for (int index = kDepsSize - 1; index >= 0; --index) {
            kDeps[index].exit();
        }
        tig_initialized_ = false;
    }

    void SetUp() override
    {
        // Initialize device state.
        tig_kb_ping();

        // Discard message queue.
        TigMessage msg;
        while (tig_message_dequeue(&msg) == TIG_OK) {
        }
    }

    void SendKeyDown(int key)
    {
        SendKey(key, false);
    }

    void SendKeyUp(int key)
    {
        SendKey(key, true);
    }

    void SendKey(int key, bool up)
    {
        INPUT input = { 0 };

        input.type = INPUT_KEYBOARD;
        input.ki.wScan = key;
        input.ki.dwFlags = KEYEVENTF_SCANCODE;

        if (up) {
            input.ki.dwFlags |= KEYEVENTF_KEYUP;
        }

        ASSERT_EQ(SendInput(1, &input, sizeof(input)), 1);
    }
};

bool TigKeyboardTest::tig_initialized_ = false;

TEST_F(TigKeyboardTest, KeyState)
{
    SendKeyDown(DIK_A);
    tig_kb_ping();
    ASSERT_TRUE(tig_kb_is_key_pressed(DIK_A));

    SendKeyUp(DIK_A);
    tig_kb_ping();
    ASSERT_FALSE(tig_kb_is_key_pressed(DIK_A));
}

TEST_F(TigKeyboardTest, Message)
{
    TigMessage msg;

    SendKeyDown(DIK_A);
    tig_kb_ping();
    ASSERT_EQ(tig_message_dequeue(&msg), TIG_OK) << "should generate press message";
    EXPECT_EQ(msg.type, TIG_MESSAGE_KEYBOARD);
    EXPECT_EQ(msg.data.keyboard.key, DIK_A);
    EXPECT_EQ(msg.data.keyboard.pressed, 1);

    SendKeyUp(DIK_A);
    tig_kb_ping();
    ASSERT_EQ(tig_message_dequeue(&msg), TIG_OK) << "should generate release message";
    EXPECT_EQ(msg.type, TIG_MESSAGE_KEYBOARD);
    EXPECT_EQ(msg.data.keyboard.key, DIK_A);
    EXPECT_EQ(msg.data.keyboard.pressed, 0);
}
