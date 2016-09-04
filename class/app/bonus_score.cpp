#include "bonus_score.h"

#include <vector>

#include <class/number_generator.h>

using namespace app_game;

bonus_score::bonus_score():
	pickup(), score(0)
{
	//TODO: Do a better work determining this...
	std::vector<int> sc{10, 20, 50, 100, 200};
	tools::int_generator ig(0, sc.size()-1);
	score=sc[ig()];
}

void bonus_score::transform_draw_struct(draw_control &dc)const
{
	dc.set(1);
	auto& b=dc[0];

	auto color=ldv::rgba8(0,0,0,255);

	switch(score)
	{
		case 10: color=ldv::rgba8(130,160,0, 255); break;
		case 20: color=ldv::rgba8(160,130,0, 255); break;
		case 50: color=ldv::rgba8(0,160,130, 255); break;
		case 100: color=ldv::rgba8(0,130,160, 255); break;
		case 200: color=ldv::rgba8(0,30,160, 255); break;
	}

	b.set_type(draw_struct::types::polygon);
	b.set_polygon_fill(ldv::polygon_representation::type::fill);

	//TODO: Shall we set the center????
	b.set_polygon_points({{0, 10}, {10, 0}, {20, 10}, {10, 20}});
	b.set_color(color);

	b.go_to({(int)get_spatiable_x(), (int)get_spatiable_y()});
}

void bonus_score::get_picked_up(player_effects& pe)
{
	pe.add_score(score);
	set_delete(true);
}
