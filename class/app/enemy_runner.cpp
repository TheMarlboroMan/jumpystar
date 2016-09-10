#include "enemy_runner.h"

#include <class/number_generator.h>

#include "definitions.h"

using namespace app_game;

enemy_runner::enemy_runner(const enemy_sideways_limit& esl, float pbottom)
	:enemy(fixed_w, fixed_h), state(states::rest), direction(directions::right),
	limits(esl), period(0.f), max_period(0.f)
{
	//Set period length.
	tools::int_generator periodg(2, 4);
	max_period=periodg();
	period=max_period;

	//Choose direction and subsequent position.	
	direction=periodg() % 2 ? directions::left : directions::right;

	float x=limits.left; //Assumes direction as right.
	if(direction==directions::left) x=limits.right - get_spatiable_w();
	set_position(x, pbottom-get_spatiable_h());

	set_vector({0.f, 0.f});
}

void enemy_runner::do_turn(float delta)
{
	enemy::do_turn(delta);

	if(!is_active()) return;

	switch(state)
	{
		case states::rest:
			period-=delta;
			if(period <= 0.f)
			{
				period=0.f;
				state=states::dash;
				set_vector(0.f, axis::x);				
			}
		break;
		case states::dash:
		{
			auto v=get_vector_x();
			manage_speed(delta);

			move(delta);

			limit_sideways_patrol(limits);

			//If sign changed, then direction changed.
			if(v * get_vector_x() < 0.f)
			{
				period=max_period;
				state=states::rest;
				direction=direction==directions::left ? directions::right : directions::left;	
			}
		}
		break;
	}
}

void enemy_runner::transform_draw_struct(draw_control& dc)const
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

void enemy_runner::collide_with_player()
{
	//Do nothing.
}

/** When stunned, this stops on its tracks. If it was running will then 
accelerate again.
*/

void enemy_runner::get_jumped_on()
{
	stun(app::definitions::default_enemy_stun_time);
	if(state==states::dash) set_vector(0.f, axis::x);
}

void enemy_runner::get_trapped()
{
	trap(app::definitions::default_enemy_trap_time);
}

void enemy_runner::be_friendly(player_effects& pe)
{
	befriend(pe, 80);
}

void enemy_runner::get_hit_by_projectile()
{
	stun(app::definitions::default_enemy_stun_time);
	if(state==states::dash) set_vector(0.f, axis::x);
}

void enemy_runner::manage_speed(float delta)
{
	auto v=fabs(get_vector_x());
	v+=delta * acceleration;
	if(v >= max_speed) v=max_speed;
	set_vector(direction==directions::left ? -v : v, axis::x);
}
