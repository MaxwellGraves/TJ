#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

char forest[30][40];
char coForest[30][40];
int rsize = 30;
int csize = 40;

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
    printf("\n\n\n");
}

void lightLeft()
{
    int r;
    for( r = 0;  r< rsize;r++)
    {
	if(forest[r][0] =='T')
	    forest[r][0] = '*';
    }
}

void burn()
{
    int z = 0;
    int x = 0;
    
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
		forest[z][x] = '*';
	    coForest[z][x] = ' ';
	}
    }
}

int main()
{
    buildForest((double)0.95);
    lightLeft();
    displayForest();
    burn();
    displayForest();
    burn();
    displayForest();
}

