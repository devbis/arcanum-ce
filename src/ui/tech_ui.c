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

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0
        || multiplayer_is_locked()) {
        degree = tech_get_degree(obj, tech);
        cost = tech_get_cost_for_degree(degree + 1);
        points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        is_pc = player_is_pc_obj(obj);

        if (cost > points) {
            if (is_pc) {
                sub_55F160();
            }
        } else {
            if (tech_inc_degree(obj, tech) > degree) {
                stat_base_set(obj, STAT_UNSPENT_POINTS, points - cost);
                if (is_pc) {
                    sub_55A230();
                    sub_550720();
                }
            } else {
                if (is_pc) {
                    sub_55F1A0();
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

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0
        || multiplayer_is_locked()) {
        degree = tech_get_degree(obj, tech);
        cost = tech_get_cost_for_degree(degree);
        points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        is_pc = player_is_pc_obj(obj);

        if (tech_dec_degree(obj, tech) < degree) {
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
