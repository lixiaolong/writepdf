#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include "hpdf.h"

HPDF_Font g_font_sun, g_font_hei;

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
    HPDF_Font font;                 /* 字体 */
    HPDF_UINT font_size;            /* 字体大小 */
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
    hcss->text_height = 0;

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
            css->font_size = 24;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = 10;
            css->seg_margin.bottom = 22;
            css->withFrame = 0;
            css->textAlign = HPDF_TALIGN_CENTER;
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
            css->seg_margin.top = css->font_size / 2;
            css->seg_margin.bottom = css->font_size / 2;
            css->withFrame = 0;
            css->textAlign = HPDF_TALIGN_LEFT;
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
            css->textAlign = HPDF_TALIGN_LEFT;
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
            css->textAlign = HPDF_TALIGN_LEFT;
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
            css->textAlign = HPDF_TALIGN_LEFT;
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
            css->textAlign = HPDF_TALIGN_LEFT;
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
            css->textAlign = HPDF_TALIGN_LEFT;
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
            css->textAlign = HPDF_TALIGN_LEFT;
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
            break;
        }
        case PDF_CSS_TYPE_TD:
        {
            css->css_type = PDF_CSS_TYPE_TD;
            css->width_ratio = 1;
            css->font = g_font_hei;
            css->font_size = 12;
            css->text_leading = css->font_size * 1.5;
            css->withFrame = 1;
            css->seg_margin.left = 2;
            css->seg_margin.right = 2;
            css->seg_margin.top = 4;
            css->seg_margin.bottom = 4;
            css->textAlign = HPDF_TALIGN_CENTER;
            break;
        }
        default:
            css->css_type = PDF_CSS_TYPE_BEGIN;
            break;
    }

    return 0;
}

