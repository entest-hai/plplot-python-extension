// ============================================================================
// Author: Hai Tran
// Date: 10 OCT 2021
// File: plplot_ctg_paper.c
// Purpose: plot ctg paper using plplot lib
// Description:
// Usage example:
//*****************************************************************************
// 30 SEP 2021 Hai Tran
// simple test plplot
// build: gcc -o app plplot_ctg_paper.c -I /usr/local/include/ -I /usr/local/include/plplot/ -lm -lgsl -lgslcblas -lplplot
//*****************************************************************************
// 07 OCT 2021 Hai Tran
// next step plot CTG simple grid
//*****************************************************************************
// 07 OCT 2021 Hai Tran
// TO DO: check pltimefmt("%H:%M"); and format CTG paper, margin
// First, check plbox and option x-axis show date time
// Second, check pltimefmt
//*****************************************************************************
// 08 OCT 2021 Hai Tran
// convert ps to png with white background and high resolution
// view port, window, box size, and page size
// view pdf is better than open ps file directly
// plspage interpretation depends on device drivers
// convert ps to png using gs
// gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r600 -sDEVICE=png16m -sOutputFile=foo.png grid.ps
// convert -density 300  grid.ps -background white -alpha remove -alpha off grid.png
// gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -sDEVICE=pdfwrite -dPDFFitPage -o grid.pdf grid.ps
//*****************************************************************************
// 10 OCT 2021 Hai Tran
// convert to png and pdf
//gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r200 -sDEVICE=png16m -sOutputFile=grid.png grid.ps
//gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -sDEVICE=pdfwrite -g5775x6207 -dPDFFitPage -o grid.pdf grid.ps
//gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -sDEVICE=pdfwrite -dPDFFitPage -o grid.pdf grid.ps
// TODO: break pages and pares input, plot heart rate, mark acels, decels, and acc table data
// ============================================================================
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<plConfig.h>
#include"plcdemos.h"
// the ten minute block length in number of second
static const int SAMPLING_RATE = 4;
static const PLINT BLOCK_SIZE_SEC = 600;
// plot ctg grid using plplot
int plot_ctg_paper(float *maternal_heartrate, float *fetal_heartrate, float *ua, int num_heartrate){
    // simple data length number of the ten minute block
    int num_ten_min_block = num_heartrate / (10*60*SAMPLING_RATE) + 1;
    // total number of minute
    int num_min = num_ten_min_block * 10;
    // dpi device dependent
    int DPI = 60;
    // left and right margin of ctg paper in minute
    int ctg_paper_left_margin_in_minute = 2;
    // top and bottom margin of ctg paper in minute
    int ctg_paper_top_margin_in_minute = 2;
    // acc table height in minute
    int acc_table_height_in_minute = 10;
    // height of fhr in minute
    int fhr_height_in_minute = 10;
    // height of ua in minute
    int ua_height_in_minute = 5;
    // width of ctg paper in minute
    int ctg_paper_width_in_minute = num_min + 2*ctg_paper_left_margin_in_minute;
    // height of ctg paper in minute
    int ctg_paper_height_in_minute = fhr_height_in_minute
                                    + 4*ctg_paper_top_margin_in_minute
                                    + ua_height_in_minute
                                    + acc_table_height_in_minute;
    // width of ctg paper in pixel given DPI
    PLINT ctg_paper_width_in_pixel = (PLINT)DPI/2.54*ctg_paper_width_in_minute;
    PLINT ctg_paper_height_in_pixel = (PLINT)DPI/2.54*ctg_paper_height_in_minute;
    // set output
    plsfnam("./grid.ps");
    // set printer or device
    plsdev("psc");
    // set pen width
    plwidth(1.0);
    // set page 100 dpi, width 1000 pixel and height 500 pixel, 0 offset
    // device dependent raster
    plspage(DPI,DPI,ctg_paper_height_in_pixel,ctg_paper_width_in_pixel,0,0);
    // set color
    plscol0(0, 255, 255, 255); /* White, color 0, background */
    plscol0(15, 0, 0, 0);      /* Black, color 15 */
    // initialise plot
    plinit();
    // set subpages
    pladv(0);
    // setup view port for upper title and padding
    PLFLT xmin = 1.0*ctg_paper_top_margin_in_minute/ctg_paper_width_in_minute;
    PLFLT xmax = 1.0 - xmin;
    PLFLT ymin = 1.0*(acc_table_height_in_minute+ua_height_in_minute
                     + fhr_height_in_minute
                     + 3*ctg_paper_top_margin_in_minute)/ctg_paper_height_in_minute;
    PLFLT ymax = 1.0;
    plvpas(xmin,xmax,ymin,ymax,0.0);
    // setup window for upper title and padding
    plwind(0.0,num_min*1.0,0.0,100.0);
    // set background color for title area
    plcol0(3);
    // fill background color for title area
    PLFLT lab_px[] = {0,0,num_min*60,num_min*60};
    PLFLT lab_py[] = {20,100,100,20};
    plfill(4,lab_px,lab_py);
    // set color for title text
    plcol0(15);
    // set text font size
    plschr(2.0, 1.0);
    // set datetime format
    pltimefmt("%H:%M");
    // add title for ctg
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char title_string[255];
    sprintf(title_string, "CTG CONNECT %d-%02d-%02d 00:00", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    plptex(0,50,0,0,0,title_string);
    // set view port aspect ratio for heart rate
    xmin = 1.0*ctg_paper_left_margin_in_minute/ctg_paper_width_in_minute;
    xmax = 1.0 - xmin;
    ymin = 1.0*(acc_table_height_in_minute
                + ua_height_in_minute
                + 3*ctg_paper_top_margin_in_minute)/ctg_paper_height_in_minute;
    ymax = 1.0*(acc_table_height_in_minute
                + ua_height_in_minute
                + fhr_height_in_minute
                + 3*ctg_paper_top_margin_in_minute)/ctg_paper_height_in_minute;
    //setup viewport for heart rate
    plvpas(xmin,xmax,ymin,ymax,0.0);
    // setup window for heart rate
    plwind(0.0,num_min*60.0,50.0,250.0);
    // time format
    pltimefmt("%H:%M");
    // set color
    plcol0(15);
    // set characters
    plschr(2.0, 1.0);
    // box and grid system heart rate range [40, 240] means 200bpm = 20boxes * 10bpm/box
    plbox("ghdnitbc", BLOCK_SIZE_SEC,  20 * 1 , "ghbc", 10, 1);
    // width of y-stick
    PLFLT pltexw = 1.5*10*60/20.0;
    // add heart rate mark ytick
    for (int i=0; i < num_ten_min_block; i++){
         // x offset in second
        int x_offset_block_in_second = i * 10 * 60;
        // set background color for marker
        plcol0(7);
        PLFLT lab_px[] = {x_offset_block_in_second,
                          x_offset_block_in_second,
                          x_offset_block_in_second+pltexw,
                          x_offset_block_in_second+pltexw};
        PLFLT lab_py[] = {31,289,289,31};
        plfill(4,lab_px,lab_py);
        //
        plcol0(15);
        plptex(x_offset_block_in_second,60,0,0,0,"60");
        plptex(x_offset_block_in_second,90,0,0,0,"90");
        plptex(x_offset_block_in_second,120,0,0,0,"120");
        plptex(x_offset_block_in_second,150,0,0,0,"150");
        plptex(x_offset_block_in_second,180,0,0,0,"180");
        plptex(x_offset_block_in_second,210,0,0,0,"210");
        plptex(x_offset_block_in_second,240,0,0,0,"240");
//        plptex(x_offset_block_in_second,270,0,0,0,"270");
    }
    //********************** mark fetal heart rate nomral region 100 to 160
    // set up color map with transparent
//    plscol0a(15, 10, 20, 30, 0.05);
//    plcol0(3);
//    PLFLT lab_px_fhr_nomral[] = {0,0,num_min*60,num_min*60};
//    PLFLT lab_py_fhr_normal[] = {100,160,160,100};
//    plfill(4,lab_px_fhr_nomral,lab_py_fhr_normal);
    //**********************************************************************
    // return color without transparent alpha
    plscol0(15, 0, 0, 0);
    // plot fetal heart rate here
    // setup color for fetal heart rate line
    plcol0(1);
    // setup linewidth for fetal heart rate line
    plwidth(1);
    // setup line style for fetal heart rate line
    pllsty(1);
    // loop over all fetal heart rate point and point them
    PLFLT lx[2],ly[2];
    for(int i = 0; i < num_heartrate-1; i++){
        // time in second given 4Hz sample heart rate
        lx[0] = (1.0*i)/4.0;
        // time in second given 4Hz sample heart rate
        lx[1] = (1.0*(i+1))/4.0;
        // heart rate point
        ly[0] = fetal_heartrate[i];
        // heart rate point
        ly[1] = fetal_heartrate[i+1];
        plpoin(2, lx, ly, 1);
    }
    // loop over all maternal heart rate and plot them
    plcol0(9);
    for (int i = 0; i < num_heartrate-1; i++){
        // time in second given 4Hz sample heart rate
        lx[0] = (1.0*i)/4.0;
        // time in second given 4Hz sample heart rate
        lx[1] = (1.0*(i+1))/4.0;
        // heart rate point
        ly[0] = maternal_heartrate[i];
        // heart rate point
        ly[1] = maternal_heartrate[i+1];
        // plot line between two points
        plpoin(2, lx, ly, 1);
    }
    //
    plcol0(15);
    // setup viewport for ua
    xmin = 1.0*ctg_paper_left_margin_in_minute/ctg_paper_width_in_minute;
    xmax = 1.0 - xmin;
    ymin = 1.0*(acc_table_height_in_minute
                + 2*ctg_paper_top_margin_in_minute)/ctg_paper_height_in_minute;
    ymax = 1.0*(acc_table_height_in_minute
                + ua_height_in_minute
                + 2*ctg_paper_top_margin_in_minute)/ctg_paper_height_in_minute;
    plvpas(xmin,xmax,ymin,ymax,0.0);
    // setup window for ua
    plwind(0.0,num_min*60.0,0.0,100.0);
    pltimefmt("%H:%M");
    // set characters
    plschr(2.0, 1.0);
    // box and grid system
    plbox("ghdnitbc", BLOCK_SIZE_SEC,  20 * 1 , "ghbc", 20, 2);
    // add heart rate mark ytick
    for (int i=0; i < num_ten_min_block; i++){
        // x offset in second
        int x_offset_block_in_second = i * 10 * 60;
        // set background color for marker
        plcol0(7);
        PLFLT lab_px[] = {x_offset_block_in_second,
                            x_offset_block_in_second,
                            x_offset_block_in_second+pltexw,
                            x_offset_block_in_second+pltexw};
        PLFLT lab_py[] = {1,99,99,1};
        plfill(4,lab_px,lab_py);
        // set color for text black
        plcol0(15);
        plptex(x_offset_block_in_second,20,0,0,0,"20");
        plptex(x_offset_block_in_second,40,0,0,0,"40");
        plptex(x_offset_block_in_second,60,0,0,0,"60");
        plptex(x_offset_block_in_second,80,0,0,0,"80");
    }
    // plot ua signal here
    // setup color for ua line
    plcol0(10);
    // setup linewidth for ua line
    plwidth(1);
    // setup line style for ua line
    pllsty(1);
    // loop over all fetal ua point and point them
    for(int i = 0; i < num_heartrate-1; i++){
        // time in second given 4Hz sample heart rate
        lx[0] = (1.0*i)/4.0;
        // time in second given 4Hz sample heart rate
        lx[1] = (1.0*(i+1))/4.0;
        // heart rate point
        ly[0] = ua[i];
        // heart rate point
        ly[1] = ua[i+1];
        plpoin(2, lx, ly, 1);
    }
    //
    plcol0(15);
    // setup view port for acc table
    xmin = 1.0*ctg_paper_left_margin_in_minute/ctg_paper_width_in_minute;
    xmax = 1.0 - xmin;
    ymin = 1.0*ctg_paper_top_margin_in_minute/ctg_paper_height_in_minute;
    ymax = 1.0*(acc_table_height_in_minute + ctg_paper_top_margin_in_minute)/ctg_paper_height_in_minute;
    plvpas(xmin,xmax,ymin,ymax,0.0);
    // setup window acc table
    plwind(0.0,num_min*60.0,0.0,100.0);
    // set background color
    plcol0(7);
    // plot acc table
    PLFLT lab_px_acc_table[] = {0,0,num_min*60,num_min*60};
    PLFLT lab_py_acc_table[] = {1,99,99,1};
    plfill(4,lab_px_acc_table,lab_py_acc_table);
    // add content for acc table
    plcol0(15);
    plschr(3.0, 1.0);
    plptex(0,95,0,0,0,"ACC TABLE");
    // end plot
    plend();
    // exit
    exit( 0 );
}
// ===================================== main =================================
int main(){
    int num_heartrate = 45*60*4;
    float maternal_heartrate[num_heartrate];
    float fetal_heartrate[num_heartrate];
    float ua[num_heartrate];
    // create data
    for (int i = 0; i < num_heartrate; i++){
        maternal_heartrate[i] = 90.0 + 10.0*sin((2.0*3.14*i)/(20*60*4));
        fetal_heartrate[i] = 150.0 + 15.0*sin((2.0*3.14*i)/(10*60*4));
        ua[i] = 70.0;
    }
    // plot ctg
    plot_ctg_paper(maternal_heartrate, fetal_heartrate, ua, num_heartrate);
    exit(0);
}
