#include "platform_dissapearing.h"

#include <vector>

#include <class/number_generator.h>

using namespace app_game;

platform_dissapearing::platform_dissapearing(float x, float y , int w)
	:platform(x, y, w, h), state{states::in}, 
	period{0.f}, max_period{0.f},
	ltf(tools::linear_timed_function<float>::from_time(255, 0, 2.f))
{
	//Randomize period and initial state.
	tools::int_generator g(2, 4);
	max_period=g();
	period=max_period;

	std::vector<states> vs{states::in, states::trans_out, states::out, states::trans_in};
	tools::int_generator vg(0, vs.size()-1);
	state=vs[vg()];
}

void platform_dissapearing::transform_draw_struct(draw_control& dc)const
{
	if(state==states::out) 
	{	
		dc.ignore();
		return;
	}

	dc.set(1);
	auto& b=dc[0];

	int alpha=255;
	switch(state)
	{
		case states::trans_out:	alpha=ltf(); break;
		case states::trans_in:	alpha=ltf(); break;
		case states::in:
		case states::out:
	 	break;
	}

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(130,0,160, alpha));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h() / 2});
}

void platform_dissapearing::do_turn(float delta)
{
	period-=delta;
	ltf.tic(delta);

	if(period <= 0.f)
	{
		period=max_period;
		switch(state)
		{
			case states::in:	
				state=states::trans_out; 
				ltf.reset_from_time(255, 0, period);
			break;
			case states::trans_out:	state=states::out; break;
			case states::out:	
				state=states::trans_in; 
				ltf.reset_from_time(0, 255, period);
			break;
			case states::trans_in:	state=states::in; break;
		}
	}
}
