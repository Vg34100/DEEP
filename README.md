# DEEP
### Dungeon Explorer's Endless Pursuit

###### Current Version
- Launches Window
- Player character controlled using WASD or Arrow Keys
- Player facing direction dependent on Mouse Cursor Location

###### To Add
- PLAYER
    - "SLASH"
        - When "Clicking" -> Attack (Summon another cube that lasts a bit before disspearing to simulate a slash for melee combat)
    - HEALTH
        - GUI for Hearts
        - Starting point 5 hearts, total of 10 hit points
    - GAME OVER
        - If Player Health == 0 -> GAME OVER (Stop the game action, spawn GUI screen, allow for RESTART or QUIT)
- ENEMIES
    - Some Cube that will move towards the Player
    - If Enemy collides with Player -> Player loses health
