#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include "hpdf.h"

HPDF_Font g_font_sun, g_font_hei;
HPDF_RGBColor g_color_green = {0, 0.8, 0.2};
HPDF_RGBColor g_color_yellow = {1.0, 0.6, 0};
HPDF_RGBColor g_color_red = {1.0, 0, 0};
HPDF_RGBColor g_color_black = {0, 0, 0};
HPDF_RGBColor g_color_white = {1, 1, 1};

HPDF_RGBColor g_color_lightbluex = {0.9, 1.0, 1.0};
HPDF_RGBColor g_color_lightblue = {0.6, 0.8, 1.0};

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
} HPDF_CSS;

typedef struct _HPDF_HANDLE {
    HPDF_Doc  pdf;
    HPDF_Page page;
    HPDF_CSS  css;
}*HPDF_HANDLE;




jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}


int HPDF_CSS_Init(HPDF_CSS *hcss) {
    hcss->width_ratio = 1;
    hcss->text_leading = 20;
    hcss->font = g_font_sun;
    hcss->font_size = 14;
    
    hcss->side_margin.left = 40;
    hcss->side_margin.right = 40;
    hcss->side_margin.top = 40;
    hcss->side_margin.bottom = 40;

    hcss->seg_margin.left = 2;
    hcss->seg_margin.right = 2;
    hcss->seg_margin.top = 2;
    hcss->seg_margin.bottom = 10;

    hcss->cur_frame_pos.left = hcss->side_margin.left;
    hcss->cur_frame_pos.right = hcss->cur_frame_pos.left;
    hcss->cur_frame_pos.top = hcss->page_height - hcss->side_margin.top;
    hcss->cur_frame_pos.bottom = hcss->cur_frame_pos.top;

    hcss->withFrame = 0;
    hcss->textAlign = HPDF_TALIGN_LEFT;
    hcss->text_height = 0.0;
    hcss->line_width = 1.0;

    hcss->font_color = g_color_black;
    hcss->bg_color = g_color_white;
    hcss->line_color = g_color_lightblue;

    //printf("page height: %f, page_width: %f\n", hcss->page_height, hcss->page_width);
    return 0;
}

int HPDF_CSS_SET(int type, HPDF_CSS *css) {
    if(type <= PDF_CSS_TYPE_BEGIN || type >= PDF_CSS_TYPE_END)
    {
        printf("error type : %d\n", type);
        return 1;
    }

    switch (type) {
        case PDF_CSS_TYPE_TITLE:
        {
            css->css_type = PDF_CSS_TYPE_TITLE;
            css->width_ratio = 1;
            css->text_leading = 30;
            css->font = g_font_hei;
            css->font_size = 30;
            css->text_height = (css->page_height - css->side_margin.top - css->side_margin.bottom) / 2;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->text_height / 2;
            css->seg_margin.bottom = css->text_height / 4;
            css->withFrame = 0;
            css->textAlign = HPDF_TALIGN_CENTER;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_black;
            break;
        }

        case PDF_CSS_TYPE_H1:
        {
            css->css_type = PDF_CSS_TYPE_H1;
            css->width_ratio = 1;
            css->font = g_font_hei;
            css->font_size = 22;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 0;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_black;
            break;
        }
        case PDF_CSS_TYPE_H2:
        {
            css->css_type = PDF_CSS_TYPE_H2;
            css->width_ratio = 1;
            css->font = g_font_hei;
            css->font_size = 18;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 0;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_black;
            break;
        }
        case PDF_CSS_TYPE_H3:
        {
            css->css_type = PDF_CSS_TYPE_H3;
            css->width_ratio = 1;
            css->font = g_font_hei;
            css->font_size = 13.5;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 0;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_black;
            break;
        }
        case PDF_CSS_TYPE_H4:
        {
            css->css_type = PDF_CSS_TYPE_H4;
            css->width_ratio = 1;
            css->font = g_font_hei;
            css->font_size = 12;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 0;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_black;
            break;
        }
        case PDF_CSS_TYPE_H5:
        {
            css->css_type = PDF_CSS_TYPE_H5;
            css->width_ratio = 1;
            css->font = g_font_hei;
            css->font_size = 10.5;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 0;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_black;
            break;
        }
        case PDF_CSS_TYPE_H6:
        {
            css->css_type = PDF_CSS_TYPE_H6;
            css->width_ratio = 1;
            css->font = g_font_hei;
            css->font_size = 10.5;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 0;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_black;
            break;
        }
        case PDF_CSS_TYPE_P:
        {
            css->css_type = PDF_CSS_TYPE_P;
            css->width_ratio = 1;
            css->font = g_font_sun;
            css->font_size = 12;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 0;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_black;
            break;
        }
        case PDF_CSS_TYPE_TR:
        {
            css->css_type = PDF_CSS_TYPE_TR;
            css->width_ratio = 1;
            css->font = g_font_sun;
            css->font_size = 12;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = 4;
            css->seg_margin.bottom = 4;
            css->withFrame = 1;
            css->textAlign = HPDF_TALIGN_CENTER;
            css->font_color = g_color_black;
            css->bg_color = g_color_white;
            css->line_color = g_color_lightblue;
            break;
        }
        case PDF_CSS_TYPE_TD:
        {
            css->css_type = PDF_CSS_TYPE_TD;
            css->width_ratio = 1;
            css->font = g_font_hei;
            css->font_size = 12;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = 4;
            css->seg_margin.bottom = 4;
            css->withFrame = 1;
            css->textAlign = HPDF_TALIGN_CENTER;
            css->font_color = g_color_black;
            css->bg_color = g_color_lightbluex;
            css->line_color = g_color_lightblue;
            break;
        }
        default:
            css->css_type = PDF_CSS_TYPE_BEGIN;
            break;
    }

    return 0;
}

