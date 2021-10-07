// 30 SEP 2021 TRAN MINH HAI
// simple test plplot
// build: gcc -o app test_plplot.c -I /usr/local/include/ -I /usr/local/include/plplot/ -lm -lgsl -lgslcblas -lplplot
// 07 OCT 2021 TRAN MINH HAI
// next step plot CTG simple grid

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include <plConfig.h>
#include "plcdemos.h"
#define NSIZE    101
//====================================Macro=====================================
// MACRO MEM ALLOCATION ERROR
#define CHECK_ALLOC_ERROR(P)                                              \
    if ((P) == NULL)                                                      \
    {                                                                     \
        printf("ERROR MEMORY ALLOCATION (%s:%d)\n", __FILE__, __LINE__);  \
        return NULL;                                                      \
    }
// MACRO CHECK ACCESS NULL POINTER
#define ERROR_OVERFLOW                                                   \
    {printf("ERROR OVERFLOW (%s:%d)\n", __FILE__, __LINE__); exit(-1);}  \
// MACRO CHECK ACCESS NULL POINTER
#define CHECK_ACCESS_NULL_POINTER(P)                                      \
    if((P)==NULL)                                                         \
    {                                                                     \
        printf("ERROR ACCESS NULL POINTER (%s:%d)\n", __FILE__, __LINE__);\
        exit(-1);                                                         \
    }
// MACRO CHECK ARRAY BOUND
#define CHECK_ARRAY_BOUND(i, imax)                                       \
    if((i) >= (imax))                                                    \
    {                                                                    \
        printf("ERROR ARRAY BOUND (%s:%d)\n", __FILE__, __LINE__);       \
        exit(-1);                                                        \
    }
//==================================== CONST====================================
// DEFINE FETAL HEART RATE OUTLIER
#define FHR_FBPM_LLIM		 	100		// LOWER LIMIT OF FETAL BPM
#define FHR_FBPM_ULIM 			200		// UPPER LIMIT OF FETAL BPM
#define FHR_FBPM_DLIM 			10       // DEVIATION LIMIT OF FETAL BPM
#define FHR_MBPM_LLIM 			60		// LOWER LIMIT OF MATERNAL BPM
#define FHR_MBPM_ULIM 			100 	// UPPER LIMIT OF MATERNAL BPM
#define FHR_MBPM_DLIM 			10 		// DEVIATION LIMIT OF MATERNAL BPM

#define FHR_UA_SCALE_MAD 10.0 				// TO SCALE UA FOR PLOTING
#define FHR_UA_MAX_AMP 	 100.0 				// MAX VALUE OF UA
#define FHR_UA_MIN_AMP   0.0

// STATIC CONST
static const PLINT SAMPLE_RATE = 4;            // SAMPLE RATE OF FHR AND UA

static const PLINT PAPER_SCALE = 1;            // PAPER SCALE
static const PLINT ANNOTATION = 0;
static const PLINT TOT_ROWS = 1;
static const PLINT TIME_MULT = 1;

static const PLINT BLOCK_SIZE_MM = 260;        // WIDTH AND HEIGHT OF A BLOCK
static const PLFLT BOX_SIZE_MM = 13;           // WIDTH OF SQUARE BOX
static const PLFLT DPI_VAL = 90.0;             // DPI FOR RESOULTION
static const PLFLT BLOCK_SIZE_MIN = 10;        // BLOCK SIZE IN MINUTE
static const PLINT BLOCK_SIZE_SEC = 600;       // BLOCK SIZE IN SECOND
static const PLFLT MAX_SIGLEN = 300000;        // MAX BLOCK SIZE IN SAMPLE
static const PLINT HEIGHT_SEPARATION[5] = {2, 6, 3, 17, 2}; // IN BOX SIZE

//==================================Data Type===================================
// DOUBLE_VECTOR
typedef struct {
    size_t len;
    double *p_data;
} double_vec;

// ACCEL AND DECEL (AD) STRUCT
typedef struct ad_point
{
	double x;
	double y;
	double z;
} ad_point;

// VECTOR OF AD (ACCEL AND DECEL)
typedef struct ad_point_vec
{
    ad_point *p_data;
    unsigned int len;
} ad_point_vec;

