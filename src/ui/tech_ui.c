#include "ui/tech_ui.h"

#include "game/multiplayer.h"
#include "game/player.h"
#include "game/stat.h"
#include "game/tech.h"
#include "ui/charedit_ui.h"
#include "ui/intgame.h"

// 0x589A20
void tech_ui_inc_degree(int64_t obj, int tech)
{
    int degree;
    int cost;
    int points;
    bool is_pc;

    if (!tig_net_is_active()
        || tig_net_is_host()
        || multiplayer_is_locked()) {
        degree = tech_degree_get(obj, tech);
        cost = tech_degree_cost_get(degree + 1);
        points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        is_pc = player_is_pc_obj(obj);

        if (cost > points) {
            if (is_pc) {
                charedit_error_not_enough_character_points();
            }
        } else {
            if (tech_degree_inc(obj, tech) > degree) {
                stat_base_set(obj, STAT_UNSPENT_POINTS, points - cost);
                if (is_pc) {
                    sub_55A230();
                    sub_550720();
                }
            } else {
                if (is_pc) {
                    charedit_error_not_enough_intelligence();
                }
            }
        }
    }
}

// 0x589AF0
void tech_ui_dec_degree(int64_t obj, int tech)
{
    int degree;
    int cost;
    int points;
    bool is_pc;

    if (!tig_net_is_active()
        || tig_net_is_host()
        || multiplayer_is_locked()) {
        degree = tech_degree_get(obj, tech);
        cost = tech_degree_cost_get(degree);
        points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        is_pc = player_is_pc_obj(obj);

        if (tech_degree_dec(obj, tech) < degree) {
            stat_base_set(obj, STAT_UNSPENT_POINTS, points + cost);
            if (is_pc) {
                sub_55A230();
                sub_550720();
            }
        }
    }
}

// 0x589B90
void tech_ui_adjust_degree(int64_t obj, int tech, int action)
{
    switch (action) {
    case 1:
        tech_ui_inc_degree(obj, tech);
        break;
    case -1:
        tech_ui_dec_degree(obj, tech);
        break;
    }
}
