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
// hello plplot
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

// transparent background
static PLINT  red[] = { 0, 255, 0, 0 };
static PLINT  green[] = { 0, 0, 255, 0 };
static PLINT  blue[] = { 0, 0, 0, 255 };
static PLFLT  alpha[] = { 1.0, 1.0, 1.0, 1.0 };
static PLFLT  px[] = { 0.1, 0.5, 0.5, 0.1 };
static PLFLT  py[] = { 0.1, 0.1, 0.5, 0.5 };
static PLFLT  pos[] = { 0.0, 1.0 };
static PLFLT  rcoord[] = { 1.0, 1.0 };
static PLFLT  gcoord[] = { 0.0, 0.0 };
static PLFLT  bcoord[] = { 0.0, 0.0 };
static PLFLT  acoord[] = { 0.0, 1.0 };
static PLBOOL rev[] = { 0, 0 };

int main(int argc, char *argv[] ){
    int   i, j;
    PLINT icol, r, g, b;
    PLFLT a;

    plparseopts( &argc, argv, PL_PARSE_FULL );

    plinit();
    plscmap0n( 4 );
    plscmap0a( red, green, blue, alpha, 4 );

    //
    // Page 1:
    //
    // This is a series of red, green and blue rectangles overlaid
    // on each other with gradually increasing transparency.
    //

    // Set up the window
    pladv( 0 );
    plvpor( 0.0, 1.0, 0.0, 1.0 );
    plwind( 0.0, 1.0, 0.0, 1.0 );
    plcol0( 0 );
    plbox( "", 1.0, 0, "", 1.0, 0 );

    // Draw the boxes
    for ( i = 0; i < 9; i++ )
    {
        icol = i % 3 + 1;

        // Get a color, change its transparency and
        // set it as the current color.
        plgcol0a( icol, &r, &g, &b, &a );
        plscol0a( icol, r, g, b, 1.0 - (PLFLT) i / 9.0 );
        plcol0( icol );

        // Draw the rectangle
        plfill( 4, px, py );

        // Shift the rectangles coordinates
        for ( j = 0; j < 4; j++ )
        {
            px[j] += 0.5 / 9.0;
            py[j] += 0.5 / 9.0;
        }
    }

    //
    // Page 2:
    //
    // This is a bunch of boxes colored red, green or blue with a single
    // large (red) box of linearly varying transparency overlaid. The
    // overlaid box is completely transparent at the bottom and completely
    // opaque at the top.
    //

    // Set up the window
    pladv( 0 );
    plvpor( 0.1, 0.9, 0.1, 0.9 );
    plwind( 0.0, 1.0, 0.0, 1.0 );

    // Draw the boxes. There are 25 of them drawn on a 5 x 5 grid.
    for ( i = 0; i < 5; i++ )
    {
        // Set box X position
        px[0] = 0.05 + 0.2 * i;
        px[1] = px[0] + 0.1;
        px[2] = px[1];
        px[3] = px[0];

        // We don't want the boxes to be transparent, so since we changed
        // the colors transparencies in the first example we have to change
        // the transparencies back to completely opaque.
        icol = i % 3 + 1;
        plgcol0a( icol, &r, &g, &b, &a );
        plscol0a( icol, r, g, b, 1.0 );
        plcol0( icol );
        for ( j = 0; j < 5; j++ )
        {
            // Set box y position and draw the box.
            py[0] = 0.05 + 0.2 * j;
            py[1] = py[0];
            py[2] = py[0] + 0.1;
            py[3] = py[2];
            plfill( 4, px, py );
        }
    }

    // Create the color map with 128 colors and use plscmap1la to initialize
    // the color values with a linearly varying red transparency (or alpha)
    plscmap1n( 128 );
    plscmap1la( 1, 2, pos, rcoord, gcoord, bcoord, acoord, rev );

    // Use that cmap1 to create a transparent red gradient for the whole
    // window.
    px[0] = 0.;
    px[1] = 1.;
    px[2] = 1.;
    px[3] = 0.;

    py[0] = 0.;
    py[1] = 0.;
    py[2] = 1.;
    py[3] = 1.;

    plgradient( 4, px, py, 90. );

    plend();
    exit( 0 );
}