// 内容的行数 = 回车符的个数 + 内容的总长度 / 当前行宽 + 1
float  HPDF_CSS_GetTextHeight(HPDF_CSS *css, char *text) {
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
    
    text_width = text_len * css->font_size / 2;

    // 当前可输入的行宽
    text_width_per_line = css->page_width - 
        css->side_margin.left - css->side_margin.right -
        css->seg_margin.left  - css->seg_margin.right;

    text_width_per_line = text_width_per_line * css->width_ratio;

    text_height_count = text_width / text_width_per_line + 1 + enter_count;

    text_height = text_height_count * css->text_leading + 
        css->seg_margin.top + css->seg_margin.bottom;

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

    // text_height
    if(hpdf->css.text_height == 0) {
        text_height = HPDF_CSS_GetTextHeight(&hpdf->css, text);
    } else {
        text_height = hpdf->css.text_height;
    }

    // calc current position
    // ====================================================
    printf("===begin: left:%f right:%f top:%f bottom:%f\n", hpdf->css.cur_frame_pos.left, 
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
    // ====================================================   


    // output string
    // ==================================================== 
    hpdf->css.cur_text_pos.left = hpdf->css.cur_frame_pos.left + hpdf->css.seg_margin.left;
    hpdf->css.cur_text_pos.right = hpdf->css.cur_frame_pos.right - hpdf->css.seg_margin.right;
    hpdf->css.cur_text_pos.top = hpdf->css.cur_frame_pos.top - hpdf->css.seg_margin.top;
    hpdf->css.cur_text_pos.bottom= hpdf->css.cur_frame_pos.bottom + hpdf->css.seg_margin.bottom;
    
    ret = HPDF_Page_TextRect(hpdf->page, hpdf->css.cur_text_pos.left, hpdf->css.cur_text_pos.top, 
        hpdf->css.cur_text_pos.right, hpdf->css.cur_text_pos.bottom, text, hpdf->css.textAlign, &outlen);

    
    HPDF_Page_EndText(hpdf->page);
    printf("output string ret:%x %d\n", ret, outlen);
    if(outlen <= 0)
    {
        // 输出失败，坐标回退到输出前的状态
        hpdf->css.cur_frame_pos.right = hpdf->css.cur_frame_pos.left;
        hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top;
    }
    else
    {
        // 输出边框
        if(hpdf->css.withFrame)
        {
            HPDF_Page_Rectangle (hpdf->page, hpdf->css.cur_frame_pos.left, hpdf->css.cur_frame_pos.bottom, 
                        hpdf->css.cur_frame_pos.right - hpdf->css.cur_frame_pos.left,
                        hpdf->css.cur_frame_pos.top - hpdf->css.cur_frame_pos.bottom);
            HPDF_Page_Stroke (hpdf->page);
        }
        
        if(hpdf->css.cur_frame_pos.right < (hpdf->css.page_width - hpdf->css.side_margin.right))
        {
            // 右边还有空间可以输入
            hpdf->css.cur_frame_pos.left = hpdf->css.cur_frame_pos.right;
            hpdf->css.cur_frame_pos.bottom = hpdf->css.cur_frame_pos.top;
        } 
        else if(hpdf->css.cur_frame_pos.right == (hpdf->css.page_width - hpdf->css.side_margin.right))
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
    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;
    HPDF_STATUS ret;
    HPDF_UINT outlen = 0;
    HPDF_CSS pdfcss;
    HPDF_HANDLE hpdf;

    hpdf = HPDF_HANDLE_New();
    if(!hpdf)
    {
        printf("hpdf handle new failed!\n");
        return 1;
    }

    HPDF_CSS_SET(PDF_CSS_TYPE_TITLE, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "杭州迪普科技");

    
    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "公司简介");
    
    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    杭州迪普科技有限公司（简称“迪普科技”）是在网络安全及应用交付领域集研发、生产、销售于一体的高科技企业。迪普科技以“让网络变得简单、智能、可靠”为愿景，专注于网络安全及应用交付领域，持续创新，为客户提供领先的产品与解决方案。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "二级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    迪普科技的总部位于杭州，在北京和杭州设有研发中心，具有一支业界领先的研发团队，自主开发了基于FPGA的高性能内容识别与加速芯片、高性能分布式APP-X硬件平台、虚拟化Conplat核心软件平台以及APP-ID特征库，并在此基础上推出了以应用防火墙、入侵防御系统（IPS）、上网行为管理及流控（UAG）、Web应用防火墙（WAF）、异常流量检测/清洗（Probe/Guard）、应用交付（ADX）、深度业务路由交换网关（DPX）、工业交换机等为代表的全面的网络安全与应用交付产品线。基于完善的产品线，迪普科技具有为客户提供深度定制化解决方案的能力。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H3, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "三级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    迪普科技通过了GB/T19001-2008（ISO9001:2008)质量管理体系标准认证，质量管理严格遵循ISC生产制造流程。同时，严控供应商的质量管理体系、职业健康与安全管理体系以及环境管理体系，为客户提供质量可靠的产品。同时，迪普科技在全国所有省、自治区及直辖市都设有销售与服务机构。依托分布于全国的分支机构及合作伙伴，迪普科技在全国范围内都具有为客户提供7×24小时服务的能力。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H4, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "四级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    从成立至今，迪普科技实现了高速成长，目前已经积累了超过10,000家客户的应用，全面进入了包括运营商、政府、电力能源、金融、教育、大企业等在内的各行各业，成为国内重要的网络安全与应用交付厂商之一。未来，迪普科技将继续在网络安全与应用交付领域进行持续投入，进一步完善产品与解决方案，为客户创造更大价值。");


    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "公司简介");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    杭州迪普科技有限公司（简称“迪普科技”）是在网络安全及应用交付领域集研发、生产、销售于一体的高科技企业。迪普科技以“让网络变得简单、智能、可靠”为愿景，专注于网络安全及应用交付领域，持续创新，为客户提供领先的产品与解决方案。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H5, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "五级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    迪普科技的总部位于杭州，在北京和杭州设有研发中心，具有一支业界领先的研发团队，自主开发了基于FPGA的高性能内容识别与加速芯片、高性能分布式APP-X硬件平台、虚拟化Conplat核心软件平台以及APP-ID特征库，并在此基础上推出了以应用防火墙、入侵防御系统（IPS）、上网行为管理及流控（UAG）、Web应用防火墙（WAF）、异常流量检测/清洗（Probe/Guard）、应用交付（ADX）、深度业务路由交换网关（DPX）、工业交换机等为代表的全面的网络安全与应用交付产品线。基于完善的产品线，迪普科技具有为客户提供深度定制化解决方案的能力。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H6, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "六级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    迪普科技通过了GB/T19001-2008（ISO9001:2008)质量管理体系标准认证，质量管理严格遵循ISC生产制造流程。同时，严控供应商的质量管理体系、职业健康与安全管理体系以及环境管理体系，为客户提供质量可靠的产品。同时，迪普科技在全国所有省、自治区及直辖市都设有销售与服务机构。依托分布于全国的分支机构及合作伙伴，迪普科技在全国范围内都具有为客户提供7×24小时服务的能力。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H3, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "三级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    从成立至今，迪普科技实现了高速成长，目前已经积累了超过10,000家客户的应用，全面进入了包括运营商、政府、电力能源、金融、教育、大企业等在内的各行各业，成为国内重要的网络安全与应用交付厂商之一。未来，迪普科技将继续在网络安全与应用交付领域进行持续投入，进一步完善产品与解决方案，为客户创造更大价值。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H3, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "三级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    从成立至今，迪普科技实现了高速成长，目前已经积累了超过10,000家客户的应用，全面进入了包括运营商、政府、电力能源、金融、教育、大企业等在内的各行各业，成为国内重要的网络安全与应用交付厂商之一。未来，迪普科技将继续在网络安全与应用交付领域进行持续投入，进一步完善产品与解决方案，为客户创造更大价值。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H3, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "三级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    从成立至今，迪普科技实现了高速成长，目前已经积累了超过10,000家客户的应用，全面进入了包括运营商、政府、电力能源、金融、教育、大企业等在内的各行各业，成为国内重要的网络安全与应用交付厂商之一。未来，迪普科技将继续在网络安全与应用交付领域进行持续投入，进一步完善产品与解决方案，为客户创造更大价值。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H3, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "三级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    从成立至今，迪普科技实现了高速成长，目前已经积累了超过10,000家客户的应用，全面进入了包括运营商、政府、电力能源、金融、教育、大企业等在内的各行各业，成为国内重要的网络安全与应用交付厂商之一。未来，迪普科技将继续在网络安全与应用交付领域进行持续投入，进一步完善产品与解决方案，为客户创造更大价值。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H3, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "三级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    从成立至今，迪普科技实现了高速成长，目前已经积累了超过10,000家客户的应用，全面进入了包括运营商、政府、电力能源、金融、教育、大企业等在内的各行各业，成为国内重要的网络安全与应用交付厂商之一。未来，迪普科技将继续在网络安全与应用交付领域进行持续投入，进一步完善产品与解决方案，为客户创造更大价值。");

    HPDF_CSS_SET(PDF_CSS_TYPE_H3, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "三级标题");

    HPDF_CSS_SET(PDF_CSS_TYPE_TD, &hpdf->css);
    hpdf->css.width_ratio = 0.2;
    HPDF_HANDLE_Print(hpdf, "表格标题");
    hpdf->css.width_ratio = 0.3;
    HPDF_HANDLE_Print(hpdf, "表格内容");
    hpdf->css.width_ratio = 0.5;
    HPDF_HANDLE_Print(hpdf, "表格内容");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.2;
    HPDF_HANDLE_Print(hpdf, "IP地址");
    hpdf->css.width_ratio = 0.8;
    HPDF_HANDLE_Print(hpdf, "192.168.0.1");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.2;
    HPDF_HANDLE_Print(hpdf, "域名");
    hpdf->css.width_ratio = 0.8;
    HPDF_HANDLE_Print(hpdf, "www.test.net");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.2;
    HPDF_HANDLE_Print(hpdf, "公司地点");
    hpdf->css.width_ratio = 0.8;
    HPDF_HANDLE_Print(hpdf, "杭州");

    HPDF_CSS_SET(PDF_CSS_TYPE_TITLE, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "使用帮助");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLE_Print(hpdf, "    使用帮助");

    
    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.1;
    hpdf->css.withFrame = 0;
    hpdf->css.textAlign = HPDF_TALIGN_RIGHT;
    HPDF_HANDLE_Print(hpdf, "·");
    hpdf->css.width_ratio = 0.9;
    hpdf->css.withFrame = 0;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;    
    HPDF_HANDLE_Print(hpdf, "192.168.0.1 192.168.0.1 192.168.0.1 192.168.0.1 192.168.0.1 192.168.0.1");

    HPDF_CSS_SET(PDF_CSS_TYPE_TR, &hpdf->css);
    hpdf->css.width_ratio = 0.1;
    hpdf->css.withFrame = 0;
    hpdf->css.textAlign = HPDF_TALIGN_RIGHT;
    HPDF_HANDLE_Print(hpdf, "·");
    hpdf->css.width_ratio = 0.9;
    hpdf->css.withFrame = 0;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;    
    HPDF_HANDLE_Print(hpdf, "192.168.0.2");
    

    /* save the document to a file */
    HPDF_SaveToFile (hpdf->pdf, fname);

    /* clean up */
    HPDF_HANDLE_Free(hpdf);

    return 0;

}
