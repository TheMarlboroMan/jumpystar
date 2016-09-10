#include "enemy_patrolling_shooter.h"

#include <class/number_generator.h>

#include "definitions.h"

using namespace app_game;

enemy_patrolling_shooter::enemy_patrolling_shooter(const enemy_sideways_limit& esl, float pbottom, std::vector<projectile_def>& pr)
	:enemy(fixed_w, fixed_h),
	projectiles(pr),
	limits(esl), 
	state(states::move), period(0.f),
	max_period(0.f)
{
	//TODO: Perhaps they could come in different speeds :).
	set_vector({-100.f, 0.f});

	//TODO: This code is repeated... Could directly go into the enemy base class.
	float x=limits.right-( (limits.right-limits.left) / 2);
	float y=pbottom-get_spatiable_h();
	set_position(x, y);

	auto g=tools::int_generator(2, 5);
	max_period=g();
	period=max_period;
}
void enemy_patrolling_shooter::do_turn(float delta)
{
	enemy::do_turn(delta);
	if(!is_active()) return;

	period-=delta;
	if(period <= 0.f)
	{
		period=max_period;
		switch(state)
		{
			case states::move: state=states::shoot; break;
			case states::shoot: state=states::pause; break;
			case states::pause: state=states::move; break;
		}
	}

	switch(state)
	{
		case states::move:
			move(delta);
			limit_sideways_patrol(limits);
		break;

		case states::shoot:
			if(!is_friendly())
			{
				//TODO: Different speeds on enemies would yield different projectile speeds.
				projectiles.push_back({{get_spatiable_cx(), get_spatiable_y()}, {2*get_vector_x(), 0.f}, projectile_def::types::straight, projectile_def::sides::enemy});
			}
			state=states::pause;
		break;

		case states::pause:

		break;
	}
}

void enemy_patrolling_shooter::transform_draw_struct(draw_control& dc) const
{
	dc.set(1);
	auto &b=dc[0];

	auto color=is_friendly() ? ldv::rgba8(255, 102, 255, 255) : ldv::rgba8(255,0,0, 255);
	if(is_stunned()) color=ldv::rgba8(0,255,0, 255);
	else if(is_trapped()) color=ldv::rgba8(0,0,255, 255);

	b.set_type(draw_struct::types::box);
	b.set_color(color);
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void enemy_patrolling_shooter::collide_with_player()
{
	if(state==states::move)
	{
		force_turnaround();
	}
}

void enemy_patrolling_shooter::get_jumped_on()
{
	stun(app::definitions::default_enemy_stun_time);
}

void enemy_patrolling_shooter::get_trapped()
{
	trap(app::definitions::default_enemy_trap_time);
}

void enemy_patrolling_shooter::be_friendly(player_effects& pe)
{
	befriend(pe, 50);
}

void enemy_patrolling_shooter::get_hit_by_projectile()
{
	stun(app::definitions::default_enemy_stun_time);
}

