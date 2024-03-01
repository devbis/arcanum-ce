#include "tig/rect.h"

#include <gtest/gtest.h>

#include "tig/memory.h"

class TigRectTest : public testing::Test {
protected:
    void SetUp() override
    {
        ASSERT_EQ(tig_memory_init(NULL), TIG_OK);
        ASSERT_EQ(tig_rect_init(NULL), TIG_OK);
    }

    void TearDown() override
    {
        tig_rect_exit();

        ASSERT_TRUE(tig_memory_validate_memory_leaks());
        tig_memory_exit();
    }
};

TEST_F(TigRectTest, CreateDestroyNode)
{
    TigRectListNode* ptrs[50];

    for (int index = 0; index < 50; index++) {
        ptrs[index] = tig_rect_node_create();
        ASSERT_NE(ptrs[index], nullptr);
    }

    for (int index = 0; index < 50; index++) {
        tig_rect_node_destroy(ptrs[index]);
    }
}

TEST_F(TigRectTest, Intersection)
{
    TigRect a;
    TigRect b;
    TigRect c;

    a = { 3, 7, 10, 10 };
    b = { 5, 11, 10, 10 };
    EXPECT_EQ(tig_rect_intersection(&a, &b, &c), TIG_OK);
    EXPECT_EQ(c.x, 5);
    EXPECT_EQ(c.y, 11);
    EXPECT_EQ(c.width, 8);
    EXPECT_EQ(c.height, 6);

    a = { 1, 2, 3, 4 };
    b = { 5, 6, 7, 8 };
    EXPECT_EQ(tig_rect_intersection(&a, &b, &c), TIG_ERR_4);
}

// baa
// aaa
// aaa
TEST_F(TigRectTest, ClipTopLeft)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 7, 9, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 2);

    EXPECT_EQ(r[0].x, 12);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 8);
    EXPECT_EQ(r[0].height, 3);

    EXPECT_EQ(r[1].x, 10);
    EXPECT_EQ(r[1].y, 13);
    EXPECT_EQ(r[1].width, 10);
    EXPECT_EQ(r[1].height, 7);
}

// aba
// aaa
// aaa
TEST_F(TigRectTest, ClipTopCenter)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 12, 9, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 3);

    EXPECT_EQ(r[0].x, 10);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 2);
    EXPECT_EQ(r[0].height, 3);

    EXPECT_EQ(r[1].x, 17);
    EXPECT_EQ(r[1].y, 10);
    EXPECT_EQ(r[1].width, 3);
    EXPECT_EQ(r[1].height, 3);

    EXPECT_EQ(r[2].x, 10);
    EXPECT_EQ(r[2].y, 13);
    EXPECT_EQ(r[2].width, 10);
    EXPECT_EQ(r[2].height, 7);
}

// aab
// aaa
// aaa
TEST_F(TigRectTest, ClipTopRight)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 18, 9, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 2);

    EXPECT_EQ(r[0].x, 10);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 8);
    EXPECT_EQ(r[0].height, 3);

    EXPECT_EQ(r[1].x, 10);
    EXPECT_EQ(r[1].y, 13);
    EXPECT_EQ(r[1].width, 10);
    EXPECT_EQ(r[1].height, 7);
}

// aaa
// baa
// aaa
TEST_F(TigRectTest, ClipCenterLeft)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 7, 14, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 3);

    EXPECT_EQ(r[0].x, 10);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 10);
    EXPECT_EQ(r[0].height, 4);

    EXPECT_EQ(r[1].x, 12);
    EXPECT_EQ(r[1].y, 14);
    EXPECT_EQ(r[1].width, 8);
    EXPECT_EQ(r[1].height, 4);

    EXPECT_EQ(r[2].x, 10);
    EXPECT_EQ(r[2].y, 18);
    EXPECT_EQ(r[2].width, 10);
    EXPECT_EQ(r[2].height, 2);
}

