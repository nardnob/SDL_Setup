/*
The SDL setup functions used in this code were created by following the lazyfoo tutorials at lazyfoo.net
*/

//The headers
#include "SDL.h"
#include "SDL_image.h"
#include "constants.cpp"
#include "Physics.h"
#include "Timer.h"
#include <string>
#include <fstream>
#include <vector>

#include "TerrainMap.h"
#include "Entity.h"
#include "Player.h"
using namespace std;

//The surfaces
SDL_Surface *surface_terrain = NULL;
SDL_Surface *surface_entities = NULL;
SDL_Surface *surface_screen = NULL;

//The event structure
SDL_Event event;

//The portions of the sprite map to be blitted
SDL_Rect terrainClip[ TERRAIN_CLIP_COUNT ];
SDL_Rect HUD_rect;
//SDL_Rect entityClip[ ENTITY_CLIP_COUNT ]; //used a vector to hold Player objects and stored the clips in Player instead
vector<Entity*> vector_entities; //this will hold a vector of all entities (for polymorphism to output all entities in one loop, regardless of specific types)
vector<Player> vector_players; //vector to hold the Player object(s)


SDL_Surface *load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

void setWindowIcon(char in_file[])
{
	Uint32 colorKey;
	SDL_Surface *image;

	image = SDL_LoadBMP(in_file);
	colorKey = SDL_MapRGB(image->format, 0, 255, 0);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorKey);
	SDL_WM_SetIcon(image, NULL);
	SDL_FreeSurface(image);
}

bool init(int screenWidth, int screenHeight)
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
	
	//set the window icon 32x32 bmp. 0 255 0: colorkey for transparency
	setWindowIcon( "icon2.bmp" );

    //Set up the screen
    surface_screen = SDL_SetVideoMode( screenWidth, screenHeight, SCREEN_BPP, SDL_FULLSCREEN );

    //If there was an error in setting up the screen
    if( surface_screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "A Pixelated Adventure", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the sprite map
    surface_terrain = load_image( "terrain.png" );
	surface_entities = load_image( "entities.png" );

    if( surface_terrain == NULL 
		|| surface_entities == NULL)
    {
        return false;
    }

    return true;
}

void clean_up()
{
	//The screen surface unallocates automatically
	
    //Free the sprite map
    SDL_FreeSurface( surface_terrain );
	SDL_FreeSurface( surface_entities );

    //Quit SDL
    SDL_Quit();
}

void display(int code_in, TerrainMap& currentMap)
{
	switch(code_in)
	{
	case CODE_TERRAIN:
		for(int numX = 0; numX < currentMap.get_sizeX(); numX++)
		{
			for(int numY = 0; numY < currentMap.get_sizeY(); numY++)
			{
				apply_surface( 
					numX * 32, 
					numY * 32, 
					surface_terrain, 
					surface_screen, 
					&terrainClip[currentMap.mapData[numX][numY]] );
			}
		}
		break;
	case CODE_ENTITY:
		for(int i = 0; i < vector_entities.size(); i++)
		{
			apply_surface( 
				vector_entities.at(i)->posX, 
				vector_entities.at(i)->posY, 
				surface_entities, 
				surface_screen, 
				&vector_entities.at(i)->rect[vector_entities.at(i)->get_currentTexture()] );
		}
		break;
	case CODE_HUD:
		SDL_FillRect( surface_screen, &HUD_rect, SDL_MapRGB( surface_screen->format, 86, 86, 86 ) );
		break;
	}
}

