#include "draw_struct.h"
#include <templates/parches_compat.h>

using namespace App_Game;

Draw_struct::Draw_struct()
	:rep_bmp(), 
	rep_group({0,0}, true),
	rep_points(DLibV::rgba8(0,0,0,255)),
	rep_box(DLibV::Representacion_primitiva_caja::tipo::relleno, {0,0,0,0}, DLibV::rgba8(0,0,0,255)),
	rep(&rep_bmp),
	type(types::bitmap),
	visible(true)
{

}

Draw_struct::~Draw_struct()
{
	rep_group.vaciar_grupo();
}

void Draw_struct::set_color(DLibV::ColorRGBA c)
{
	rep_points.mut_rgba(c);
	rep_box.mut_rgba(c);
}

void Draw_struct::set_type(types t)
{
	type=t;

	switch(type)
	{
		case types::bitmap: 
			rep_bmp.reiniciar_transformacion();
			rep=&rep_bmp;
		break;

		case types::group:
			rep_group.vaciar_grupo();
			rep=&rep_group;
		break;

		case types::points:
			rep_points.limpiar_puntos();
			rep=&rep_points;
		break;

		case types::box:
			rep=&rep_box;
		break;

		case types::external:
			rep=nullptr;
		break;
	}

	set_alpha(255);
	set_blend(DLibV::Representacion::blends::alpha);
	visible=true;
}

void Draw_struct::set_alpha(unsigned int v) 
{
	if(v > 255) v=255;
	rep->establecer_alpha(v);
}

void Draw_struct::set_blend(DLibV::Representacion::blends v) 
{
	rep->establecer_modo_blend(v);
}

///////////////////
// Metodos para manipular el grupo de representaciones...

void Draw_struct::insert_in_group(DLibV::Representacion * r) 
{
	switch(type)
	{
		case types::bitmap:
		case types::points:
		case types::box:
		case types::external: break;
		case types::group: rep_group.insertar_representacion(r); break;
	}
}

void Draw_struct::set_resource(unsigned int i) //Por defecto establece el recorte al tamaño del recurso.
{
	if(!DLibV::Gestor_texturas::comprobar(i))
	{
		throw std::runtime_error("Unknown resource "+compat::to_string(i)+" requested.");
	}
	else 
	{
		rep_bmp.establecer_textura(DLibV::Gestor_texturas::obtener(i));
		rep_bmp.recorte_a_medidas_textura();
	}
}

void Draw_struct::set_external(DLibV::Representacion& p_rep)
{
	type=types::external;
	rep=&p_rep;
}
