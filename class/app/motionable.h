#ifndef MOVIL_H
#define MOVIL_H

/*Modelo de cosa que se mueve en el espacio...  En todo caso, esta es la base 
para el cálculo de movimiento y tiene algunas cosas de gravedad también.
*/

#include <herramientas/vector_2d/vector_2d.h>

namespace App_Interfaces
{

class Motionable
{
	//////////////////////
	// Definiciones...
	public:

	enum class axis {x,y};
	typedef DLibH::Vector_2d<float> t_vector;

	///////////////////////
	// Interface pública.
	public:

	Motionable();
	virtual ~Motionable();

	const t_vector&			 	get_vector() const {return vector;}
	t_vector		 		get_vector() {return vector;}

	float 					get_vector_x() {return vector.x;}
	float 					get_vector_y() {return vector.y;}

	void 					do_gravity(float delta, float=1.0f);

	//delta: tiempo que ha pasado, vector: referencia a la parte del vector, factor: cantidad de fuerza a aplicar al vector.
	float 					integrate_vector(float delta, float& vector, float factor);
	void 					add_vector(float, axis);
	void 					set_vector(float, axis);
	void 					set_vector(t_vector v) {vector=v;}
	float& 					ref_vector_x() {return vector.x;}
	float& 					ref_vector_y() {return vector.y;}

	///////////////////////
	//A implementar.
	virtual float 				get_weight() const=0;
	virtual float 				get_max_fall_speed() const=0;

	//////////////////////
	// Propiedades...
	private:

	t_vector				vector;

};

}
#endif
