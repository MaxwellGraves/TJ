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
    point s;
    point r;
    //
} ray ;
//
point e = { 0.50, 0.50, -1.00 } ;   // the eye
point l = { 0.00, 1.25, -0.50 } ;   // the light
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

sphere s[4];
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

double distance(point* a, point* b)
{
    double t = (b->x-a->x)*(b->x-a->x) + (b->y-a->y)*(b->y-a->y) + (b->z-a->z)*(b->z-a->z);
    return sqrt(t);
}

void vAndNormalize(point* a, point* b, ray* c)
{
    c->s.x = a->x;
    c->s.y = a->y;
    c->s.z = a->z;
    double mag = distance(a, b);
    c->r.x = (b->x-a->x)/mag;
    c->r.y = (b->y-a->y)/mag;
    c->r.z = (b->z-a->z)/mag;
}
int rgb[N][M][3] ;

point p;
point eToC;
point lToC;
ray sight;
ray light;

int isShadowed(point* target)
{
    int i;
    double b;
    double c;
    double minT = 10000000;
    point lightPoint;
    vAndNormalize(&l, &p, &light);
    for(i = 0; i<4; i++)
    {

        lToC.x = l.x-s[i].c.x;
        lToC.y = l.y-s[i].c.y;
        lToC.z = l.z-s[i].c.z;
        b = 2*(light.r.x*(lToC.x) + light.r.y*(lToC.y) + light.r.z*(lToC.z));
        c = (lToC.x*lToC.x) + (lToC.y*lToC.y) + (lToC.z*lToC.z)- (s[i].r*s[i].r);
        double disc = (b*b)-4*c;
        if (disc >= 0)
        {
            double T1 = (-b + sqrt(disc))/2;
            if(T1 > 0 && T1 < minT)
            {
                minT = T1;
            }

            double T2 = (-b - sqrt(disc))/2;
            if(T2 > 0 && T2 < minT)
            {
                minT = T2;
            }
        }
    }
    lightPoint.x = light.s.x + minT*light.r.x;
    lightPoint.y = light.s.y + minT*light.r.y;
    lightPoint.z = light.s.z + minT*light.r.z;
    if( distance(&lightPoint, &p) < 0.00001)
    {
        return 0;
    }
    return 1;
}
//
point p;
point eToC;
ray sight;

int main(void)
{
    init();
    int y, x ;
    for( y = 0 ; y < N ; y++ )
    {
        for( x = 0 ; x < M ; x++)
        {
            p.x = (x+0.5)*1.333/640 - 0.167;
            p.y = 1 - (y+0.5)/480;
            p.z = 0.0;
            vAndNormalize(&e, &p, &sight);
            int i;
            double b;
            double c;
            double minT = 10000000;
            for(i = 0; i<4; i++)
            {

                eToC.x = e.x-s[i].c.x;
                eToC.y = e.y-s[i].c.y;
                eToC.z = e.z-s[i].c.z;
                b = 2*(sight.r.x*(eToC.x) + sight.r.y*(eToC.y) + sight.r.z*(eToC.z));
                c = (eToC.x*eToC.x) + (eToC.y*eToC.y) + (eToC.z*eToC.z)- (s[i].r*s[i].r);
                double disc = (b*b)-4*c;
                if (disc >= 0)
                {
                    double T1 = (-b + sqrt(disc))/2;
                    if(T1 > 0 && T1 < minT)
                    {
                        minT = T1;
                        rgb[y][x][0] = s[i].h.r;
                        rgb[y][x][1] = s[i].h.g;
                        rgb[y][x][2] = s[i].h.b;
                    }

                    double T2 = (-b - sqrt(disc))/2;
                    if(T2 > 0 && T2 < minT)
                    {
                        minT = T2;
                        rgb[y][x][0] = s[i].h.r;
                        rgb[y][x][1] = s[i].h.g;
                        rgb[y][x][2] = s[i].h.b;
                    }
                }
            }
            p.x = sight.s.x + minT*sight.r.x;
            p.y = sight.s.y + minT*sight.r.y;
            p.z = sight.s.z + minT*sight.r.z;
            if( isShadowed(&p) == 1)
            {
                rgb[y][x][0] = rgb[y][x][0]*0.5;
                rgb[y][x][1] = rgb[y][x][1]*0.5;
                rgb[y][x][2] = rgb[y][x][2]*0.5;
            }
        }
    }
    FILE* fout ;
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
