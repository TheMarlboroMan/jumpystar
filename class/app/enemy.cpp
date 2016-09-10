#include "enemy.h"

using namespace app_game;

enemy::enemy(int w, int h)
	:motion_actor(0.f, 0.f, w, h),
	game_object(),
	state(states::harmful),
	state_period(0.f),
	score_divisor(1)
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
			if(state_period <= 0.f) 
			{
				state=states::harmful;
				score_divisor=1;
			}
		break;
	}
}

void enemy::force_turnaround()
{
	set_vector(-get_vector().x, axis::x);
}

void enemy::limit_sideways_patrol(const enemy_sideways_limit& esl)
{
	auto vx=get_vector_x();
	if(vx < 0.f)
	{
		if(get_spatiable_x() <= esl.left)
		{
			force_turnaround();
			set_box_x(esl.left);
		}
	}
	else if(vx > 0.f)
	{
		if(get_spatiable_ex() >= esl.right)
		{
			force_turnaround();
			set_box_x(esl.right-get_spatiable_w());
		}
	}
}

/* Stunned, cannot move, the score divisor diminishes until waking from the 
stun.*/

void enemy::stun(float sp)
{
	state_period=sp; 
	state=states::stunned;
	score_divisor=2;
}

/* Trapped, cannot move, vulnerable to player touch, which would make it 
friendly. */

void enemy::trap(float sp)
{
	state_period=sp; 
	state=states::trapped;
}

void enemy::befriend(player_effects& pe, int score)
{
	pe.add_score(score / score_divisor, {(int)get_spatiable_x(), (int)get_spatiable_y()});
	state=states::friendly;
}

