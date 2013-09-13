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
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport 库使用帮助");

    //HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    //height = HPDF_HANDLER_PrintImage(hpdf, "logo.png");
    hpdf->css.font_size = 18;
    hpdf->css.font = g_font_sun;
    hpdf->css.textAlign = HPDF_TALIGN_CENTER;
    HPDF_HANDLER_Print(hpdf, "创建人：李小龙\n输出时间：2013-9-10");

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "相关代码文件");

    HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport.h");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport库的头文件，定义了可引用的方法和相关结构");

    HPDF_CSS_SET(PDF_CSS_TYPE_H2, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport.c");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport库的源码文件，里面实现了各种方法，如果需要修改方法的实现，可以修改此文件");    

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "pdfreport使用方法");

    HPDF_CSS_SET(PDF_CSS_TYPE_P, &hpdf->css);
    HPDF_HANDLER_PrintEx(hpdf, "首先拷贝pdfreport.h和pdfreport.c两个文件到对应的工程目录，然后在\
需要生成pdf报表的地方，引入pdfreport.h头文件，接着定义一个HPDF_HANDLER结构，调用HPDF_HANDLER_New\
生成一个pdf handler结构，接下来就可以对这个结构进行输出了。");

    HPDF_HANDLER_PrintEx(hpdf, "在需要输出内容时，先要设置一下输出的样式，设置方法是调用HPDF_CSS_SET\
方法，输出样式是仿照html语言的，目前支持的输出样式有PDF_CSS_TYPE_H1至PDF_CSS_TYPE_H6六种标题，\
PDF_CSS_TYPE_P，段落文本，PDF_CSS_TYPE_TD表格头，PDF_CSS_TYPE_TR表格体。");

    HPDF_HANDLER_PrintEx(hpdf, "内容输出完成后，调用HPDF_SaveToFile保存到文件，然后调用HPDF_HANDLER_Free释放pdf句柄即可。");

    HPDF_HANDLER_PrintEx(hpdf, "在HPDF_CSS结构中有很多属性，控制了内容如何展现，比如字体，字体大小，\
字体颜色，是否需要边框，页边距等等，每一个属性都可以在输出前去自由调整。");

    hpdf->css.width_ratio = 0.2;
    hpdf->css.textAlign = HPDF_TALIGN_LEFT;
    height = HPDF_HANDLER_PrintImage(hpdf, "1dptechlogo.png");

    HPDF_CSS_SET(PDF_CSS_TYPE_H1, &hpdf->css);
    hpdf->css.width_ratio = 0.8;
    hpdf->css.text_height = height;
    hpdf->css.textAlign = HPDF_TALIGN_RIGHT;
    HPDF_HANDLER_Print(hpdf, "内部公开");

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
    HPDF_HANDLER_PrintEx(hpdf, "上面是输出图片的例子，居右。");
    
        
    /* save the document to a file */
    HPDF_SaveToFile (hpdf->pdf, fname);

    /* clean up */
    HPDF_HANDLER_Free(hpdf);
}