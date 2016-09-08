#include "enemy_parabol_shooter.h"

#include <class/number_generator.h>

#include "definitions.h"

using namespace app_game;

enemy_parabol_shooter::enemy_parabol_shooter(std::vector<projectile_def>& vp, const app_interfaces::spatiable& pt, const app_interfaces::spatiable& pl)
	:enemy(fixed_w, fixed_h),
	projectiles(vp), player_target(pt), period(0.f)
{
	tools::int_generator gen(min_time, max_time);
	period=(float)gen() / 1000.f;

	float x=pl.get_spatiable_cx();
	float y=pl.get_spatiable_y()-get_spatiable_h();
	set_position(x, y);
}

void enemy_parabol_shooter::do_turn(float delta)
{
	enemy::do_turn(delta);

	if(!is_active()) return;

	if(!is_friendly())
	{
		period-=delta;
	
		if(period <= 0.f)
		{
			tools::int_generator gen(min_time, max_time);
			period=(float)gen() / 1000.f;

			t_vector direction{0.f, -100.f};
			direction.x=player_target.is_left_of(*this) ? -100.f : 100.f;
			projectiles.push_back({{get_spatiable_cx(), get_spatiable_y()}, direction, projectile_def::types::parabol, projectile_def::sides::enemy});
		}
	}
}

void enemy_parabol_shooter::transform_draw_struct(draw_control& dc)const
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

void enemy_parabol_shooter::collide_with_player()
{

}

void enemy_parabol_shooter::get_jumped_on()
{
	stun(app::definitions::default_enemy_stun_time);
}

void enemy_parabol_shooter::get_trapped()
{
	trap(app::definitions::default_enemy_trap_time);
}

void enemy_parabol_shooter::be_friendly(player_effects& pe)
{
	pe.add_score(50, {(int)get_spatiable_x(), (int)get_spatiable_y()});
	befriend();
}

void enemy_parabol_shooter::get_hit_by_projectile()
{
	stun(app::definitions::default_enemy_stun_time);
}
