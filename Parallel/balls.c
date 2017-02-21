#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#define M 640
#define N 480

typedef struct
{
    double x ;
    double y ;
    double z ;
    //
} point ;

typedef struct
{
    int r;
    int g;
    int b;
} color;

typedef struct
{
    point c;
    double r;
    color h;
} sphere;

typedef struct
{
    point* s;
    point* r;
    //
} vector ;
//
point e = { 0.50, 0.50, -1.00 } ;   // the eye
point g = { 0.00, 1.25, -0.50 } ;   // the light
//
double dotp( point t, point u )
{
    return t.x * u.x + t.y * u.y + t.z * u.z ;
}
//
void diff( point* t, point u, point v )   // t = u - v
{
    t->x = u.x - v.x ;
    t->y = u.y - v.y ;
    t->z = u.z - v.z ;
}
//
sphere s[10];
void init()
{
    s[0].c.x =      0.50 ;
    s[0].c.y = -20000.00 ; // the floor
    s[0].c.z =      0.50 ;
    s[0].r   =  20000.25 ;
    s[0].h.r =    205    ; // color is Peru
    s[0].h.g =    133    ;
    s[0].h.b =     63    ;
    //
    s[1].c.x =      0.50 ;
    s[1].c.y =      0.50 ;
    s[1].c.z =      0.50 ;
    s[1].r   =      0.25 ;
    s[1].h.r =      0    ; // color is Blue
    s[1].h.g =      0    ;
    s[1].h.b =    255    ;
    //
    s[2].c.x =      1.00 ;
    s[2].c.y =      0.50 ;
    s[2].c.z =      1.00 ;
    s[2].r   =      0.25 ;
    s[2].h.r =      0    ; // color is Green
    s[2].h.g =    255    ;
    s[2].h.b =      0    ;
    //
    s[3].c.x =      0.00 ;
    s[3].c.y =      0.75 ;
    s[3].c.z =      1.25 ;
    s[3].r   =      0.50 ;
    s[3].h.r =    255    ; // color is Red
    s[3].h.g =      0    ;
    s[3].h.b =      0    ;
}

vector* vAndNormalize(point* a, point* b)
{
    vector* c = malloc(sizeof(vector));
    c->s = a;
    double mag = (b->x-a->x)*(b->x-a->x) + (b->y-a->y)*(b->y-a->y) + (b->z-a->z)*(b->z-a->z);
    c->r->x = (b->x-a->x)/mag;
    c->r->y = (b->y-a->y)/mag;
    c->r->z = (b->z-a->z)/mag;
    return c;
}

//
int main(void)
{
    int rgb[N][M][3] ; // red-green-blue for each pixel
    //
    int y, x ;
    //
    FILE* fout ;
    //
    for( y = 0 ; y < N ; y++ )
    {
        for( x = 0 ; x < M ; x++)
        {
            point* p = malloc(sizeof(point));
            p->x = (x+0.5)*1.333/640 - 0.167;
            p->y = (y+0.5)/480 - 0.167;
            p->z = 0.0;
            vector* sight = vAndNormalize(&e, p);
            int i;
            double b;
            double c;
            double minT = 10000000;
            for(i = 0;i<4; i++)
            {
                point* eToC = malloc(sizeof(point));
                eToC->x = e.x-s[i].c.x;
                eToC->y = e.y-s[i].c.y;
                eToC->z = e.z-s[i].c.z;
                b = 2*(sight->r->x*(eToC->x) + sight->r->y*(eToC->y) + sight->r->z*(eToC->z));
                c = (eToC->x*eToC->x) + (eToC->y*eToC->y) + (eToC->z*eToC->z)- (s[i].r*s[i].r);
                double disc = (b*b)-4*c;
                if (disc >= 0)
                {
                    double T1 = (-b + sqrt(disc))/2;
                    if(T1 > 0 && T1 < minT)
                    {
                        minT = T1;
                        rgb[x][y][0] = s[i].h.r;
                        rgb[x][y][1] = s[i].h.g;
                        rgb[x][y][2] = s[i].h.b;
                    }

                    double T2 = (-b - sqrt(disc))/2;
                    if(T2 > 0 && T2 < minT)
                    {
                        minT = T2;
                        rgb[x][y][0] = s[i].h.r;
                        rgb[x][y][1] = s[i].h.g;
                        rgb[x][y][2] = s[i].h.b;
                    }
                }
            }

        }
    }
    //
    //
    //
    fout = fopen( "balls.ppm", "w" ) ;
    //
    fprintf( fout, "P3\n" ) ;
    fprintf( fout, "%d %d\n", M, N ) ;
    fprintf( fout, "255\n" ) ;
    //
    for( y = 0 ; y < N ; y++ )
    {
        for( x = 0 ; x < M ; x++)
        {
            fprintf( fout, "%d %d %d\n",
                     rgb[y][x][0], rgb[y][x][1], rgb[y][x][2] ) ;
        }
    }
    fclose( fout ) ;
    //
    return 0 ;
}
