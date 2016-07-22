#include "draw_struct.h"
#include <templates/compatibility_patches.h>

using namespace app_game;

draw_struct::draw_struct(ldv::resource_manager& v_m)
	:v_manager(v_m), rep_bmp(), 
	rep_group({0,0}, true),
	rep_points(ldv::rgba8(0,0,0,255)),
	rep_box(ldv::box_representation::type::fill, {0,0,0,0}, ldv::rgba8(0,0,0,255)),
	rep(&rep_bmp),
	type(types::bitmap),
	visible(true)
{

}

draw_struct::~draw_struct()
{
	rep_group.clear();
}

void draw_struct::set_color(ldv::rgba_color c)
{
	rep_points.set_rgba(c);
	rep_box.set_rgba(c);
}

void draw_struct::set_type(types t)
{
	type=t;

	switch(type)
	{
		case types::bitmap: 
			rep=&rep_bmp;
		break;

		case types::group:
			rep_group.clear();
			rep=&rep_group;
		break;

		case types::points:
			rep_points.clear();
			rep=&rep_points;
		break;

		case types::box:
			rep=&rep_box;
		break;

		case types::external:
			rep=nullptr;
		break;
	}

	rep->reset_transform();
	set_alpha(255);
	set_blend(ldv::representation::blends::alpha);
	visible=true;
}

void draw_struct::set_alpha(unsigned int v) 
{
	if(v > 255) v=255;
	rep->set_alpha(v);
}

void draw_struct::set_blend(ldv::representation::blends v) 
{
	rep->set_blend(v);
}

///////////////////
// Metodos para manipular el grupo de representaciones...

void draw_struct::insert_in_group(ldv::representation * r) 
{
	switch(type)
	{
		case types::bitmap:
		case types::points:
		case types::box:
		case types::external: break;
		case types::group: rep_group.insert(r); break;
	}
}

void draw_struct::set_resource(unsigned int i) //Por defecto establece el recorte al tamaño del recurso.
{
	rep_bmp.set_texture(v_manager.get_texture(i));
	rep_bmp.clip_to_texture();
}

void draw_struct::set_external(ldv::representation& p_rep)
{
	type=types::external;
	rep=&p_rep;
}
