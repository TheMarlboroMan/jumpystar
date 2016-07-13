#include "player.h"

using namespace App_Game;

Player::Player()
	:Motion_actor(0.f, 0.f, 20, 32), 
	previous_position(get_box()), 
	state(states::air)
{

}

void Player::adjust_callback(float position, Motion_actor::adjust_pos apos)
{
	switch(apos)
	{
		case Motion_actor::adjust_pos::bottom:
			set_vector(0.f, axis::y);
			state=states::ground;
		break;
		case Motion_actor::adjust_pos::top:

		break;
		case Motion_actor::adjust_pos::left:
			set_vector(0.f, axis::x);
		break;
		case Motion_actor::adjust_pos::right:
			set_vector(0.f, axis::x);
		break;
		case Motion_actor::adjust_pos::none:

		break;
	}
}

void Player::transform_draw_struct(Draw_struct& b)const
{
	b.set_type(Draw_struct::types::box);
	b.set_color(DLibV::rgba8(0,0,160, 255));
	b.set_box_position({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void Player::turn(float delta)
{

	const float ACELERACION_HORIZONTAL_SEGUNDO=300.f;
	const float MAXIMA_VELOCIDAD_HORIZONTAL=250.f;

	//We did this before and worked nice in Winter.
	//Frenada o aceleración.
	if(player_input.x)
	{
		float v=get_vector_x();
		//Controlar el "contravolante...".
		float extra=(player_input.x > 0 && v > 0.0f) 
			|| (player_input.x < 0 && v < 0.0f) 
			? 1.0f : 2.5f; 

		v+=(delta * ACELERACION_HORIZONTAL_SEGUNDO * extra) * player_input.x;
		if(v > MAXIMA_VELOCIDAD_HORIZONTAL) v=MAXIMA_VELOCIDAD_HORIZONTAL;
		else if(v < -MAXIMA_VELOCIDAD_HORIZONTAL) v=-MAXIMA_VELOCIDAD_HORIZONTAL;

		set_vector(v, axis::x);

//TODO: Set direction
//		if(player_input.x < 0) direccion=App_Definiciones::direcciones::izquierda;
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
		if(player_input.jump)
		{
			set_vector(-300.f, axis::y);
			//TODO: Not really... Better check each frame what's the current state.
			state=states::air;
		}
	}

}
