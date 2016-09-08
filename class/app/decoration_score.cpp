#include "decoration_score.h"

#include <cassert>

using namespace app_game;

std::map<int, ldv::ttf_representation>	decoration_score::score_map=std::map<int, ldv::ttf_representation>();

decoration_score::decoration_score(t_point origin, int ps, const ldv::ttf_font& font)
	:decoration({origin.x, origin.y, 0, 0}), score(ps)
{
	if(!score_map.count(score))
	{
		add_to_map(score, font);
	}

	set_box_w(score_map.at(score).get_base_view_position().w);
	set_box_h(score_map.at(score).get_base_view_position().h);

	set_vector(-100.f, axis::y);
}

void decoration_score::do_turn(float delta)
{
	move(delta);
}

void decoration_score::transform_draw_struct(draw_control& dc) const
{
	dc.set(1);
	auto& b=dc[0];

	auto& sc_rep=score_map.at(score);
	sc_rep.go_to((int)get_spatiable_x(), (int)get_spatiable_y());
	b.set_type(draw_struct::types::external);
	b.set_external(sc_rep);
}

void decoration_score::add_to_map(int sc, const ldv::ttf_font& font)
{
	assert(!score_map.count(sc));

	std::cout<<"This should spawn a score... But won't work :S"<<std::endl;

	//TODO: This is royally fucked.
	//TODO: Move operator for the representation????... This memory is lost otherwise!!!!... Not that it matters since it's static :P.
	//ldv::ttf_representation * cosa=new ldv::ttf_representation{font, ldv::rgba8(0, 0, 0, 255), std::to_string(sc)};
	//cosa->set_blend(ldv::representation::blends::alpha);
	//score_map.insert({score, *cosa});
}
