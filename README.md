# DEEP
### Dungeon Explorer's Endless Pursuit

###### Version Update 11/19/2023 as of 12:00 AM
- Camera Shake on Hit
- Improved Options Screen
- Fixed Fullscreen and Screensizes
- Enemy Movement
- Display for Level Cleared
- Stats Display while Playing (CTRL+Z)
- Pause State
- Custom Keybinds
- Camera Move w/ Mouse
- Letter/Number Writing Method
- Sword Sprites

###### Version Update 10/11/2023 as of 5:00 PM
- Added the image files
    - Can load an image and render it
    - Can render a sprite sheet out of it 
    - The first pixel of the sprite is considered the green screen
- Added back the font solution from asteroids, despite it being super small
- prodriguezqu
    - Added a simple bar health bar
    - Created a functional Title Screen
    - Delve starts the game | Options opens the options page | Quit quits the game
        - Select using up and down arrow, and then enter key
    - Options menu allows for change between fullscreen and windowed, as well as screen size
        - Actual creen size and fullscreen changing could use some tweaking
    - Added some text to help track level progression, enemies remaining, and enemies defeated
- CollisionManager
    - Fixed the collision so now created levels do not have the offset bug
    - Now when there are still enemies in the level, hallways will not let you pass
- Coverted everything to tabs thanks to VSCODE


###### Plans
- Plan on adding camera movement in relation to the mouse cursor
- Camera zoom out based on speed for more depth
- Visuals for Fullscreen and Screensize options in options menu
- Apply button for options menu instead of immidiate change
- More options like changing of keybinds
- Sprites/Visuals for other things
- A sound solution, something like midi or whatever
- Spritesheet for the alphabet to use to create a custom font for the game


###### PREVIOUS VERSION
- Launches Window
- CONTROLS
    - WASD  - Move
    - Mouse - Look
    - R     - UseWeapon
    - ESC   - Quit
    - F     - Fullscreen (kinda bugged)
- Player facing direction dependent on Mouse Cursor Location
- Top Down Camera with Camera Lag
- Basic Dummy Enemy (Orc)
    - Health
    - Takes Damage
- Based Dummy Weapon (LIGHTS)
    - Deals Damage
- Hitbox structs handle collision between Weapon and Enemy
- World generates a random Level at initialization
    - After Enemies are cleared
        - Collision Detection with Hallways opens
        - If pass Hallways, new Level generated in direction
    - World collision with player handled by raycast, checks what TILE is underneath Player (could use some work)
    - World made up of Tiles
        - WALL | FLOOR | ENEMY | PIT | HALLWAY(N|E|S|W)
###### Current State of Files - TBI = To be implemented
- Collision Manager
    - Manages the Colliions between various objects (Player->World | Weapon->Enemy)
- Health
    - Manages health, with an InitialHealth, MaxHealth, and TakeDamge functions
    - Displays healthbar if rendering the function
    - Should/Could handle Temperary Health, Invulnerability, and Regeneration
- ICollidable
    - for collidable objects? 
- Vector 2
    - Holds Vector 2 Struct, holds x,y variables and has various functions for Vector2
    - Also holds Hitbox Struct which is composed of two Vectors, a topLeft and bottomRight position, could be moved
- Enemy
    - Holds the base enemy class, enemies hold a name, attackDamage, attackCooldown, health, size, position, a reference to world, invicibilityDuration, invulnerability bool, lastDamageTime float, a hitBox, and a isDead bool
    - Add subchildren to base enemy class with other enemies
        - Can override to have specific render or attack functions (probably should have others too)
- GUI-TitleScreen
    - Titlescreen when the game starts
- Player
    - is the Player class
    - Players have stats for Health, Mana(TBI), Ammo(TBI), other Major Attributes
    - Speed, Damage, Resistance, AttackSpeed, Range, Luck, Size, playerWidth, and playerHeight
    - playerPos, mousePos, cameraPos, playerVelocity, playerDirection, activeWeapon, and a Hitbox
    - player movement is handled in handleInput and in handleMovement
    - useWeapon causes an attack from the atctive weapon
- FPSManger
    - idk man
- initGLX
    - most of the initialization stuff for starting the window, or closing the window, so stuff related to X11, and OpenGL
    - also holds GLUT's text rendering method, which could still be improved opon
    - handles initial events, which are the initial inputs
    - exports the keyspressed, the width and height of the window, and the mouse position
    - handles window resizing and fullscreen
- Weapon
    - has a base weapon class, extends to 4 different classes of Melee, Magic, Ranged, or Summoning
    - weapon has a hitbox, attacking "activates" the hitbox
    - weapons should override use, render, and update?
    - weapons hold a name, damage, cooldown (time till next attack), attackSize, speed, duration(how far it will go), damageType(TBI), weaponClass(no real difference yet), bool for isAttacking, bool for onCooldown, and a lastUseTime float, as well as a hitBox
- Level
    - holds the level class
    - a level is just a class that holds 3 things, a name, a layout, and enemies
    - the layout is made up of strings that each character represents a tiletype
    - "-" = Wall | "x" = Floor | "^" = Pit (doesnt do anything yet) | "*" = Enemy (Causes an enemy to spawn from the determined enemy list) | "||" = Doors | Directional Dependent Hallways "N"(North) "E"(East) "W"(West) "S"(South)
    - also holds a levelManager class with some helpful functions when working with levels
- World
    - World on initialzation talks to LevelManger to get a randomLevel to spawn
    - generated the Level based on the tiles
    - spawns the proceeding level when it communiates with collisionManager to know which directional hallway the player took


