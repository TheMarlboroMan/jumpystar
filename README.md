# Jumpy Star


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

Features:
	- Day - Night cycle.
	- More enemy types:
		- Patrolling with pauses.
		- Patrolling horizontal shooter.
		- Flying, descending after the player.
		- Patrolling, no jumping on.
		- Flying, stops and shoots directly at the player.
		- We need more variety here. Think a while...
		
	- Scoring bonus with different points.
	- Game speed: do no make it faster as it's done now.
	- Speed special, do it gradually.
	- Add pause.
	- Add bonus random chance.
	- Add score markers.
	- Platform types:
		- Dissapearing platforms (no spawns)
		- Moving platforms (no spawns), horizontal and... vertical?. Maybe even circular motions?
		- (or platforms with other fun features).
