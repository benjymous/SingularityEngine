#include "Singularity.h"

SpriteHandle handles[4];

// 4 shapes, two rotations per shape
#define NUM_PIECES 8

int pieces[NUM_PIECES][8] = {
	// #O##
	{0,0, -1,0, 1,0, 2,0},

	// #
	// #
	// O
	// #
	{0,0, 0,-2, 0,-1, 0,1},

	//  O# 
	// ##
	{0,0, 1,0, -1,1, 0,1},

	// #
	// #O
	//  #
	{0,0, -1,-1, -1,0, 0,1},

	// #O
	//  ##
	{0,0, -1,0, 0,1, 1,1},

	//  #
	// #O
	// #
	{0,0, 0,-1, -1,0, -1,1},

	// #O#
	//  #
	{0,0, -1,0, 1,0, 0,1},

	//  #
	// #O#
	{0,0, 0,-1, -1,0, 1,0}
};

int nextPiece[NUM_PIECES] = {
	1,
	0,

	3,
	2,

	5,
	4,

	7,
	6
};

int pieceOffsetBottom[NUM_PIECES] =
{
	0,
	1,
	1,
	1,
	1,
	1,
	1,
	0
};

char playfield[10][24];

char checkPlayfield(int x, int y)
{
	if (x < 0 || x >9 || y < 0)
	{
		return 0;
	}
	if (y > 23)
	{
		return 1;
	}
	return playfield[x][y];
}

void setPlayfield(int x, int y)
{
	playfield[x][y] = 1;
	SE_PutTile(x + 11, y, 1);
}

int piece = 0;

SENumber pieceX = 128;
SENumber pieceY = 8;

void Init(void)
{
	for (int x = 0; x < 10; ++x)
	{
		for (int y = 0; y < 24; ++y)
		{
			playfield[x][y] = 0;
		}
	}

	setPlayfield(0,0);
	setPlayfield(9,0);

	piece = rand() % NUM_PIECES;
	for (int i = 0; i < 4; ++i)
	{
		handles[i] = SE_CreateSprite(pieceX + (pieces[piece][i*2]*8), pieceY + (pieces[piece][(i*2)+1]*8), 8, 8, piece/2);
	}
}

void SetPiece()
{
	for (int i = 0; i < 4; ++i)
	{
		SE_MoveSpriteAbs(handles[i], pieceX + (pieces[piece][i*2]*8), pieceY + (pieces[piece][(i*2)+1]*8));
		SE_SetSpriteImage(handles[i], 0);
	}
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

int latched = 1;

int Update()
{
	int gp = SE_GetGamepad();

	dy = 8; // 8 "pixels" per second

	if (gp & SE_Gamepad_Left)
	{
		dx = -32;
	}
	else if (gp & SE_Gamepad_Right)
	{
		dx = 32;
	}
	else
	{
		if (SE_BoundaryDistance(pieceX, 8) == 0)
		{
			dx = 0;
		}
	}

	if (gp & SE_Gamepad_Down)
	{
		dy = 64;
	}

	if (gp & SE_Gamepad_Up)
	{
		if (latched == 1)
		{
			piece = nextPiece[piece];
			SetPiece();
			latched = 0;
		}
	}
	else
	{
		latched = 1;
	}
	
	for (int i = 0; i < 4; ++i)
	{
		SE_MoveSpriteRel(handles[i], dx, dy);
	}
	SE_GetSpritePos(handles[0], &pieceX, &pieceY);

	int y = pieceY / 8;
	int x = (pieceX / 8) - 10;

	int blocked = 0;
	for (int i = 0; i < 4; ++i)
	{
		int tile = checkPlayfield(x + pieces[piece][i*2], y + pieces[piece][(i*2)+1]+1);
		if (tile != 0)
		{
			blocked = 1;
			break;
		}
	}

	if (blocked)
	{
		// paint piece into background
		for (int i = 0; i < 4; ++i)
		{
			setPlayfield(x + pieces[piece][i * 2], y + pieces[piece][(i * 2) + 1]);
		}

		// new piece
		pieceX = 128;
		pieceY = 8;
		piece = rand() % NUM_PIECES;
		SetPiece();
	}

	return 1;
}