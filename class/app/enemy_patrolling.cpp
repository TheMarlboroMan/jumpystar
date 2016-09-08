#include "enemy_patrolling.h"

#include "definitions.h"

using namespace app_game;

enemy_patrolling::enemy_patrolling(const enemy_sideways_limit& esl, float pbottom)
	:enemy(fixed_w, fixed_h),
	limits(esl)
{
	//TODO: Perhaps they could come in different speeds :).
	set_vector({-100.f, 0.f});

	//TODO: This code is repeated... Could directly go into the enemy base class.

	float x=limits.right-( (limits.right-limits.left) / 2);
	float y=pbottom-get_spatiable_h();
	set_position(x, y);
}

void enemy_patrolling::do_turn(float delta)
{
	enemy::do_turn(delta);

	if(!is_active()) return;

	move(delta);
	limit_sideways_patrol(limits);
}

void enemy_patrolling::transform_draw_struct(draw_control& dc)const
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

void enemy_patrolling::collide_with_player()
{
	force_turnaround();
}

void enemy_patrolling::get_jumped_on()
{
	stun(app::definitions::default_enemy_stun_time);
}

void enemy_patrolling::get_trapped()
{
	trap(app::definitions::default_enemy_trap_time);
}

void enemy_patrolling::be_friendly(player_effects& pe)
{
	pe.add_score(50, {(int)get_spatiable_x(), (int)get_spatiable_y()});
	befriend();
}

void enemy_patrolling::get_hit_by_projectile()
{
	stun(app::definitions::default_enemy_stun_time);
}
