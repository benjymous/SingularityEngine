void MemMap_UserCodeStart(){};

#include <arch/zx/sp1.h>
#include <arch/zx.h>
#include <intrinsic.h>

#include <input.h>
#include <input/input_zx.h>

#include <stdlib.h>
#include <stdbool.h>

#include "Singularity.h"

//#include "int.h"

extern unsigned char circle[];

struct sp1_pss ps;

struct udk_s keys;

int debugVal = 0;

struct sp1_Rect fullrect = {0, 0, 32, 24}; // rectangle covering the full screen

int frame = 0;

struct pos
{
    int x;
    int y;
};

#define maxSprite 10
struct sp1_ss* sprites[maxSprite];
struct pos sprpos[maxSprite];

void Init();
void Shutdown();
int Update(SENumber frametime);

int gamepadVal = 0;

int SE_GetGamepad(void)
{
    return gamepadVal;
}

int numSprites = 0;

SpriteHandle SE_CreateSprite(int x, int y, int w, int h)
{
    int index = numSprites++;
    struct sp1_ss* spr = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, (int)circle, 0);
    sp1_AddColSpr(spr, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);
    sp1_MoveSprAbs(spr, &fullrect, 0, 0, 0, 0, 0);
    sp1_MoveSprPix(spr, &fullrect, 0, x, y);
    sprites[index] = spr;
    sprpos[index].x = x;
    sprpos[index].y = y;
    return index;
}

void SE_MoveSpriteRel(SpriteHandle index, int frametime, int dx, int dy)
{
    sprpos[index].x += dx;
    sprpos[index].y += dy;
    sp1_MoveSprPix(sprites[index], &fullrect, 0, sprpos[index].x, sprpos[index].y);
    //sprites[index].position += glm::vec2(dx*frametime, dy*frametime);
}

void SE_DestroySprite(SpriteHandle handle)
{

}

main()
{
    srand(3562);

    intrinsic_di(); // inline di instruction without impeding the optimizer

    zx_border(INK_BLUE);

    // Initialize SP1.LIB
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, INK_WHITE | PAPER_BLACK, ' ');

    sp1_Invalidate(&fullrect);

    ps.bounds = &fullrect;
    ps.flags = SP1_PSSFLAG_INVALIDATE;
    ps.visit = 0;
    ps.attr = INK_WHITE | PAPER_BLACK;
    ps.attr_mask = SP1_AMASK_TRANS;

    Init();

    zx_border(INK_GREEN);

    // initialise input
    keys.fire = in_key_scancode('m');
    keys.left = in_key_scancode('o');
    keys.right = in_key_scancode('p');
    keys.up = in_key_scancode('q');
    keys.down = in_key_scancode('a');

    static char buffer[10];

    // interrupt mode 2
    //setup_int();

    zx_border(INK_BLACK);

    while (1)
    {
        gamepadVal = in_stick_keyboard(&keys);

        //zx_border(INK_RED);
        Update(1);
        //zx_border(INK_YELLOW);

        sp1_UpdateNow(); // draw screen now
        //zx_border(INK_BLACK);

        //switch (frame % 10)
        //{
            //case 0:
            //{
            //    itoa(debugVal/10, buffer, 10);
            //    debugVal = 0;
            //    sp1_SetPrintPos(&ps, 0, 1);
            //    sp1_PrintString(&ps, buffer);
            //    sp1_PrintString(&ps, "  ");
            //}
            //break;
            // case 1:
            // {
            //     for (int y = 0; y < 10; ++y)
            //     {
            //         for (int x = 0; x < 10; ++x)
            //         {
            //             int m = GETMAP(y, x);
            //             if (m==6) 
            //             {
            //                 DrawTile((x * 2) + 6, (y * 2) + 2, randomInt(10)<5 ? 6 : 15);
            //             }
            //         }
            //     }
            //     sp1_Invalidate(&gamerect);
            // }
            // break;
        //}

        frame++;
        //debugVal += wait();
    }
}