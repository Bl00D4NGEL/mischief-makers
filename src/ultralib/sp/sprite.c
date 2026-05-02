#include <assert.h>
#include <sp.h>
#include <PR/os_version.h>

void spSetZ(Sprite* sp, s32 z) {
    sp->zdepth = (s16)z;
}

void spMove(Sprite* sp, s32 x, s32 y) {
    sp->x = (s16)x;
    sp->y = (s16)y;
}

static s32 scissor_xmax;
static s32 scissor_ymax;

static s32 scissor_xmin;
static s32 scissor_ymin;

void spScissor(s32 xmin, s32 xmax, s32 ymin, s32 ymax) {
    scissor_xmin = xmin;
    scissor_ymin = ymin;

    scissor_xmax = xmax;
    scissor_ymax = ymax;
}

static int* prev_bmbuf = NULL;

static void drawbitmap(Gfx** glp, Sprite* s, Bitmap* b, s32 x, s32 y, s32 xx, s32 yy, s32 fs, s32 ft, s32 sx, s32 sy) {
    s32 rs, rt;
    s32 rxh, ryh;
    s32 rxl, ryl;
    Gfx* gl;
    s32 tex_width, tex_height;
    Gtexrect gt;
    Gtexrect* g;
    s32 s_clamp, t_clamp,
        s_mask, t_mask,
        s_lod, t_lod;

    g = &gt;

    tex_width = b->width_img;
    tex_height = s->bmHreal;

    gl = *glp;

    // Scissoring
    if ((x >= scissor_xmax) || (y >= scissor_ymax)) {
        return;
    }

    if ((xx < scissor_xmin) || (yy < scissor_ymin)) {
        return;
    }

    if (x < scissor_xmin) {
        rxh = scissor_xmin * 4;
        rs = (b->s << 5) + fs + (((scissor_xmin - x) * sx) >> 5);
    }
    else {
        rxh = x * 4;
        rs = (b->s << 5) + fs;
    };

    if (y < scissor_ymin) {
        ryh = scissor_ymin * 4;
        rt = (b->t << 5) + ft + (((scissor_ymin - y) * sy) >> 5);
    }
    else {
        ryh = y * 4;
        rt = (b->t << 5) + ft;
    }

    if (xx >= scissor_xmax) {
        rxl = scissor_xmax * 4;
    }
    else {
        rxl = xx * 4;
    };

    if (yy >= scissor_ymax) {
        ryl = scissor_ymax * 4;
    }
    else {
        ryl = yy * 4;
    };

    s_clamp = G_TX_CLAMP;
    t_clamp = G_TX_CLAMP;
    s_mask = G_TX_NOMASK;
    t_mask = G_TX_NOMASK;
    s_lod = G_TX_NOLOD;
    t_lod = G_TX_NOLOD;

    if (b->buf != prev_bmbuf) {
        switch (s->bmsiz) {
            case G_IM_SIZ_4b:
                if (s->attr & SP_TEXSHUF) {
                    gDPLoadTextureBlock_4bS(gl++, b->buf, s->bmfmt,
                                            tex_width, tex_height, 0,
                                            s_clamp, t_clamp,
                                            s_mask, t_mask,
                                            s_lod, t_lod);
                }
                else {
                    gDPLoadTextureBlock_4b(gl++, b->buf, s->bmfmt,
                                           tex_width, tex_height, 0,
                                           s_clamp, t_clamp,
                                           s_mask, t_mask,
                                           s_lod, t_lod);
                };
                break;

            case G_IM_SIZ_8b:
                if (s->attr & SP_TEXSHUF) {
                    gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_8b,
                                         tex_width, tex_height, 0,
                                         s_clamp, t_clamp,
                                         s_mask, t_mask,
                                         s_lod, t_lod);
                }
                else {
                    gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_8b,
                                        tex_width, tex_height, 0,
                                        s_clamp, t_clamp,
                                        s_mask, t_mask,
                                        s_lod, t_lod);
                };
                break;
            case G_IM_SIZ_16b:
                if (s->bmfmt == G_IM_FMT_YUV) {
                    if (s->attr & SP_TEXSHUF) {
                        gDPLoadTextureBlockYuvS(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
                                                tex_width, tex_height, 0,
                                                s_clamp, t_clamp,
                                                s_mask, t_mask,
                                                s_lod, t_lod);
                    }
                    else {
                        if (b->LUToffset != 0) { // Split Y and UV areas
                            unsigned char* uv;
                            unsigned char* addr;
                            int j;
                            s32 tmem, siz;

                            gDPSetTextureImage(gl++, G_IM_FMT_I, G_IM_SIZ_8b, 1, b->buf);
                            gDPSetTile(gl++, G_IM_FMT_I, G_IM_SIZ_8b, 0, 256, G_TX_LOADTILE, 0,
                                       t_clamp, t_mask, t_lod,
                                       s_clamp, s_mask, s_lod);
                            gDPLoadSync(gl++);
                            gDPLoadBlock(gl++, G_TX_LOADTILE, 0, 0, tex_width * tex_height - 1,
                                         CALC_DXT(tex_width, 1));
                            gDPLoadSync(gl++);

                            uv = ((unsigned char*)b->buf) +
                                 ((tex_width * tex_height) / 2) * b->LUToffset;

                            for (j = 0; j <= (tex_height / 2); j++) {
                                addr = uv + ((tex_height / 2) - 2 - j) * tex_width;
                                siz = 3 * tex_width;
                                tmem = (tex_width / 8) * (tex_height - 2 - 2 * j);
                                if (tmem < 0) {
                                    addr += (-tmem) * 8;
                                    siz -= (-tmem) * 8;
                                    tmem = 0;
                                };
                                if (j == 0) {
                                    siz = 2 * tex_width;
                                }

                                gDPSetTextureImage(gl++, G_IM_FMT_I, G_IM_SIZ_8b, 1, addr);
                                gDPSetTile(gl++, G_IM_FMT_I, G_IM_SIZ_8b, 0, tmem,
                                           G_TX_LOADTILE, 0,
                                           t_clamp, t_mask, t_lod,
                                           s_clamp, s_mask, s_lod);
                                gDPLoadSync(gl++);
                                gDPLoadBlock(gl++, G_TX_LOADTILE, 0, 0, siz - 1,
                                             CALC_DXT(tex_width, 1));
                                gDPLoadSync(gl++);
                            };

                            gDPSetTile(gl++, s->bmfmt, G_IM_SIZ_16b, (((tex_width) * 1) + 7) >> 3, 0,
                                       G_TX_RENDERTILE, 0,
                                       t_clamp, t_mask, t_lod,
                                       s_clamp, s_mask, s_lod);
                            gDPSetTileSize(gl++, G_TX_RENDERTILE, 0, 0,
                                           ((tex_width)-1) << G_TEXTURE_IMAGE_FRAC,
                                           ((tex_height)-1) << G_TEXTURE_IMAGE_FRAC);
                        }
                        else {
                            gDPLoadTextureBlockYuv(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
                                                   tex_width, tex_height, 0,
                                                   s_clamp, t_clamp,
                                                   s_mask, t_mask,
                                                   s_lod, t_lod);
                        };
                    };
                }
                else {
                    if (s->attr & SP_TEXSHUF) {
                        gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
                                             tex_width, tex_height, 0,
                                             s_clamp, t_clamp,
                                             s_mask, t_mask,
                                             s_lod, t_lod);
                    }
                    else {
                        gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
                                            tex_width, tex_height, 0,
                                            s_clamp, t_clamp,
                                            s_mask, t_mask,
                                            s_lod, t_lod);
                    };
                };
                break;
            case G_IM_SIZ_32b:
                if (s->attr & SP_TEXSHUF) {
                    gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_32b,
                                         tex_width, tex_height, 0,
                                         s_clamp, t_clamp,
                                         s_mask, t_mask,
                                         s_lod, t_lod);
                }
                else {
                    gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_32b,
                                        tex_width, tex_height, 0,
                                        s_clamp, t_clamp,
                                        s_mask, t_mask,
                                        s_lod, t_lod);
                };
                break;
        }
        prev_bmbuf = b->buf;
    }

    g->cmd = G_TEXRECT;
    g->tile = 0;

    g->xh = rxh;
    g->xl = rxl;
    g->yh = ryh;
    g->yl = ryl;

    g->s = rs;
    g->t = rt;

    if ((s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV)) {
        g->dsdx = sx * 4;
    }
    else {
        g->dsdx = sx;
    }

    g->dtdy = sy;

    gSPTextureRectangle(gl++, g->xh, g->yh, g->xl, g->yl, g->tile, g->s, g->t, g->dsdx, g->dtdy);

    gDPPipeSync(gl++);

    *glp = gl;
}

