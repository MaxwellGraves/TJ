#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Q.h"
#include "timeAndDelay.h"
#include <mpi.h>
const int rsize = 30;
const int csize = 40;
char forest[30][40];
const int trials = 100;

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

double trial(double prob)
{
    int n;
    Q* q;
    srand(4);
    double normalizedSum = 0;
    int steps = 0;
    for(n = 0; n < trials; n++)
    {
        buildForest(prob);
        q = lightLeft();
        while(q->head != NULL)
        {
            q = burn(q);
            steps++;
        }
        normalizedSum += (double)steps/(double)csize;
        steps = 0;
    }
    //printf("%d\t%g\t%g\n", m, (double)m*0.05, (double)normalizedSum/100.0);
    return normalizedSum/(double)trials;
}

int main (int argc, char* argv[])
{
    int rank;
    int size;
    MPI_Status status;
    int tag = 0;

    int k, j;
    double prob, val;
    int m = 1;
    int t;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if( rank ==0)
    {
        for( j = 1; j<size; j++)//send out first values
        {
            MPI_Send(&m, 1, MPI_INT, j, m, MPI_COMM_WORLD);
            m++;
        }
        while( m < 20)//work through values
        {
            MPI_Recv( &val, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            j = status.MPI_SOURCE;
            t = status.MPI_TAG;
            printf("%g\t%g\n", t * 0.05, val);
            MPI_Send(&m, 1, MPI_INT, j, m, MPI_COMM_WORLD);
            m++;
        }
        for( j = 1; j<size; j++)//send out first values
        {
            MPI_Recv( &val, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            t = status.MPI_TAG;
            printf("%g\t%g\n", t * 0.05, val);
        }
    }

    else
    {
        MPI_Recv(&m, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        val = trial(m*0.05);
        j = status.MPI_SOURCE;
        MPI_Send(&val, 1, MPI_DOUBLE, j, m, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
