#include "flying_enemy.h"

#include <class/number_generator.h>

#include "definitions.h"

using namespace app_game;

const float flying_enemy::period_multiplier=3.f;
const float flying_enemy::y_vector_multiplier=50.f;

flying_enemy::flying_enemy(float pleft, float pright, float vertical_pos)
	:enemy(fixed_w, fixed_h),
	limit_left(pleft), limit_right(pright), period(0.f)
{
	tools::int_generator gen(pleft, pright),
				vgen(-vertical_sway, vertical_sway);

	//TODO: Perhaps they could come in different speeds :).
	set_vector({gen() % 2 ? -100.f : 100.f, 0.f});
	set_position(gen(), vertical_pos+vgen());
}

void flying_enemy::do_turn(float delta)
{
	enemy::do_turn(delta);

	if(get_state()!=states::regular) return;

	period+=delta;
	set_vector(sin(period*period_multiplier)*y_vector_multiplier, axis::y);

	move(delta);
	limit_sideways_patrol(limit_left, limit_right);
}

void flying_enemy::transform_draw_struct(draw_control& dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255,0,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void flying_enemy::collide_with_player()
{
	//DO NOTHING.
}

void flying_enemy::get_jumped_on()
{
	//Can't happen, sorry.
}

void flying_enemy::get_trapped()
{
	//Can't happen.
}

void flying_enemy::be_friendly(player_effects&)
{
	//Can't happen.
}
