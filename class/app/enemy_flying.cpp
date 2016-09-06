#include "enemy_flying.h"

#include <class/number_generator.h>

#include "definitions.h"

using namespace app_game;

const float enemy_flying::period_multiplier=3.f;
const float enemy_flying::y_vector_multiplier=50.f;

enemy_flying::enemy_flying(const enemy_sideways_limit& esl, float vertical_pos)
	:enemy(fixed_w, fixed_h),
	limits(esl), period(0.f)
{
	tools::int_generator gen(limits.left, limits.right),
				vgen(-vertical_sway, vertical_sway);

	//TODO: Perhaps they could come in different speeds :).
	set_vector({gen() % 2 ? -100.f : 100.f, 0.f});
	set_position(gen(), vertical_pos+vgen());
}

void enemy_flying::do_turn(float delta)
{
	enemy::do_turn(delta);

	period+=delta;
	set_vector(sin(period*period_multiplier)*y_vector_multiplier, axis::y);

	move(delta);
	limit_sideways_patrol(limits);
}

void enemy_flying::transform_draw_struct(draw_control& dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255,0,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void enemy_flying::collide_with_player()
{
	//DO NOTHING.
}

void enemy_flying::get_jumped_on()
{
	//Can't happen, sorry.
}

void enemy_flying::get_trapped()
{
	//Can't happen.
}

void enemy_flying::be_friendly(player_effects&)
{
	//Can't happen.
}

void enemy_flying::get_hit_by_projectile()
{

}
