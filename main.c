#include <stdlib.h>
#include <stdio.h>
#include "pdfreport.h"

int main() {
    HPDF_HANDLER hpdf;
    const char *fname = "1.pdf";
    int height = 0;
    
    hpdf = HPDF_HANDLER_New();
    if(!hpdf) {
        printf("hpdf handle new failed!\n");
        return 1;   
    }
    
    HPDF_CSS_SET(PDF_CSS_TYPE_TITLE, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport ��ʹ�ð���");

    //HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    //height = HPDF_HANDLER_PrintImage(hpdf, "logo.png");
    hpdf->css.font_size = 18;
    hpdf->css.font = g_font_sun;
    hpdf->css.textAlign = HPDF_TALIGN_CENTER;
    HPDF_HANDLER_Print(hpdf, "�����ˣ���С��\n���ʱ�䣺2013-9-10");

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "��ش����ļ�");

    HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport.h");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport���ͷ�ļ��������˿����õķ�������ؽṹ");

    HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport.c");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport���Դ���ļ�������ʵ���˸��ַ����������Ҫ�޸ķ�����ʵ�֣������޸Ĵ��ļ�");    

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreportʹ�÷���");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "���ȿ���pdfreport.h��pdfreport.c�����ļ�����Ӧ�Ĺ���Ŀ¼��Ȼ����\
��Ҫ����pdf����ĵط�������pdfreport.hͷ�ļ������Ŷ���һ��HPDF_HANDLER�ṹ������HPDF_HANDLER_New\
����һ��pdf handler�ṹ���������Ϳ��Զ�����ṹ��������ˡ�");

    HPDF_HANDLER_PrintEx(hpdf, "����Ҫ�������ʱ����Ҫ����һ���������ʽ�����÷����ǵ���HPDF_CSS_SET\
�����������ʽ�Ƿ���html���Եģ�Ŀǰ֧�ֵ������ʽ��PDF_CSS_TYPE_H1��PDF_CSS_TYPE_H6���ֱ��⣬\
PDF_CSS_TYPE_P�������ı���PDF_CSS_TYPE_TD���ͷ��PDF_CSS_TYPE_TR����塣");

    HPDF_HANDLER_PrintEx(hpdf, "���������ɺ󣬵���HPDF_SaveToFile���浽�ļ���Ȼ�����HPDF_HANDLER_Free�ͷ�pdf������ɡ�");

    HPDF_HANDLER_PrintEx(hpdf, "��HPDF_CSS�ṹ���кܶ����ԣ��������������չ�֣��������壬�����С��\
������ɫ���Ƿ���Ҫ�߿�ҳ�߾�ȵȣ�ÿһ�����Զ����������ǰȥ���ɵ�����");

    hpdf->css.width_ratio = 0.2;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    height = HPDF_HANDLER_PrintImage(hpdf, "1dptechlogo.png");

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    hpdf->css.width_ratio = 0.8;
    hpdf->css.text_height = height;
    hpdf->css.textAlign = HPDF_TALIGN_RIGHT;
    HPDF_HANDLER_Print(hpdf, "�ڲ�����");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    hpdf->css.text_height = 0;
    hpdf->css.width_ratio = 1;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;

    HPDF_HANDLER_Print(hpdf, "------------------------------------------------------------------------------------");

    hpdf->css.width_ratio = 0.5;
    height = HPDF_HANDLER_PrintImage(hpdf, "1.png");
    hpdf->css.text_height = height;
    hpdf->css.withFrame = 1;
    HPDF_HANDLER_PrintImage(hpdf, "1.jpg");

    hpdf->css.width_ratio = 1;
    hpdf->css.withFrame = 0;    
    HPDF_HANDLER_PrintEx(hpdf, "���������ͼƬ�����ӣ����ҡ�");
    
        
    /* save the document to a file */
    HPDF_SaveToFile (hpdf->pdf, fname);

    /* clean up */
    HPDF_HANDLER_Free(hpdf);
}