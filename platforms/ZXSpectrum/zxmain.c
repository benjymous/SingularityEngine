void MemMap_UserCodeStart(){};

#include <arch/zx/sp1.h>
#include <arch/zx.h>
#include <intrinsic.h>

#include <input.h>
#include <input/input_zx.h>

#include <stdlib.h>
#include <stdbool.h>

#include "Singularity.h"

#include "int.h"

extern unsigned char spr0_col0[];
extern unsigned char tile0[];

struct sp1_pss ps;

struct udk_s keys;

int debugVal = 0;

struct sp1_Rect fullrect = {0, 0, 32, 24}; // rectangle covering the full screen

int frame = 0;

struct sprs
{
    int x;
    int y;
    int image;
};

#define maxSprite 10
struct sp1_ss* sprites[maxSprite];
struct sprs sprdat[maxSprite];

void Init();
void Shutdown();
int Update();

int gamepadVal = 0;

int SE_GetGamepad(void)
{
    return gamepadVal;
}

int numSprites = 0;

SpriteHandle SE_CreateSprite(int x, int y, int w, int h, int image)
{
    int index = numSprites++;

    struct sp1_ss* spr = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 2, (int)spr0_col0, 0);
    sp1_AddColSpr(spr, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);

    sp1_MoveSprAbs(spr, &fullrect, 0, 0, 0, 0, 0);
    sp1_MoveSprPix(spr, &fullrect, 0, x, y);

    sprites[index] = spr;
    sprdat[index].x = x << 5;
    sprdat[index].y = y << 5;
    sprdat[index].image = 32*image;

    spr->frame = (void*)sprdat[index].image;

    return index;
}

void SE_MoveSpriteAbs(SpriteHandle index, int x, int y)
{
    sprdat[index].x = x << 5;
    sprdat[index].y = y << 5;
    sp1_MoveSprPix(sprites[index], &fullrect, 0, x, y);
}

void SE_MoveSpriteRel(SpriteHandle index, int dx, int dy)
{
    sprdat[index].x += dx;
    sprdat[index].y += dy;
    sp1_MoveSprPix(sprites[index], &fullrect, 0, sprdat[index].x >> 5, sprdat[index].y >> 5);
}

void SE_SetSpriteImage(SpriteHandle index, int image)
{
    struct sp1_ss* spr = sprites[index];
    sprdat[index].image = 32*image;
    //se->tile = (se->startTile + se->tileFrame + (se->flip ? NUM_SPR_GFX : 0)) * SPRITE_SPACING;
    
}

void SE_GetSpritePos(SpriteHandle index, int* x, int* y)
{
    *x = sprdat[index].x >> 5;
    *y = sprdat[index].y >> 5;
}

void SE_DestroySprite(SpriteHandle handle)
{

}

void SE_PutTile(int x, int y, int tile)
{
    sp1_PrintAt(y, x-1, INK_BLACK | PAPER_WHITE, tile0);
    struct sp1_Rect tilerect;
    tilerect.row = y;
    tilerect.col = x;
    tilerect.width = 1;
    tilerect.height = 1;
    sp1_Invalidate(&tilerect);
}

int SE_BoundaryDistance(SENumber val, int mod)
{
    return val % mod;
}

main()
{
    srand(3562);

    intrinsic_di(); // inline di instruction without impeding the optimizer

    zx_border(INK_RED);

    // Initialize SP1.LIB
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, INK_BLACK | PAPER_WHITE, ' ');

    sp1_Invalidate(&fullrect);

    ps.bounds = &fullrect;
    ps.flags = SP1_PSSFLAG_INVALIDATE;
    ps.visit = 0;
    ps.attr = INK_WHITE | PAPER_BLACK;
    ps.attr_mask = SP1_AMASK_TRANS;
    
    zx_border(INK_GREEN);

    Init();

    zx_border(INK_BLUE);

    // initialise input
    keys.fire = in_key_scancode('m');
    keys.left = in_key_scancode('o');
    keys.right = in_key_scancode('p');
    keys.up = in_key_scancode('q');
    keys.down = in_key_scancode('a');

    static char buffer[10];

    // interrupt mode 2
    setup_int();

    zx_border(INK_BLACK);

    while (1)
    {
        gamepadVal = in_stick_keyboard(&keys);

        zx_border(INK_CYAN);

        // Call game's Update function
        Update(); 

        zx_border(INK_YELLOW);

        // Draw sprites
        sp1_UpdateNow();

        zx_border(INK_BLACK);

        // // frame time debug
        // switch (frame % 10)
        // {
        //     case 0:
        //     {
        //        itoa(debugVal/10, buffer, 10);
        //        debugVal = 0;
        //        sp1_SetPrintPos(&ps, 0, 1);
        //        sp1_PrintString(&ps, buffer);
        //        sp1_PrintString(&ps, "  ");
        //     }
        //     break;
        // }

        frame++;
        debugVal += wait(1); // wait for next vblank
    }
}