// BASELINE STRUCT
typedef struct  baseline
{
	double max_var;
	double mean_var;
	unsigned int ndecel;
	unsigned int naccel;
	double_vec *p_base;
	double_vec *p_fast;
	double_vec *p_slow;
	double_vec *p_diff;
	ad_point_vec *p_ad;
} baseline;

// =============================== Auxiliary Function==========================
void read_from_bin(char *path, double_vec *p)
{
    FILE *f_name = fopen(path, "rb");
    if (NULL == f_name)
    {
        printf("Error open file %s\n", path);
        exit(1);
    }
    else
    {
        fread(p->p_data, sizeof(double), p->len, f_name);
        fclose(f_name);
    }
}

/*------------------------------------------------------------------------------
* Function Name: double_vec_length
* Description  : get length of double vector
*-----------------------------------------------------------------------------*/
size_t double_vec_length(double_vec *p)
{
    CHECK_ACCESS_NULL_POINTER(p);
    return p->len;
}

/*------------------------------------------------------------------------------
* Function Name: read_from_txt
* Description  :
*-----------------------------------------------------------------------------*/
void read_from_txt_cols(const char* p_path, double_vec **p_buff, int ncol)
{

    FILE *p_file;
    p_file = fopen(p_path,"r");

    // check error open file
    CHECK_ACCESS_NULL_POINTER(p_file);

    double tmp_1, tmp_2, tmp_3, tmp_4, tmp_5, tmp_6 = 0.0;

    // 2 channel
    if (ncol == 2)
    {
            unsigned int siglen = p_buff[0]->len;
            for (unsigned int i = 0; i < siglen ; i++)
            {
                fscanf(p_file, "%lg %lg\n", &tmp_1, &tmp_2);
                p_buff[0]->p_data[i] = tmp_1;
                p_buff[1]->p_data[i] = tmp_2;
            }
    }

    // 3 channel
    if (ncol == 3)
    {
            unsigned int siglen = p_buff[0]->len;
            for (unsigned int i = 0; i < siglen ; i++)
            {
                fscanf(p_file, "%lg %lg %lg\n", &tmp_1, &tmp_2, &tmp_3);
                p_buff[0]->p_data[i] = tmp_1;
                p_buff[1]->p_data[i] = tmp_2;
                p_buff[2]->p_data[i] = tmp_3;
            }
    }

    // 4 channel
    if (ncol == 4)
    {
            unsigned int siglen = p_buff[0]->len;
            for (unsigned int i = 0; i < siglen ; i++)
            {
                fscanf(p_file, "%lg %lg %lg %lg\n", &tmp_1, &tmp_2, &tmp_3, &tmp_4);
                p_buff[0]->p_data[i] = tmp_1;
                p_buff[1]->p_data[i] = tmp_2;
                p_buff[2]->p_data[i] = tmp_3;
                p_buff[3]->p_data[i] = tmp_4;
            }
    }

    // 5 channel
    if (ncol == 5)
    {
            unsigned int siglen = p_buff[0]->len;
            for (unsigned int i = 0; i < siglen ; i++)
            {
                fscanf(p_file, "%lg %lg %lg %lg %lg\n", &tmp_1, &tmp_2, &tmp_3, &tmp_4, &tmp_5);
                p_buff[0]->p_data[i] = tmp_1;
                p_buff[1]->p_data[i] = tmp_2;
                p_buff[2]->p_data[i] = tmp_3;
                p_buff[3]->p_data[i] = tmp_4;
                p_buff[4]->p_data[i] = tmp_5;
            }
    }

    // 6 channel
    if (ncol == 6)
    {
            unsigned int siglen = p_buff[0]->len;
            for (unsigned int i = 0; i < siglen ; i++)
            {
                fscanf(p_file, "%lg %lg %lg %lg %lg %lg\n", &tmp_1, &tmp_2,
                    &tmp_3, &tmp_4, &tmp_5, &tmp_6);
                p_buff[0]->p_data[i] = tmp_1;
                p_buff[1]->p_data[i] = tmp_2;
                p_buff[2]->p_data[i] = tmp_3;
                p_buff[3]->p_data[i] = tmp_4;
                p_buff[4]->p_data[i] = tmp_5;
                p_buff[5]->p_data[i] = tmp_6;
            }
    }

}

