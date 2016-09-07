#ifndef PLAYER_EFFECTS
#define PLAYER_EFFECTS

#include <set>

namespace app_game
{

class player_effects
{
	public:
	
	enum class specials{triple_jump, all_friendly, extend_trap, slow_down, invulnerability, high_jump, score_multiplier, projectile, always_trap};

	int				get_score() const {return score;}
	const std::set<specials>&	get_effects() const {return effects;}
	void				add_effect(specials s) {effects.insert(s);}

	//Stores different scores for the same tic. 
	//TODO: In the future we'll need to add a position for the score marker to appear!.
	void		add_score(int s)
	{
		score+=s;
		scores.push_back(s);
	}

	private:

	std::vector<int>	scores;
	std::set<specials>	effects;

	int		score=0;
};

}

#endif
