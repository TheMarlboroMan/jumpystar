#ifndef PLAYER_EFFECTS
#define PLAYER_EFFECTS

#include <vector>

namespace app_game
{

class player_effects
{
	public:
	
	enum class specials{triple_jump, all_friendly, extend_trap, slow_down, invulnerability, high_jump, score_multiplier};
	enum types {triple_jump=1, all_friendly=2, extend_trap=4, slow_down=8, invulnerability=16, high_jump=32, score_multiplier=64};

	int		get_score() const {return score;}
	int		get_effects() const {return effects;}
	void		add_effect(int s) {effects|=s;}

	//Stores different scores for the same tic. 
	//TODO: In the future we'll need to add a position for the score marker to appear!.
	void		add_score(int s)
	{
		score+=s;
		scores.push_back(s);
	}

	private:

	std::vector<int>	scores;

	int		score=0,
			effects=0;
};

}

#endif