static u16 sp_attr = 0;

void spInit(Gfx** glistp) {
    Gfx* gl;

    gl = *glistp;

    sp_attr = 0;

    scissor_xmin = 0;
    scissor_ymin = 0;

    scissor_xmax = 320;
    scissor_ymax = 240;

    // Turn on texturing
    gDPPipeSync(gl++);
    gDPSetCycleType(gl++, G_CYC_1CYCLE);
    gSPTexture(gl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
    gDPSetAlphaCompare(gl++, G_AC_NONE);
    gDPSetTexturePersp(gl++, G_TP_NONE);
    gDPSetTextureFilter(gl++, G_TF_BILERP);
    gDPSetTextureConvert(gl++, G_TC_FILT);
    gDPSetTextureDetail(gl++, G_TD_CLAMP);
    gDPSetTextureLOD(gl++, G_TL_TILE);
    gDPSetTextureLUT(gl++, G_TT_NONE);

#define SPRITE_SURF G_RM_TEX_EDGE
#define SPRITE_SURF2 G_RM_TEX_EDGE2

    gDPSetRenderMode(gl++, SPRITE_SURF, SPRITE_SURF2);

    *glistp = gl;
}
Gfx* spDraw(Sprite* s) {
    int i;
    s32 x, y;
    float sx, sy;
    Bitmap* b;
    Gfx* gl;
    Gfx* ogl;
    Gfx* dl_start;
    s32 isx, isy;
    s32 tx, ty;
    s32 tx2, ty2;
    s32 x2, y2;
    float ftx, fty;
    s32 fs, ft;
    s32 ex, ey;

    if (s->attr & SP_HIDDEN) {
        return (NULL);
    }

    prev_bmbuf = NULL;

    gl = s->rsp_dl_next;
    if (gl == NULL) {
        gl = s->rsp_dl;
    }
    dl_start = gl;

    ogl = gl;

    b = s->bitmap;
    ex = 0;
    ey = 0;

    if (sp_attr & SP_EXTERN) { // Previous attr was external
        sp_attr = ~s->attr;    // Force all modes to refresh
    }

    if (s->attr & SP_EXTERN) { // Current attr is external
        sp_attr = s->attr;     // Assume settings should remain unchanged
    }

    if (s->attr != sp_attr) {
        if ((s->attr & SP_TRANSPARENT) && !(sp_attr & SP_TRANSPARENT)) {
            gDPSetRenderMode(gl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        }
        else if (!(s->attr & SP_TRANSPARENT) &&
                 (sp_attr & SP_TRANSPARENT)) {
            gDPSetRenderMode(gl++,
                             SPRITE_SURF, SPRITE_SURF2);
        }

        if ((s->attr & SP_CUTOUT) && !(sp_attr & SP_CUTOUT)) {
            gDPSetBlendColor(gl++, 0xff, 0xff, 0xff, 0x01);
            gDPSetAlphaCompare(gl++, G_AC_THRESHOLD);
        }
        else if (!(s->attr & SP_CUTOUT) && (sp_attr & SP_CUTOUT)) {
            gDPSetAlphaCompare(gl++, G_AC_NONE);
        }

        if ((s->attr & SP_TEXSHIFT) && !(sp_attr & SP_TEXSHIFT)) {
            gDPSetTextureFilter(gl++, G_TF_AVERAGE);
        }
        else if (!(s->attr & SP_TEXSHIFT) && (sp_attr & SP_TEXSHIFT)) {
            gDPSetTextureFilter(gl++, G_TF_BILERP);
        };
    };

    sp_attr = s->attr;

    if (sp_attr & SP_Z) {
        gDPSetPrimDepth(gl++, s->zdepth, 0);
    };

    gDPSetPrimColor(gl++, 0, 0, s->red, s->green, s->blue, s->alpha);

    if (s->bmfmt == G_IM_FMT_IA || s->bmfmt == G_IM_FMT_I) {
        if (s->alpha == 255) {
            gDPSetCombineMode(gl++,
                              G_CC_MODULATEIDECALA_PRIM,
                              G_CC_MODULATEIDECALA_PRIM);
        }
        else {
            gDPSetCombineMode(gl++,
                              G_CC_MODULATEIA_PRIM,
                              G_CC_MODULATEIA_PRIM);
        };
    }
    else {
        if (s->alpha == 255) {
            gDPSetCombineMode(gl++,
                              G_CC_MODULATEIDECALA_PRIM,
                              G_CC_MODULATEIDECALA_PRIM);
        }
        else {
            gDPSetCombineMode(gl++,
                              G_CC_MODULATERGBA_PRIM,
                              G_CC_MODULATERGBA_PRIM);
        };
    }

    if (s->bmfmt == G_IM_FMT_CI) {
        gDPSetTextureLUT(gl++, G_TT_RGBA16);
        gDPLoadTLUT(gl++, s->nTLUT, 256 + s->startTLUT, s->LUT);
        gDPLoadSync(gl++);
    };

#define MY_K0 (175 & 0x1ff)
#define MY_K1 (-43 & 0x1ff)
#define MY_K2 (-89 & 0x1ff)
#define MY_K3 (222 & 0x1ff)
#define MY_K4 (114 & 0x1ff)
#define MY_K5 (42 & 0x1ff)

    if ((s->bmfmt == G_IM_FMT_YUV) && (s->attr & SP_FASTCOPY)) {
        gDPSetTextureConvert(gl++, G_TC_CONV);
        gDPSetTextureFilter(gl++, G_TF_POINT);
        gDPSetCombineMode(gl++, G_CC_1CYUV2RGB, G_CC_1CYUV2RGB);
        gDPSetConvert(gl++, MY_K0, MY_K1, MY_K2, MY_K3, MY_K4, MY_K5);
    }
    else if (s->bmfmt == G_IM_FMT_YUV) {
        gDPSetCycleType(gl++, G_CYC_2CYCLE);
        gDPSetTextureFilter(gl++, G_TF_BILERP);
        gDPSetTextureConvert(gl++, G_TC_FILTCONV);
        gDPSetConvert(gl++, MY_K0, MY_K1, MY_K2, MY_K3, MY_K4, MY_K5);
        gDPSetCombineMode(gl++, G_CC_YUV2RGB, G_CC_PASS2);
    }

    if (s->attr & SP_SCALE) {
        sx = s->scalex;
        sy = s->scaley;
    }
    else {
        sx = 1.0F;
        sy = 1.0F;
    }

    isx = (int)((1 << 10) / sx + 0.5F);
    isy = (int)((1 << 10) / sy + 0.5F);

    if (b) {
        if ((s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV)) {
            gDPSetCycleType(gl++, G_CYC_COPY);
        }

        x = 0.0F;
        y = 0.0F;

        fty = s->y + y * sy;
        ty = (int)(fty + 0.99999F);
        ft = (int)(isy * (ty - fty));
        ft = (ft + 16) >> 5;
        if (s->attr & SP_TEXSHIFT) {
            ft += 16; // Half-texel antialiasing offset
        }

        if (s->attr & SP_FRACPOS) {
            ft += s->frac_t; // Micro-positioning
        }

        y2 = y + s->bmheight;
        ty2 = (int)(s->y + y2 * sy + 0.99999F);

        if ((s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV)) {
            ty2--;
        }

        for (i = 0; (i < s->nbitmaps) && (b->width > 0); i++, b++) {
            if ((x + b->width) > s->width) { // Next bitmap hits edge
                int bh;

                x = 0;
                fs = 0;
                ex = 0;
                ey += s->expy;
                y += s->bmheight;     // Wrap to next line
                y2 = y + s->bmheight; // Wrap to next line

                fty = s->y + y * sy;
                ty = (int)(fty + 0.9999F);
                ft = (int)(isy * (ty - fty));
                ft = (ft + 16) >> 5;
                if (s->attr & SP_TEXSHIFT) {
                    ft += 16; // Half-texel antialiasing offset
                }

                if (s->attr & SP_FRACPOS) {
                    ft += s->frac_t; // Micro-positioning
                }

                ty2 = (int)(s->y + y2 * sy + 0.9999F);

                ty += ey;
                ty2 += ey;

                if ((s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV)) {
                    ty2--;
                }

                // Fill out to the rectangle boundary

                if ((b->actualHeight != 0)) {
                    bh = b->actualHeight;
                }
                else {
                    bh = s->bmheight;
                }

                if ((y + bh) > s->height) { // Cannot wrap any farther
                    break;
                }
            };

            ftx = s->x + x * sx;
            tx = (s32)(ftx + 0.9999F);
            fs = (s32)(isx * (tx - ftx));
            fs = (fs + 16) >> 5;
            if (s->attr & SP_TEXSHIFT) {
                fs += 16; // Half-texel antialiasing offset
            }

            if (s->attr & SP_FRACPOS) {
                fs += s->frac_s; // Micro-positioning
            }

            x2 = x + b->width;
            tx2 = (int)(s->x + x2 * sx + 0.9999F);

            if ((b->actualHeight != 0)) {
                y2 = y + b->actualHeight;
                ty2 = (s32)(s->y + y2 * sy + 0.9999F);
                ty2 += ey;

                if ((s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV)) {
                    ty2--;
                }
            };

            tx += ex;
            tx2 += ex;

            if ((s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV)) {
                tx2--;
            }

            if (b->buf != NULL) { // Skip blank bitmap entries
                drawbitmap(&gl, s, b, tx, ty, tx2, ty2, fs, ft, isx, isy);
            }

            x += b->width;
            ex += s->expx;
        }

        if (s->attr & SP_FASTCOPY) {
            gDPSetCycleType(gl++, G_CYC_1CYCLE);
        }

        if (s->bmfmt == G_IM_FMT_YUV) {
            gDPSetCycleType(gl++, G_CYC_1CYCLE);
            gDPSetTextureFilter(gl++, G_TF_BILERP);
            gDPSetTextureConvert(gl++, G_TC_FILT);
        };
    }
    else {
        int rgba;

        x = (s32)s->x;
        y = (s32)s->y;
        x2 = s->x + (s->width * sx) - 1;
        y2 = s->y + (s->height * sy) - 1;

        if ((x >= scissor_xmax) || (y >= scissor_ymax)) {
        }
        else if ((x2 < scissor_xmin) || (y2 < scissor_ymin)) {
        }
        else {
            if (x < scissor_xmin) {
                x = scissor_xmin;
            }

            if (x2 >= scissor_xmax) {
                x2 = scissor_xmax - 1;
            }

            if (y < scissor_ymin) {
                y = scissor_ymin;
            }

            if (y2 >= scissor_ymax) {
                y2 = scissor_ymax - 1;
            }

            rgba = GPACK_RGBA5551((s->red), (s->green), (s->blue), (s->alpha >> 7));

            gSPTexture(gl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF);
            gDPSetCycleType(gl++, G_CYC_FILL);
            gDPSetFillColor(gl++, (rgba << 16) | (rgba));

            gDPFillRectangle(gl++, x, y, x2, y2);
            gDPSetCycleType(gl++, G_CYC_1CYCLE);
            gSPTexture(gl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
        }
    }

    if (s->bmfmt == G_IM_FMT_CI) {
        gDPSetTextureLUT(gl++, G_TT_NONE);
    };

    gSPEndDisplayList(gl++);

#line 714
    assert((gl - ogl) < s->ndisplist, "../sp/sprite.c", 715);
    s->rsp_dl_next = gl;
    return (dl_start);
}

void spFinish(Gfx** glistp) {
    Gfx* gl;

    gl = *glistp;

    // Turn off texturing
    gSPTexture(gl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetCombineMode(gl++, G_CC_SHADE, G_CC_SHADE);

    if (sp_attr & SP_TRANSPARENT) {
        // Turn off transparency
        gDPSetRenderMode(gl++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    }

    if (sp_attr & SP_CUTOUT) {
        // Turn off alpha write enable
        gDPSetAlphaCompare(gl++, G_AC_NONE);
    }

    gSPEndDisplayList(gl++);

    *glistp = gl;
}