// 内容的行数 = 回车符的个数 + 内容的总长度 / 当前行宽 + 1
float  HPDF_CSS_GetTextHeight(HPDF_HANDLE pdf, char *text) {
    HPDF_REAL text_width = 0, text_height = 0;
    HPDF_REAL text_width_per_line = 0;
    int text_height_count = 0, text_len = 0, enter_count = 0, i = 0;

    text_len = strlen(text);

    // 计算回车符的个数
    for( i = 0; i < text_len; i++)
    {
        if(text[i] == 0x0a)
            enter_count++;
    }

    text_width = HPDF_Page_TextWidth(pdf->page, text);
    
    //text_width = text_len * css->font_size / 2;

    // 当前可输入的行宽
    text_width_per_line = pdf->css.page_width - 
        pdf->css.side_margin.left - pdf->css.side_margin.right -
        pdf->css.seg_margin.left  - pdf->css.seg_margin.right;

    text_width_per_line = text_width_per_line * pdf->css.width_ratio;

    text_height_count = text_width / text_width_per_line + 1 + enter_count;

    text_height = text_height_count * pdf->css.text_leading + 
        pdf->css.seg_margin.top + pdf->css.seg_margin.bottom;

    printf("line count: %d, strlen: %d, text_width: %f, text_width_per_line: %f\n", 
        text_height_count, text_len, text_width, text_width_per_line);

    return text_height;
}


