#include "player_trap.h"

#include "definitions.h"

using namespace app_game;

int player_trap::max_width=3*app::definitions::unit;

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

void player_trap::transform_draw_struct(draw_control &dc)const
{
	dc.set(3);

	//Main part.
	auto& main=dc[0];
	main.set_type(draw_struct::types::box);
	main.set_color(ldv::rgba8(245, 120, 176, 255));

	int factor=ltf();

	unsigned int 	w=get_spatiable_w()-factor, 
			h=get_spatiable_h()-factor;

	main.set_location_box({(int)get_spatiable_x()+(factor/2), (int)get_spatiable_y()+(factor/2), w, h});

	auto set_marker=[](draw_struct& ds, int x, int y, int ey)
	{
		ds.set_type(draw_struct::types::line);
		ds.set_line({x, y}, {x, ey});
		ds.set_color(ldv::rgba8(245, 120, 176, 128));
	};

	//Left and right markers...
	int 	left=get_spatiable_cx() - (max_width / 2),
		right=get_spatiable_cx() + (max_width / 2);

	set_marker(dc[1], left, get_spatiable_y(), get_spatiable_ey()),
	set_marker(dc[2], right, get_spatiable_y(), get_spatiable_ey());
}
