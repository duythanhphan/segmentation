/*
 ============================================================================
 Name        : myem.c
 Author      : duythanhphan
 Description : simple image segmentation by EM algorithms
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ELE 16
int samples[MAX_ELE] = { 5, 15, 40, 10,
                        40, 50, 80, 80,
                        80, 80, 40, 50,
                        10, 20, 40, 10 };

double P_C1[MAX_ELE];
double P_C2[MAX_ELE];

double sum_of_P_C1 = 0;
double sum_of_P_C2 = 0;

double u1, o1, p1;
double u2, o2, p2;

double prev_u1, prev_o1, prev_p1;
double prev_u2, prev_o2, prev_p2;

#define EPSILON     (0.1)
#define _2PI        (8 * atan(1))

double P_Xi_Cj(double x, double u, double o) {
    return ( ( 1 / (sqrt(_2PI) * o) ) * exp( (-1) * (x - u) * (x - u) / (2 * o * o) ) );
}

double P_Xi(int i) {
    return ((P_Xi_Cj(samples[i], u1, o1) * p1) + ((P_Xi_Cj(samples[i], u2, o2) * p2)));
}

void P_Cj_Xi(void) {
    int i = 0;

    // compute P_C1
    sum_of_P_C1 = 0;
    for (i = 0; i < MAX_ELE; i++) {
        P_C1[i] = (P_Xi_Cj(samples[i], u1, o1) * p1) / P_Xi(i);
        sum_of_P_C1 += P_C1[i];
    }

    // compute P_C2
    sum_of_P_C2 = 0;
    for (i = 0; i < MAX_ELE; i++) {
        P_C2[i] = (P_Xi_Cj(samples[i], u2, o2) * p2) / P_Xi(i);
        sum_of_P_C2 += P_C2[i];
    }
}

void expectation(void)  {
    P_Cj_Xi();
}

void maximization(void)  {
    int i = 0;

    // compute u1, o1, p1
    double sum_1_u = 0;
    double sum_1_o = 0;
    for (i = 0; i < MAX_ELE; i++) {
        sum_1_u += P_C1[i] * samples[i];
    }
    u1 = sum_1_u / sum_of_P_C1;

    for (i = 0; i < MAX_ELE; i++) {
        sum_1_o += P_C1[i] * (samples[i] - u1) * (samples[i] - u1);
    }
    o1 = sqrt(sum_1_o / sum_of_P_C1);

    p1 = sum_of_P_C1 / MAX_ELE;

    // compute u2, o2, p2
    double sum_2_u = 0;
    double sum_2_o = 0;
    for (i = 0; i < MAX_ELE; i++) {
        sum_2_u += P_C2[i] * samples[i];
    }
    u2 = sum_2_u / sum_of_P_C2;

    for (i = 0; i < MAX_ELE; i++) {
        sum_2_o += P_C2[i] * (samples[i] - u2) * (samples[i] - u2);
    }
    o2 = sqrt(sum_2_o / sum_of_P_C2);

    p2 = sum_of_P_C2 / MAX_ELE;
}

int check(void) {
    if ((fabs(u1 - prev_u1) < EPSILON) && (fabs(o1 - prev_o1) < EPSILON) &&
        (fabs(u2 - prev_u2) < EPSILON) && (fabs(o2 - prev_o2) < EPSILON) &&
        (fabs(p1 - prev_p1) < EPSILON) && (fabs(p2 - prev_p2) < EPSILON)) {
        return 1;
    } else {
        prev_p1 = p1; prev_p2 = p2;
        prev_u1 = u1; prev_u2 = u2;
        prev_o1 = o1; prev_o2 = o2;
        return 0;
    }
}

void init(void) {
    p1 = p2 = 0.5;
    u1 = 20; u2 = 50;
    o1 =  4; o2 =  4;

    prev_p1 = p1; prev_p2 = p2;
    prev_u1 = u1; prev_u2 = u2;
    prev_o1 = o1; prev_o2 = o2;

    int i = 0;
    for (i = 0; i < MAX_ELE; i++) {
        P_C1[i] = P_C2[i] = 0;
    }
}

int main(void) {
    int count = 0;
    int i = 0;

    init();

    printf("\nOriginal Image - > Array Data:\n");
    for (i = 0; i < MAX_ELE; i++) {
        printf("%4d ", samples[i]);
    }

    printf("\n\nINITIAL:\n");
    printf("%11s", " ");
    for (i = 0; i < MAX_ELE; i++) {
        printf("%4d ", samples[i]);
    }

    printf("\n%11s", "Cluster 1: ");
    for (i = 0; i < MAX_ELE; i++) {
        printf("%.2f ", P_C1[i]);
    }

    printf("\n%11s", "Cluster 2: ");
    for (i = 0; i < MAX_ELE; i++) {
        printf("%.2f ", P_C2[i]);
    }
    printf("\n\n");
    printf("Cluster 1(u1: %03.2f o1: %03.2f p1: %03.2f)\n", u1, o1, p1);
    printf("Cluster 2(u2: %03.2f o2: %03.2f p2: %03.2f)\n", u2, o2, p2);


    do {
        count++;
        printf("\n\nITER %d:\n", count);

        expectation();
        printf("%11s", " ");
        for (i = 0; i < MAX_ELE; i++) {
            printf("%4d ", samples[i]);
        }

        printf("\n%11s", "Cluster 1: ");
        for (i = 0; i < MAX_ELE; i++) {
            printf("%.2f ", P_C1[i]);
        }

        printf("\n%11s", "Cluster 2: ");
        for (i = 0; i < MAX_ELE; i++) {
            printf("%.2f ", P_C2[i]);
        }

        maximization();
        printf("\n\n");
        printf("Cluster 1(u1: %03.2f o1: %03.2f p1: %03.2f)\n", u1, o1, p1);
        printf("Cluster 2(u2: %03.2f o2: %03.2f p2: %03.2f)\n", u2, o2, p2);
    } while (!check());

    printf("\nSTOP AFTER %d ITERATION\n", count);

    printf("\nOriginal Image:\n");
    for (i = 0; i < MAX_ELE; i++) {
        if (i % 4 == 0) printf("\n");
        printf("%4d ", samples[i]);
    }

    printf("\n\nEM Segmentation Image:\n");
    for (i = 0; i < MAX_ELE; i++) {
        if (i % 4 == 0) printf("\n");
        if (P_C1[i] >= P_C2[i]) {
            printf("%4d ", 0);
        } else {
            printf("%4d ", 1);
        }
    }

    printf("\n");

    return 0;
}
