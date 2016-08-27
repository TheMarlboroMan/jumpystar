#include "patrolling_enemy.h"

#include "definitions.h"

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
	enemy::do_turn(delta);

	if(get_state()!=states::regular) return;

	move(delta);
	limit_sideways_patrol(limit_left, limit_right);
}

void patrolling_enemy::transform_draw_struct(draw_control& dc)const
{
	dc.set(1);
	auto &b=dc[0];

	auto color=ldv::rgba8(255,0,0, 255);
	if(is_stunned()) color=ldv::rgba8(0,255,0, 255);
	else if(is_trapped()) color=ldv::rgba8(0,0,255, 255);

	b.set_type(draw_struct::types::box);
	b.set_color(color);
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void patrolling_enemy::collide_with_player()
{
	force_turnaround();
}

void patrolling_enemy::get_jumped_on()
{
	stun(app::definitions::default_enemy_stun_time);
}

void patrolling_enemy::get_trapped()
{
	trap(app::definitions::default_enemy_trap_time);
}

void patrolling_enemy::be_friendly(player_effects& pe)
{
	pe.add_score(50);
	set_delete(true);
}
