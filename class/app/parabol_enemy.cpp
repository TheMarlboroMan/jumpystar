#include "parabol_enemy.h"

#include <class/number_generator.h>

#include "definitions.h"

using namespace app_game;

const float parabol_enemy::force=350.f;

parabol_enemy::parabol_enemy(t_point target_pos, float below_position)
	:enemy(fixed_w, fixed_h)
{
	tools::int_generator gen(-100, 100);
	float x_pos(target_pos.x+gen());

	set_position(x_pos, below_position);
	set_vector(ldt::for_points(get_spatiable_x(), get_spatiable_y(), target_pos.x, target_pos.y)*force);
}

void parabol_enemy::do_turn(float delta)
{
	enemy::do_turn(delta);

	if(get_state()!=states::regular) return;

	move(delta);
	do_gravity(delta, app::definitions::default_gravity);
}

void parabol_enemy::transform_draw_struct(draw_control& dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255,0,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void parabol_enemy::collide_with_player()
{
	//DO NOTHING...
}

void parabol_enemy::get_jumped_on()
{
	//Can't happen.
}

void parabol_enemy::get_trapped()
{
	//Can't happen.
}

void parabol_enemy::be_friendly(player_effects&)
{
	//Can't happen.
}
