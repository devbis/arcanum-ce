#include "tig/art.h"

#include <gtest/gtest.h>

// -----------------------------------------------------------------------------
// ART ID TESTS
// -----------------------------------------------------------------------------

// These tests check art id generation and data extraction. The test data is
// obtained from Arcanum via the following IDA breakpoints:
//
// ```idc
// msg("EXPECT_EQ(" + get_func_name(EIP) + "("
//  + ltoa(get_wide_dword(ESP+4), 10) + ", "
//  + ltoa(get_wide_dword(ESP+8), 10) + ", "
//  + ltoa(get_wide_dword(ESP+12), 10) + ", "
//  + ltoa(get_wide_dword(ESP+16), 10) + ", "
//  + ltoa(get_wide_dword(ESP+20), 10) + ", "
//  + ltoa(get_wide_dword(ESP+24), 10) + ", "
//  + ltoa(get_wide_dword(ESP+28), 10) + ", "
//  + ltoa(get_wide_dword(ESP+32), 10) + ", "
//  + "&art_id), TIG_OK);\n"), 0
// ```
//
// ```idc
// msg("EXPECT_EQ(art_id, 0x" + ltoa(EAX, 16) + ");\n"), 0
// ```

TEST(TigArtIdTest, MiscIdCreate)
{
    tig_art_id_t art_id;

    EXPECT_EQ(tig_art_misc_id_create(0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x80000000);

    EXPECT_EQ(tig_art_misc_id_create(10, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x80500000);
}

TEST(TigArtIdTest, TileIdCreate)
{
    tig_art_id_t art_id;

    EXPECT_EQ(tig_art_tile_id_create(6, 6, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x186F000);

    EXPECT_EQ(tig_art_tile_id_create(1, 1, 15, 0, 0, 1, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x41F0C0);

    EXPECT_EQ(tig_art_tile_id_create(4, 4, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x104F000);

    EXPECT_EQ(tig_art_tile_id_create(5, 5, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x145F000);

    EXPECT_EQ(tig_art_tile_id_create(3, 3, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xC3F000);

    EXPECT_EQ(tig_art_tile_id_create(0, 0, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xF000);

    EXPECT_EQ(tig_art_tile_id_create(8, 8, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x208F000);

    EXPECT_EQ(tig_art_tile_id_create(1, 1, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x41F000);

    EXPECT_EQ(tig_art_tile_id_create(9, 9, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x249F000);

    EXPECT_EQ(tig_art_tile_id_create(10, 10, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x28AF000);

    EXPECT_EQ(tig_art_tile_id_create(11, 11, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x2CBF000);

    EXPECT_EQ(tig_art_tile_id_create(12, 12, 15, 0, 0, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x30CF000);
}

TEST(TigArtIdTest, RoofIdCreate)
{
    tig_art_id_t art_id;

    EXPECT_EQ(tig_art_roof_id_create(10, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0500000);

    EXPECT_EQ(tig_art_roof_id_create(2, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0100000);

    EXPECT_EQ(tig_art_roof_id_create(4, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0200000);

    EXPECT_EQ(tig_art_roof_id_create(5, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0280000);

    EXPECT_EQ(tig_art_roof_id_create(6, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0300000);

    EXPECT_EQ(tig_art_roof_id_create(1, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0080000);

    EXPECT_EQ(tig_art_roof_id_create(7, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0380000);

    EXPECT_EQ(tig_art_roof_id_create(11, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0580000);

    EXPECT_EQ(tig_art_roof_id_create(9, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0480000);

    EXPECT_EQ(tig_art_roof_id_create(3, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0180000);

    EXPECT_EQ(tig_art_roof_id_create(12, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0600000);

    EXPECT_EQ(tig_art_roof_id_create(13, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0680000);

    EXPECT_EQ(tig_art_roof_id_create(14, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0700000);

    EXPECT_EQ(tig_art_roof_id_create(15, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xA0780000);
}

TEST(TigArtIdTest, EyeCandyIdCreate)
{
    tig_art_id_t art_id;

    EXPECT_EQ(tig_art_eye_candy_id_create(27, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0D80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(27, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0D80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(381, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBE80018);

    EXPECT_EQ(tig_art_eye_candy_id_create(381, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBE80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(402, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC900098);

    EXPECT_EQ(tig_art_eye_candy_id_create(402, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC900088);

    EXPECT_EQ(tig_art_eye_candy_id_create(403, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC980098);

    EXPECT_EQ(tig_art_eye_candy_id_create(403, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC980088);

    EXPECT_EQ(tig_art_eye_candy_id_create(404, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECA00098);

    EXPECT_EQ(tig_art_eye_candy_id_create(404, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECA00088);

    EXPECT_EQ(tig_art_eye_candy_id_create(405, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECA80098);

    EXPECT_EQ(tig_art_eye_candy_id_create(405, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECA80088);

    EXPECT_EQ(tig_art_eye_candy_id_create(406, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECB00098);

    EXPECT_EQ(tig_art_eye_candy_id_create(406, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECB00088);

    EXPECT_EQ(tig_art_eye_candy_id_create(407, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECB80098);

    EXPECT_EQ(tig_art_eye_candy_id_create(407, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECB80088);

    EXPECT_EQ(tig_art_eye_candy_id_create(408, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECC00098);

    EXPECT_EQ(tig_art_eye_candy_id_create(408, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECC00088);

    EXPECT_EQ(tig_art_eye_candy_id_create(409, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECC80098);

    EXPECT_EQ(tig_art_eye_candy_id_create(409, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECC80088);

    EXPECT_EQ(tig_art_eye_candy_id_create(410, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECD00098);

    EXPECT_EQ(tig_art_eye_candy_id_create(410, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECD00088);

    EXPECT_EQ(tig_art_eye_candy_id_create(411, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECD80098);

    EXPECT_EQ(tig_art_eye_candy_id_create(411, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECD80088);

    EXPECT_EQ(tig_art_eye_candy_id_create(412, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECE00098);

    EXPECT_EQ(tig_art_eye_candy_id_create(412, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECE00088);

    EXPECT_EQ(tig_art_eye_candy_id_create(413, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECE80098);

    EXPECT_EQ(tig_art_eye_candy_id_create(413, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECE80088);

    EXPECT_EQ(tig_art_eye_candy_id_create(414, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECF00098);

    EXPECT_EQ(tig_art_eye_candy_id_create(414, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECF00088);

    EXPECT_EQ(tig_art_eye_candy_id_create(415, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECF80098);

    EXPECT_EQ(tig_art_eye_candy_id_create(415, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECF80088);

    EXPECT_EQ(tig_art_eye_candy_id_create(416, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED000098);

    EXPECT_EQ(tig_art_eye_candy_id_create(416, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED000088);

    EXPECT_EQ(tig_art_eye_candy_id_create(417, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED080098);

    EXPECT_EQ(tig_art_eye_candy_id_create(417, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED080088);

    EXPECT_EQ(tig_art_eye_candy_id_create(418, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED100098);

    EXPECT_EQ(tig_art_eye_candy_id_create(418, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED100088);

    EXPECT_EQ(tig_art_eye_candy_id_create(419, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED180098);

    EXPECT_EQ(tig_art_eye_candy_id_create(419, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED180088);

    EXPECT_EQ(tig_art_eye_candy_id_create(420, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED200098);

    EXPECT_EQ(tig_art_eye_candy_id_create(420, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED200088);

    EXPECT_EQ(tig_art_eye_candy_id_create(421, 0, 0, 0, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED280098);

    EXPECT_EQ(tig_art_eye_candy_id_create(421, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED280088);

    EXPECT_EQ(tig_art_eye_candy_id_create(377, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBC80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(377, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBC80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(378, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBD00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(378, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBD00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(379, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBD80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(379, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBD80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(379, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBD80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(380, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBE00018);

    EXPECT_EQ(tig_art_eye_candy_id_create(380, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBE00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(401, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC880018);

    EXPECT_EQ(tig_art_eye_candy_id_create(401, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC880008);

    EXPECT_EQ(tig_art_eye_candy_id_create(382, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBF00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(382, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBF00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(383, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBF80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(383, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBF80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(386, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC100118);

    EXPECT_EQ(tig_art_eye_candy_id_create(386, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC100008);

    EXPECT_EQ(tig_art_eye_candy_id_create(386, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC100048);

    EXPECT_EQ(tig_art_eye_candy_id_create(387, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC180198);

    EXPECT_EQ(tig_art_eye_candy_id_create(387, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC180088);

    EXPECT_EQ(tig_art_eye_candy_id_create(388, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC200118);

    EXPECT_EQ(tig_art_eye_candy_id_create(388, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC200008);

    EXPECT_EQ(tig_art_eye_candy_id_create(389, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC280118);

    EXPECT_EQ(tig_art_eye_candy_id_create(389, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC280008);

    EXPECT_EQ(tig_art_eye_candy_id_create(395, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC580018);

    EXPECT_EQ(tig_art_eye_candy_id_create(395, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC580008);

    EXPECT_EQ(tig_art_eye_candy_id_create(350, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAF00018);

    EXPECT_EQ(tig_art_eye_candy_id_create(350, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAF00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(500, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFA00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(500, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFA00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(501, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFA80110);

    EXPECT_EQ(tig_art_eye_candy_id_create(501, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFA80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(502, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFB00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(502, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFB00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(502, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFB00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(503, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFB80110);

    EXPECT_EQ(tig_art_eye_candy_id_create(503, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFB80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(503, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFB80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(504, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFC00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(504, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFC00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(504, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFC00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(505, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFC80110);

    EXPECT_EQ(tig_art_eye_candy_id_create(505, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFC80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(505, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFC80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(506, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFD00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(506, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFD00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(506, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFD00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(507, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFD80110);

    EXPECT_EQ(tig_art_eye_candy_id_create(507, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFD80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(507, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFD80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(508, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFE00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(508, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFE00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(508, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFE00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(509, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFE80110);

    EXPECT_EQ(tig_art_eye_candy_id_create(509, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFE80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(509, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFE80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(510, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFF00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(510, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFF00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(510, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFF00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(511, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFF80110);

    EXPECT_EQ(tig_art_eye_candy_id_create(511, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFF80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(511, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEFF80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(400, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC800118);

    EXPECT_EQ(tig_art_eye_candy_id_create(400, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC800008);

    EXPECT_EQ(tig_art_eye_candy_id_create(424, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED400108);

    EXPECT_EQ(tig_art_eye_candy_id_create(424, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED400008);

    EXPECT_EQ(tig_art_eye_candy_id_create(424, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED400118);

    EXPECT_EQ(tig_art_eye_candy_id_create(424, 0, 0, 1, 0, 2, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED400128);

    EXPECT_EQ(tig_art_eye_candy_id_create(424, 0, 0, 1, 0, 3, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED400138);

    EXPECT_EQ(tig_art_eye_candy_id_create(425, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED480108);

    EXPECT_EQ(tig_art_eye_candy_id_create(425, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(425, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED480048);

    EXPECT_EQ(tig_art_eye_candy_id_create(426, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED500108);

    EXPECT_EQ(tig_art_eye_candy_id_create(426, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(426, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED500048);

    EXPECT_EQ(tig_art_eye_candy_id_create(427, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED580108);

    EXPECT_EQ(tig_art_eye_candy_id_create(427, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED580008);

    EXPECT_EQ(tig_art_eye_candy_id_create(427, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED580048);

    EXPECT_EQ(tig_art_eye_candy_id_create(98, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3100118);

    EXPECT_EQ(tig_art_eye_candy_id_create(98, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3100008);

    EXPECT_EQ(tig_art_eye_candy_id_create(350, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAF00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(428, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED600118);

    EXPECT_EQ(tig_art_eye_candy_id_create(428, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED600008);

    EXPECT_EQ(tig_art_eye_candy_id_create(25, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0C80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(25, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0C80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(25, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0C80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(349, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAE80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(349, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAE80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(439, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDB80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(439, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDB80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(440, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDC00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(440, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDC00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(441, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDC80108);

    EXPECT_EQ(tig_art_eye_candy_id_create(441, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDC80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(441, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDC80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(441, 0, 0, 1, 0, 2, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDC80128);

    EXPECT_EQ(tig_art_eye_candy_id_create(441, 0, 0, 1, 0, 3, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDC80138);

    EXPECT_EQ(tig_art_eye_candy_id_create(442, 0, 0, 1, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD00188);

    EXPECT_EQ(tig_art_eye_candy_id_create(442, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD00088);

    EXPECT_EQ(tig_art_eye_candy_id_create(442, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD00198);

    EXPECT_EQ(tig_art_eye_candy_id_create(442, 0, 0, 1, 2, 2, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD001A8);

    EXPECT_EQ(tig_art_eye_candy_id_create(442, 0, 0, 1, 2, 3, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD001B8);

    EXPECT_EQ(tig_art_eye_candy_id_create(443, 0, 0, 1, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD80188);

    EXPECT_EQ(tig_art_eye_candy_id_create(443, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD80088);

    EXPECT_EQ(tig_art_eye_candy_id_create(443, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD80198);

    EXPECT_EQ(tig_art_eye_candy_id_create(443, 0, 0, 1, 2, 2, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD801A8);

    EXPECT_EQ(tig_art_eye_candy_id_create(443, 0, 0, 1, 2, 3, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDD801B8);

    EXPECT_EQ(tig_art_eye_candy_id_create(438, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDB00108);

    EXPECT_EQ(tig_art_eye_candy_id_create(438, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEDB00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(11, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0580110);

    EXPECT_EQ(tig_art_eye_candy_id_create(11, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0580008);

    EXPECT_EQ(tig_art_eye_candy_id_create(15, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0780198);

    EXPECT_EQ(tig_art_eye_candy_id_create(10, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0500114);

    EXPECT_EQ(tig_art_eye_candy_id_create(10, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(10, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0500048);

    EXPECT_EQ(tig_art_eye_candy_id_create(16, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0800118);

    EXPECT_EQ(tig_art_eye_candy_id_create(16, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0800008);

    EXPECT_EQ(tig_art_eye_candy_id_create(15, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0780118);

    EXPECT_EQ(tig_art_eye_candy_id_create(15, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0780008);

    EXPECT_EQ(tig_art_eye_candy_id_create(15, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0780048);

    EXPECT_EQ(tig_art_eye_candy_id_create(10, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(22, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0B00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(22, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0B00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(28, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0E00114);

    EXPECT_EQ(tig_art_eye_candy_id_create(28, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0E00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(35, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1180198);

    EXPECT_EQ(tig_art_eye_candy_id_create(35, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1180088);

    EXPECT_EQ(tig_art_eye_candy_id_create(28, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0E00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(41, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1480118);

    EXPECT_EQ(tig_art_eye_candy_id_create(41, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(28, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0E0011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(47, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1780110);

    EXPECT_EQ(tig_art_eye_candy_id_create(47, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1780008);

    EXPECT_EQ(tig_art_eye_candy_id_create(47, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1780048);

    EXPECT_EQ(tig_art_eye_candy_id_create(362, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(362, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(68, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2200114);

    EXPECT_EQ(tig_art_eye_candy_id_create(68, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2200008);

    EXPECT_EQ(tig_art_eye_candy_id_create(55, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1B80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(55, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1B80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(68, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2200118);

    EXPECT_EQ(tig_art_eye_candy_id_create(48, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1800118);

    EXPECT_EQ(tig_art_eye_candy_id_create(48, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1800008);

    EXPECT_EQ(tig_art_eye_candy_id_create(48, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1800048);

    EXPECT_EQ(tig_art_eye_candy_id_create(47, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1780118);

    EXPECT_EQ(tig_art_eye_candy_id_create(59, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1D80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(59, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1D80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(47, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE178011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(69, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2280114);

    EXPECT_EQ(tig_art_eye_candy_id_create(69, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2280008);

    EXPECT_EQ(tig_art_eye_candy_id_create(69, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2280048);

    EXPECT_EQ(tig_art_eye_candy_id_create(363, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB580118);

    EXPECT_EQ(tig_art_eye_candy_id_create(363, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB580008);

    EXPECT_EQ(tig_art_eye_candy_id_create(90, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2D00114);

    EXPECT_EQ(tig_art_eye_candy_id_create(90, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2D00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(70, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2300114);

    EXPECT_EQ(tig_art_eye_candy_id_create(70, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2300008);

    EXPECT_EQ(tig_art_eye_candy_id_create(74, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2500018);

    EXPECT_EQ(tig_art_eye_candy_id_create(74, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(90, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2D00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(69, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2280118);

    EXPECT_EQ(tig_art_eye_candy_id_create(85, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2A80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(85, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2A80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(85, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2A80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(69, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE228011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(91, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2D80114);

    EXPECT_EQ(tig_art_eye_candy_id_create(91, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2D80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(91, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2D80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(364, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB600118);

    EXPECT_EQ(tig_art_eye_candy_id_create(364, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB600008);

    EXPECT_EQ(tig_art_eye_candy_id_create(114, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3900114);

    EXPECT_EQ(tig_art_eye_candy_id_create(114, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3900008);

    EXPECT_EQ(tig_art_eye_candy_id_create(114, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3900118);

    EXPECT_EQ(tig_art_eye_candy_id_create(99, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3180118);

    EXPECT_EQ(tig_art_eye_candy_id_create(99, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3180008);

    EXPECT_EQ(tig_art_eye_candy_id_create(91, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2D8011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(105, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3480118);

    EXPECT_EQ(tig_art_eye_candy_id_create(105, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(91, 0, 0, 1, 0, 1, 6, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE2D8011C);

    EXPECT_EQ(tig_art_eye_candy_id_create(115, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3980114);

    EXPECT_EQ(tig_art_eye_candy_id_create(115, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3980008);

    EXPECT_EQ(tig_art_eye_candy_id_create(115, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3980048);

    EXPECT_EQ(tig_art_eye_candy_id_create(365, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB680118);

    EXPECT_EQ(tig_art_eye_candy_id_create(365, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB680008);

    EXPECT_EQ(tig_art_eye_candy_id_create(135, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4380114);

    EXPECT_EQ(tig_art_eye_candy_id_create(135, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4380008);

    EXPECT_EQ(tig_art_eye_candy_id_create(116, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3A00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(116, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3A00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(117, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3A80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(117, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3A80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(135, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4380118);

    EXPECT_EQ(tig_art_eye_candy_id_create(123, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3D80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(123, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3D80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(124, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3E00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(124, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3E00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(115, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3980118);

    EXPECT_EQ(tig_art_eye_candy_id_create(126, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3F00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(126, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE3F00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(115, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE398011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(9, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0480118);

    EXPECT_EQ(tig_art_eye_candy_id_create(9, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(159, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4F80114);

    EXPECT_EQ(tig_art_eye_candy_id_create(159, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4F80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(137, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4480118);

    EXPECT_EQ(tig_art_eye_candy_id_create(137, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(138, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(138, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(139, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4580118);

    EXPECT_EQ(tig_art_eye_candy_id_create(139, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4580008);

    EXPECT_EQ(tig_art_eye_candy_id_create(141, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4680118);

    EXPECT_EQ(tig_art_eye_candy_id_create(141, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4680008);

    EXPECT_EQ(tig_art_eye_candy_id_create(141, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4680048);

    EXPECT_EQ(tig_art_eye_candy_id_create(159, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4F80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(159, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4F8011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(146, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4900118);

    EXPECT_EQ(tig_art_eye_candy_id_create(146, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4900008);

    EXPECT_EQ(tig_art_eye_candy_id_create(150, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4B00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(150, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4B00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(150, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4B00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(136, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE440011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(136, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4400008);

    EXPECT_EQ(tig_art_eye_candy_id_create(136, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4400048);

    EXPECT_EQ(tig_art_eye_candy_id_create(154, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4D00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(154, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4D00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(158, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4F00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(158, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE4F00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(160, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5000114);

    EXPECT_EQ(tig_art_eye_candy_id_create(160, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5000008);

    EXPECT_EQ(tig_art_eye_candy_id_create(160, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5000048);

    EXPECT_EQ(tig_art_eye_candy_id_create(161, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5080118);

    EXPECT_EQ(tig_art_eye_candy_id_create(161, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5080008);

    EXPECT_EQ(tig_art_eye_candy_id_create(161, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5080048);

    EXPECT_EQ(tig_art_eye_candy_id_create(175, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5780114);

    EXPECT_EQ(tig_art_eye_candy_id_create(175, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5780008);

    EXPECT_EQ(tig_art_eye_candy_id_create(164, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5200118);

    EXPECT_EQ(tig_art_eye_candy_id_create(164, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5200008);

    EXPECT_EQ(tig_art_eye_candy_id_create(164, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5200048);

    EXPECT_EQ(tig_art_eye_candy_id_create(175, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5780118);

    EXPECT_EQ(tig_art_eye_candy_id_create(167, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5380118);

    EXPECT_EQ(tig_art_eye_candy_id_create(167, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5380008);

    EXPECT_EQ(tig_art_eye_candy_id_create(167, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5380048);

    EXPECT_EQ(tig_art_eye_candy_id_create(160, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5000118);

    EXPECT_EQ(tig_art_eye_candy_id_create(170, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(170, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(170, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5500048);

    EXPECT_EQ(tig_art_eye_candy_id_create(160, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE500011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(173, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5680118);

    EXPECT_EQ(tig_art_eye_candy_id_create(173, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5680008);

    EXPECT_EQ(tig_art_eye_candy_id_create(173, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5680048);

    EXPECT_EQ(tig_art_eye_candy_id_create(174, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5700118);

    EXPECT_EQ(tig_art_eye_candy_id_create(174, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5700008);

    EXPECT_EQ(tig_art_eye_candy_id_create(174, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5700048);

    EXPECT_EQ(tig_art_eye_candy_id_create(176, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5800110);

    EXPECT_EQ(tig_art_eye_candy_id_create(176, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5800008);

    EXPECT_EQ(tig_art_eye_candy_id_create(176, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5800048);

    EXPECT_EQ(tig_art_eye_candy_id_create(177, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5880118);

    EXPECT_EQ(tig_art_eye_candy_id_create(177, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5880008);

    EXPECT_EQ(tig_art_eye_candy_id_create(177, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5880048);

    EXPECT_EQ(tig_art_eye_candy_id_create(178, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5900118);

    EXPECT_EQ(tig_art_eye_candy_id_create(178, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5900008);

    EXPECT_EQ(tig_art_eye_candy_id_create(180, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5A00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(180, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5A00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(180, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5A00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(176, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5800114);

    EXPECT_EQ(tig_art_eye_candy_id_create(183, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5B80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(183, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5B80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(183, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5B80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(184, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5C00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(184, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5C00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(191, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5F80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(191, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5F80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(192, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6000118);

    EXPECT_EQ(tig_art_eye_candy_id_create(192, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6000008);

    EXPECT_EQ(tig_art_eye_candy_id_create(176, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5800118);

    EXPECT_EQ(tig_art_eye_candy_id_create(187, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5D80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(187, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5D80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(187, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5D80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(188, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5E00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(188, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5E00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(188, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE5E00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(195, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6180114);

    EXPECT_EQ(tig_art_eye_candy_id_create(195, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6180008);

    EXPECT_EQ(tig_art_eye_candy_id_create(195, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6180048);

    EXPECT_EQ(tig_art_eye_candy_id_create(196, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6200118);

    EXPECT_EQ(tig_art_eye_candy_id_create(196, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6200008);

    EXPECT_EQ(tig_art_eye_candy_id_create(196, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6200048);

    EXPECT_EQ(tig_art_eye_candy_id_create(195, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6180118);

    EXPECT_EQ(tig_art_eye_candy_id_create(205, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6680118);

    EXPECT_EQ(tig_art_eye_candy_id_create(205, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6680008);

    EXPECT_EQ(tig_art_eye_candy_id_create(202, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(202, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(195, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE618011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(211, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6980114);

    EXPECT_EQ(tig_art_eye_candy_id_create(211, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6980008);

    EXPECT_EQ(tig_art_eye_candy_id_create(211, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6980048);

    EXPECT_EQ(tig_art_eye_candy_id_create(215, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6B80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(215, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6B80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(234, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(234, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(212, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6A00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(212, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6A00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(212, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6A00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(213, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6A80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(213, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6A80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(213, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6A80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(211, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6980118);

    EXPECT_EQ(tig_art_eye_candy_id_create(218, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6D00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(218, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6D00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(219, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6D80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(219, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6D80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(211, 0, 0, 1, 0, 1, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6980110);

    EXPECT_EQ(tig_art_eye_candy_id_create(211, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE698011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(221, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6E80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(221, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6E80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(222, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6F00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(222, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE6F00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(250, 0, 0, 1, 0, 0, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7D00104);

    EXPECT_EQ(tig_art_eye_candy_id_create(250, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7D00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(236, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7600118);

    EXPECT_EQ(tig_art_eye_candy_id_create(236, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7600008);

    EXPECT_EQ(tig_art_eye_candy_id_create(236, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7600048);

    EXPECT_EQ(tig_art_eye_candy_id_create(235, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7580108);

    EXPECT_EQ(tig_art_eye_candy_id_create(235, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7580008);

    EXPECT_EQ(tig_art_eye_candy_id_create(235, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7580048);

    EXPECT_EQ(tig_art_eye_candy_id_create(242, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7900118);

    EXPECT_EQ(tig_art_eye_candy_id_create(242, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7900008);

    EXPECT_EQ(tig_art_eye_candy_id_create(243, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7980118);

    EXPECT_EQ(tig_art_eye_candy_id_create(243, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7980008);

    EXPECT_EQ(tig_art_eye_candy_id_create(246, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7B00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(246, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7B00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(235, 0, 0, 1, 0, 0, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE758010A);

    EXPECT_EQ(tig_art_eye_candy_id_create(248, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7C00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(248, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7C00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(248, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7C00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(264, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8400114);

    EXPECT_EQ(tig_art_eye_candy_id_create(264, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8400008);

    EXPECT_EQ(tig_art_eye_candy_id_create(252, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7E00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(252, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7E00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(264, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8400118);

    EXPECT_EQ(tig_art_eye_candy_id_create(255, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7F80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(255, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7F80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(255, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7F80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(235, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7580118);

    EXPECT_EQ(tig_art_eye_candy_id_create(258, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8100118);

    EXPECT_EQ(tig_art_eye_candy_id_create(258, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8100008);

    EXPECT_EQ(tig_art_eye_candy_id_create(258, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8100048);

    EXPECT_EQ(tig_art_eye_candy_id_create(259, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8180118);

    EXPECT_EQ(tig_art_eye_candy_id_create(259, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8180008);

    EXPECT_EQ(tig_art_eye_candy_id_create(259, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8180048);

    EXPECT_EQ(tig_art_eye_candy_id_create(260, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8200118);

    EXPECT_EQ(tig_art_eye_candy_id_create(260, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8200008);

    EXPECT_EQ(tig_art_eye_candy_id_create(260, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8200048);

    EXPECT_EQ(tig_art_eye_candy_id_create(235, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE758011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(263, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8380118);

    EXPECT_EQ(tig_art_eye_candy_id_create(263, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8380008);

    EXPECT_EQ(tig_art_eye_candy_id_create(263, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8380048);

    EXPECT_EQ(tig_art_eye_candy_id_create(432, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED800118);

    EXPECT_EQ(tig_art_eye_candy_id_create(432, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED800008);

    EXPECT_EQ(tig_art_eye_candy_id_create(265, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8480114);

    EXPECT_EQ(tig_art_eye_candy_id_create(265, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(265, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8480048);

    EXPECT_EQ(tig_art_eye_candy_id_create(266, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(266, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(283, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8D80114);

    EXPECT_EQ(tig_art_eye_candy_id_create(283, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8D80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(265, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8480118);

    EXPECT_EQ(tig_art_eye_candy_id_create(276, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8A00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(276, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8A00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(276, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8A00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(277, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8A80018);

    EXPECT_EQ(tig_art_eye_candy_id_create(277, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8A80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(277, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8A80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(283, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8D80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(265, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE848011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(290, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9100118);

    EXPECT_EQ(tig_art_eye_candy_id_create(290, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9100008);

    EXPECT_EQ(tig_art_eye_candy_id_create(328, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA400118);

    EXPECT_EQ(tig_art_eye_candy_id_create(328, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA400008);

    EXPECT_EQ(tig_art_eye_candy_id_create(299, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9580118);

    EXPECT_EQ(tig_art_eye_candy_id_create(299, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9580008);

    EXPECT_EQ(tig_art_eye_candy_id_create(299, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9580048);

    EXPECT_EQ(tig_art_eye_candy_id_create(300, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9600018);

    EXPECT_EQ(tig_art_eye_candy_id_create(300, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9600008);

    EXPECT_EQ(tig_art_eye_candy_id_create(300, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9600048);

    EXPECT_EQ(tig_art_eye_candy_id_create(293, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9280114);

    EXPECT_EQ(tig_art_eye_candy_id_create(293, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9280008);

    EXPECT_EQ(tig_art_eye_candy_id_create(293, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9280048);

    EXPECT_EQ(tig_art_eye_candy_id_create(306, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9900118);

    EXPECT_EQ(tig_art_eye_candy_id_create(306, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9900008);

    EXPECT_EQ(tig_art_eye_candy_id_create(306, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9900048);

    EXPECT_EQ(tig_art_eye_candy_id_create(293, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9280118);

    EXPECT_EQ(tig_art_eye_candy_id_create(313, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9C80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(313, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9C80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(313, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE9C80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(327, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA380118);

    EXPECT_EQ(tig_art_eye_candy_id_create(327, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA380008);

    EXPECT_EQ(tig_art_eye_candy_id_create(327, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA380048);

    EXPECT_EQ(tig_art_eye_candy_id_create(293, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE928011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(320, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA000118);

    EXPECT_EQ(tig_art_eye_candy_id_create(320, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA000008);

    EXPECT_EQ(tig_art_eye_candy_id_create(320, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA000048);

    EXPECT_EQ(tig_art_eye_candy_id_create(329, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA480114);

    EXPECT_EQ(tig_art_eye_candy_id_create(329, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(329, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA480048);

    EXPECT_EQ(tig_art_eye_candy_id_create(330, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(330, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(331, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA580118);

    EXPECT_EQ(tig_art_eye_candy_id_create(331, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA580008);

    EXPECT_EQ(tig_art_eye_candy_id_create(333, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA680118);

    EXPECT_EQ(tig_art_eye_candy_id_create(333, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA680008);

    EXPECT_EQ(tig_art_eye_candy_id_create(333, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA680048);

    EXPECT_EQ(tig_art_eye_candy_id_create(334, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA700118);

    EXPECT_EQ(tig_art_eye_candy_id_create(334, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA700008);

    EXPECT_EQ(tig_art_eye_candy_id_create(334, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA700048);

    EXPECT_EQ(tig_art_eye_candy_id_create(329, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA480118);

    EXPECT_EQ(tig_art_eye_candy_id_create(339, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA980118);

    EXPECT_EQ(tig_art_eye_candy_id_create(339, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA980008);

    EXPECT_EQ(tig_art_eye_candy_id_create(339, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA980048);

    EXPECT_EQ(tig_art_eye_candy_id_create(340, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAA00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(340, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAA00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(340, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAA00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(336, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA800118);

    EXPECT_EQ(tig_art_eye_candy_id_create(336, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA800008);

    EXPECT_EQ(tig_art_eye_candy_id_create(336, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA800048);

    EXPECT_EQ(tig_art_eye_candy_id_create(337, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA880118);

    EXPECT_EQ(tig_art_eye_candy_id_create(337, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA880008);

    EXPECT_EQ(tig_art_eye_candy_id_create(337, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA880048);

    EXPECT_EQ(tig_art_eye_candy_id_create(329, 0, 0, 1, 0, 1, 5, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEA48011A);

    EXPECT_EQ(tig_art_eye_candy_id_create(343, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAB80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(343, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAB80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(343, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAB80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(402, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC900198);

    EXPECT_EQ(tig_art_eye_candy_id_create(403, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC980198);

    EXPECT_EQ(tig_art_eye_candy_id_create(404, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECA00198);

    EXPECT_EQ(tig_art_eye_candy_id_create(405, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECA80198);

    EXPECT_EQ(tig_art_eye_candy_id_create(406, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECB00198);

    EXPECT_EQ(tig_art_eye_candy_id_create(407, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECB80198);

    EXPECT_EQ(tig_art_eye_candy_id_create(408, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECC00198);

    EXPECT_EQ(tig_art_eye_candy_id_create(409, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECC80198);

    EXPECT_EQ(tig_art_eye_candy_id_create(410, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECD00198);

    EXPECT_EQ(tig_art_eye_candy_id_create(411, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECD80198);

    EXPECT_EQ(tig_art_eye_candy_id_create(412, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECE00198);

    EXPECT_EQ(tig_art_eye_candy_id_create(413, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECE80198);

    EXPECT_EQ(tig_art_eye_candy_id_create(414, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECF00198);

    EXPECT_EQ(tig_art_eye_candy_id_create(415, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xECF80198);

    EXPECT_EQ(tig_art_eye_candy_id_create(416, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED000198);

    EXPECT_EQ(tig_art_eye_candy_id_create(417, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED080198);

    EXPECT_EQ(tig_art_eye_candy_id_create(418, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED100198);

    EXPECT_EQ(tig_art_eye_candy_id_create(419, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED180198);

    EXPECT_EQ(tig_art_eye_candy_id_create(420, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED200198);

    EXPECT_EQ(tig_art_eye_candy_id_create(261, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8280118);

    EXPECT_EQ(tig_art_eye_candy_id_create(261, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8280008);

    EXPECT_EQ(tig_art_eye_candy_id_create(261, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE8280048);

    EXPECT_EQ(tig_art_eye_candy_id_create(239, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7780118);

    EXPECT_EQ(tig_art_eye_candy_id_create(239, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7780008);

    EXPECT_EQ(tig_art_eye_candy_id_create(239, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE7780048);

    EXPECT_EQ(tig_art_eye_candy_id_create(384, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC000118);

    EXPECT_EQ(tig_art_eye_candy_id_create(384, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC000008);

    EXPECT_EQ(tig_art_eye_candy_id_create(385, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC080118);

    EXPECT_EQ(tig_art_eye_candy_id_create(385, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC080008);

    EXPECT_EQ(tig_art_eye_candy_id_create(371, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB980018);

    EXPECT_EQ(tig_art_eye_candy_id_create(371, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB980008);

    EXPECT_EQ(tig_art_eye_candy_id_create(371, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB980048);

    EXPECT_EQ(tig_art_eye_candy_id_create(372, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBA00018);

    EXPECT_EQ(tig_art_eye_candy_id_create(372, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBA00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(372, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBA00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(373, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBA80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(373, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBA80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(373, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBA80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(368, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB800118);

    EXPECT_EQ(tig_art_eye_candy_id_create(368, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB800008);

    EXPECT_EQ(tig_art_eye_candy_id_create(373, 0, 0, 1, 0, 1, 2, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBA80114);

    EXPECT_EQ(tig_art_eye_candy_id_create(373, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBA80108);

    EXPECT_EQ(tig_art_eye_candy_id_create(368, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB800108);

    EXPECT_EQ(tig_art_eye_candy_id_create(369, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB880118);

    EXPECT_EQ(tig_art_eye_candy_id_create(369, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB880008);

    EXPECT_EQ(tig_art_eye_candy_id_create(367, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB780118);

    EXPECT_EQ(tig_art_eye_candy_id_create(367, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB780008);

    EXPECT_EQ(tig_art_eye_candy_id_create(366, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB700118);

    EXPECT_EQ(tig_art_eye_candy_id_create(366, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB700008);

    EXPECT_EQ(tig_art_eye_candy_id_create(368, 0, 0, 0, 2, 2, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB8000A8);

    EXPECT_EQ(tig_art_eye_candy_id_create(55, 0, 0, 1, 0, 2, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE1B80128);

    EXPECT_EQ(tig_art_eye_candy_id_create(392, 0, 0, 1, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC400108);

    EXPECT_EQ(tig_art_eye_candy_id_create(392, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC400008);

    EXPECT_EQ(tig_art_eye_candy_id_create(392, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC400048);

    EXPECT_EQ(tig_art_eye_candy_id_create(393, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC480118);

    EXPECT_EQ(tig_art_eye_candy_id_create(393, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(393, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC480048);

    EXPECT_EQ(tig_art_eye_candy_id_create(396, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC600118);

    EXPECT_EQ(tig_art_eye_candy_id_create(396, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC600008);

    EXPECT_EQ(tig_art_eye_candy_id_create(391, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC380118);

    EXPECT_EQ(tig_art_eye_candy_id_create(391, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC380008);

    EXPECT_EQ(tig_art_eye_candy_id_create(390, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC300118);

    EXPECT_EQ(tig_art_eye_candy_id_create(390, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC300008);

    EXPECT_EQ(tig_art_eye_candy_id_create(394, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC500118);

    EXPECT_EQ(tig_art_eye_candy_id_create(394, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC500008);

    EXPECT_EQ(tig_art_eye_candy_id_create(433, 0, 0, 1, 2, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED880198);

    EXPECT_EQ(tig_art_eye_candy_id_create(433, 0, 0, 0, 2, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED880088);

    EXPECT_EQ(tig_art_eye_candy_id_create(1, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0080018);

    EXPECT_EQ(tig_art_eye_candy_id_create(1, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0080008);

    EXPECT_EQ(tig_art_eye_candy_id_create(360, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB400118);

    EXPECT_EQ(tig_art_eye_candy_id_create(360, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB400008);

    EXPECT_EQ(tig_art_eye_candy_id_create(358, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB300118);

    EXPECT_EQ(tig_art_eye_candy_id_create(358, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB300008);

    EXPECT_EQ(tig_art_eye_candy_id_create(359, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB380118);

    EXPECT_EQ(tig_art_eye_candy_id_create(359, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB380008);

    EXPECT_EQ(tig_art_eye_candy_id_create(2, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0100018);

    EXPECT_EQ(tig_art_eye_candy_id_create(2, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xE0100008);

    EXPECT_EQ(tig_art_eye_candy_id_create(361, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB480018);

    EXPECT_EQ(tig_art_eye_candy_id_create(361, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB480008);

    EXPECT_EQ(tig_art_eye_candy_id_create(429, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED680018);

    EXPECT_EQ(tig_art_eye_candy_id_create(429, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED680008);

    EXPECT_EQ(tig_art_eye_candy_id_create(430, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED700018);

    EXPECT_EQ(tig_art_eye_candy_id_create(430, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED700008);

    EXPECT_EQ(tig_art_eye_candy_id_create(431, 0, 0, 0, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED780018);

    EXPECT_EQ(tig_art_eye_candy_id_create(431, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED780008);

    EXPECT_EQ(tig_art_eye_candy_id_create(370, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB900118);

    EXPECT_EQ(tig_art_eye_candy_id_create(370, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB900008);

    EXPECT_EQ(tig_art_eye_candy_id_create(370, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB900048);

    EXPECT_EQ(tig_art_eye_candy_id_create(375, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBB80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(375, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBB80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(375, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBB80048);

    EXPECT_EQ(tig_art_eye_candy_id_create(422, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED300118);

    EXPECT_EQ(tig_art_eye_candy_id_create(422, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED300008);

    EXPECT_EQ(tig_art_eye_candy_id_create(422, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED300048);

    EXPECT_EQ(tig_art_eye_candy_id_create(423, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED380118);

    EXPECT_EQ(tig_art_eye_candy_id_create(423, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED380008);

    EXPECT_EQ(tig_art_eye_candy_id_create(423, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xED380048);

    EXPECT_EQ(tig_art_eye_candy_id_create(398, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC700118);

    EXPECT_EQ(tig_art_eye_candy_id_create(398, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC700008);

    EXPECT_EQ(tig_art_eye_candy_id_create(398, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEC700048);

    EXPECT_EQ(tig_art_eye_candy_id_create(376, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBC00118);

    EXPECT_EQ(tig_art_eye_candy_id_create(376, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBC00008);

    EXPECT_EQ(tig_art_eye_candy_id_create(376, 0, 0, 0, 1, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEBC00048);

    EXPECT_EQ(tig_art_eye_candy_id_create(355, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB180118);

    EXPECT_EQ(tig_art_eye_candy_id_create(355, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB180008);

    EXPECT_EQ(tig_art_eye_candy_id_create(356, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB200118);

    EXPECT_EQ(tig_art_eye_candy_id_create(356, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB200008);

    EXPECT_EQ(tig_art_eye_candy_id_create(351, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAF80118);

    EXPECT_EQ(tig_art_eye_candy_id_create(351, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEAF80008);

    EXPECT_EQ(tig_art_eye_candy_id_create(354, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB100118);

    EXPECT_EQ(tig_art_eye_candy_id_create(354, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB100008);

    EXPECT_EQ(tig_art_eye_candy_id_create(353, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB080118);

    EXPECT_EQ(tig_art_eye_candy_id_create(353, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB080008);

    EXPECT_EQ(tig_art_eye_candy_id_create(352, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB000118);

    EXPECT_EQ(tig_art_eye_candy_id_create(352, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB000008);

    EXPECT_EQ(tig_art_eye_candy_id_create(357, 0, 0, 1, 0, 1, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB280118);

    EXPECT_EQ(tig_art_eye_candy_id_create(357, 0, 0, 0, 0, 0, 4, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0xEB280008);
}

TEST(TigArtIdTest, LightIdCreate)
{
    tig_art_id_t art_id;

    EXPECT_EQ(tig_art_light_id_create(1, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90080000);

    EXPECT_EQ(tig_art_light_id_create(4, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90200000);

    EXPECT_EQ(tig_art_light_id_create(8, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90400000);

    EXPECT_EQ(tig_art_light_id_create(3, 0, 0, 0, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90180000);

    EXPECT_EQ(tig_art_light_id_create(0, 0, 0, 1, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90000001);

    EXPECT_EQ(tig_art_light_id_create(1, 0, 0, 1, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90080001);

    EXPECT_EQ(tig_art_light_id_create(2, 0, 0, 1, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90100001);

    EXPECT_EQ(tig_art_light_id_create(3, 0, 0, 1, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90180001);

    EXPECT_EQ(tig_art_light_id_create(4, 0, 0, 1, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90200001);

    EXPECT_EQ(tig_art_light_id_create(5, 0, 0, 1, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90280001);

    EXPECT_EQ(tig_art_light_id_create(6, 0, 0, 1, &art_id), TIG_OK);
    EXPECT_EQ(art_id, 0x90300001);
}
