// Singularity Engine

enum Gamepad
{
	None = 0,
	Up = 1,
	Right = 2,
	Down = 4,
	Left = 8,
	A = 16,
	B = 32,
	Start = 64
};

typedef int SpriteHandle;

enum Gamepad SE_GetGamepad(void);

SpriteHandle SE_CreateSprite(float x, float y, float w, float h);
void SE_MoveSpriteRel(SpriteHandle handle, float dx, float dy);
void SE_DestroySprite(SpriteHandle handle);