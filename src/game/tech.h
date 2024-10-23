#ifndef ARCANUM_GAME_TECH_H_
#define ARCANUM_GAME_TECH_H_

#include "game/context.h"
#include "game/obj.h"

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
} Tech;

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
} Degree;

static_assert(DEGREE_COUNT == 8, "wrong size");

bool tech_init(GameInitInfo* init_info);
void tech_exit();
void tech_set_defaults(object_id_t obj);
const char* tech_get_name(int tech);
const char* tech_get_description(int tech);
const char* degree_get_name(int degree);
const char* degree_get_description(int degree, int tech);
int tech_get_degree(long long object_id, int tech);
int tech_inc_degree(int64_t obj, int tech);
int tech_dec_degree(int64_t obj, int tech);
int tech_get_cost_for_degree(int a1);
int sub_4B00B0(object_id_t obj, int tech);
int tech_get_min_intelligence_for_degree(int degree);
void tech_learn_schematic(int64_t pc_obj, int64_t written_obj);
bool sub_4B02B0(object_id_t obj, int intelligence);
int sub_4B0320(int a1, int a2);

#endif /* ARCANUM_GAME_TECH_H_ */
