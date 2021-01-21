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

int pieceOffsetLeft[NUM_PIECES] =
{
	1,
	0,
	1,
	1,
	1,
	1,
	1,
	1
};


int pieceOffsetRight[NUM_PIECES] =
{
	2,
	0,
	1,
	0,
	1,
	0,
	1,
	1
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

void setPlayfield(int x, int y, int i)
{
	playfield[x][y] = 1;
	SE_PutTile(x + 11, y, i-1);
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
		SE_SetSpriteImage(handles[i], piece/2);
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

	int bd = SE_BoundaryDistance(pieceX, 8);

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
		if (bd == 0)
		{
			dx = 0;
		}
	}

	if (dx !=0 && bd == 0)
	{
		if (dx < 0 && (int)(pieceX/8) - pieceOffsetLeft[piece] == 10)
		{
			dx = 0;
		} 
		else if (dx > 0 && (int)(pieceX / 8) + pieceOffsetRight[piece] == 19)
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
			setPlayfield(x + pieces[piece][i * 2], y + pieces[piece][(i * 2) + 1], 1+(piece/2));
		}

		// new piece
		pieceX = 128;
		pieceY = 8;
		piece = rand() % NUM_PIECES;
		SetPiece();
	}

	return 1;
}