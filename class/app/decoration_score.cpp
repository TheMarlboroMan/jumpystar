#include "decoration_score.h"

#include <cassert>

using namespace app_game;

decoration_score::t_map decoration_score::score_map=decoration_score::t_map();

decoration_score::decoration_score(t_point origin, int ps, const ldv::ttf_font& font)
	:decoration({origin.x, origin.y, 0, 0}), score(ps), period_ltf(tools::linear_timed_function<float>::from_time(255, 0, 1.f))
{
	if(!score_map.count(score))
	{
		add_to_map(score, font);
	}

	set_box_w(score_map.at(score)->get_base_view_position().w);
	set_box_h(score_map.at(score)->get_base_view_position().h);

	set_vector(-100.f, axis::y);
}

void decoration_score::do_turn(float delta)
{
	move(delta);
	period_ltf.tic(delta);
	if(!period_ltf()) set_delete(true);
}

void decoration_score::transform_draw_struct(draw_control& dc) const
{
	dc.ignore();
	dc.set(1);
	auto& b=dc[0];

	auto& sc_rep=*(score_map[score]);
	sc_rep.go_to((int)get_spatiable_x(), (int)get_spatiable_y());
	sc_rep.set_alpha(period_ltf());
	b.set_type(draw_struct::types::external);
	b.set_external(sc_rep);
}

void decoration_score::add_to_map(int sc, const ldv::ttf_font& font)
{
	assert(!score_map.count(sc));
	score_map[score]=t_uptr(new ldv::ttf_representation{font, ldv::rgba8(0, 0, 0, 255), std::to_string(sc)});
	score_map[score]->set_blend(ldv::representation::blends::alpha);
}
