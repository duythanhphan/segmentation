/*
 ============================================================================
 Name        : otsu.c
 Author      : duythanhphan
 Description : otsu's method
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ELENUM      16
static int d[] = {  5, 15, 40, 10,
                    40, 50, 80, 80,
                    80, 80, 40, 50,
                    10, 20, 40, 10 };

static int total = ELENUM;
static int histogram[256];

double otsu(void) {
    int sum = 0;
    int i;

    for (i = 1; i < 256; ++i) {
        sum += i * histogram[i];
    }
    printf("sum=%05d\n", sum);

    int sumB = 0;
    int wB = 0;
    int wF = 0;
    int mB;
    int mF;

    double max = 0.0;
    double between = 0.0;
    double threshold1 = 0.0;
    double threshold2 = 0.0;

    printf("\n");
    for (i = 0; i < 256; ++i) {
        wB += histogram[i];
        if (wB == 0) continue;
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

        if (histogram[i] != 0) {
            printf ("i=%03d ", i);
            printf ("wB=%05d ", wB);
            printf ("wF=%05d ", wF);
            printf ("sumB=%05d ", sumB);
            printf ("mB=%05d ", mB);
            printf ("mF=%05d ", mF);
            printf ("between=%08f ", between);
            printf ("max=%08f ", max);
            printf ("t1=%07f ", threshold1);
            printf ("t2=%07f ", threshold2);
            printf ("\n");
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