// aaa
// aba
// aaa
TEST_F(TigRectTest, ClipCenter)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 12, 14, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 4);

    EXPECT_EQ(r[0].x, 10);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 10);
    EXPECT_EQ(r[0].height, 4);

    EXPECT_EQ(r[1].x, 10);
    EXPECT_EQ(r[1].y, 14);
    EXPECT_EQ(r[1].width, 2);
    EXPECT_EQ(r[1].height, 4);

    EXPECT_EQ(r[2].x, 17);
    EXPECT_EQ(r[2].y, 14);
    EXPECT_EQ(r[2].width, 3);
    EXPECT_EQ(r[2].height, 4);

    EXPECT_EQ(r[3].x, 10);
    EXPECT_EQ(r[3].y, 18);
    EXPECT_EQ(r[3].width, 10);
    EXPECT_EQ(r[3].height, 2);
}

// aaa
// aab
// aaa
TEST_F(TigRectTest, ClipCenterRight)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 18, 14, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 3);

    EXPECT_EQ(r[0].x, 10);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 10);
    EXPECT_EQ(r[0].height, 4);

    EXPECT_EQ(r[1].x, 10);
    EXPECT_EQ(r[1].y, 14);
    EXPECT_EQ(r[1].width, 8);
    EXPECT_EQ(r[1].height, 4);

    EXPECT_EQ(r[2].x, 10);
    EXPECT_EQ(r[2].y, 18);
    EXPECT_EQ(r[2].width, 10);
    EXPECT_EQ(r[2].height, 2);
}

// aaa
// aaa
// baa
TEST_F(TigRectTest, ClipBottomRight)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 7, 18, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 2);

    EXPECT_EQ(r[0].x, 10);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 10);
    EXPECT_EQ(r[0].height, 8);

    EXPECT_EQ(r[1].x, 12);
    EXPECT_EQ(r[1].y, 18);
    EXPECT_EQ(r[1].width, 8);
    EXPECT_EQ(r[1].height, 2);
}

// aaa
// aaa
// aba
TEST_F(TigRectTest, ClipBottomCenter)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 12, 18, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 3);

    EXPECT_EQ(r[0].x, 10);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 10);
    EXPECT_EQ(r[0].height, 8);

    EXPECT_EQ(r[1].x, 10);
    EXPECT_EQ(r[1].y, 18);
    EXPECT_EQ(r[1].width, 2);
    EXPECT_EQ(r[1].height, 2);

    EXPECT_EQ(r[2].x, 17);
    EXPECT_EQ(r[2].y, 18);
    EXPECT_EQ(r[2].width, 3);
    EXPECT_EQ(r[2].height, 2);
}

// aaa
// aaa
// aab
TEST_F(TigRectTest, ClipBottomLeft)
{
    TigRect a;
    TigRect b;
    TigRect r[4];

    a = { 10, 10, 10, 10 };
    b = { 18, 18, 5, 4 };
    EXPECT_EQ(tig_rect_clip(&a, &b, r), 2);

    EXPECT_EQ(r[0].x, 10);
    EXPECT_EQ(r[0].y, 10);
    EXPECT_EQ(r[0].width, 10);
    EXPECT_EQ(r[0].height, 8);

    EXPECT_EQ(r[1].x, 10);
    EXPECT_EQ(r[1].y, 18);
    EXPECT_EQ(r[1].width, 8);
    EXPECT_EQ(r[1].height, 2);
}

TEST_F(TigRectTest, Union)
{
    TigRect a;
    TigRect b;
    TigRect c;

    a = { 3, 7, 10, 10 };
    b = { 5, 11, 10, 10 };
    EXPECT_EQ(tig_rect_union(&a, &b, &c), TIG_OK);
    EXPECT_EQ(c.x, 3);
    EXPECT_EQ(c.y, 7);
    EXPECT_EQ(c.width, 12);
    EXPECT_EQ(c.height, 14);
}
