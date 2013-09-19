#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include "pdfreport.h"

jmp_buf env;


HPDF_RGBColor g_color_green = {0, 0.8, 0.2};
HPDF_RGBColor g_color_yellow = {1.0, 0.6, 0};
HPDF_RGBColor g_color_red = {1.0, 0, 0};
HPDF_RGBColor g_color_black = {0, 0, 0};
HPDF_RGBColor g_color_white = {1, 1, 1};

HPDF_RGBColor g_color_lightbluex = {0.9, 1.0, 1.0};
HPDF_RGBColor g_color_lightblue = {0.6, 0.8, 1.0};
HPDF_RGBColor g_color_gray = {0.8, 0.8, 0.8};
HPDF_RGBColor g_color_lightgray = {0.9, 0.9, 0.9};
HPDF_RGBColor g_color_darkgray = {0.2, 0.2, 0.2};

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
    hcss->text_indent = 1;

    hcss->font_color = g_color_black;
    hcss->bg_color = g_color_white;
    hcss->line_color = g_color_lightblue;

    hcss->title_num_h1 = 0;
    hcss->title_num_h2 = 0;
    hcss->title_num_h3 = 0;
    hcss->title_num_h4 = 0;
    hcss->title_num_h5 = 0;
    hcss->title_num_h6 = 0;

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
            css->text_indent = 0;
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
            css->text_indent = 0;
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
            css->text_indent = 0;
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
            css->text_indent = 0;
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
            css->text_indent = 0;
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
            css->text_indent = 0;
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
            css->text_indent = 0;
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
            css->text_indent = 1;
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
            css->text_indent = 0;
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
        case PDF_CSS_TYPE_PRE:
        {
            css->css_type = PDF_CSS_TYPE_PRE;
            css->width_ratio = 1;
            css->font = g_font_sun;
            css->font_size = 12;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = css->font_size * 1;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 1;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_black;
            css->bg_color = g_color_lightgray;
            css->line_color = g_color_gray;
            css->text_indent = 0;
            break;
        }
        case PDF_CSS_TYPE_BLOCK:
        {
            css->css_type = PDF_CSS_TYPE_BLOCK;
            css->width_ratio = 1;
            css->font = g_font_sun;
            css->font_size = 12;
            css->text_leading = css->font_size * 1.5;
            css->seg_margin.left = css->font_size * 1.5;
            css->seg_margin.right = 2;
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 1;
            css->text_height = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
            css->font_color = g_color_darkgray;
            css->bg_color = g_color_lightgray;
            css->line_color = g_color_gray;
            css->text_indent = 0;
            break;
        }
        default:
            css->css_type = PDF_CSS_TYPE_BEGIN;
            break;
    }

    return 0;
}

