#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include "hpdf.h"

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

HPDF_Font g_font_sun, g_font_hei;

typedef struct _HPDFCSS {
    HPDF_REAL page_width;          /* 页面宽度 */
    HPDF_REAL page_height;         /* 页面高度 */
    HPDF_REAL width_ratio;          /* 宽度比率 */
    HPDF_Font font;                     /* 字体           */
    HPDF_UINT font_size;             /* 字体大小 */
    HPDF_Rect cur_pos;               /* 当前位置 */
    HPDF_Rect side_margin;        /* 页面边距 */
    int            withFrame;          /* 是否输出边框,1:输出，0:不输出 */
    int            textAlign;            /* 0:文本居左，1:居右，2:居中，3:自动 */
} HPDFCSS;

int HPDFCSS_Init(HPDF_Page page, HPDFCSS *hcss) {
    hcss->page_height = HPDF_Page_GetHeight(page);
    hcss->page_width = HPDF_Page_GetWidth(page);
    hcss->font = g_font_sun;
    hcss->font_size = 14;
    hcss->side_margin.left = 40;
    hcss->side_margin.right = 40;
    hcss->side_margin.top = 80;
    hcss->side_margin.bottom = 80;
    hcss->width_ratio = 1;
    hcss->cur_pos.left = hcss->side_margin.left;
    hcss->cur_pos.right = hcss->cur_pos.left;
    hcss->cur_pos.top = hcss->page_height - hcss->side_margin.top;
    hcss->cur_pos.bottom = hcss->cur_pos.top;
    hcss->withFrame = 0;
    hcss->textAlign = 0;

    //printf("page height: %f, page_width: %f\n", hcss->page_height, hcss->page_width);
    return 0;
}