/*------------------------------------------------------------------------------
* Function Name: double_vec_set
* Description  : insert a number to vector at location idx
*-----------------------------------------------------------------------------*/
void double_vec_set(double_vec *p, size_t idx, double x)
{
    // error access NULL pointer
    CHECK_ACCESS_NULL_POINTER(p);

    // error access over bound array
    CHECK_ARRAY_BOUND(idx, p->len);

    // correct index
    p->p_data[idx] = x;

}

/*------------------------------------------------------------------------------
* Function Name: double_vec_read
* Description  : read from txt
*-----------------------------------------------------------------------------*/

void double_vec_read(double_vec *p, const char *p_path)
{
    FILE *p_file;
    p_file = fopen(p_path,"r");

    // TODO handle open file error

    // read from txt to double vec
    double tmp = 0.0;

    for(size_t i = 0; i < double_vec_length(p); i++)
    {
        fscanf(p_file, "%lg", &tmp);
        double_vec_set(p, i, tmp);
    }
}

/*------------------------------------------------------------------------------
* Function Name: double_vec_new
* Description  : create a vector of double given len and init to ZERO
*-----------------------------------------------------------------------------*/
double_vec * double_vec_new(size_t len)
{
    if (len == 0 || len > SIZE_MAX / sizeof(double))
    {
        ERROR_OVERFLOW;
        return NULL;
    }
    double_vec *p_vec = malloc(sizeof(double_vec));
    CHECK_ALLOC_ERROR(p_vec);

    // allocate and init to zero by calloc
    p_vec->len = len;
    p_vec->p_data = calloc(len, sizeof(double));
    CHECK_ALLOC_ERROR(p_vec->p_data);

    #ifdef BIORITHM_AUX_DEBUG
    // printf("length = %ul\n", p_vec->len);
    #endif

    return p_vec;
}
// ============================== Render CTG Function =========================
int render_img(unsigned int trace_speed, double_vec *p_fhr, double_vec *p_mhr,
baseline *p_fbase, double_vec *p_ua, const char *p_path)
{

    // SIGLEN IN TERM OF NUMBER OF SAMPLE EACH 250ms
    unsigned int siglen = p_fhr->len;

    // SETUP DEVICE OUTPUT
    plsdev("psc");

    // SETUP FILE PATH FOR OUTPUT TRACE
    char *filename = malloc(strlen(p_path) + 50);
    sprintf(filename, "%s%s%d%s%s", p_path, "trace_", trace_speed, "cm", ".ps");
    printf("filename = %s\n", filename);
    plsfnam(filename);

    // SETUP PARAMETER FOR PPLOT
    int num_blk = 0;
    int num_row = 0;
    int num_time_blk = 0;
    int paper_len = 0;
    int paper_height = 0;
    int row_length = 0;
    int sample_blk = 0;

    // NUMBER OF SAMPLE PER BLOCK OF 10 MINUTES
    sample_blk = SAMPLE_RATE * BLOCK_SIZE_SEC;

    //  NUMBER OF BLOCK EACH 10 MINUTES
    num_time_blk = ceil(p_fhr->len / sample_blk);

    // NUMBER OF ROW DEFAULT 1
    paper_len    = BLOCK_SIZE_MM * num_time_blk * trace_speed;
    paper_height = BOX_SIZE_MM * (HEIGHT_SEPARATION[0] + HEIGHT_SEPARATION[1]
    + HEIGHT_SEPARATION[2] + HEIGHT_SEPARATION[3] + HEIGHT_SEPARATION[4]);

    #ifdef RENDER_DEBUG
    // printf("tracespeed = %d, nblock = %d, paperlen = %d, paperheight = %d\n",
     // trace_speed, num_time_blk, paper_len, paper_height);
    #endif

    // DEFINE VIEWPORT FOR FHR AND MHR
    PLFLT xminv1 =  (4.0 * BOX_SIZE_MM) / paper_len;
    PLFLT xmaxv1 = 1.0 - (4.0 * BOX_SIZE_MM) / paper_len;
    PLFLT yminv1 = (1.0 * BOX_SIZE_MM * (HEIGHT_SEPARATION[0] + HEIGHT_SEPARATION[1]
    + HEIGHT_SEPARATION[2])) / paper_height;
    PLFLT ymaxv1 = (1.0 * BOX_SIZE_MM * (HEIGHT_SEPARATION[0] + HEIGHT_SEPARATION[1]
    + HEIGHT_SEPARATION[3]))/ paper_height;

    // DEFINE VIEWPORT FOR UA
    PLFLT xminv2 = xminv1;
    PLFLT xmaxv2 = xmaxv1;
    PLFLT yminv2 = (1.0 * BOX_SIZE_MM * (HEIGHT_SEPARATION[0]))/ paper_height;
    PLFLT ymaxv2 = (1.0 * BOX_SIZE_MM * (HEIGHT_SEPARATION[0]
    + HEIGHT_SEPARATION[1])) / paper_height;

    // WIDTH of PLPTEX
    PLFLT pltexw = BLOCK_SIZE_SEC / (20.0 * trace_speed) ;

    // DEFINE VIEWPORT FOR MARK CONNECT
    PLFLT xminv3 = xminv1 - BOX_SIZE_MM / paper_len;
    PLFLT xmaxv3 = xminv1;
    PLFLT yminv3 = yminv2;
    PLFLT ymaxv3 = ymaxv1;

    // SET COLOR
    plscol0(0, 255, 255, 255); /* White, color 0, background */
    plscol0(15, 0, 0, 0);      /* Black, color 15 */

    // GET PAGE
    PLFLT xp0, yp0;
    PLINT xleng0, yleng0, xoff0, yoff0;

    // SET PAGE
    plspage(DPI_VAL, DPI_VAL, paper_height, paper_len, 0, 0);

    // INIT PLPLOT
    plinit();

    // DATA BUFFER FOR PLOT TODO: MORE MEANING VARIABLE NAME
    PLFLT *x  = malloc(sizeof(PLFLT) * (siglen));  // TIME INDEX
    PLFLT *y  = malloc(sizeof(PLFLT) * (siglen));  // FETAL HEART RATE
    PLFLT *y2 = malloc(sizeof(PLFLT) * (siglen));  // FETAL BASE LINE
    PLFLT *y3 = malloc(sizeof(PLFLT) * (siglen));  // MATERNAL HEART RATE
    PLFLT *to = malloc(sizeof(PLFLT) * (siglen));  // MATERNAL UA

    // TODO: DOUBLE CHECK
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // ANNOTATION STRING
    char connect_string[255];

    // TODO: DOUBLE CHECK
    pladv(0); //Advance the (sub-)page

    // PREPARE FETAL HEAR TRATE DATA FOR PLPLOT
    for (unsigned int i = 0; i < siglen; i++)
    {
        // CONVERT SAMPLE INDEX TO TIME IN MINUTE OR SECOND
        x[i]  = (1.0 * i) / (SAMPLE_RATE * TIME_MULT);

        // FETAL HEART RATE
        y[i]  = p_fhr->p_data[i];

        // FETAL BASELINE
        y2[i] = p_fbase->p_base->p_data[i];

        // UA
        to[i] = p_ua->p_data[i];
    }

    // AXIS LIMIT OR RANGE
    PLFLT xmin_fhr = 0.0;               // TIME MIN
    PLFLT xmax_fhr = x[siglen -1];      // TIME MAX
    PLFLT ymin_fhr = 50.0;              // HEART RATE MIN
    PLFLT ymax_fhr = 220.0;             // HEART RATE MAX

    PLFLT xmin_ua = 0.0;                // STARTING TIME UA
    PLFLT xmax_ua = x[siglen -1]; ;     // END TIME UA
    PLFLT ymin_ua = 0;                  // MIN AMPLITUDE UA
    PLFLT ymax_ua = 120;                // MAX AMPLITUDE UA

    #ifdef RENDER_DEBUG
    printf("xmin=%lg,xmax=%lg,ymin=%lg,ymax=%lg\n",xmin_fhr, xmax_fhr,
    ymin_fhr, ymax_fhr );
    #endif

    // BUFFER FOR 2 POINTS
    PLFLT lx[2], ly[2];

    //
    for (num_row = 0; num_row < TOT_ROWS; num_row++)
    {
        plwidth(3);
        // DEFINE VIEWPORT FOR FHR AND MHR
        plvpas(xminv1, xmaxv1, yminv1, ymaxv1, 0.00);
        // plvpor(xminv1, xmaxv1, yminv1, ymaxv1);

        // DEFINE WINDOW FOR FHR AND MHR
        plwind(xmin_fhr, xmax_fhr, ymin_fhr, ymax_fhr);

        //SET CHARACTER SIZE
        plschr(3.0, 1.0);

        // SET COLOR
        plcol0(15);

        // TIME FORMAT
        pltimefmt("%H:%M");

        // SET CORLOR
        plcol0(15);

        plschr(3.0, 1.0);

        plbox("ghdnitbc", BLOCK_SIZE_SEC,  20 * trace_speed , "ghbc", 20, 2); //180 1

        pllab("","","FHR");

        // SET LINE STYLE
        pllsty(1);
        plwidth(5);
        plcol0(15);

        // LOOOP THROUGH ALL FHR SAMPLES TO PLOT FHR
        for (int i = 0; i < siglen - 1; i++)
        {
            lx[0] = x[i];
            lx[1] = x[i + 1];
            ly[0] = y[i];
            ly[1] = y[i + 1];

            // SKIP IF EITHER IS NAN
            if (ly[0] < FHR_FBPM_ULIM && ly[0] > FHR_FBPM_LLIM &&
                ly[1] < FHR_FBPM_ULIM && ly[1] > FHR_FBPM_LLIM &&
                fabs(ly[1]-ly[0]) < FHR_FBPM_DLIM)
            {
                // plline(2, lx, ly);
                plpoin(2, lx, ly, 1);
            }
        }

        // LOOOP THROUGH ALL FHR SAMPLES TO PLOT FHR BASELINE
        pllsty(1);
        plwidth(3);
        plcol0(1);

        for (int i = 0; i < siglen - 1; i++)
        {
            lx[0] = x[i];
            lx[1] = x[i + 1];
            ly[0] = y2[i];
            ly[1] = y2[i + 1];

            // SKIP IF EITHER IS NAN
            if (ly[0] < FHR_FBPM_ULIM && ly[0] > FHR_FBPM_LLIM &&
                ly[1] < FHR_FBPM_ULIM && ly[1] > FHR_FBPM_LLIM)
            {
                plline(2, lx, ly);
                // plpoin(2, lx, ly, 1);
            }
        }

        // SET COLOR
        plcol0(5);
        plwidth(3);

        // SET CHARACTER SIZE
        plschr(3.0, 1.0);
        float coord_lab;

        // PLOT PLPTEX FOR Y AXIS
        while (num_blk <= num_time_blk)
        {
            //TODO:
            coord_lab = 0.09 + num_blk * BLOCK_SIZE_SEC; //0.12

            // WITH OF PLPTEX
            PLFLT lab_px[] = {coord_lab, coord_lab,
            coord_lab + pltexw, coord_lab + pltexw};

            PLFLT lab_py[] = {ymin_fhr + 1, ymax_fhr - 1, ymax_fhr - 1, ymin_fhr + 1};

            // SET COLOR
            plcol0(7);

            // FILL COLOR
            plfill(4, lab_px, lab_py);

            // SET CHAR
            plschr(3.0, 1);

            // SET COLOR
            plcol0(15);

            //
            #ifdef RENDER_DEBUG
            printf("coord_lab = %3.15f\n", coord_lab);
            #endif

            plptex(coord_lab, 60, 0, 0, 0, "60");
            plptex(coord_lab, 80, 0, 0, 0, "80");
            plptex(coord_lab, 100, 0, 0, 0, "100");
            plptex(coord_lab, 120, 0, 0, 0, "120");
            plptex(coord_lab, 140, 0, 0, 0, "140");
            plptex(coord_lab, 160, 0, 0, 0, "160");
            plptex(coord_lab, 180, 0, 0, 0, "180");
            plptex(coord_lab, 200, 0, 0, 0, "200");
            plschr(3.0, 1.0);

            num_blk++;
        }

        // ADD VERTIAL LINE IN CASE OF PLBOX NOT SO GOOD
        lx[0] = coord_lab;
        lx[1] = coord_lab;
        ly[0] = ymin_fhr;
        ly[1] = ymax_fhr;
        plline(2,lx,ly);

        //============================  PLOT UA ================================
        plcol0(15);
        plschr(3.0, 1.0);
        plwidth(3);
        // DEFINE VIEWPORT FOR UA
        plvpas(xminv2, xmaxv2, yminv2, ymaxv2, 0.00);

        // DEFINE WINDOW FOR UA
        plwind(xmin_ua, xmax_ua, ymin_ua, ymax_ua);

        plbox("ghdnitbc", BLOCK_SIZE_SEC, 20 * trace_speed, "ghbc",
        20, 1);

        // LOOOP THROUGH ALL FHR SAMPLES TO PLOT FHR
        plwidth(5);
        for (int i = 0; i < siglen - 1; i++)
        {
            lx[0] = x[i];
            lx[1] = x[i + 1];
            ly[0] = to[i];
            ly[1] = to[i + 1];

            // SKIP IF EITHER IS NAN
            if (ly[0] < FHR_UA_MAX_AMP && ly[0] > FHR_UA_MIN_AMP &&
                ly[1] < FHR_UA_MAX_AMP && ly[1] > FHR_UA_MIN_AMP)
            {
                plline(2, lx, ly);
            }
        }

        plwidth(3);
        pllab("Time (minute)","","");

        // PLTEX MARK YAXIS STICK
        PLFLT lab_px[] = {0.09, 0.09, 0.09 + pltexw, 0.09 + pltexw};
        PLFLT lab_py[] = {1, 121, 121, 1};
        plcol0(7);
        plfill(4, lab_px, lab_py);

        plcol0(15);
        plschr(3.0, 1.0);
        plptex(0, 20,  0, 0, 0,  "20");
        plptex(0, 40,  0, 0, 0,  "40");
        plptex(0, 60,  0, 0, 0,  "60");
        plptex(0, 80,  0, 0, 0,  "80");
        plptex(0, 100, 0, 0, 0, "100");

        // ADD VERTIAL LINE IN CASE OF PLBOX NOT SO GOOD
        lx[0] = coord_lab;
        lx[1] = coord_lab;
        ly[0] = ymin_ua;
        ly[1] = ymax_ua;
        plline(2,lx,ly);

        // CONNECT MARK
        plvpas(xminv3, xmaxv3, yminv3, ymaxv3, 0.0);

        plwind(0, 1, 0, 1);
        plbox("", 1, 1, "", 1, 1);
        PLFLT px_label_xaxis[] = {0, 0, 1, 1};
        PLFLT py_label_yaxis[] = {0, 1, 1, 0};

        plcol0(3);
        plfill(4, px_label_xaxis, py_label_yaxis);

        plschr(3.0, 1);
        plcol0(15);
        sprintf(connect_string, "CONNECT %d-%02d-%02d 00:00", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        plptex(0.5, 0.5, 0, -1, 0.5, connect_string);

    }

    // Close PLplot library
    plend();
    printf("PLOT END\n");

    // FREE MEM
    free(x);
    free(y);
    free(y2);
    free(y3);
    free(filename);

    // RETURN
    return 1;
}

// ============================= test plot a simple graph======================
int testPlotSimpleGrap()
{
    // Set image name
    plsfnam("./test.ps");
    // Setup printer
    plsdev("ps");
    // Prepare data for plot
    PLFLT x[NSIZE], y[NSIZE];
    PLFLT xmin = 0., xmax = 1., ymin = 0., ymax = 100.;
    int   i;
    // Prepare data to be plotted.
    for ( i = 0; i < NSIZE; i++ )
    {
        x[i] = (PLFLT) ( i ) / (PLFLT) ( NSIZE - 1 );
        y[i] = ymax * x[i] * x[i];
    }
    // Initialize plplot
    plinit();
    // Create a labelled box to hold the plot.
    plenv( xmin, xmax, ymin, ymax, 0, 0 );
    pllab( "x", "y=100 x#u2#d", "Simple PLplot demo of a 2D line plot" );
    // Plot the data that was prepared above.
    plline( NSIZE, x, y );
    // Close PLplot library
    plend();
    // Exit
    exit( 0 );
}

// ============================== test plot CTG paper grid=====================
int testPlotCtgPaper()
{

    const char *p_testcase = "NUH1021";

    if (strcmp(p_testcase,"NUH1021")==0)
    {
        // LOAD INPUT
        unsigned int siglen = 13362;
        double_vec *p_fhr   = double_vec_new(siglen);
        double_vec *p_mhr   = double_vec_new(siglen);
        double_vec *p_ua    = double_vec_new(siglen);
        baseline *p_fbase = malloc(sizeof(baseline));

        //
        double_vec **pp_mfhr = malloc(5 * sizeof(double_vec *));
        pp_mfhr[0] = double_vec_new(siglen);
        pp_mfhr[1] = double_vec_new(siglen);
        pp_mfhr[2] = double_vec_new(siglen);
        pp_mfhr[3] = double_vec_new(siglen);
        pp_mfhr[4] = double_vec_new(siglen);

        read_from_txt_cols("./../data/fheartrate_trace_all.txt", pp_mfhr, 5);
        double_vec_read(p_fhr, "./../data/baseline_ch5.txt");
//         double_vec_read(p_mhr, "./data/BASELINE_CH_4.txt");
//         double_vec_read(p_ua, "./data/BASELINE_CH_4.txt");

        #ifdef TEST_RENDER_PRINT
        // double_vec_print(p_fhr,10);
        double_vec_print(pp_mfhr[3],100);
        #endif

        p_fbase->max_var = 10.00;
        p_fbase->mean_var = 0.01;
        p_fbase->ndecel  = 10;
        p_fbase->naccel  = 20;
        p_fbase->p_base  = p_fhr;
        p_fbase->p_fast  = p_fhr;
        p_fbase->p_slow  = p_fhr;
        p_fbase->p_diff  = p_fhr;
        p_fbase->p_ad    = NULL;

        // RENDER IMAGE
        render_img(3, pp_mfhr[3], p_mhr, p_fbase, p_ua,
            "./trace");

        render_img(2, pp_mfhr[3], p_mhr, p_fbase, p_ua,
            "./trace");

        render_img(1, pp_mfhr[3], p_mhr, p_fbase, p_ua,
            "./trace");

    }

    if (strcmp(p_testcase,"FHRBIN")==0)
    {
        // LOAD INPUT
        unsigned int siglen = 10140;
        double_vec *p_fhr   = double_vec_new(siglen);
        double_vec *p_mhr   = double_vec_new(siglen);
        double_vec *p_ua    = double_vec_new(siglen);
        baseline *p_fbase   = malloc(sizeof(baseline));

        p_fbase->max_var = 10.00;
        p_fbase->mean_var = 0.01;
        p_fbase->ndecel  = 10;
        p_fbase->naccel  = 20;
        p_fbase->p_base  = p_fhr;
        p_fbase->p_fast  = p_fhr;
        p_fbase->p_slow  = p_fhr;
        p_fbase->p_diff  = p_fhr;
        p_fbase->p_ad    = NULL;

        read_from_bin("./../test/data/fhr.bin", p_fhr);
        read_from_bin("./../test/data/fhr.bin", p_mhr);
        read_from_bin("./../test/data/fhr.bin", p_ua);

        #ifdef TEST_RENDER_PRINT
        double_vec_print(p_fhr,10);
        #endif

        // RENDER IMAGE
        render_img(1, p_fhr, p_mhr, p_fbase, p_ua,
            "./../test/data/trace");

        render_img(2, p_fhr, p_mhr, p_fbase, p_ua,
            "./../test/data/trace");

        render_img(3, p_fhr, p_mhr, p_fbase, p_ua,
            "./../test/data/trace");
    }


    if (strcmp(p_testcase, "CTU1001")==0)
    {

        // LOAD INPUT
        unsigned int siglen = 5000;
        double_vec *p_fhr   = double_vec_new(siglen);
        double_vec *p_mhr   = double_vec_new(siglen);
        double_vec *p_ua    = double_vec_new(siglen);
        baseline *p_fbase   = malloc(sizeof(baseline));
        p_fbase->p_base = double_vec_new(siglen);


        double_vec_read(p_fhr,   "./../test/data/CTU_1001m.txt");
        double_vec_read(p_mhr,    "./../test/data/CTU_1001m.txt");
        double_vec_read(p_ua,     "./../test/data/CTU_1001m.txt");
        double_vec_read(p_fbase->p_base,  "./../test/data/CTU101Baseline.txt");

        p_fbase->max_var = 10.00;
        p_fbase->mean_var = 0.01;
        p_fbase->ndecel  = 10;
        p_fbase->naccel  = 20;
        // p_fbase->p_base  = p_fhr;
        p_fbase->p_fast  = p_fhr;
        p_fbase->p_slow  = p_fhr;
        p_fbase->p_diff  = p_fhr;
        p_fbase->p_ad    = NULL;

        #ifdef TEST_RENDER_PRINT
        double_vec_print(p_fhr,10);
        #endif

        // RENDER IMAGE
        render_img(1, p_fhr, p_mhr, p_fbase, p_ua,
            "./../test/data/trace");

        render_img(2, p_fhr, p_mhr, p_fbase, p_ua,
            "./../test/data/trace");

        render_img(3, p_fhr, p_mhr, p_fbase, p_ua,
            "./../test/data/trace");
    }
    return 0;
}

// ==============================test plot a grid==============================
int testPlotGrid(){
    // SIGLEN IN TERM OF NUMBER OF SAMPLE EACH 250ms
    int trace_speed = 1;
    unsigned int siglen = 2400*3;
    // SETUP DEVICE OUTPUT
    plsdev("psc");
    plsfnam("./grid.ps");
    // SETUP PARAMETER FOR PPLOT
    int num_time_blk = 0;
    int paper_len = 0;
    int paper_height = 0;
    int row_length = 0;
    int sample_blk = 0;
    // NUMBER OF SAMPLE PER BLOCK OF 10 MINUTES
    sample_blk = SAMPLE_RATE * BLOCK_SIZE_SEC;
    //  NUMBER OF BLOCK EACH 10 MINUTES
    num_time_blk = ceil(siglen / sample_blk);
    // NUMBER OF ROW DEFAULT 1
    paper_len    = BLOCK_SIZE_MM * num_time_blk * trace_speed;
    paper_height = BOX_SIZE_MM * (HEIGHT_SEPARATION[0] + HEIGHT_SEPARATION[1] + HEIGHT_SEPARATION[2] + HEIGHT_SEPARATION[3] + HEIGHT_SEPARATION[4]);
    // DEFINE VIEWPORT FOR FHR AND MHR
    PLFLT xminv1 =  (4.0 * BOX_SIZE_MM) / paper_len;
    PLFLT xmaxv1 = 1.0 - (4.0 * BOX_SIZE_MM) / paper_len;
    PLFLT yminv1 = (1.0 * BOX_SIZE_MM * (HEIGHT_SEPARATION[0] + HEIGHT_SEPARATION[1] + HEIGHT_SEPARATION[2])) / paper_height;
    PLFLT ymaxv1 = (1.0 * BOX_SIZE_MM * (HEIGHT_SEPARATION[0] + HEIGHT_SEPARATION[1] + HEIGHT_SEPARATION[3]))/ paper_height;
    // SET COLOR
    plscol0(0, 255, 255, 255); /* White, color 0, background */
    plscol0(15, 0, 0, 0);      /* Black, color 15 */
    // SET PAGE
    plspage(DPI_VAL, DPI_VAL, paper_height, paper_len, 0, 0);
    // INIT PLPLOT
    plinit();
    pladv(0); //Advance the (sub-)page
    // AXIS LIMIT OR RANGE
    PLFLT xmin_fhr = 0.0;               // TIME MIN
    PLFLT xmax_fhr = num_time_blk * 10.0 * 60.0;      // TIME MAX
    PLFLT ymin_fhr = 50.0;              // HEART RATE MIN
    PLFLT ymax_fhr = 220.0;             // HEART RATE MAX
    //
    plwidth(1);
    // DEFINE VIEWPORT FOR FHR AND MHR
    plvpas(xminv1, xmaxv1, yminv1, ymaxv1, 0.00);
    // DEFINE WINDOW FOR FHR AND MHR
    plwind(xmin_fhr, xmax_fhr, ymin_fhr, ymax_fhr);
    //SET CHARACTER SIZE
//    plschr(3.0, 1.0);
    // SET COLOR
//    plcol0(15);
    // TIME FORMAT
    pltimefmt("%H:%M");
    // SET CORLOR
    plcol0(15);
    plschr(3.0, 1.0);
    plbox("ghdnitbc", BLOCK_SIZE_SEC,  20 * trace_speed , "ghbc", 20, 2); //180 1
    // End plotting
    plend();
    // Exit
    exit(0);
}
// ===================================== main =================================
int main(){
//    testPlotSimpleGrap();
//    testPlotCtgPaper();
    testPlotGrid();
    exit(0);
}