// 内容的行数 = 回车符的个数 + 内容的总长度 / 当前行宽 + 1
float  HPDF_CSS_GetTextHeight(HPDF_HANDLER pdf, char *text) {
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


HPDF_HANDLER HPDF_HANDLER_New() {
    HPDF_HANDLER ret;
    ret = malloc(sizeof(struct _HPDF_HANDLER));
    memset(ret, 0, sizeof(struct _HPDF_HANDLER));
    
    ret->pdf = HPDF_New(error_handler, NULL);

    if (!ret->pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return NULL;
    }

    if (setjmp(env)) {
        HPDF_HANDLER_Free (ret);
        return NULL;
    }
    /* set compression mode */
    HPDF_SetCompressionMode (ret->pdf, HPDF_COMP_ALL);
 
    /* create default-font */
    HPDF_UseCNSFonts(ret->pdf);
    HPDF_UseCNSEncodings(ret->pdf);
    
    /* Set page mode to use outlines. */
    HPDF_SetPageMode(ret->pdf, HPDF_PAGE_MODE_USE_OUTLINE);
    
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

void HPDF_HANDLER_Free(HPDF_HANDLER hpdf) {
    if(hpdf)
    {
        HPDF_Free (hpdf->pdf);
        free(hpdf);
    }
    return;
}

int HPDF_HANDLER_PrintEx(HPDF_HANDLER hpdf, char *text) {
    char *output = NULL;
    int output_len = 0;

    output_len = strlen(text) + 128;
    switch (hpdf->css.css_type) {
        case PDF_CSS_TYPE_TITLE:
        {
            
            break;
        }
        case PDF_CSS_TYPE_H1:
        case PDF_CSS_TYPE_H2:
        case PDF_CSS_TYPE_H3:
        case PDF_CSS_TYPE_H4:
        case PDF_CSS_TYPE_H5:
        case PDF_CSS_TYPE_H6:
        {
            output = malloc(output_len);
            if( PDF_CSS_TYPE_H1 == hpdf->css.css_type ) {
                /* calc the title number */
                hpdf->css.title_num_h1++;
                hpdf->css.title_num_h2 = 0;
                hpdf->css.title_num_h3 = 0;
                hpdf->css.title_num_h4 = 0;
                hpdf->css.title_num_h5 = 0;
                hpdf->css.title_num_h6 = 0;
                hpdf->css.withFrame = 2;

                snprintf(output, output_len, "%d %s", hpdf->css.title_num_h1, text);
            } else if(PDF_CSS_TYPE_H2 == hpdf->css.css_type ) {
                /* calc the title number */
                if(hpdf->css.title_num_h1 == 0) {
                    hpdf->css.title_num_h1++;
                }
                hpdf->css.title_num_h2++;
                hpdf->css.title_num_h3 = 0;
                hpdf->css.title_num_h4 = 0;
                hpdf->css.title_num_h5 = 0;
                hpdf->css.title_num_h6 = 0;

                snprintf(output, output_len, "%d.%d %s", hpdf->css.title_num_h1,
                    hpdf->css.title_num_h2, text);
            } else if(PDF_CSS_TYPE_H3 == hpdf->css.css_type ) {
                /* calc the title number */
                if(hpdf->css.title_num_h1 == 0) {
                    hpdf->css.title_num_h1++;
                }
                if(hpdf->css.title_num_h2 == 0) {
                    hpdf->css.title_num_h2++;
                }
                hpdf->css.title_num_h3++;
                hpdf->css.title_num_h4 = 0;
                hpdf->css.title_num_h5 = 0;
                hpdf->css.title_num_h6 = 0;

                snprintf(output, output_len, "%d.%d.%d %s", hpdf->css.title_num_h1,
                    hpdf->css.title_num_h2, hpdf->css.title_num_h3, text);
            } else if(PDF_CSS_TYPE_H4 == hpdf->css.css_type ) {
                /* calc the title number */
                hpdf->css.title_num_h4++;
                hpdf->css.title_num_h5 = 0;
                hpdf->css.title_num_h6 = 0;

                snprintf(output, output_len, "(%d) %s", hpdf->css.title_num_h4, text);
            } else if(PDF_CSS_TYPE_H5 == hpdf->css.css_type ) {
                /* calc the title number */
                hpdf->css.title_num_h5++;
                hpdf->css.title_num_h6 = 0;
                
                snprintf(output, output_len, "[%d] %s", hpdf->css.title_num_h5, text);
            } else if(PDF_CSS_TYPE_H6 == hpdf->css.css_type ) {
                /* calc the title number */
                hpdf->css.title_num_h6++;

                snprintf(output, output_len, "<%d> %s", hpdf->css.title_num_h6, text);
            }
            break;
        }
        case PDF_CSS_TYPE_P:
        {
            if(hpdf->css.text_indent)
            {
                // 首行缩进
                output = malloc(output_len);
                snprintf(output, output_len, "    %s", text);
            }
            break;
        }
        case PDF_CSS_TYPE_PRE:
        {
            break;
        }
        case PDF_CSS_TYPE_BLOCK:
        {
            hpdf->css.width_ratio = 0.99;            
            hpdf->css.bg_color = g_color_gray;
            hpdf->css.seg_margin.left = 2;
            hpdf->css.seg_margin.right = 2;
            hpdf->css.text_height = HPDF_CSS_GetTextHeight(hpdf, text);
            hpdf->css.width_ratio = 0.01;
            HPDF_HANDLER_Print(hpdf, "");
            HPDF_CSS_SET(PDF_CSS_TYPE_BLOCK, &hpdf->css);
            hpdf->css.text_height = 0;
            hpdf->css.bg_color = g_color_lightgray;
            hpdf->css.width_ratio = 1;
            hpdf->css.withFrame = 0;
            if(hpdf->css.text_indent)
            {
                // 首行缩进
                output = malloc(output_len);
                snprintf(output, output_len, "    %s", text);
            }
            break;
        }
        default:
        {
            break;
        }
    }
    
    if(output) {
        HPDF_HANDLER_Print(hpdf, output);
    } else {
        HPDF_HANDLER_Print(hpdf, text);
    }

    /* output outline */
    if( PDF_CSS_TYPE_TITLE == hpdf->css.css_type ) {
        if(! hpdf->root)
        {
            /* create outline root. */
            hpdf->root = HPDF_CreateOutline (hpdf->pdf, NULL, output ? output : text, 
                HPDF_GetEncoder (hpdf->pdf, "GBK-EUC-H"));
            HPDF_Outline_SetOpened (hpdf->root, HPDF_TRUE);
            hpdf->dst = HPDF_Page_CreateDestination (hpdf->page);
            HPDF_Outline_SetDestination(hpdf->root, hpdf->dst);
        }
    } else if( PDF_CSS_TYPE_H1 == hpdf->css.css_type ) {
        /* create outline root. */
        hpdf->title= HPDF_CreateOutline (hpdf->pdf, hpdf->root, output ? output : text, 
            HPDF_GetEncoder (hpdf->pdf, "GBK-EUC-H"));
        hpdf->dst = HPDF_Page_CreateDestination (hpdf->page);
        HPDF_Outline_SetDestination(hpdf->title, hpdf->dst);
    } else if(PDF_CSS_TYPE_H2 == hpdf->css.css_type ) {
        /* create outline root. */
        hpdf->sub_title= HPDF_CreateOutline (hpdf->pdf, hpdf->title, output ? output : text, 
            HPDF_GetEncoder (hpdf->pdf, "GBK-EUC-H"));
        hpdf->dst = HPDF_Page_CreateDestination (hpdf->page);
        HPDF_Outline_SetDestination(hpdf->sub_title, hpdf->dst);
    }
    
    if(output)
    {
        free(output);
    }

    return 0;
}

int HPDF_HANDLER_Print(HPDF_HANDLER hpdf, char *text) {
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

    if(text_height > hpdf->css.page_height - 
        hpdf->css.side_margin.top - hpdf->css.side_margin.bottom - 
        hpdf->css.seg_margin.top - hpdf->css.seg_margin.bottom)
    {
        text_height = hpdf->css.page_height - 
            hpdf->css.side_margin.top - hpdf->css.side_margin.bottom - 
            hpdf->css.seg_margin.top - hpdf->css.seg_margin.bottom;   
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
    if(hpdf->css.withFrame == 1)
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

    if(hpdf->css.withFrame == 2)
    {
        HPDF_Page_SetLineWidth (hpdf->page, hpdf->css.line_width);
        HPDF_Page_SetRGBStroke (hpdf->page, hpdf->css.line_color.r, hpdf->css.line_color.g, hpdf->css.line_color.b);

        HPDF_Page_MoveTo (hpdf->page, hpdf->css.cur_text_pos.left, hpdf->css.cur_text_pos.bottom);
        HPDF_Page_LineTo (hpdf->page, hpdf->css.cur_text_pos.right, hpdf->css.cur_text_pos.bottom);
        HPDF_Page_Stroke (hpdf->page);
    }

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

HPDF_Image HPDF_HANDLER_LoadImageFromFile(HPDF_Doc pdf, char *imageFile) {
    HPDF_Image image = NULL;
    char *pFType = NULL;

    // 判断文件是否存在
    if(access(imageFile, F_OK)) {
        return NULL;
    }

    // 判断文件后缀
    if(pFType = strrchr(imageFile, '.')) {
        //printf("====%s\n", pFType);
        if(strcmp(pFType, ".png") == 0) {
            image =  HPDF_LoadPngImageFromFile(pdf, imageFile);
        } else if (strcmp(pFType, ".jpg") == 0) {
            image =  HPDF_LoadJpegImageFromFile(pdf, imageFile);
        } else {
            image = NULL;
        }
    }
    return image;
}

int HPDF_HANDLER_PrintImage(HPDF_HANDLER hpdf, char *imageFile) {
    HPDF_Image image = NULL;
    HPDF_REAL image_height, image_width, image_hw_ratio;
    HPDF_Point pos;
    
    image =  HPDF_HANDLER_LoadImageFromFile(hpdf->pdf, imageFile);
    if(image == NULL) {
        int withFrame = 0;
        printf("load image file failed!\n");
        withFrame = hpdf->css.withFrame;
        hpdf->css.withFrame = 1;
        HPDF_HANDLER_Print(hpdf, "加载失败");
        hpdf->css.withFrame = withFrame;
        return 40;
    }
    
    image_height = HPDF_Image_GetHeight (image);
    image_width = HPDF_Image_GetWidth (image);
    image_hw_ratio = image_height / image_width;

    // 图片的高度不能高于页面的高度
    if(image_height > hpdf->css.page_height - hpdf->css.side_margin.top - hpdf->css.side_margin.bottom)
    {
        image_height = hpdf->css.page_height - hpdf->css.side_margin.top - hpdf->css.side_margin.bottom;   
    }

    hpdf->css.cur_frame_pos.left = hpdf->css.cur_frame_pos.right;
    hpdf->css.cur_frame_pos.right = hpdf->css.cur_frame_pos.left + hpdf->css.width_ratio *
        (hpdf->css.page_width - hpdf->css.side_margin.left - hpdf->css.side_margin.right);
    hpdf->css.cur_frame_pos.top = hpdf->css.cur_frame_pos.bottom;

    // 图片的宽度不能大于当前输出的宽度
    if(image_width > hpdf->css.cur_frame_pos.right - hpdf->css.cur_frame_pos.left) {
        image_width = hpdf->css.cur_frame_pos.right - hpdf->css.cur_frame_pos.left;
        image_height = image_width * image_hw_ratio;
    }
    
    hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top - image_height - 
        hpdf->css.line_width * 2;

    if(hpdf->css.cur_frame_pos.right > hpdf->css.page_width - hpdf->css.side_margin.right)
    {
        hpdf->css.cur_frame_pos.right = hpdf->css.page_width - hpdf->css.side_margin.right;
    }

    if(hpdf->css.cur_frame_pos.bottom < hpdf->css.side_margin.bottom)
    {
        printf("need new page\n");
        
        hpdf->page = HPDF_AddPage (hpdf->pdf);
        HPDF_Page_SetSize(hpdf->page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
        
        hpdf->css.cur_frame_pos.left = hpdf->css.side_margin.left;
        hpdf->css.cur_frame_pos.right = hpdf->css.cur_frame_pos.left + hpdf->css.width_ratio *
            (hpdf->css.page_width - hpdf->css.side_margin.left - hpdf->css.side_margin.right);
        hpdf->css.cur_frame_pos.top = hpdf->css.page_height - hpdf->css.side_margin.top;
        hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top - image_height;
    }

    if(hpdf->css.textAlign == HPDF_TALIGN_LEFT) {
        pos.x = hpdf->css.cur_frame_pos.left;
        pos.y = hpdf->css.cur_frame_pos.top - image_height;
    } else if (hpdf->css.textAlign == HPDF_TALIGN_RIGHT) {
        pos.x = hpdf->css.cur_frame_pos.right - image_width;
        pos.y = hpdf->css.cur_frame_pos.top - image_height;
    } else {
        pos.x = hpdf->css.cur_frame_pos.right + hpdf->css.cur_frame_pos.left - image_width;
        pos.x = pos.x / 2;
        pos.y = hpdf->css.cur_frame_pos.top - image_height;
    } 

    // 输出边框，如果要输出背景色，必须先输背景色再输文字，所以这里先关闭文字，输出背景色，再开启文字输出
    if(hpdf->css.withFrame)
    {   
        HPDF_Page_SetLineWidth (hpdf->page, hpdf->css.line_width);
        HPDF_Page_SetRGBStroke (hpdf->page, hpdf->css.line_color.r, hpdf->css.line_color.g, hpdf->css.line_color.b);
        HPDF_Page_SetRGBFill (hpdf->page, hpdf->css.bg_color.r, hpdf->css.bg_color.g, hpdf->css.bg_color.b);
        HPDF_Page_Rectangle (hpdf->page, pos.x, pos.y, image_width, image_height);
        //HPDF_Page_Stroke (hpdf->page);
        HPDF_Page_FillStroke(hpdf->page);
    }

    HPDF_Page_DrawImage (hpdf->page, image, 
        pos.x + hpdf->css.line_width, pos.y + hpdf->css.line_width, 
        image_width - hpdf->css.line_width * 2, image_height - hpdf->css.line_width * 2);
    
    if(0)
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
    
    return image_height;
}

int HPDF_HANDLER_PrintLine(HPDF_HANDLER hpdf) {
    hpdf->css.cur_frame_pos.left = hpdf->css.cur_frame_pos.right;
    hpdf->css.cur_frame_pos.right = hpdf->css.cur_frame_pos.left + hpdf->css.width_ratio *
        (hpdf->css.page_width - hpdf->css.side_margin.left - hpdf->css.side_margin.right);
    hpdf->css.cur_frame_pos.top = hpdf->css.cur_frame_pos.bottom;
    hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top - hpdf->css.line_width -
        hpdf->css.text_height;

    HPDF_Page_SetLineWidth (hpdf->page, hpdf->css.line_width);
    HPDF_Page_SetRGBStroke (hpdf->page, hpdf->css.line_color.r, hpdf->css.line_color.g, hpdf->css.line_color.b);

    HPDF_Page_MoveTo (hpdf->page, hpdf->css.cur_frame_pos.left, hpdf->css.cur_frame_pos.bottom);
    HPDF_Page_LineTo (hpdf->page, hpdf->css.cur_frame_pos.right, hpdf->css.cur_frame_pos.bottom);
    HPDF_Page_Stroke (hpdf->page);
    
    if(0)
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

    return 0;
}