int HPDFCSS_Printf(HPDF_Doc pdf, HPDF_Page *page, HPDFCSS *hcss, char *text) {
    HPDF_UINT outlen = 0, text_height = 100;
    HPDF_REAL text_len = 100;
    HPDF_STATUS ret;
    HPDF_TextAlignment textAlign = HPDF_TALIGN_LEFT;

    HPDF_UINT temp_len = 0;
    HPDF_REAL temp_real = 0;

    HPDF_TextWidth tw;

    HPDF_Page_BeginText(*page);
    HPDF_Page_SetFontAndSize(*page, hcss->font, hcss->font_size);
    HPDF_Page_SetTextLeading (*page, 20);

    text_len = HPDF_Page_TextWidth(*page, "abcde xxxxxxxxx xxxxxxxxxxxxx xxxxxxxxxx xxxxxxxxxx xxxxxxxx ");

    tw = HPDF_Font_TextWidth (hcss->font, (HPDF_BYTE *)"abcde nihao", strlen("abcde nihao"));

    printf("====text_len:%d %d %d %d\n", tw.width, tw.numchars, tw.numspace, tw.numwords);

    temp_len = HPDF_Page_MeasureText(*page, "abcde xxxxxxxxx xxxxxxxxxxxxx xxxxxxxxxx xxxxxxxxxx xxxxxxxx ", 
    //    (hcss->page_width - hcss->side_margin.left - hcss->side_margin.right), 1, &temp_real);
        100, 0, &temp_real);

    printf("text_len:%d, %f\n", temp_len, temp_real);

    printf("left:%f right:%f top:%f bottom:%f\n", hcss->cur_pos.left, hcss->cur_pos.right, hcss->cur_pos.top, hcss->cur_pos.bottom);
    hcss->cur_pos.left = hcss->cur_pos.right;
    hcss->cur_pos.right = hcss->cur_pos.left + hcss->width_ratio *
        (hcss->page_width - hcss->side_margin.left - hcss->side_margin.right);
    hcss->cur_pos.top = hcss->cur_pos.bottom - 2;
    hcss->cur_pos.bottom = hcss->cur_pos.top - text_height;

    if(hcss->cur_pos.right > hcss->page_width - hcss->side_margin.right)
    {
        hcss->cur_pos.right = hcss->page_width - hcss->side_margin.right;
    }

    if(hcss->cur_pos.bottom < hcss->side_margin.bottom)
    {
        printf("need new page\n");
        HPDF_Page_EndText(*page);
        
        *page = HPDF_AddPage (pdf);
        HPDF_Page_SetSize(*page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
        
        hcss->cur_pos.left = hcss->side_margin.left;
        hcss->cur_pos.right = hcss->cur_pos.left;
        hcss->cur_pos.top = hcss->page_height - hcss->side_margin.top;
        hcss->cur_pos.bottom = hcss->cur_pos.top;

        hcss->cur_pos.left = hcss->cur_pos.right;
        hcss->cur_pos.right = hcss->cur_pos.left + hcss->width_ratio *
            (hcss->page_width - hcss->side_margin.left - hcss->side_margin.right);
        hcss->cur_pos.top = hcss->cur_pos.bottom - 2;
        hcss->cur_pos.bottom = hcss->cur_pos.top - text_height;

        HPDF_Page_BeginText(*page);
        HPDF_Page_SetFontAndSize(*page, hcss->font, hcss->font_size);
        HPDF_Page_SetTextLeading (*page, 20);
    }
    
    printf("left:%f right:%f top:%f bottom:%f\n", hcss->cur_pos.left, hcss->cur_pos.right, hcss->cur_pos.top, hcss->cur_pos.bottom);

    switch (hcss->textAlign) {
        case 0:
            textAlign = HPDF_TALIGN_LEFT;
            break;
        case 1:
            textAlign = HPDF_TALIGN_RIGHT;
            break;
        case 2:
            textAlign = HPDF_TALIGN_CENTER;
            break;
        case 3:
            textAlign = HPDF_TALIGN_JUSTIFY;
            break;
        default:
            textAlign = HPDF_TALIGN_LEFT;
            break;
    }
    
    
    ret = HPDF_Page_TextRect(*page, hcss->cur_pos.left, hcss->cur_pos.top, hcss->cur_pos.right, 
        hcss->cur_pos.bottom, text, textAlign, &outlen);

    
    HPDF_Page_EndText(*page);
    printf("ret:%x %d\n", ret, outlen);
    if(outlen <= 0)
    {
        hcss->cur_pos.right = hcss->cur_pos.left;
        hcss->cur_pos.bottom = hcss->cur_pos.top;
    }
    else
    {
        if(hcss->withFrame)
        {
            HPDF_Page_Rectangle (*page, hcss->cur_pos.left - 1, hcss->cur_pos.bottom - 1, hcss->cur_pos.right - hcss->cur_pos.left + 2,
                        hcss->cur_pos.top - hcss->cur_pos.bottom + 2);
            HPDF_Page_Stroke (*page);
        }
        
        if(hcss->cur_pos.right < (hcss->page_width - hcss->side_margin.right))
        {
            hcss->cur_pos.left = hcss->cur_pos.right;
            hcss->cur_pos.bottom = hcss->cur_pos.top;
        } 
        else if(hcss->cur_pos.right == (hcss->page_width - hcss->side_margin.right))
        {
            hcss->cur_pos.right = hcss->cur_pos.left;
            hcss->cur_pos.top = hcss->cur_pos.bottom;
        }
        else
        {
            printf("[ERROR]left:%f right:%f top:%f bottom:%f\n", hcss->cur_pos.left, hcss->cur_pos.right, hcss->cur_pos.top, hcss->cur_pos.bottom);
        }
    }
    
    printf("left:%f right:%f top:%f bottom:%f\n", hcss->cur_pos.left, hcss->cur_pos.right, hcss->cur_pos.top, hcss->cur_pos.bottom);
    
    return outlen;
}

int main (int argc, char **argv)
{
    const char *page_title = "Text Demo";
    const char *fname = "1.pdf";
    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;
    HPDF_STATUS ret;
    HPDF_UINT outlen = 0;
    HPDFCSS pdfcss;

    pdf = HPDF_New (error_handler, NULL);

    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }
    /* set compression mode */
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
 
    /* create default-font */
    HPDF_UseCNSFonts(pdf);
    HPDF_UseCNSEncodings(pdf);
    g_font_sun = HPDF_GetFont (pdf, "SimSun", "GBK-EUC-H");
    g_font_hei =  HPDF_GetFont (pdf, "SimHei", "GBK-EUC-H");

    /* add a new page object. */
    page = HPDF_AddPage (pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);


    HPDFCSS_Init(page, &pdfcss);

    pdfcss.textAlign = 2;
    pdfcss.withFrame = 0;
    pdfcss.font = g_font_hei;
    pdfcss.font_size = 16;
    HPDFCSS_Printf(pdf, &page, &pdfcss, "文档标题");

    
    pdfcss.textAlign = 0;
    pdfcss.withFrame = 1;
    pdfcss.font = g_font_sun;
    pdfcss.font_size = 14;
    HPDFCSS_Printf(pdf, &page, &pdfcss, "中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你的不能自动换行中文测试，你妹的不能自动换行");


    pdfcss.withFrame = 1;
    HPDFCSS_Printf(pdf, &page, &pdfcss, "titletitletitletitletitletitletitletitletitletitletitletitletitletitletitletitle");
    
    HPDFCSS_Printf(pdf, &page, &pdfcss, "中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你的不能自动换行中文测试，你妹的不能自动换行");

    HPDFCSS_Printf(pdf, &page, &pdfcss, "中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你的不能自动换行中文测试，你妹的不能自动换行");

    HPDFCSS_Printf(pdf, &page, &pdfcss, "中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你的不能自动换行中文测试，你妹的不能自动换行");

    HPDFCSS_Printf(pdf, &page, &pdfcss, "titletitletitletitletitletitletitletitletitletitletitletitletitletitletitletitle");

    HPDFCSS_Printf(pdf, &page, &pdfcss, "中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你的不能自动换行中文测试，你妹的不能自动换行");

    HPDFCSS_Printf(pdf, &page, &pdfcss, "中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你妹的不能自动换行中文测试，你的不能自动换行中文测试，你妹的不能自动换行");

    /* save the document to a file */
    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    return 0;

}
