#include "player.h"

using namespace app_game;

player::player()
	:motion_actor(0.f, 0.f, 20, 32), 
	previous_position(get_box()), 
	state(states::air)
{

}

void player::adjust_callback(float position, motion_actor::adjust_pos apos)
{
	switch(apos)
	{
		//This is falling against a platform: spot y and half x.
		case motion_actor::adjust_pos::bottom:
			set_vector(0.f, axis::y);
			
			if(state!=states::ground)
			{
				set_vector(get_vector_x()/2.f, axis::x);
				state=states::ground;
			}
		break;
		case motion_actor::adjust_pos::top:

		break;
		case motion_actor::adjust_pos::left:
			set_vector(0.f, axis::x);
		break;
		case motion_actor::adjust_pos::right:
			set_vector(0.f, axis::x);
		break;
		case motion_actor::adjust_pos::none:

		break;
	}
}

void player::transform_draw_struct(draw_struct& b)const
{
	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(0,0,160, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void player::turn(float delta)
{

	const float ACELERACION_HORIZONTAL_SEGUNDO=200.f; //300.f;
	const float MAXIMA_VELOCIDAD_HORIZONTAL=150.f; //250.f;

	//We did this before and worked nice in Winter.
	//Frenada o aceleración.
	if(p_input.x)
	{
		float v=get_vector_x();
		//Controlar el "contravolante...".
		float extra=(p_input.x > 0 && v > 0.0f) 
			|| (p_input.x < 0 && v < 0.0f) 
			? 1.0f : 2.5f; 

		v+=(delta * ACELERACION_HORIZONTAL_SEGUNDO * extra) * p_input.x;
		if(v > MAXIMA_VELOCIDAD_HORIZONTAL) v=MAXIMA_VELOCIDAD_HORIZONTAL;
		else if(v < -MAXIMA_VELOCIDAD_HORIZONTAL) v=-MAXIMA_VELOCIDAD_HORIZONTAL;

		set_vector(v, axis::x);

//TODO: Set direction
//		if(p_input.x < 0) direccion=App_Definiciones::direcciones::izquierda;
//		else direccion=App_Definiciones::direcciones::derecha;
	}
	else
	{
		float v=abs(get_vector_x());
		v-=(delta * ACELERACION_HORIZONTAL_SEGUNDO);
		if(v < 0.0) v=0;
			//Aproximarse al cero.
		float vr=get_vector_x();
		if(vr > 0.0) set_vector(v, axis::x);
		else if(vr < 0.0) set_vector(-v, axis::x);
	}

	if(state==states::ground)
	{
		if(p_input.jump)
		{
			set_vector(-340.f, axis::y);
			//TODO: Not really... Better check each frame what's the current state.
			state=states::air;
		}
	}

}
