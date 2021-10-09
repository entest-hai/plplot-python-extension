//=============================================================================
// 30 SEP 2021 TRAN MINH HAI
// simple test plplot
// build: gcc -o app test_plplot.c -I /usr/local/include/ -I /usr/local/include/plplot/ -lm -lgsl -lgslcblas -lplplot
//*****************************************************************************
// 07 OCT 2021 TRAN MINH HAI
// next step plot CTG simple grid
//*****************************************************************************
// 07 OCT 2021 TRAN MINH HAI
// TO DO: check pltimefmt("%H:%M"); and format CTG paper, margin
// First, check plbox and option x-axis show date time
// Second, check pltimefmt
//*****************************************************************************
// 08 OCT 2021 TRAN MINH HAI
// convert ps to png with white background and high resolution
// view port, window, box size, and page size
// view pdf is better than open ps file directly
// plspage interpretation depends on device drivers
// convert ps to png using gs
// gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r600 -sDEVICE=png16m -sOutputFile=foo.png grid.ps
// convert -density 300  grid.ps -background white -alpha remove -alpha off grid.png
//=============================================================================
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include <plConfig.h>
#include "plcdemos.h"
// =================================CONST======================================
static const PLINT BLOCK_SIZE_SEC = 600;       // BLOCK SIZE IN SECOND
static const int NSIZE = 101;                  // DATA LENGTH FOR SIMPLE PLOT
// ==============================test plot a grid==============================
int hello_plplot()
{
    // set output
    plsfnam("./grid.ps");
    // set printer or device
    plsdev("ps");
    // prepare data for plot
    PLFLT x[NSIZE], y[NSIZE];
    PLFLT xmin = 0., xmax = 1., ymin = 0., ymax = 100.;
    int   i;
    // prepare data for plot
    for ( i = 0; i < NSIZE; i++ )
    {
        x[i] = (PLFLT) ( i ) / (PLFLT) ( NSIZE - 1 );
        y[i] = ymax * x[i] * x[i];
    }
    // initialise plot
    plinit();
    // create environment both view port and window
    plenv( xmin, xmax, ymin, ymax, 0, 0 );
    pllab( "x", "y=100 x#u2#d", "Simple PLplot demo of a 2D line plot" );
    // plot data as line
    plline( NSIZE, x, y );
    // close plot
    plend();
    // exit
    exit( 0 );
}
// ================================create a simple grid =======================
int test_plot_simple_grid(){
    // simple data length number of the ten minute block
    int num_ten_min_block = 2;
    // total number of minute
    int num_min = num_ten_min_block * 10;
    // dpi
    int DPI = 80;
    // margin of ctg paper in minute
    int ctg_paper_left_margin_in_minute = 2;
    int ctg_paper_top_margin_in_minute = 2;
    // height of fhr in minute
    int fhr_height_in_minute = 10;
    // width of ctg paper in minute
    int ctg_paper_width_in_minute = num_min + 2*ctg_paper_left_margin_in_minute;
    // height of ctg paper in minute
    int ctg_paper_height_in_minute = fhr_height_in_minute + 2*ctg_paper_top_margin_in_minute;
    // width of ctg paper in pixel given DPI
    PLINT ctg_paper_width_in_pixel = (PLINT)DPI/2.54*ctg_paper_width_in_minute;
    PLINT ctg_paper_height_in_pixel = (PLINT)DPI/2.54*ctg_paper_height_in_minute;
    // set output
    plsfnam("./grid.ps");
    // set printer or device
    plsdev("ps");
    // set pen width
    plwidth(1.0);
    // set page 100 dpi, width 1000 pixel and height 500 pixel, 0 offset
    plspage(DPI,DPI,ctg_paper_height_in_pixel,ctg_paper_width_in_pixel,0,0);
    // initialise plot
    plinit();
    // set color
    plcol0(15);
    // set subpages
    pladv(0);
    // set view port using coordinate and aspect ration
    PLFLT xmin = 1.0*ctg_paper_left_margin_in_minute/ctg_paper_width_in_minute;
    PLFLT xmax = 1.0 - xmin;
    PLFLT ymin = 1.0*ctg_paper_top_margin_in_minute/ctg_paper_height_in_minute;
    PLFLT ymax = 1.0 - ymin;
    plvpas(xmin,xmax,ymin,ymax,0.0);
//     set view port absolute coordinate in mm
//    PLFLT xmin = 10;
//    PLFLT xmax = xmin + num_min*10.0;
//    PLFLT ymin = 10;
//    PLFLT ymax = ymin + ctg_paper_height_in_minute*10;
//    plsvpa(xmin,xmax,ymin,ymax);
    // setup window
    plwind(0.0,num_min*60.0,30.0,300.0);
    // time format
    pltimefmt("%H:%M");
    // set color
    plcol0(15);
    // set characters
    plschr(3.0, 1.0);
    // box and grid system
    plbox("ghdnitbc", BLOCK_SIZE_SEC,  20 * 1 , "ghbc", 20, 2); //180 1
    // end plot
    plend();
    // exit
    exit( 0 );
}
// ===================================== main =================================
int main(){
    test_plot_simple_grid();
    exit(0);
}