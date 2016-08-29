#include "enemy.h"

using namespace app_game;

enemy::enemy(int w, int h)
	:motion_actor(0.f, 0.f, w, h),
	game_object(),
	state(states::harmful),
	state_period(0.f)
{

}

void enemy::do_turn(float delta)
{
	switch(state)
	{
		case states::harmful:
		case states::friendly:  
		break;
		case states::trapped:
		case states::stunned:  
			state_period-=delta;
			if(state_period <= 0.f) state=states::harmful;
		break;
	}
}

void enemy::force_turnaround()
{
	set_vector(-get_vector().x, axis::x);
}

void enemy::limit_sideways_patrol(float limit_left, float limit_right)
{
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
