#include "Singularity.h"

SpriteHandle handles[4];

void Init(void)
{
	SENumber x = 120;
	SENumber y = 4;

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

SENumber dx = 0;
SENumber dy = 1;

int Update(SENumber frametime)
{
	enum Gamepad gp = SE_GetGamepad();

	dx = 0;
	dy = 1;
	if (gp & SE_Gamepad_Left)
	{
		dx = -1;
	}
	else if (gp & SE_Gamepad_Right)
	{
		dx = 1;
	}

	if (gp & SE_Gamepad_Down)
	{
		dy = 2;
	}

	for (int i = 0; i < 4; ++i)
	{
		SE_MoveSpriteRel(handles[i], frametime, dx, dy);
	}

	return 1;
}