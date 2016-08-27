#include "parabol_shooter_enemy.h"

#include <class/number_generator.h>

#include "definitions.h"

using namespace app_game;

parabol_shooter_enemy::parabol_shooter_enemy(std::vector<projectile_def>& vp, const app_interfaces::spatiable& pt, const app_interfaces::spatiable& pl)
	:enemy(fixed_w, fixed_h),
	projectiles(vp), player_target(pt), period(0.f)
{
	tools::int_generator gen(min_time, max_time);
	period=(float)gen() / 1000.f;

	float x=pl.get_spatiable_cx();
	float y=pl.get_spatiable_y()-get_spatiable_h();
	set_position(x, y);
}

void parabol_shooter_enemy::do_turn(float delta)
{
	enemy::do_turn(delta);

	if(get_state()!=states::regular) return;

	period-=delta;
	
	if(period <= 0.f)
	{

		tools::int_generator gen(min_time, max_time);
		period=(float)gen() / 1000.f;

		//TODO: If possible, check also that the enemy is below the camera drawing distance.
		if(abs(get_spatiable_y() - player_target.get_spatiable_y()) <= max_shooting_distance)
		{
			t_vector direction{0.f, -100.f};
			direction.x=player_target.is_left_of(*this) ? -100.f : 100.f;
			projectiles.push_back({{get_spatiable_cx(), get_spatiable_y()}, direction, projectile_def::types::curve});
		}
	}
}

void parabol_shooter_enemy::transform_draw_struct(draw_struct& b)const
{
	auto color=ldv::rgba8(255,0,0, 255);
	if(is_stunned()) color=ldv::rgba8(0,255,0, 255);
	else if(is_trapped()) color=ldv::rgba8(0,0,255, 255);

	b.set_type(draw_struct::types::box);
	b.set_color(color);
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void parabol_shooter_enemy::collide_with_player()
{

}

void parabol_shooter_enemy::get_jumped_on()
{
	stun(app::definitions::default_enemy_stun_time);
}

void parabol_shooter_enemy::get_trapped()
{
	trap(app::definitions::default_enemy_trap_time);
}

