#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Q.h"
#include "timeAndDelay.h"
//#include "mpi.h"
const int rsize = 30;
const int csize = 40;
char forest[30][40];

void buildForest(double prob)
{
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
        }
    }
}

void displayForest()
{
    clear();
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

Q* lightLeft()
{
    int r;
    Q* q = makeQ();
    for( r = 0;  r< rsize; r++)
    {
        if(forest[r][0] =='T')
        {
            forest[r][0] = '*';
            enQ(q,r, 0);
        }
    }
    return q;
}

Q* burn(Q* q)
{
    QNode* current = deQ(q);
    Q* newQ = makeQ();
    int r;
    int c;
    while(current != NULL)
    {
        r = current->row;
        c = current->col;
        if(r<rsize-1&&forest[r+1][c] == 'T')//down
        {
            forest[r+1][c] = '*';
            enQ(newQ, r+1, c);
        }
        if(r>0&&forest[r-1][c] == 'T')//up
        {
            forest[r-1][c] = '*';
            enQ(newQ, r-1, c);
        }
        if(c<csize-1&&forest[r][c+1] == 'T')//right
        {
            forest[r][c+1] = '*';
            enQ(newQ, r, c+1);
        }
        if(c>0&&forest[r][c-1] == 'T')//left
        {
            forest[r][c-1] = '*';
            enQ(newQ, r, c-1);
        }
        forest[r][c] = ' ';
        free(current);
        current = deQ(q);
    }
    free(q);
    return newQ;
}

int main()
{
    int n;
    int m = 13;
    Q* q;
    srand(4);
    for(m = 1; m <20; m++)
    {
        double normalizedSum = 0;
        int steps = 0;
        for(n = 0; n < 100; n++)
        {
            buildForest((double)m * 0.05);
            q = lightLeft();
            while(q->head != NULL)
            {
                q = burn(q);
                steps++;
            }
            normalizedSum += (double)steps/(double)csize;
            steps = 0;
        }
        printf("%d\t%g\t%g\n", m, (double)m*0.05, (double)normalizedSum/100.0);
        normalizedSum = 0;
    }
    return 0;
}

