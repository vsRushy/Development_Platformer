﻿# Development_Platformer

## About

Welcome to our first ever Platformer, Little Big King! This is a project for the Video Game Development subject, in the first semester of our University degree. You will need to collect coins in order to unlock your two main abilities: the dash and the teleport!

**GitHub repository:** [Development Platformer Repository](https://github.com/vsRushy/Development_Platformer)

**Webpage:** [Development Platformer Webpage](https://vsrushy.github.io/Development_Platformer)

**Release:** [Release section](https://github.com/vsRushy/Development_Platformer/releases)

<video src="Little_Big_King.mp4" width="512" height="384" controls preload></video>

## Authors

Marcos Freixas, Gerard.

<img src="Gerard_Photo.jpg" width="500" />

• GitHub account: [vsRushy](https://github.com/vsRushy)

• What has he done: entity system, pathfinding, timing control, load/save, GUI.

Torras Isanta, Martí.

<img src="marti_photo.png" width="480" height="640"/>

• GitHub account: [martitorras](https://github.com/martitorras)

• What has he done: particles, player logic, delta time, Brofiler, coins system.

## Controls

**Left arrow:** Move the player to the left

**Right arrow:** Move the player to the right

**Spacebar:** Jump

**X:** Dash

**Z:** Launch sword

**Left shift:** Teleport to the sword

## Debug keys

**F1:** Start from the very first level

**F2:** Start from the beginning of the current level

**F5:** Save the current state

**F6:** Load the previous state

**F8:** Debug GUI

**F9:** View colliders/logic

**F10:** God mode

**A:** Move camera right

**D:** Move camera left

**W:** Move camera down

**S:** Move camera up

**+:** Volume up

**-:** Volume down

If god mode is activated:

**Left arrow:** Move the player to the left

**Right arrow:** Move the player to the right

**Up arrow:** Move player up

**Down arrow:** Move player down

## Main core subsystems

Our videogame is formed by the main module called "Application". This module "Application" has its sub-modules, which are: 1. Module Window, where the game window is created and setted up; 2. Module Textures, where we load , store and unload the textures of the videogame; 3. Module Render, where we render to the screen all the textures; 4. Module Audio, where we can play, load and unload music and special audio effects; 5. Module Input, where we read the input from the keyboard and mouse from the user. There are also other secondary modules in the same category as the previous ones, such as a Module Pathfinding, a Module Entity Manager and a GUI Manager. All of these secondary modules inherit from a sample Module template. We can also read and write .xml files. The user can load a .tmx file (Tile Map xml).

## Features

Physics based in real life equations, producing a really dynamic player movement.

You can shoot a sword! The fun thing is, you can teleport to it conserving the kinematic energy of the jump.

Improved playability by adding coins that the player needs to collect in order to get stronger and unlock the two main abilities.

You can type your nickname on the start screen and you will see it in the game, while you are playing.

## Notes

You can access to the website by pressing our logo in the starting menu of the videogame.

## Credits

GUI created by Gerard Marcos.

Author 1 nickname: Buch.

• OpenGameArt account: [Buch](https://opengameart.org/users/buch)

• Spritesheet: [Buch's spritesheet](https://opengameart.org/content/a-platformer-in-the-forest)

Author 2 nickname: surt.

• OpenGameArt account: [surt](https://opengameart.org/users/surt)

• Spritesheet: [surt's spritesheet](https://opengameart.org/content/generic-platformer-tiles)

Author 3 nickname: Dameged Panda.

• OpenGameArt account: [Damaged Panda](https://opengameart.org/users/damaged-panda)

• Audio files: [Damaged Panda audio files](https://opengameart.org/content/100-plus-game-sound-effects-wavoggm4a)

Author 4 nickname: CodeManu.

• OpenGameArt account: [CodeManu](https://opengameart.org/users/codemanu)

• Audio files: [CodeManu audio files](https://opengameart.org/content/platformer-game-music-pack)

Author 5 nickname: UntiedGames.

• Itch.io account: [UntiedGames](untiedgames.itch.io/)
 
• Spritesheet: [UntiedGames's spritesheet](https://untiedgames.itch.io/floating-skull-enemy)
 
 Author 6 nickname: (Unknown)
 
 • Spritesheet: [spritesheet](https://forum.unity.com/attachments/linkedit-png.80767/)
 
 Author 7 nickname: (Unknown)
 
 • Spritesheet: [spritesheet](https://www.spriters-resource.com/game_boy_advance/pokemonmysterydungeonredrescueteam/sheet/5251/)

## License

MIT License

Copyright (c) 2018 Gerard Marcos & Martí Torras

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
