#ifndef SPATIABLE_H
#define SPATIABLE_H

/*Interfaz que exige los métodos para definir una caja en 2d y a partir de ahí,
monta el resto. Que defina una caja 2d no significa que realmente haya una
caja por ahí, por ejemplo, las celdas de un nivel no tienen cajas reales, aunque
devuelven una caja calculada. 

Está implícito también que implementa muchas funciones para temas de colisión,
como por ejemplo, saber si es sólido por todos lados o no, obtener información
de colisión y demás. Intentamos separarlo en dos interfaces pero teníamos tanto
el problema del diamante como que son casi la misma cosa. Se quedan, por tanto,
como están.
*/

#include <libDan2.h>

namespace App_Interfaces
{

class Spatiable
{
	///////////////////////////////
	// Definiciones

	public:

	typedef DLibH::Caja<float, unsigned int> t_box;
	enum t{T_X=1, T_Y=2};

	///////////////////////////////
	// Propiedades e internas...

	protected:

	//////////////////////////////
	// Interfaz pública.

	public:

	virtual ~Spatiable() {}

	float 				get_spatiable_x() const {return get_box().origen.x;}
	float 				get_spatiable_y() const {return get_box().origen.y;}
	float 				get_spatiable_cx() const {return get_spatiable_x() + (get_spatiable_w() / 2);}
	float 				get_spatiable_cy() const {return get_spatiable_y() + (get_spatiable_h() / 2);}
	float 				get_spatiable_ex() const {return get_spatiable_x() + get_spatiable_w();}
	float 				get_spatiable_ey() const {return get_spatiable_y() + get_spatiable_h();}
	unsigned int 			get_spatiable_w() const {return get_box().w;}
	unsigned int 			get_spatiable_h() const {return get_box().h;}
	DLibH::Punto_2d<float> 		get_spatiable_posicion() const {return get_box().origen;}

	DLibH::Vector_2d_pantalla<float>	screen_vector_for(const Spatiable& e) const {return screen_vector_for(*this, e);}
	DLibH::Vector_2d_pantalla<float>	screen_vector_for(const Spatiable& a, const Spatiable& b) const;
	DLibH::Vector_2d_cartesiano<float>	cartesian_vector_for(const Spatiable& e) const {return cartesian_vector_for(*this, e);}
	DLibH::Vector_2d_cartesiano<float>	cartesian_vector_for(const Spatiable& a, const Spatiable& b) const;

	float		 		angle_for(const Spatiable& e) const {return angle_for(*this, e);}
	float		 		angle_for(const Spatiable& a, const Spatiable& b) const;
	float		 		cartesian_angle_for(const Spatiable& e) const {return cartesian_angle_for(*this, e);}
	float		 		cartesian_angle_for(const Spatiable& a, const Spatiable& b) const;

	bool 				is_colliding_with(const Spatiable&, bool=false) const;

	//Estos son siempre según la geometría en la que 0.0 es el punto superior
	//a la izquierda.

	bool 				is_over(const Spatiable& otra) const {return is_over(get_box(), otra.get_box());}
	bool 				is_under(const Spatiable& otra) const {return is_under(get_box(), otra.get_box());}
	bool 				is_left_of(const Spatiable& otra) const {return is_left_of(get_box(), otra.get_box());}
	bool 				is_right_of(const Spatiable& otra) const {return is_right_of(get_box(), otra.get_box());}
	bool				is_inside_of(const Spatiable& otra) const {return is_inside_of(get_box(), otra.get_box());}

	bool 				is_over(const t_box& otra) const {return is_over(get_box(), otra);}
	bool 				is_under(const t_box& otra) const {return is_under(get_box(), otra);}
	bool 				is_left_of(const t_box& otra) const {return is_left_of(get_box(), otra);}
	bool 				is_right_of(const t_box& otra) const {return is_right_of(get_box(), otra);}
	bool				is_inside_of(const t_box& otra) const {return is_inside_of(get_box(), otra);}

	static bool 			is_over(const t_box& una, const t_box& otra) {return una.origen.y+una.h <= otra.origen.y;}
	static bool			is_under(const t_box& una, const t_box& otra) {return una.origen.y >= otra.origen.y+otra.h;}
	static bool			is_left_of(const t_box& una, const t_box& otra) {return una.origen.x+una.w <= otra.origen.x;}
	static bool			is_right_of(const t_box& una, const t_box& otra) {return una.origen.x >= otra.origen.x+otra.w;}
	static bool			is_inside_of(const t_box& una, const t_box& otra) {return una.es_contenida_en(otra);}

	void 				set_position(const Spatiable& e);
	void 				set_position(float x, float y);
	t_box 				get_box_displaced(float x, float y) const;

	////////////////////////////
	// A implementar

	virtual t_box 			get_box() const=0;	//Debe devolver una COPIA de la caja.

	virtual void 			set_box_x(float)=0;	//Estos no tienen porqué hacer nada si no hay una caja real.
	virtual void 			set_box_y(float)=0;
	virtual void 			displace_box(float, float)=0;	//Este desplazaría x e y unidades.
	virtual void 			set_box_w(unsigned int)=0;
	virtual void 			set_box_h(unsigned int)=0;
};

}

#endif
