#include "timeAndDelay.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
double myRand()
{
    return ((double)(rand()%100))/100;
}

double gettime()
{
    double t ;
    //
    struct timeval* ptr = (struct timeval*)malloc( sizeof(struct timeval) ) ;
    //
    gettimeofday( ptr, NULL ) ;  // second argument is time zone... NULL
    //
    t = ptr->tv_sec * 1000000.0 + ptr->tv_usec ;
    //
    free( ptr ) ;
    //
    return t / 1000000.0 ;
}

void delay()
{
    int j ;
    //
    double tic, toc ;
    //
    tic = gettime() ;
    //
    for( j = 0 ; j < 123456789 ; j++ ) ;
    //
    toc = gettime() ;
    //
    //printf("Time: %0.16f seconds\n" , toc - tic ) ;
    //
}

void clear()
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}
