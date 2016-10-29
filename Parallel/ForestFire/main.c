#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

const int rsize = 30;
const int csize = 40;
char forest[30][40];
char coForest[30][40];

double myRand()
{
    return ((double)(rand()%100))/100;
}

void buildForest(double prob)
{
    srand(4);
    int i=0;
    int j=0;
    for(i = 0; i < rsize; i++)
    {
        for(j = 0; j<csize; j++)
        {
            if(myRand()<prob)
            {
                forest[i][j] = 'T';
            }
            else forest[i][j] = ' ';
            coForest[i][j] = ' ';
        }
    }
}

void displayForest()
{
    int q = 0;
    int w= 0;
    for( q = 0; q<rsize; q++)
    {
        for( w = 0; w<csize; w++)
        {
            printf("%c", forest[q][w]);
        }
        printf("\n");
    }
}

void lightLeft()
{
    int r;
    for( r = 0;  r< rsize; r++)
    {
        if(forest[r][0] =='T')
            forest[r][0] = '*';
    }
}

int burn()
{
    int z = 0;
    int x = 0;

    int isBurntOut = 1;

    for( z = 0; z < rsize; z++)
    {
        for( x = 0; x < csize; x++)
        {
            if( forest[z][x] == '*')
            {
                if( z > 0)
                    coForest[z-1][x] = '*';
                if( z< rsize-1)
                    coForest[z+1][x] = '*';
                if( x > 0)
                    coForest[z][x-1] = '*';
                if( x < csize )
                    coForest[z][x+1] = '*';
                forest[z][x] = ' ';
            }
        }
    }
    for( z = 0; z < rsize; z ++)
    {
        for( x = 0; x < csize; x ++)
        {
            if( coForest[z][x] == '*'&&forest[z][x] == 'T')
            {
                forest[z][x] = '*';
                isBurntOut = 0;
            }
            coForest[z][x] = ' ';
        }
    }
    return isBurntOut;
}

double gettime()
{
	double t ;
	//
	struct timeval* ptr = (struct timeval*)malloc( sizeof(struct timeval) ) ;
	//
	gettimeofday( ptr , NULL ) ; // second argument is time zone... NULL
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
	double tic , toc ;
	//
	tic = gettime() ;
	//
	for( j = 0 ; j < 123456789 ; j++ ) ;
	//
	toc = gettime() ;
	//
	//printf("Time: %0.16f seconds\n" , toc - tic ) ;
	//
	return 0;
}

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

int main()
{
    buildForest((double)0.65);
    //displayForest();
    lightLeft();
    int isBurntOut = burn();
    while(isBurntOut == 0)
    {
        //displayForest();
        //delay();
        //clear();
        isBurntOut = burn();
    }
    //displayForest();
    return 0;
}

