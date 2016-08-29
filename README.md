# Jumpy Star

A happy, family oriented little platformer.

Bugfix:

	- Check triple jump special, see if you can pick it and jump.
	- Fix special stacking (or at least, check it).
	- Do not shoot from outside screen (or rather, do no add projectiles outside the camera box...).
		- This keeps failing.
	- is_vulnerable should be different from is_invulnerable: right now we can't stomp on things if we're invulnerable.
		- better add "can stomp":

Do better:

	- World generation (perhaps branching paths?... Think about it on paper first...)
	- What's the purpose of discarding items?. Is there any reward?
		- There could be a reward if the best specials are only given when discarding others, for example. A naive implementation could go with a random good special given (shown before) and another replacing it once three items have been discarded. This would only make sense if "discard" is an action that would discard everything. More complicated approaches could be that combinations of weak specials yield better specials, but that could be tricky to manage in game (jumping, avoiding undesirable specials and crafting the neccesary recipes seems like a lot).

Features:
	- Day - Night cycle for the background: starts at midday, ends at night.
	- More enemy types:
		- Patrolling with pauses.
		- Patrolling horizontal shooter.
		- Flying, descending after the player.
		- Patrolling, no jumping on.
		- Flying, stops and shoots directly at the player.
		- Descending from a side of the screen, shooting nasty patterns.
		- We need more variety here. Think a while...
		
	- Scoring bonus with different points.
	- Score multiplier bonus.
	- Game speed: do no make it faster as it's done now.
	- Speed special, do it gradually.
		- I think this is already done.
	- Tweaks to the engine so two states can be rendered at once (for example, render the game and on top of it, render the menu).
	- Add pause.
	- Add bonus random chance.
	- Add score markers (each thing you pick up makes a score decoration appear).
	- Platform types:
		- Dissapearing platforms (no spawns)
		- Moving platforms (no spawns), horizontal and... vertical?. Maybe even circular motions?
			- Would involve platform "latching" for the player, with the risk of these latches being deleted.
		- Other fun features (friction, completely solid.
