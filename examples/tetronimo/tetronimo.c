#include "Singularity.h"

SpriteHandle handles[4];

void Init(void)
{
	float x = 124;
	float y = 0;

	handles[0] = SE_CreateSprite(x, y, 8, 8);
	handles[1] = SE_CreateSprite(x+8, y, 8, 8); 
	handles[2] = SE_CreateSprite(x+8, y+8, 8, 8); 
	handles[3] = SE_CreateSprite(x+16, y+8, 8, 8); 
}

void Shutdown(void)
{
	for (int i = 0; i < 4; ++i)
	{
		SE_DestroySprite(handles[i]);
	}
}

float dx = 0.0f;
float dy = 16.0f;

int Update(float frametime)
{
	enum Gamepad gp = SE_GetGamepad();

	dx = 0;
	dy = 16.0f;
	if (gp & Left)
	{
		dx = -32;
	}
	else if (gp & Right)
	{
		dx = 32;
	}

	if (gp & Down)
	{
		dy = 32;
	}

	for (int i = 0; i < 4; ++i)
	{
		SE_MoveSpriteRel(handles[i], dx * frametime, dy * frametime);
	}

	return 1;
}