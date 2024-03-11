#include "tig/map.h"

#include <gtest/gtest.h>

#include "tig/file.h"
#include "tig/memory.h"

class TigMapTest : public testing::Test {
protected:
    void SetUp() override
    {
        ASSERT_EQ(tig_memory_init(NULL), TIG_OK);
    }

    void TearDown() override
    {
        ASSERT_TRUE(tig_memory_validate_memory_leaks());
        tig_memory_exit();
    }
};

TEST_F(TigMapTest, Init)
{
    TigMap map;
    tig_map_init(&map, 1);

    EXPECT_GT(map.buckets_count, 0);
    EXPECT_NE(map.buckets, nullptr);

    for (int index = 0; index < map.buckets_count; index++) {
        EXPECT_EQ(map.buckets[index], nullptr) << "should be NULL at " << index;
    }

    EXPECT_EQ(map.count, 0);

    tig_map_exit(&map);
}

TEST_F(TigMapTest, Copy)
{
    TigMap map;
    tig_map_init(&map, sizeof(int));

    int value = 1;
    tig_map_set(&map, 100, &value);

    TigMap copy;
    tig_map_copy(&copy, &map);

    EXPECT_EQ(copy.buckets_count, map.buckets_count);
    EXPECT_NE(copy.buckets, nullptr);
    EXPECT_EQ(copy.value_size, sizeof(int));
    EXPECT_EQ(copy.count, 1);

    for (int index = 0; index < copy.buckets_count; index++) {
        if (index != 100 % copy.buckets_count) {
            EXPECT_EQ(map.buckets[index], nullptr) << "should be NULL at " << index;
        } else {
            EXPECT_NE(map.buckets[index], nullptr);
        }
    }

    EXPECT_NE(copy.buckets[100 % copy.buckets_count], nullptr);
    EXPECT_EQ(copy.buckets[100 % copy.buckets_count]->key, 100);
    EXPECT_NE(copy.buckets[100 % copy.buckets_count]->value, nullptr);
    EXPECT_EQ(memcmp(copy.buckets[100 % copy.buckets_count]->value, &value, sizeof(int)), 0);

    tig_map_exit(&copy);
    tig_map_exit(&map);
}

TEST_F(TigMapTest, Set)
{
    TigMap map;
    tig_map_init(&map, sizeof(int));

    int value = 13;
    tig_map_set(&map, 20, &value);

    EXPECT_EQ(map.count, 1);
    EXPECT_NE(map.buckets[20 % map.buckets_count], nullptr);
    EXPECT_EQ(map.buckets[20 % map.buckets_count]->key, 20);
    EXPECT_NE(map.buckets[20 % map.buckets_count]->value, nullptr);
    EXPECT_EQ(memcmp(map.buckets[20 % map.buckets_count]->value, &value, sizeof(int)), 0);

    tig_map_exit(&map);
}

TEST_F(TigMapTest, Get)
{
    TigMap map;
    tig_map_init(&map, sizeof(int));

    int value = 29;
    tig_map_set(&map, 7, &value);

    int stored;
    EXPECT_TRUE(tig_map_get(&map, 7, &stored));
    EXPECT_EQ(stored, 29);

    tig_map_exit(&map);
}

TEST_F(TigMapTest, Remove)
{
    TigMap map;
    tig_map_init(&map, sizeof(int));

    int value = 31;
    tig_map_set(&map, 5, &value);

    tig_map_remove(&map, 5);

    EXPECT_EQ(map.count, 0);
    EXPECT_EQ(map.buckets[31 % map.buckets_count], nullptr);

    tig_map_exit(&map);
}
