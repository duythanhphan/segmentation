/*
 ============================================================================
 Name        : kmean.c
 Author      : duythanhphan
 Version     : 0.1
 Copyright   :
 Description : k-mean clustering
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define ELENUM      25
#define CLUSNUM     3

static int n = ELENUM;
static int p = CLUSNUM;
static int d[] = {  100, 100, 100, 100, 100,
                     50,  80, 200,  80,  50,
                     50,  80, 200,  80,  50,
                     50,  80, 200,  80,  50,
                    100, 100, 100, 100, 100  };

static int k[CLUSNUM][ELENUM];
static int tempk[CLUSNUM][ELENUM];
static double m[CLUSNUM] = {60, 120, 200} ;
static double diff[CLUSNUM];

static int count1, count2, count3;

// This method will determine the cluster in which an element go at a particular step.
static int cal_diff(int a) {
    int i;

    for(i = 0; i < p; ++i) {
        diff[i] = (a > m[i]) ? a - m[i] : m[i] - a;
    }

    int val = 0;
    double temp = diff[0];
    for (i = 0; i < p; ++i) {
        if (diff[i] < temp) {
            temp = diff[i];
            val = i;
        }
    }

    return val;
}

// This method will determine intermediate mean values
static void cal_mean(void) {
    int i, j;

    for(i = 0; i < p; ++i) {
        m[i] = 0; // initializing means to 0
    }

    int cnt = 0;
    for(i = 0; i < p; ++i) {
        cnt = 0;
        for(j = 0; j < n - 1; ++j) {
            if(k[i][j] != -1) {
                m[i] += k[i][j];
                ++cnt;
            }
        }

        m[i] = m[i] / cnt;
    }
}

// This checks if previous k ie. tempk and current k are same.Used as terminating case.
static int check1(void) {
    int i, j;

    for(i = 0; i < p; ++i) { // clusters
        for(j = 0; j < n; ++j) { // elements
            if(tempk[i][j] != k[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}

int main(int argc, char **argv) {
    int i, j;
    printf("The number of elements: %d\n", n);

    printf("Elements:\n");
    for(i = 0; i < n; ++i) {
        printf("%d ", d[i]);
    }
    printf("\n");

    /* Accepting num of clusters */
    printf("The number of clusters: %d\n", p);

    printf("Value of m\n");
    for(i = 0; i < p; ++i) {
        printf("m%d=%.7f  ", i + 1, m[i]);
    }
    printf("V\n");


    int temp=0;
    int flag=0;
    int times=0;
    do {
        times++;
        for(i = 0; i < p; ++i) {
            for(j = 0; j < n; ++j) {
                k[i][j] = -1;
            }
        }

        // for loop will cal cal_diff(int) for every element.
        for(i = 0; i < n; ++i) {
            temp = cal_diff(d[i]);
            if (temp == 0) {
                k[temp][count1++] = d[i];
            } else if (temp == 1) {
                k[temp][count2++] = d[i];
            } else if (temp == 2) {
                k[temp][count3++] = d[i];
            }
        }

        cal_mean(); // call to method which will calculate mean at this step.
        flag = check1(); // check if terminating condition is satisfied.

        /*Take backup of k in tempk so that you can check for equivalence in next step*/
        if(flag != 1) {
            for(i = 0; i < p; ++i) {
                for(j = 0; j < n; ++j) {
                    tempk[i][j] = k[i][j];
                }
            }
        }

        printf("\nAt this step (%d)\n", times);
        printf("Value of clusters\n");

        for(i = 0; i < p; ++i) {
            printf("K%d{ ", i + 1);
            for(j = 0; k[i][j] != -1 && j < n - 1; ++j) {
                printf("%d ", k[i][j]);
            }
            printf("}\n");
        }

        printf("Value of m\n");
        for(i = 0; i < p; ++i) {
            printf("m%d=%.7f  ", i + 1, m[i]);
        }
        printf("V\n");

        count1 = 0;
        count2 = 0;
        count3 = 0;
    } while (flag == 0);

    printf("\nThe Final Clusters By Kmeans are as follows:\n");
    for(i = 0; i < p; ++i) {
        printf("K%d{ ", i + 1);
        for(j = 0; k[i][j] != -1 && j < n - 1; ++j) {
            printf("%d ", k[i][j]);
        }

        printf("}\n");
    }

    return 0;
}
