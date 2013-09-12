#include <stdlib.h>
#include <stdio.h>
#include "pdfreport.h"

int main() {
    HPDF_HANDLER hpdf;
    const char *fname = "1.pdf";
    
    hpdf = HPDF_HANDLER_New();
    if(!hpdf) {
        printf("hpdf handle new failed!\n");
        return 1;   
    }
    
    HPDF_CSS_SET(PDF_CSS_TYPE_TITLE, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport ��ʹ�ð���");
    
    hpdf->css.font_size = 18;
    hpdf->css.font = g_font_sun;
    HPDF_HANDLER_PrintEx(hpdf, "�����ˣ���С��\n���ʱ�䣺2013-9-10");

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
        
    /* save the document to a file */
    HPDF_SaveToFile (hpdf->pdf, fname);

    /* clean up */
    HPDF_HANDLER_Free(hpdf);
}