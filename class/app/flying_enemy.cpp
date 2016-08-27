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
	period+=delta;

	set_vector(sin(period*period_multiplier)*y_vector_multiplier, axis::y);

	move(delta);

	//TODO: Unify this code in the base class.

	auto vx=get_vector_x();
	if(vx < 0.f)
	{
		if(get_spatiable_x() <= limit_left)
		{
			force_turnaround();
			set_box_x(limit_left);
		}
	}
	else if(vx > 0.f)
	{
		if(get_spatiable_ex() >= limit_right)
		{
			force_turnaround();
			set_box_x(limit_right-get_spatiable_w());
		}
	}
}

void flying_enemy::transform_draw_struct(draw_struct& b)const
{
	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255,0,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void flying_enemy::collide_with_player()
{
	//DO NOTHING.
}

//TODO: This goes in the base class.
void flying_enemy::force_turnaround()
{
	set_vector(-get_vector().x, axis::x);
}

void flying_enemy::get_jumped_on()
{
	set_delete(true);
}
