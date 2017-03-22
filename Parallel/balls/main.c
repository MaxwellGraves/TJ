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

void sum( point* t, point u, point v )   // t = u - v
{
    t->x = u.x + v.x ;
    t->y = u.y + v.y ;
    t->z = u.z + v.z ;
}

sphere s[4];
void init()
{

    s[0].c.x =      0.50 ;
    s[0].c.y = -20000.00 ; // the floor
    s[0].c.z =      0.50 ;
    s[0].r   =  20000.25 ;
    s[0].h.r =    250    ; // color is Peru
    s[0].h.g =    250    ;
    s[0].h.b =     250    ;
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
point c;

int intersection (point* a, point* be, point* contact)
{
    int i;
    double b;
    double c;
    double minT = 10000000;
    point aToC;
    ray ight;
    vAndNormalize(a, be, &ight);
    int sphere;
    for(i = 0; i<4; i++)
    {

        aToC.x = (a->x-s[i].c.x);
        aToC.y = (a->y-s[i].c.y);
        aToC.z = (a->z-s[i].c.z);
        b = 2*(ight.r.x*(aToC.x) + ight.r.y*(aToC.y) + ight.r.z*(aToC.z));
        c = (aToC.x*aToC.x) + (aToC.y*aToC.y) + (aToC.z*aToC.z)- (s[i].r*s[i].r);
        double disc = (b*b)-4*c;
        if (disc >= 0)
        {
            double T1 = (-b + sqrt(disc))/2;
            if(T1 > 0 && T1 < minT)
            {
                minT = T1;
                sphere = i;
            }

            double T2 = (-b - sqrt(disc))/2;
            if(T2 > 0 && T2 < minT)
            {
                minT = T2;
                sphere = i;
            }
        }
    }
    if( minT >= 10000000)
        return -1;
    contact->x = ight.s.x + minT*0.999999*ight.r.x;
    contact->y = ight.s.y + minT*0.999999*ight.r.y;
    contact->z = ight.s.z + minT*0.999999*ight.r.z;
    return sphere;
}

int isShadowed(point* target)
{
    point lightPoint;
    intersection(&l, target, &lightPoint);
    if( distance(&lightPoint, &c) < 0.00001)
    {
        return 0;
    }
    return 1;
}

void reflectPoint ( point* result, point incident, point normal)
{
    result->x = 2*(dotp(incident, normal))*normal.x;
    result->y = 2*(dotp(incident, normal))*normal.y;
    result->z = 2*(dotp(incident, normal))*normal.z;
}


int main(void)
{
    init();
    int y, x ;
    double dot;
    ray norm;
    ray toLight;
    ray reflect;
    ray toC;
    ray temp;
    int m0d;
    int sphere;
    point summ;
    point useful;
    for( y = 0 ; y < N ; y++ )
    {
        for( x = 0 ; x < M ; x++)
        {
            p.x = (x+0.5)*1.333/640 - 0.167;
            p.y = 1 - (y+0.5)/480;
            p.z = 0.0;
            int sphere = intersection(&e, &p, &c);
            if( sphere >= 0)
            {
                rgb[y][x][0] = s[sphere].h.r;
                rgb[y][x][1] = s[sphere].h.g;
                rgb[y][x][2] = s[sphere].h.b;
                if( sphere == 0) //checkerboard
                {
                    m0d = ((int)(c.x*10+1000)+(int) (c.z*10+1000))%2;

                    if( m0d != 0)
                    {
                        rgb[y][x][0] = 150;
                        rgb[y][x][1] = 150;
                        rgb[y][x][2] = 150;
                    }


                }

                    

                if( isShadowed(&c) == 1) //binary shadow
                {
                    rgb[y][x][0] = rgb[y][x][0]*0.5;
                    rgb[y][x][1] = rgb[y][x][1]*0.5;
                    rgb[y][x][2] = rgb[y][x][2]*0.5;
                }
                else // gradient shadow
                {
                    vAndNormalize(&s[sphere].c, &c, &norm);
                    vAndNormalize(&c, &l, &toLight);
                    dot = dotp(norm.r, toLight.r);
                    rgb[y][x][0] -= rgb[y][x][0]*0.5*(1-dot);
                    rgb[y][x][1] -= rgb[y][x][1]*0.5*(1-dot);
                    rgb[y][x][2] -= rgb[y][x][2]*0.5*(1-dot);
                }
                reflect.s = c;//reflect
                vAndNormalize(&e, &c, &toC);
                vAndNormalize(&s[sphere].c, &c, &norm); 
                reflectPoint(&temp.r, toC.r, norm.r);
                diff(&reflect.r, toC.r, temp.r); // R - 2(R*N)N
                sum(&summ, c, reflect.r);
                sphere = intersection(&c, &summ, &useful);
                if( sphere >= 0)
                {
                    rgb[y][x][0] = rgb[y][x][0]*0.5;
                    rgb[y][x][1] = rgb[y][x][1]*0.5;
                    rgb[y][x][2] = rgb[y][x][2]*0.5;

                    if( sphere == 0) //checkerboard
                    {
                        m0d = ((int)(useful.x*10+1000)+(int) (useful.z*10+1000))%2;

                        if( m0d != 0)
                        {
                            rgb[y][x][0] += (int)150*0.5;
                            rgb[y][x][1] += (int)150*0.5;
                            rgb[y][x][2] += (int)150*0.5;
                       }
                       
                    }
                    else
                    {
                        rgb[y][x][0] += s[sphere].h.r*0.5;
                        rgb[y][x][1] += s[sphere].h.g*0.5;
                        rgb[y][x][2] += s[sphere].h.b*0.5;
                  
                    }
                }
            }
            else //sky
            {
                rgb[y][x][0] = 178;
                rgb[y][x][1] = 255;
                rgb[y][x][2] = 255;
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
