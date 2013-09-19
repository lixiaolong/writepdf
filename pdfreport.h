#ifndef __PDFREPORT_H__
#define __PDFREPORT_H__
#include "hpdf.h"

HPDF_Font g_font_sun, g_font_hei;

extern HPDF_RGBColor g_color_green;
extern HPDF_RGBColor g_color_yellow;
extern HPDF_RGBColor g_color_red;
extern HPDF_RGBColor g_color_black;
extern HPDF_RGBColor g_color_white;

extern HPDF_RGBColor g_color_lightbluex;
extern HPDF_RGBColor g_color_lightblue;

typedef enum _HPDF_CSS_TYPE {
    PDF_CSS_TYPE_BEGIN = 1000,
    PDF_CSS_TYPE_TITLE,
    PDF_CSS_TYPE_H1,
    PDF_CSS_TYPE_H2,
    PDF_CSS_TYPE_H3,
    PDF_CSS_TYPE_H4,
    PDF_CSS_TYPE_H5,
    PDF_CSS_TYPE_H6,
    PDF_CSS_TYPE_P,
    PDF_CSS_TYPE_TD,
    PDF_CSS_TYPE_TR,
    PDF_CSS_TYPE_PRE,
    PDF_CSS_TYPE_BLOCK,  //blockquote,
    PDF_CSS_TYPE_END
} HPDF_CSS_TYPE;

/*
typedef enum _HPDF_TextAlignment {
    HPDF_TALIGN_LEFT = 0,
    HPDF_TALIGN_RIGHT,
    HPDF_TALIGN_CENTER,
    HPDF_TALIGN_JUSTIFY
} HPDF_TextAlignment;
*/

typedef struct _HPDF_CSS {
    HPDF_UINT css_type;             /* 当前样式类型 */
    HPDF_REAL page_width;           /* 页面宽度 */
    HPDF_REAL page_height;          /* 页面高度 */
    HPDF_REAL width_ratio;          /* 宽度比率 */
    HPDF_REAL text_leading;         /* 行距 */
    HPDF_REAL line_width;           /* 边框线的宽度 */
    HPDF_Font font;                 /* 字体 */
    HPDF_UINT font_size;            /* 字体大小 */
    HPDF_RGBColor font_color;       /* 字体颜色 */
    HPDF_RGBColor bg_color;         /* 背景颜色 */
    HPDF_RGBColor line_color;       /* 边框颜色 */
    HPDF_Rect side_margin;          /* 页面边距 */
    HPDF_Rect seg_margin;           /* 段落边距 */
    HPDF_Rect cur_text_pos;         /* 当前文本位置 */
    HPDF_Rect cur_frame_pos;        /* 当前框体位置 */
    HPDF_BOOL withFrame;            /* 是否输出边框,1:输出，0:不输出 */
    HPDF_UINT textAlign;            /* 0:文本居左，1:居右，2:居中，3:自动 */
    HPDF_REAL text_height;          /* 内容占用的高度最大值 */
    HPDF_BOOL text_indent;          /* 是否首行缩进 */
    
    HPDF_UINT title_num_h1;         /* 标题h1的序号 */
    HPDF_UINT title_num_h2;         /* 标题h2的序号 */
    HPDF_UINT title_num_h3;         /* 标题h3的序号 */
    HPDF_UINT title_num_h4;         /* 标题h4的序号 */
    HPDF_UINT title_num_h5;         /* 标题h5的序号 */
    HPDF_UINT title_num_h6;         /* 标题h6的序号 */
} HPDF_CSS;

typedef struct _HPDF_HANDLER {
    HPDF_Doc  pdf;
    HPDF_Page page;
    HPDF_CSS  css;
    HPDF_Outline root;
    HPDF_Outline title;
    HPDF_Outline sub_title;
    HPDF_Destination dst;
}*HPDF_HANDLER;




int HPDF_CSS_Init(HPDF_CSS *hcss);

int HPDF_CSS_SET(int type, HPDF_CSS *css);

float  HPDF_CSS_GetTextHeight(HPDF_HANDLER pdf, char *text);

HPDF_HANDLER HPDF_HANDLER_New();

void HPDF_HANDLER_Free(HPDF_HANDLER hpdf);

int HPDF_HANDLER_PrintEx(HPDF_HANDLER hpdf, char *text);

int HPDF_HANDLER_Print(HPDF_HANDLER hpdf, char *text);

int HPDF_HANDLER_PrintImage(HPDF_HANDLER hpdf, char *imageFile);

int HPDF_HANDLER_PrintLine(HPDF_HANDLER hpdf);

#endif /* __PDFREPORT_H__ */
