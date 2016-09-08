#ifndef PLAYER_EFFECTS
#define PLAYER_EFFECTS

#include <set>

#include <tools/point_2d/point_2d.h>

namespace app_game
{

class player_effects
{
	public:
	
	class score_struct
	{
		public:
		int score;
		ldt::point_2d<int> pt;
	};

	enum class specials{triple_jump, all_friendly, extend_trap, slow_down, invulnerability, high_jump, score_multiplier, projectile, always_trap};

	int				get_score() const {return score;}
	const std::set<specials>&	get_effects() const {return effects;}
	void				add_effect(specials s) {effects.insert(s);}
	const std::vector<score_struct> get_scores() const {return scores;}
	bool				is_significant() const {return score || effects.size();}

	//Stores different scores for the same tic. 
	void		add_score(int s, ldt::point_2d<int> pt)
	{
		score+=s;
		scores.push_back({s, pt});
	}

	void		reset()
	{
		effects.clear();
		scores.clear();
		score=0;
	}

	private:

	std::vector<score_struct>	scores;
	std::set<specials>		effects;

	int		score=0;
};

}

#endif
