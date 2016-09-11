#include "enemy_flying_chaser.h"

#include "definitions.h"

using namespace app_game;

enemy_flying_chaser::enemy_flying_chaser(const app_interfaces::spatiable& pt, const app_interfaces::spatiable& pl, const std::vector<std::unique_ptr<platform>>& plt)
	:enemy(fixed_w, fixed_h), state(states::chasing),
	player_target(pt), platforms(plt), period(0.f)
{
	set_position(pl.get_spatiable_x(), pl.get_spatiable_y());
	correct_vector();
}

void enemy_flying_chaser::do_turn(float delta)
{
	enemy::do_turn(delta);

	//This is gonna be different...
	if(!is_active())
	{
		fall(delta);
	}
	else
	{
		//A friendly enemy will always drift away.
		if(!is_friendly())
		{
			move(delta);
			switch(state)
			{
				case states::chasing: correct_vector(); break;
				case states::drifting: 
					period-=delta;
					if(period <= 0.f) state=states::chasing;
				break;
			}
		}
	}
}

void enemy_flying_chaser::transform_draw_struct(draw_control& dc)const
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

void enemy_flying_chaser::collide_with_player()
{
	period=2.5f;
	state=states::drifting;
}

void enemy_flying_chaser::get_jumped_on()
{
	set_vector(0.f, axis::y);
	stun(app::definitions::default_enemy_stun_time);
}

void enemy_flying_chaser::get_trapped()
{
	set_vector(0.f, axis::y);
	trap(app::definitions::default_enemy_trap_time);
}

void enemy_flying_chaser::be_friendly(player_effects& pe)
{
	set_vector({-1.f, 1.f}); 
	befriend(pe, 100);
	state=states::drifting;
}

void enemy_flying_chaser::get_hit_by_projectile()
{
	stun(app::definitions::default_enemy_stun_time);
}

void enemy_flying_chaser::adjust_callback(float, motion_actor::adjust_pos apos)
{
	switch(apos)
	{
		case motion_actor::adjust_pos::bottom:
			set_vector({0.f, 0.f});
		break;
		case motion_actor::adjust_pos::top:
		case motion_actor::adjust_pos::left:
		case motion_actor::adjust_pos::right:
		case motion_actor::adjust_pos::none:
		break;
	}
}

void enemy_flying_chaser::correct_vector()
{
	//TODO: Correct current vector, sum them and stuff...

	//TODO That's not it, of course...
//	auto cv=get_vector();
	auto v=ldt::for_points(get_spatiable_x(), get_spatiable_y(), player_target.get_spatiable_x(), player_target.get_spatiable_y());

//	if(cv.x && cv.y)
//	{
//		set_vector((cv*v)*speed);
//	}
//	else
//	{
		set_vector(v * speed);
//	}

}

void enemy_flying_chaser::fall(float delta)
{
	move(delta);
	do_gravity(delta);

	for(auto& i : platforms)
	{
		auto& p=*i;

		if(is_colliding_with(p) && p.is_under(get_previous_position()) && p.is_collidable())
		{
			adjust(p, app_game::motion_actor::adjust_pos::bottom);
			break;
		}
	}
}
