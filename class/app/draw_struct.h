#ifndef DRAW_STRUCT_H
#define DRAW_STRUCT_H

#include <def_video.h>

namespace App_Game
{

struct Draw_struct
{
	///////////////////////////////////
	// Definiciones...

	enum class types{
		bitmap,
		group,
		points,
		box,
		external
	};

	///////////////////////////////////
	// Interfaz pública

	public:

								Draw_struct();
								~Draw_struct();

	int							get_cycle() const {return cycle;}
	Draw_struct&						set_cycle(int v) {cycle=v; return *this;}
	bool 							is_visible() const {return visible;}
	void							go_to(DLibV::Punto p) {rep->ir_a(p);}
	DLibV::Rect 						get_draw_position() const {return rep->acc_posicion_vista();}
	void 							set_type(types t);
	void 							hide() {visible=false;}
	void 							set_alpha(unsigned int v);
	void 							set_blend(DLibV::Representacion::blends);
	void							set_color(DLibV::ColorRGBA);
	void 							set_rotation_center(float x, float y);
	void 							set_rotation(float v) {rep->transformar_rotar(v);}
	void							set_external(DLibV::Representacion * rep) {set_external(*rep);}
	void							set_external(DLibV::Representacion&);

	///////////////////
	// Metodos para manipular el grupo de representaciones...

	void 							insert_in_group(DLibV::Representacion * r);

	////////////////////
	//Métodos para manipular la representación de puntos.

	void 							insert_point(DLibV::Punto p) {rep_points.insertar(p);}

	////////////////////
	//Métodos para manipular la representación de caja.

	void 							set_box_position(DLibV::Rect p) {rep_box.establecer_posicion(p);}

	///////////////////
	// Métodos para manipular la representación de bitmap...

	void 							set_clip(DLibV::Rect r) {rep_bmp.establecer_recorte(r);}
	void 							set_position(DLibV::Rect r) {rep_bmp.establecer_posicion(r);}
	void 							set_bitmap_horizontal_invert(bool v) {rep_bmp.transformar_invertir_horizontal(v);}
	void 							set_bitmap_vertical_invert(bool v) {rep_bmp.transformar_invertir_vertical(v);}
	void 							set_resource(unsigned int i);
	void 							set_texture_manual(DLibV::Textura * r) {rep_bmp.establecer_textura(r);}

	///////////////////////////////////
	// Propiedades

	DLibV::Representacion_bitmap 			rep_bmp;
	DLibV::Representacion_agrupada 			rep_group;
	DLibV::Representacion_primitiva_puntos 		rep_points;
	DLibV::Representacion_primitiva_caja 		rep_box;
	DLibV::Representacion * 			rep;
	types 						type;
	bool 						visible;
	int						cycle;
};

}

#endif
