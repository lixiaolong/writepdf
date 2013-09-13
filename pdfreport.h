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
    HPDF_UINT css_type;             /* ��ǰ��ʽ���� */
    HPDF_REAL page_width;           /* ҳ���� */
    HPDF_REAL page_height;          /* ҳ��߶� */
    HPDF_REAL width_ratio;          /* ��ȱ��� */
    HPDF_REAL text_leading;         /* �о� */
    HPDF_REAL line_width;           /* �߿��ߵĿ�� */
    HPDF_Font font;                 /* ���� */
    HPDF_UINT font_size;            /* �����С */
    HPDF_RGBColor font_color;       /* ������ɫ */
    HPDF_RGBColor bg_color;         /* ������ɫ */
    HPDF_RGBColor line_color;       /* �߿���ɫ */
    HPDF_Rect side_margin;          /* ҳ��߾� */
    HPDF_Rect seg_margin;           /* ����߾� */
    HPDF_Rect cur_text_pos;         /* ��ǰ�ı�λ�� */
    HPDF_Rect cur_frame_pos;        /* ��ǰ����λ�� */
    HPDF_BOOL withFrame;            /* �Ƿ�����߿�,1:�����0:����� */
    HPDF_UINT textAlign;            /* 0:�ı�����1:���ң�2:���У�3:�Զ� */
    HPDF_REAL text_height;          /* ����ռ�õĸ߶����ֵ */
    HPDF_BOOL text_indent;          /* �Ƿ��������� */
    
    HPDF_UINT title_num_h1;         /* ����h1����� */
    HPDF_UINT title_num_h2;         /* ����h2����� */
    HPDF_UINT title_num_h3;         /* ����h3����� */
    HPDF_UINT title_num_h4;         /* ����h4����� */
    HPDF_UINT title_num_h5;         /* ����h5����� */
    HPDF_UINT title_num_h6;         /* ����h6����� */
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


#endif /* __PDFREPORT_H__ */
