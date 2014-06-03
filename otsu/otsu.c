/*
 ============================================================================
 Name        : otsu.c
 Author      : duythanhphan
 Version     : 0.1
 Copyright   :
 Description : otsu's method
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ELENUM      25
static int d[] = {  100, 100, 100, 100, 100,
                     50,  80, 110,  80,  50,
                     50,  80, 110,  80,  50,
                     50,  80, 110,  80,  50,
                    100, 100, 100, 100, 100  };

static int total = ELENUM;
static int histogram[256];

double otsu(void) {
    int sum = 0;
    int i;

    for (i = 1; i < 256; ++i) {
        sum += i * histogram[i];
    }

    int sumB = 0;
    int wB = 0;
    int wF = 0;
    int mB;
    int mF;

    double max = 0.0;
    double between = 0.0;
    double threshold1 = 0.0;
    double threshold2 = 0.0;

    for (i = 0; i < 256; ++i) {
        // accumulated probabilities
        wB += histogram[i];
        if (wB == 0) continue;

        //
        wF = total - wB;
        if (wF == 0) break;

        sumB += i * histogram[i];
        mB = sumB / wB;
        mF = (sum - sumB) / wF;

        between = wB * wF * pow(mB - mF, 2);

        if ( between >= max ) {
            threshold1 = i;
            if ( between > max ) {
                threshold2 = i;
            }
            max = between;
        }
    }

    return ( threshold1 + threshold2 ) / 2.0;
}

static void compute_hist(void) {
    int i;

    // initialize 0
    for (i = 0; i < 256; ++i) {
        histogram[i] = 0;
    }

    // update histogram
    for (i = 0; i < ELENUM; ++i) {
        histogram[d[i]]++;
    }
}

int main(int argc, char **argv) {
    int i;

    printf("The number of elements: %d\n", ELENUM);
    printf("Elements:\n");
    for(i = 0; i < ELENUM; ++i) {
        printf("%d ", d[i]);
    }
    printf("\n");

    compute_hist();
    printf("Threshold: %f\n", otsu());

    return 0;
}
