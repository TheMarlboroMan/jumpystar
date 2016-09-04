#include "platform_crumbling.h"

#include <vector>

#include <class/number_generator.h>

using namespace app_game;

platform_crumbling::platform_crumbling(float x, float y , int w)
	:platform(x, y, w, h), 
	period{1.f}, crumbling(false),
	ltf(tools::linear_timed_function<float>::from_time(255, 0, period))
{

}

void platform_crumbling::transform_draw_struct(draw_control& dc)const
{
	dc.set(1);
	auto& b=dc[0];
	int alpha=255, shake=0;

	if(crumbling)
	{
		alpha=ltf();
		tools::int_generator gen(-2, 2);
		shake=gen();
	}

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255,32,32, alpha));
	b.set_location_box({(int)get_spatiable_x()+shake, (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h() / 2});
}

void platform_crumbling::do_turn(float delta)
{
	if(crumbling)
	{
		period-=delta;
		ltf.tic(delta);
		if(period <= 0.f)
		{
			period=0.f;
			set_delete(true);
		}
	}
}

void platform_crumbling::get_jumped_on()
{
	crumbling=true;
}
