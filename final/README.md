Evan Hastings
CPSC 6160
Boxman

# Credits for Resources used
The only assets I designed myself were the player sprites, the start screen, and the game over screen.
The rest have links below corresponding to where I got them from.
[Tileset Author's Page] (https://draconimous.itch.io/free-platform-tileset?download)
[Parallax Background Author's Page] (https://digitalmoons.itch.io/parallax-forest-background)
[Other Background Page] (https://gameartpartners.com/downloads/tileset-platformer-game-pack-2d-city/)
[Font (Calibri)] (https://fontsnetwork.com/calibri-font-family/)
[Tiled Level Editor] (https://www.mapeditor.org/)
[XML Parsing Lib] (https://github.com/leethomason/tinyxml2)

# CONTROLS:
Up - Jump; can be held to "fly"
Right - move right
Left - move left

# GENERAL:
You win by touching the treasure chest on each level.
You will die by touching any water on the level.
Run the game with "make run", as it requires a game setup file to run

# FINAL PROJECT REQUIREMENTS:
Out of the possible options, I chose to do the following from the list:
   1. Start/Finish Screens
         These can be seen when the game starts, and when the player runs out of lives.
   2. In-play UI
         The in-play UI is a frame-rate display and a life counter that are in the top two corners.
   3. Textual elements with fonts
         Both in-play UI elements use a Calibri TTF to display their text.
   4. Level editing method
         I used an external editor called Tiled to write levels for my project. (There's a link to it above).
         I would use ids from a tilesheet and it puts them into csv encoded data inside of an XML sheet.
         I also wrote my own format for a game setup XML document, which I discuss a bit below.

# DESIGN
Most classes in this game are controlled by a singleton class, such as the TextureController, GameEngine, and Camera.
All of the rendering is done from a camera perspective, and each object only knows the id of its texture.
This allowed for a better rendering process, as all of the rendering can be handled by one class, with minimal knowledge by other classes.

Almost everything that renders is a GameObject, and every object inherits from this in some way.
Things like the UI and Screens have their own classes separate from the GameObject.

The majority of the game can be customized using a couple XML files, one written by the user, and the other coming from the Tiled editor.
The map files from Tiled use a map encoded as a csv of tile id's from a tileset image.
The other XML file is the game setup file, which allows the user to include details about levels, UI elements, and background layers.
For an example of the game setup file, see "./res/joust.xml" and for the map data, see "./res/maps/joust_level.tmx"
The game setup file is interesting, because you can have a variable amount of levels, that will only progress when the win condition is met.
The joust file above includes both levels I used in testing, and it shows off that the game "can" scroll, but that it does not have to.

Next, I'll talk about the sprites.
The sprites expect a standard format, and they are not very flexible unfortunately.
The sprite sheet should follow the following format:
   First row: Idle animation
   Second row: Running animation
   Third row: Jumping animation
I only implemented sprites in the Player class, although the rendering method uses one of the generalized methods from the TextureController singleton.
This means it would not be difficult to add in another object that has sprite animations, as long as it follows the above format.

