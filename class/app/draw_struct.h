#ifndef DRAW_STRUCT_H
#define DRAW_STRUCT_H

#include <def_video.h>

namespace app_game
{

struct draw_struct
{
	///////////////////////////////////
	// Definiciones...

	enum class types{
		bitmap,
		group,
		points,
		box,
		line,
		polygon,
		external
	};

	///////////////////////////////////
	// Interfaz pública

	public:

								draw_struct(ldv::resource_manager&);
								~draw_struct();

	int							get_cycle() const {return cycle;}
	draw_struct&						set_cycle(int v) {cycle=v; return *this;}
	bool 							is_visible() const {return visible;}
	void							go_to(ldv::point p) {rep->go_to(p);}
	ldv::rect 						get_view_position() const {return rep->get_view_position();}
	void 							set_type(types t);
	void 							hide() {visible=false;}
	void 							set_alpha(unsigned int v);
	void 							set_blend(ldv::representation::blends);
	void							set_color(ldv::rgba_color);
	void 							set_rotation_center(float x, float y);
	void 							set_rotation(float v) {rep->set_rotation(v);}
	void							set_external(ldv::representation * prep) {set_external(*prep);}
	void							set_external(ldv::representation&);

	///////////////////
	// Metodos para manipular el grupo de representaciones...

	void 							insert_in_group(ldv::representation * r);

	////////////////////
	//Métodos para manipular la representación de puntos.

	void 							insert_point(ldv::point p) {rep_points.insert(p);}

	////////////////////
	//Métodos para manipular la representación de líneas.

	void 							set_line(ldv::point p1, ldv::point p2) {rep_line.set_points(p1.x, p1.y, p2.x, p2.y);}

	////////////////////
	//Methods for polygon manipulation...
	void							set_polygon_fill(ldv::polygon_representation::type f) {rep_polygon.set_filltype(f);}
	void							set_polygon_points(const std::vector<ldv::point>& p) {rep_polygon.set_points(p);}


	////////////////////
	//Métodos para manipular la representación de caja.

	void 							set_location_box(ldv::rect p) {rep_box.set_location(p);}

	///////////////////
	// Métodos para manipular la representación de bitmap...

	void 							set_clip(ldv::rect r) {rep_bmp.set_clip(r);}
	void 							set_location(ldv::rect r) {rep_bmp.set_location(r);}
	void 							set_set_invert_horizontal_bitmap(bool v) {rep_bmp.set_invert_horizontal(v);}
	void 							set_set_invert_vertical_bitmap(bool v) {rep_bmp.set_invert_vertical(v);}
	void 							set_resource(unsigned int i);
	void 							set_texture_manual(ldv::texture& r) {rep_bmp.set_texture(r);}

	///////////////////////////////////
	// Propiedades

	ldv::resource_manager&				v_manager;

	ldv::bitmap_representation 			rep_bmp;
	ldv::group_representation 			rep_group;
	ldv::point_representation 			rep_points;
	ldv::box_representation 			rep_box;
	ldv::line_representation 			rep_line;
	ldv::polygon_representation 			rep_polygon;
	ldv::representation * 				rep;
	types 						type;
	bool 						visible;
	int						cycle;
};

}

#endif
