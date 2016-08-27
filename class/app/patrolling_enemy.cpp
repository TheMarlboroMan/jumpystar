#include "patrolling_enemy.h"

using namespace app_game;

patrolling_enemy::patrolling_enemy(float pleft, float pright, float pbottom)
	:enemy(fixed_w, fixed_h),
	limit_left(pleft), limit_right(pright)
{
	//TODO: Perhaps they could come in different speeds :).
	set_vector({-100.f, 0.f});

	float x=pright-( (pright-pleft) / 2);
	float y=pbottom-get_spatiable_h();
	set_position(x, y);
}

void patrolling_enemy::do_turn(float delta)
{
	move(delta);

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

void patrolling_enemy::transform_draw_struct(draw_struct& b)const
{
	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255,0,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void patrolling_enemy::collide_with_player()
{
	force_turnaround();
}

void patrolling_enemy::force_turnaround()
{
	set_vector(-get_vector().x, axis::x);
}

void patrolling_enemy::get_jumped_on()
{
	set_delete(true);
}
