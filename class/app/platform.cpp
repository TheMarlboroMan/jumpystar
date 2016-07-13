#include "platform.h"

using namespace App_Game;

Platform::Platform(float x, float y , int w):
	Actor(x, y, w, 8),
	Game_object()
{

}

void Platform::transform_draw_struct(App_Game::Draw_struct &b)const
{
	b.set_type(Draw_struct::types::box);
	b.set_color(DLibV::rgba8(130,0,160, 255));
	b.set_box_position({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}
