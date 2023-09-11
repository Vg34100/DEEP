# DEEP
### Dungeon Explorer's Endless Pursuit

###### Current Version
- Launches Window
- Player character controlled using WASD or Arrow Keys
- Player facing direction dependent on Mouse Cursor Location

###### To Add
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