void defineClip(int code_in, int screen_w = 0, int screen_h = 0)
{
	switch(code_in)
	{
	case CODE_TERRAIN:
		//define the terrainClip array to the corresponding position of each terrain in the terrain text file
		for(int i = 0; i < TERRAIN_CLIP_COUNT; i++)
		{
			terrainClip[ i ].x = (i * TERRAIN_CLIP_W) % TERRAIN_FILE_W;
			terrainClip[ i ].y = ((i * TERRAIN_CLIP_W) / TERRAIN_FILE_W) * TERRAIN_CLIP_H;
			terrainClip[ i ].w = TERRAIN_CLIP_W;
			terrainClip[ i ].h = TERRAIN_CLIP_H;
		}
		break;
	case CODE_ENTITY:	
		//define a player by putting them into the player vector, then putting a pointer of them into the entity vector (for polymorphism output)
		vector_players.push_back(Player(
			( (0 * ENTITY_CLIP_W) % ENTITY_FILE_W ), //the x value of the entity clip (in the entity texture file)
			( ((0 * ENTITY_CLIP_W) / ENTITY_FILE_W) * ENTITY_CLIP_H ) //the y value of the entity to clip (in the entity texture file)
			));

		vector_entities.push_back(&vector_players[0]);
		break;
	case CODE_HUD:
		HUD_rect.w = screen_w;
		HUD_rect.h = HUD_H;
		HUD_rect.x = 0;
		HUD_rect.y = screen_h;
		break;
	}
}

void eventHandler(bool& quit)
{
	    //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
			switch(event.type)
			{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_LEFT:
				case SDLK_a:
					vector_players.at(0).pressKey(KEY_LEFT);
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					vector_players.at(0).pressKey(KEY_RIGHT);
					break;
				case SDLK_UP:
				case SDLK_w:
					vector_players.at(0).pressKey(KEY_UP);
					break;
				case SDLK_DOWN:
				case SDLK_s:
					vector_players.at(0).pressKey(KEY_DOWN);
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
				case SDLK_LEFT:
				case SDLK_a:
					vector_players.at(0).releaseKey(KEY_LEFT);
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					vector_players.at(0).releaseKey(KEY_RIGHT);
					break;
				case SDLK_UP:
				case SDLK_w:
					vector_players.at(0).releaseKey(KEY_UP);
					break;
				case SDLK_DOWN:
				case SDLK_s:
					vector_players.at(0).releaseKey(KEY_DOWN);
					break;
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
        }	
}

int main( int argc, char* args[] )
{		
	Timer fpsTimer;

	//initialize the currentMap object with the mapFileName text file
	string mapFileName = "map_001.txt";
	TerrainMap currentMap = TerrainMap(mapFileName);	

	//define the clips (clip up the texture files)
	defineClip(CODE_TERRAIN);
	defineClip(CODE_ENTITY);
	defineClip(CODE_HUD, currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H);

	int counter = 0;

    bool quit = false;

	if( init(currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_H) == false)
    {
        return 1;
    }

    if( load_files() == false )
	{
        return 1;
    }

	//fill with black
    SDL_FillRect( surface_screen, &surface_screen->clip_rect, SDL_MapRGB( surface_screen->format, 0xFF, 0xFF, 0xFF ) );

    //***********************************************************************************
	//*********** The game loop *********************************************************
	//***********************************************************************************
    while( !quit )
    {
		fpsTimer.start();

		//call the eventHandler (send quit as a reference)
		eventHandler(quit); 
		
		Physics::doPhysics(vector_players);
		
	
		//apply the terrain 
		display(CODE_TERRAIN, currentMap);
		//apply the entities
		display(CODE_ENTITY, currentMap);
		//display the HUD
		display(CODE_HUD, currentMap);
		
		//Update the screen
		if( SDL_Flip( surface_screen ) == -1 )
		{
			return 1;
		}

		//If we want to cap the frame rate
        if(fpsTimer.get_ticks() < 1000 / FRAMES_PER_SECOND)
        {
            //Sleep the remaining frame time
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fpsTimer.get_ticks());
        }
    }
    //***********************************************************************************
	//*********** End of the game loop **************************************************
	//***********************************************************************************

    //Free the images and quit SDL
    clean_up();

    return 0;
}