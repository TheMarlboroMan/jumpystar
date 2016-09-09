#ifndef DECORATION_SCORE_H
#define DECORATION_SCORE_H

#include <map>
#include <memory>

#include <templates/linear_timed_function.h>

#include "decoration.h"

namespace app_game
{
class decoration_score:
	public decoration
{
	public:

	using t_uptr=std::unique_ptr<ldv::ttf_representation>;
	using t_map=std::map<int, std::unique_ptr<ldv::ttf_representation>>;

						decoration_score(t_point, int, const ldv::ttf_font& font);
	virtual	void				do_turn(float);

	//////////////////////
	//Drawable.

	virtual int				get_draw_order() const {return 10;}
	virtual void 				transform_draw_struct(draw_control&) const;

	////////////////////
	//Motion actor

	virtual float 				get_weight() const {return 0.f;}
	virtual float 				get_max_fall_speed() const {return 0.f;};
	virtual void 				adjust_callback(float, motion_actor::adjust_pos) {}

	private:

	void					add_to_map(int, const ldv::ttf_font& font);
	static t_map				score_map;

	private:

	int					score;
	tools::linear_timed_function<float>	period_ltf;
};
}

#endif
