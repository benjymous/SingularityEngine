// Singularity Engine
#ifndef SINGULARITY_H
#define SINGULARITY_H

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

SpriteHandle SE_CreateSprite(SENumber x, SENumber y, SENumber w, SENumber h);
void SE_MoveSpriteRel(SpriteHandle handle, SENumber frametime, SENumber dx, SENumber dy);
void SE_DestroySprite(SpriteHandle handle);

#endif