// Singularity Engine
#ifndef SINGULARITY_H
#define SINGULARITY_H

#include <stdlib.h>

#include "SingularityPlatformDefs.h"

// These intentionally match the z88dk input values

#define SE_Gamepad_None  0
#define SE_Gamepad_Up    0x01
#define SE_Gamepad_Down  0x02
#define SE_Gamepad_Left  0x04
#define SE_Gamepad_Right 0x08
#define SE_Gamepad_Fire  0x80

typedef int SpriteHandle;

int SE_GetGamepad(void);

SpriteHandle SE_CreateSprite(SENumber x, SENumber y, int w, int h, int image);
void SE_MoveSpriteAbs(SpriteHandle handle, SENumber x, SENumber y);
void SE_MoveSpriteRel(SpriteHandle handle, SENumber dx, SENumber dy);
void SE_SetSpriteImage(SpriteHandle handle, int image);
void SE_GetSpritePos(SpriteHandle handle, SENumber* x, SENumber* y);
void SE_DestroySprite(SpriteHandle handle);


void SE_PutTile(int x, int y, int tile);

int SE_BoundaryDistance(SENumber val, int mod);

#endif