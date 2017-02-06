#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//
#include <GL/glut.h>

#define W     800
#define H     600


double xmin = -2.0;
double xmax = 2.0;
double ymin = -1.5;
double ymax = 1.5;
int        rank    ;
int        size    ;
MPI_Status status  ;
int        tag = 0 ;

typedef struct Column
{
    double miny;
    double maxy;
    double minx;
    double maxx;
    int x;
    int m;
	//
} Column ;

int stepLimit = 20;
int calcSteps( double a, double b, int maxSteps)
{
    double c = a;
    double d = b;
    double ab;
    int s = 0;
    while( s < maxSteps)
    {
        ab = a*b;
        a = (a*a - b*b) + c;
        b = 2*ab + d;
        if(a < -2 || a >2 || b >1.5 || b < -1.5)
            break;
        s++;
    }
    return s;
}

void displayfunc()
{
    glBegin(GL_POINTS);
    int x, y;
    double a, b;
    int steps;
    int r;
    double color;
    Column* p = (Column*)malloc(sizeof(Column));
    p -> miny = ymin;
    p -> maxy = ymax;
    p -> minx
    p-> m = stepLimit;

    glClear(GL_COLOR_BUFFER_BIT); // white
    glColor3f( 1.0, 0.0, 0.0 ) ;   // Black


//    glBegin(GL_POINTS);
//    glVertex2f(0.5,0.5);
//    glEnd();
    for( x = 0; x<n; x++)
    {
        p -> x = x;

        MPI_Send( &p , sizeof(Column) , sizeof(struct Column) , x , 0 , MPI_COMM_WORLD ) ;
    }
    for( x = x ; x < W ; x++ )
    {
        int s[600];
        MPI_Recv( &s , sizeof(s) , sizeof(struct Column) , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
        rank = status.MPI_SOURCE;
        r = status.MPI_TAG;
        int j = 0;
        for(j = 0; j < 600)
        {
            if(s[j] == stepLimit)
                color = 0.0;
            else
                color = 1 - (stepLimit -s[j])/(double)stepLimit;
            glColor3f(1-color, 0.0, 1-color);
            glVertex2f(r, 600-j);
        }

        p->x = (x/800.0) * (xmax-xmin) + (xmin);
        MPI_Send( &p , sizeof(Column) , sizeof(struct Column) , rank , 0 , MPI_COMM_WORLD ) ;

    }
    for()
    glEnd();
    glutSwapBuffers(); // single buffering... call glFlush();
}
void reshapefunc(int wscr,int hscr)
{
    glViewport(0,0,(GLsizei)W,(GLsizei)H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
}
void mousefunc(int button,int state,int xscr,int yscr) //aka zoom
{
    if( state == 0)
    {
        double a, b;
        a = (xscr/800.0) * (xmax-xmin) + (xmin);
        b = (yscr/600.0) * (ymax-ymin) + (ymin);
        xmin = a - (xmax-xmin)/4;
        xmax = a+ (a-xmin);
        ymin = b - (ymax-ymin)/4;
        ymax = b+ (b-ymin);
        glutPostRedisplay();
        printf("xmin: %d \n xmax:%d \n ymin: %d \n ymax: %d", xmin, xmax, ymin, ymax);
    }
}

void specialInput(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        stepLimit = stepLimit + 10;
        break;
    case GLUT_KEY_DOWN:
        if(stepLimit>10)
            stepLimit = stepLimit - 10;
        break;
    case GLUT_KEY_LEFT:
        if( stepLimit >10)
            stepLimit = stepLimit - 10;
        break;
    case GLUT_KEY_RIGHT:
        stepLimit = stepLimit + 10;
        break;
    }
    glutPostRedisplay();
}


void idlefunc()
{

}

int main( int argc, char* argv[] )
{
    //
    // MPI variables
    //

    //
    // other variables
    //
    // boilerplate
    //
    MPI_Init(      &argc, &argv ) ;
    MPI_Comm_size( MPI_COMM_WORLD, &size ) ;  // same
    MPI_Comm_rank( MPI_COMM_WORLD, &rank ) ;  // different

    if( rank == 0)
    {
        glutInit(&argc,argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(W,H);
        glutInitWindowPosition(100,50);
        glutCreateWindow("");
        glClearColor(1.0,1.0,1.0,0.0);
        glShadeModel(GL_SMOOTH);

        glutIdleFunc(idlefunc);
        glutDisplayFunc(displayfunc);
        glutReshapeFunc(reshapefunc);
        glutMouseFunc(mousefunc);
        glutSpecialFunc(specialInput);

        //
        glutMainLoop();
    }

    else
    {
        while (1)
        {
            double a, b;
            Column s;
            MPI_Recv( &s , sizeof(Column) , sizeof(struct Column) , 0 , tag , MPI_COMM_WORLD , &status ) ;
            int steps[600];
            int i = 0;
            for(i = 0; i < 600; i++)
            {
                a = (p->x/800.0) * ((s->maxx)-(s->minx)) + (s->minx);
                b = (i/600.0) * ((s->maxy)-(s->miny) + (s->miny);
                steps[i] = calcSteps(a, b, s->m);
            }
            MPI_Send( &steps , sizeof(steps) , sizeof(struct Column) , 0 , s->x , MPI_COMM_WORLD ) ;
        }
    }

    //
    return 0;
}