###### Stuff to Fix
- ~~Memory Leaks when Game Closes~~
+ No fix for this
- ~~FPSManager doesn't actually fix the same FPS problem~~
    - ~~Fix FPS so the game runs the same on different systems~~
+ Implemented back the Asteroids FPS soluton
- ~~Movement - Specifically Direction Movement~~
+ Added back diagonal movement
- World not loading sometimes for Vincent? 
- Fix the Fullscreen stuff
- ~~World collision with player not all accurate.~~
+ Figured out to use the same collision for walls with everything else
- ~~Text thickness, or a better method for text in general~~
+ Added back the font from asteroids for debug text, and we can create images for other text
+ A text solution using a sprite sheet would be good
- Attacking should probably be reworked to allow multiple attacks out at a time when it comes to ranged weapons

###### To Do
- ~~Figure out importing sprites~~
- Make a revamped title screen
    - Pixel art, background of a hole, title of "DEEP" in the top center, stylied, 3 buttons, selectable with up and down and then selecting it with enter
    - ~~The button "DELVE IN" starts the game (gamestate changes to PLAYING)~~
    - ~~Next button should be a settings button, changes the gamestate to settings, and has different things like screen mode,~~ or controls
    - ~~Final button is Exit to close the game~~
- GUI in game
    - Info card, should show up at the bottom center of the game, and provide the player relavant information like if they get a new gun, if there is a stat change, or if there is a boss incoming
    - Health GUI that takes the health in increments of 10 for one bar, metroid style health
    - Weapon Class dependent GUI [The weapon icon and class icon should always be shown to the right]
        - Melee Class has a crit bar, attack when the bar is at a certain spot, crit is applied
            - Needs to incorporate a crit multiplier, and possibly a crit chance
        - Magic Class has a charge bar and mana bar, hold the attack button to charge the power of the attack, the more you charge, the more mana you consume
            - Needs to incorporate a charge rate and charge multiplier, affects attack and mana usage
        - Ranged Class has a magazine size based on the weapon, after mag runs out, you need to reload, the ability to reload depends on the players ammo count
            - Needs to incorporate a mag size and a reload rate
        - Summon Class has a max summons number
    - Equipment GUI, looks like a pass port
        - should display player information like stats, a picture of the player, the weapons [4 weapons, 1 per class], and equipment
- Player
    - Needs a Mana mangement system. Naturally regens. Decreases when using a Mana baed weapon
    - Needs a Ammo management system. Decreases when reloading, decreases the amount of the mag size, 
    - Needs a Summon management system. Maximum of n (start at 2) summons. Reusing causes respawning of minion, right click changes targeting?
    - Possible right click action for weapon classes?
- Add more tiles (Wildcard idea)
- Make tiles rendering dependent on a theme (Forest? - Green Tiles | Cavern? - Dark Tiles | etc...)
    - Theme ideas
        - Forest - "Foiliage", Water (slows player)
        - Cavern - Pits (player "falls" and takes damage), Rocks (basically walls) - If progression, unlock Ammo
        - Hallow - Crystals (basically walls) - If progression, unlock Mana/Summon
        - Lava - Lava Pits (same as pits, but lava), Fire (takes damage as long as u stay on it, u can survive quickly passing it), Geyser (stand on it, it heats up, then after causes damage hitbox, even if player had left, then coolsdown)
        - Frozen - High Snow (slows, like water, but masks visibility), Icicles (they are spikes), Frozen Zones (everything is slowed)
        - Underworld - Yellow Fire (its fire, faster?), Geysers that aim
        - Crimson - Flesh themed, Acidic Pools (slows, like water, damages, like fire), Roller Blades (these are spikes)
        - Abyss - idk, probably dark, i just thought it sounded cool
- Enemy AI 
    - Enemy travelling towards enemy, causing damage to the player when collision with itself or it's attack

###### Initial Draft
- PLAYER (Player.cpp)
    - COMBAT
        - "SLASH"
            - When "Clicking" -> Attack (Summon another cube that lasts a bit before disspearing to simulate a slash for melee combat)
        - Weapon Variety?
            - Ranged weapons, magic wands, different melee weapons, etc.
    - HEALTH - (Modularity - Should be Health.cpp - Can be used with Player.cpp {enemy}.cpp)
        - GUI for Hearts
        - Starting point 5 hearts, total of 10 hit points
    - GAME OVER
        - If Player Health == 0 -> GAME OVER (Stop the game action, spawn GUI screen, allow for RESTART or QUIT)
    - DODGE
        - A quick dodge or roll to evade enemy attacks.
    - INVENTORY?
        - To carry potions, artifacts, and other usable items.
    - UPGRADES?
        - Acquire new skills or enhance existing ones.
    - CURRENCY
        - Coins or some form of currency to purchase items or upgrades.
- ENEMIES
    - Some Cube that will move towards the Player
    - If Enemy collides with Player -> Player loses health
    - (LATER)
        - Different types of enemies with unique behaviors and attack patterns.
        - Boss Battles
- WORLD
    - Procedurally Generated Dungeons
    - Different Themes
    - Traps
        - Spikes - Pitfalls, Moving Walls, etc.
    - Backstory? 
        - Why is the player exploring these dungeons? What's the ultimate goal?
        - Non-player characters that offer quests, lore, or other interactions.
- PROGRESSION
    - While players start from the beginning upon death, they retain some form of progression (e.g., unlocked weapons, skills).
- GRAPHICS/SOUNDS (LATER)
    - ART STYLE
    - SOUND EFFECTS
        - Background music for different biomes, sound effects for actions like attacks, getting hit, etc.
- UI
    - HEALTH
    - PAUSE & SETTINGS
    - MINIMAP?
    - TOOL TIPS? 
        - Hover over items to see their descriptions.
