#ifndef ARCANUM_GAME_LIB_TECH_H_
#define ARCANUM_GAME_LIB_TECH_H_

#include "game/context.h"

typedef enum Tech {
    TECH_HERBOLOGY,
    TECH_CHEMISTRY,
    TECH_ELECTRIC,
    TECH_EXPLOSIVES,
    TECH_GUN,
    TECH_MECHANICAL,
    TECH_SMITHY,
    TECH_THERAPEUTICS,
    TECH_COUNT,
};

static_assert(TECH_COUNT == 8, "wrong size");

typedef enum Degree {
    DEGREE_LAYMAN,
    DEGREE_NOVICE,
    DEGREE_ASSISTANT,
    DEGREE_ASSOCIATE,
    DEGREE_TECHNICIAN,
    DEGREE_ENGINEER,
    DEGREE_PROFESSOR,
    DEGREE_DOCTORATE,
    DEGREE_COUNT,
};

static_assert(DEGREE_COUNT == 8, "wrong size");

bool tech_init(GameContext* ctx);
void tech_exit();
void tech_set_defaults(object_id_t obj);
const char* tech_get_name(int tech);
const char* tech_get_description(int tech);
const char* degree_get_name(int degree);
const char* degree_get_description(int degree, int tech);
int sub_4B00A0(int a1);
int sub_4B00B0(object_id_t obj, int tech);
int tech_get_min_intelligence_for_degree(int degree);
bool sub_4B02B0(object_id_t obj, int intelligence);

#endif /* ARCANUM_GAME_LIB_TECH_H_ */