HPDF_HANDLE HPDF_HANDLE_New() {
    HPDF_HANDLE ret;
    ret = malloc(sizeof(struct _HPDF_HANDLE));
    memset(ret, 0, sizeof(struct _HPDF_HANDLE));
    
    ret->pdf = HPDF_New(error_handler, NULL);

    if (!ret->pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return NULL;
    }

    if (setjmp(env)) {
        HPDF_Free (ret->pdf);
        return NULL;
    }
    /* set compression mode */
    HPDF_SetCompressionMode (ret->pdf, HPDF_COMP_ALL);
 
    /* create default-font */
    HPDF_UseCNSFonts(ret->pdf);
    HPDF_UseCNSEncodings(ret->pdf);
    g_font_sun = HPDF_GetFont (ret->pdf, "SimSun", "GBK-EUC-H");
    g_font_hei =  HPDF_GetFont (ret->pdf, "SimHei", "GBK-EUC-H");

    /* add a new page object. */
    ret->page = HPDF_AddPage (ret->pdf);
    HPDF_Page_SetSize(ret->page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    ret->css.page_height = HPDF_Page_GetHeight(ret->page);
    ret->css.page_width = HPDF_Page_GetWidth(ret->page);
    HPDF_CSS_Init(&(ret->css));

    return ret;
}

void HPDF_HANDLE_Free(HPDF_HANDLE hpdf) {
    if(hpdf)
    {
        HPDF_Free (hpdf->pdf);
        free(hpdf);
    }
    return;
}



int HPDF_HANDLE_Print(HPDF_HANDLE hpdf, char *text) {
    HPDF_REAL text_height = 0;
    HPDF_UINT outlen = 0;
    HPDF_STATUS ret;

    HPDF_Page_BeginText(hpdf->page);

    // font and font_size
    HPDF_Page_SetFontAndSize(hpdf->page, hpdf->css.font, hpdf->css.font_size);

    // text_leading
    HPDF_Page_SetTextLeading (hpdf->page, hpdf->css.text_leading);

    printf("------------------------%f\n", hpdf->css.text_height);

    // text_height
    if(hpdf->css.text_height > 0) {
        text_height = hpdf->css.text_height;
    } else {
        text_height = HPDF_CSS_GetTextHeight(hpdf, text);
    }

    // calc current position
    // ====================================================
    printf("===begin: height:%f left:%f right:%f top:%f bottom:%f\n", text_height, hpdf->css.cur_frame_pos.left, 
        hpdf->css.cur_frame_pos.right, hpdf->css.cur_frame_pos.top, hpdf->css.cur_frame_pos.bottom);

    hpdf->css.cur_frame_pos.left = hpdf->css.cur_frame_pos.right;
    hpdf->css.cur_frame_pos.right = hpdf->css.cur_frame_pos.left + hpdf->css.width_ratio *
        (hpdf->css.page_width - hpdf->css.side_margin.left - hpdf->css.side_margin.right);
    hpdf->css.cur_frame_pos.top = hpdf->css.cur_frame_pos.bottom;
    hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top - text_height;

    if(hpdf->css.cur_frame_pos.right > hpdf->css.page_width - hpdf->css.side_margin.right)
    {
        hpdf->css.cur_frame_pos.right = hpdf->css.page_width - hpdf->css.side_margin.right;
    }

    if(hpdf->css.cur_frame_pos.bottom < hpdf->css.side_margin.bottom)
    {
        printf("need new page\n");
        HPDF_Page_EndText(hpdf->page);
        
        hpdf->page = HPDF_AddPage (hpdf->pdf);
        HPDF_Page_SetSize(hpdf->page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
        
        hpdf->css.cur_frame_pos.left = hpdf->css.side_margin.left;
        hpdf->css.cur_frame_pos.right = hpdf->css.cur_frame_pos.left + hpdf->css.width_ratio *
            (hpdf->css.page_width - hpdf->css.side_margin.left - hpdf->css.side_margin.right);
        hpdf->css.cur_frame_pos.top = hpdf->css.page_height - hpdf->css.side_margin.top;
        hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top - text_height;

        HPDF_Page_BeginText(hpdf->page);

        // font and font_size
        HPDF_Page_SetFontAndSize(hpdf->page, hpdf->css.font, hpdf->css.font_size);

        // text_leading
        HPDF_Page_SetTextLeading (hpdf->page, hpdf->css.text_leading);
    }

    printf("===middle: left:%f right:%f top:%f bottom:%f\n", hpdf->css.cur_frame_pos.left, 
        hpdf->css.cur_frame_pos.right, hpdf->css.cur_frame_pos.top, hpdf->css.cur_frame_pos.bottom);

    // 输出边框，如果要输出背景色，必须先输背景色再输文字，所以这里先关闭文字，输出背景色，再开启文字输出
    if(hpdf->css.withFrame)
    {
        HPDF_Page_EndText(hpdf->page);
        
        HPDF_Page_SetLineWidth (hpdf->page, hpdf->css.line_width);
        HPDF_Page_SetRGBStroke (hpdf->page, hpdf->css.line_color.r, hpdf->css.line_color.g, hpdf->css.line_color.b);
        HPDF_Page_SetRGBFill (hpdf->page, hpdf->css.bg_color.r, hpdf->css.bg_color.g, hpdf->css.bg_color.b);
        HPDF_Page_Rectangle (hpdf->page, hpdf->css.cur_frame_pos.left, hpdf->css.cur_frame_pos.bottom, 
                    hpdf->css.cur_frame_pos.right - hpdf->css.cur_frame_pos.left,
                    hpdf->css.cur_frame_pos.top - hpdf->css.cur_frame_pos.bottom);
        //HPDF_Page_Stroke (hpdf->page);
        HPDF_Page_FillStroke(hpdf->page);

        HPDF_Page_BeginText(hpdf->page);

        // font and font_size
        HPDF_Page_SetFontAndSize(hpdf->page, hpdf->css.font, hpdf->css.font_size);

        // text_leading
        HPDF_Page_SetTextLeading (hpdf->page, hpdf->css.text_leading);

    }
    
    // ====================================================   


    // output string
    // ==================================================== 
    hpdf->css.cur_text_pos.left = hpdf->css.cur_frame_pos.left + hpdf->css.seg_margin.left;
    hpdf->css.cur_text_pos.right = hpdf->css.cur_frame_pos.right - hpdf->css.seg_margin.right;
    hpdf->css.cur_text_pos.top = hpdf->css.cur_frame_pos.top - hpdf->css.seg_margin.top;
    hpdf->css.cur_text_pos.bottom= hpdf->css.cur_frame_pos.bottom + hpdf->css.seg_margin.bottom;

    HPDF_Page_SetRGBFill (hpdf->page, hpdf->css.font_color.r, hpdf->css.font_color.g, hpdf->css.font_color.b);
    ret = HPDF_Page_TextRect(hpdf->page, hpdf->css.cur_text_pos.left, hpdf->css.cur_text_pos.top, 
        hpdf->css.cur_text_pos.right, hpdf->css.cur_text_pos.bottom, text, hpdf->css.textAlign, &outlen);
    
    HPDF_Page_EndText(hpdf->page);
    printf("output string ret:%x outlen:%d height:%f width:%f\n", ret, outlen, hpdf->css.page_height, hpdf->css.page_width);

    if(outlen <= 0)
    {
        // 输出失败，坐标回退到输出前的状态
        hpdf->css.cur_frame_pos.right = hpdf->css.cur_frame_pos.left;
        hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top;
    }
    else
    {   
        if((int)hpdf->css.cur_frame_pos.right < (int)(hpdf->css.page_width - hpdf->css.side_margin.right))
        {
            // 右边还有空间可以输入
            hpdf->css.cur_frame_pos.left = hpdf->css.cur_frame_pos.right;
            hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top;
        } 
        else if((int)hpdf->css.cur_frame_pos.right == (int)(hpdf->css.page_width - hpdf->css.side_margin.right))
        {
            // 当前行已经输满
            hpdf->css.cur_frame_pos.left = hpdf->css.side_margin.left;
            hpdf->css.cur_frame_pos.right = hpdf->css.side_margin.left;
            hpdf->css.cur_frame_pos.top = hpdf->css.cur_frame_pos.bottom;
        }
        else
        {
            printf("[ERROR]left:%f right:%f top:%f bottom:%f\n", hpdf->css.cur_frame_pos.left, 
                hpdf->css.cur_frame_pos.right, hpdf->css.cur_frame_pos.top, hpdf->css.cur_frame_pos.bottom);
        }
    }
    // ==================================================== 
    printf("===end: left:%f right:%f top:%f bottom:%f\n", hpdf->css.cur_frame_pos.left, 
        hpdf->css.cur_frame_pos.right, hpdf->css.cur_frame_pos.top, hpdf->css.cur_frame_pos.bottom);

    return outlen;
}

int main (int argc, char **argv)
{
    const char *fname = "1.pdf";
    HPDF_HANDLE hpdf;
    HPDF_CSS pdfcss;
    HPDF_STATUS ret;
    HPDF_UINT outlen = 0;
    


    hpdf = HPDF_HANDLE_New();
    if(!hpdf)
    {
        printf("hpdf handle new failed!\n");
        return 1;
    }

    HPDF_CSS_SET(PDF_CSS_TYPE_TITLE, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "主机漏洞扫描报表");

    hpdf->css.font_size = 18;
    hpdf->css.font = g_font_sun;
    HPDF_HANDLE_Print(hpdf, "报表创建人:system\n报表生成时间:2013-9-10");
    
    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "1 扫描概要信息");
    
    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);    
    HPDF_HANDLE_Print(hpdf, "    DPtech Scanner从 2013-08-10 14:50:00 开始对您网络的IP段 10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24; 进行扫描，此次扫描总共扫描了 73 台主机，其中有 59 台在线，此次扫描于 2013-08-10 15:15:02 结束。");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.3;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "扫描任务名称");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.7;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    hpdf->css.font_color = g_color_black;
    HPDF_HANDLE_Print(hpdf, "global");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.3;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "扫描对象");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.7;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, " ");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.7;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    hpdf->css.text_height = HPDF_CSS_GetTextHeight(hpdf, "10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;");
    hpdf->css.width_ratio = 0.3;
    HPDF_HANDLE_Print(hpdf, "扫描范围");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.7;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;10.121.11.1/24;");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.3;
    hpdf->css.text_height = 0;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "扫描开始时间");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.7;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "2013-08-10 14:50:00");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.3;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "扫描结束时间");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.7;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "2013-08-10 14:50:00");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.3;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "扫描模版");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.7;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    HPDF_HANDLE_Print(hpdf, "默认漏洞扫描策略");

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "2 网络安全概要信息");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    DPtech Scanner本次共扫描了 73 台主机，其中有 59 台在线，其中风险等级为 比较\
危险的主机数 10 台，其中风险等级为 中度危险 的主机数 3 台，其中风险等级为 低度\
危险 的主机数 8 台，其中风险等级为 比较安全 的主机数 38 台，网络风险等级为 中度\
危险。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "3 在线主机概要信息");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.13;
    HPDF_HANDLE_Print(hpdf, "IP地址");

    printf("====================\n");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.13;
    HPDF_HANDLE_Print(hpdf, "主机名称");

    printf("====================\n");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.14;
    HPDF_HANDLE_Print(hpdf, "操作系统");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "风险等级");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "风险分值");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "漏洞总数");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "高风险");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "中风险");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "低风险");

    printf("====================\n");    

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.13;
    hpdf->css.text_height = 3 * hpdf->css.text_leading;
    HPDF_HANDLE_Print(hpdf, "192.168.0.1");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.13;
    HPDF_HANDLE_Print(hpdf, "localhost");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.14;
    HPDF_HANDLE_Print(hpdf, "Linux 2.6.8 - 2.6.32");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    hpdf->css.font_color = g_color_red;
    HPDF_HANDLE_Print(hpdf, "非常危险");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "206.7");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "39");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "5");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "3");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "31");

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "4 漏洞概要信息");

    HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "4.1 漏洞数量概要信息");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    DPtech Scanner本次扫描发现您的网络共有 951 个漏洞，其中高风险漏洞有 62 个，\
