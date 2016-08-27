#include "player_trap.h"

using namespace app_game;

player_trap::player_trap(t_point pos)
	:actor(pos.x, pos.y-h, w, h),
	game_object(),
	period(0.f), ltf(tools::linear_timed_function<float>::from_time(min_draw_size, max_draw_size, max_period))
{

}

void player_trap::reset(t_point pos)
{
	ltf.reset_from_time(min_draw_size, max_draw_size, max_period);
	set_position(pos.x, pos.y-h);
	period=0.f;
}

void player_trap::do_turn(float delta)
{
	ltf.tic(delta);

	period+=delta;
	if(period >= max_period)
	{
		set_delete(true);
	}
}

void player_trap::transform_draw_struct(app_game::draw_struct &b)const
{
	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(245, 120, 176, 255));

	int factor=ltf();

	unsigned int 	w=get_spatiable_w()-factor, 
			h=get_spatiable_h()-factor;

	b.set_location_box({(int)get_spatiable_x()+(factor/2), (int)get_spatiable_y()+(factor/2), w, h});


}
