
#include <3ds.h>
#include <citro2d.h>
#include <math.h>

float x = 200;
float y = 120;

void drawGota(float x, float y) {
    u32 azul = C2D_Color32(30, 160, 255, 255);
    u32 branco = C2D_Color32(255, 255, 255, 255);
    u32 preto = C2D_Color32(15, 25, 40, 255);

    // Bracos
    C2D_DrawLine(x-12,y,azul,x-24,y+8,azul,5,0);
    C2D_DrawLine(x+12,y,azul,x+24,y+8,azul,5,0);

    // Pernas
    C2D_DrawLine(x-7,y+12,azul,x-9,y+24,azul,5,0);
    C2D_DrawLine(x+7,y+12,azul,x+9,y+24,azul,5,0);

    // Corpo
    C2D_DrawTriangle(
        x,y-28,azul,
        x-15,y,azul,
        x+15,y,azul,0
    );

    C2D_DrawCircleSolid(x,y,0,16,azul);

    // Olhos
    C2D_DrawCircleSolid(x-6,y-3,0,5,branco);
    C2D_DrawCircleSolid(x+6,y-3,0,5,branco);

    C2D_DrawCircleSolid(x-5,y-2,0,2,preto);
    C2D_DrawCircleSolid(x+7,y-2,0,2,preto);

    // Boca
    C2D_DrawLine(x-4,y+7,preto,x+4,y+7,preto,2,0);
}

int main() {
    gfxInitDefault();

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* top =
        C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    C3D_RenderTarget* bottom =
        C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    C2D_TextBuf textBuf = C2D_TextBufNew(256);

    C2D_Text title;
    C2D_TextParse(&title,textBuf,"CLUB HIDRO");
    C2D_TextOptimize(&title);

    C2D_Text version;
    C2D_TextParse(&version,textBuf,"v0.0.1 - Tech Demo");
    C2D_TextOptimize(&version);

    while (aptMainLoop()) {
        hidScanInput();

        u32 down = hidKeysDown();
        u32 held = hidKeysHeld();

        if (down & KEY_START) break;

        circlePosition pad;
        hidCircleRead(&pad);

        x += pad.dx / 60.0f;
        y -= pad.dy / 60.0f;

        if (held & KEY_UP) y -= 2;
        if (held & KEY_DOWN) y += 2;
        if (held & KEY_LEFT) x -= 2;
        if (held & KEY_RIGHT) x += 2;

        if (x < 25) x = 25;
        if (x > 375) x = 375;
        if (y < 30) y = 30;
        if (y > 215) y = 215;

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // Tela superior
        C2D_TargetClear(top,C2D_Color32(170,225,255,255));
        C2D_SceneBegin(top);

        C2D_DrawRectSolid(
            0,65,0,400,175,
            C2D_Color32(100,200,140,255)
        );

        C2D_DrawRectSolid(
            165,65,0,70,175,
            C2D_Color32(235,215,170,255)
        );

        drawGota(x,y);

        // Tela inferior
        C2D_TargetClear(bottom,C2D_Color32(15,45,85,255));
        C2D_SceneBegin(bottom);

        C2D_DrawText(&title,C2D_WithColor,
            65,65,0,0.9f,0.9f,
            C2D_Color32(80,210,255,255));

        C2D_DrawText(&version,C2D_WithColor,
            65,110,0,0.5f,0.5f,
            C2D_Color32(255,255,255,255));

        C3D_FrameEnd(0);
    }

    C2D_TextBufDelete(textBuf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();

    return 0;
}