中风险漏洞有 44 个，低风险漏洞有 845 个。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "4.2 漏洞类别分布");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.13;
    HPDF_HANDLE_Print(hpdf, "IP地址");

    printf("====================\n");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.13;
    HPDF_HANDLE_Print(hpdf, "主机名称");

    printf("====================\n");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.14;
    HPDF_HANDLE_Print(hpdf, "操作系统");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "风险等级");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "风险分值");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "漏洞总数");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    hpdf->css.font_color = g_color_red;
    HPDF_HANDLE_Print(hpdf, "高风险");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    hpdf->css.font_color = g_color_yellow;
    HPDF_HANDLE_Print(hpdf, "中风险");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    hpdf->css.font_color = g_color_green;
    HPDF_HANDLE_Print(hpdf, "低风险");

    printf("====================\n");    

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.13;
    hpdf->css.text_height = 3 * hpdf->css.text_leading;
    HPDF_HANDLE_Print(hpdf, "192.168.0.1");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.13;
    HPDF_HANDLE_Print(hpdf, "localhost");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.14;
    HPDF_HANDLE_Print(hpdf, "Linux 2.6.8 - 2.6.32");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    hpdf->css.font_color = g_color_red;
    HPDF_HANDLE_Print(hpdf, "非常危险");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "206.7");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.12;
    HPDF_HANDLE_Print(hpdf, "39");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "5");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "3");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.08;
    HPDF_HANDLE_Print(hpdf, "31");

    /* save the document to a file */
    HPDF_SaveToFile (hpdf->pdf, fname);

    /* clean up */
    HPDF_HANDLE_Free(hpdf);

    return 0;

}
