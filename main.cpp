#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <ncurses.h>

#include <omp.h>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

#define WORLD_MAX_X 600
#define WORLD_MAX_Y 400

#define SOFTENING 2
#define NUM_PARTICLES 1000

using namespace std;
using namespace cv;

typedef struct{
    double x, y, mass;
    double vx,vy;} Body;

void randomlyAllocate(Body *data, int n)
{
for (int i=0; i<n; i++){
    data[i].x = ( std::rand() % ( 100 ));
    data[i].y = ( std::rand() % ( 100 ));
    data[i].mass = ( std::rand() % (3)) + 1;
    //data[i].mass = 1;
    }
}

void calcForce(Body *p, float dt, int numBodies)
{
#pragma omp parallel for schedule(dynamic)
for (int i=0; i<numBodies; i++){
    double fx = 0.0f;
    double fy = 0.0f;

    for(int j=0; j<numBodies; j++){
        double dx = p[j].x - p[i].x;
        double dy = p[j].y - p[i].y;
        double sqdist = dx*dx + dy*dy + SOFTENING;
        double InvDist = 1.0f / (sqrtf(sqdist));
        double InvDist3 = InvDist* InvDist* InvDist;
        fx += (dx * InvDist3 * p[j].mass) / p[i].mass ;
        fy += (dy * InvDist3 * p[j].mass) / p[i].mass ;
    }

    p[i].vx += dt*fx;
    p[i].vy += dt*fy;

    if(p[i].x < 0 || p[i].x > 100 ){ p[i].vx *= -1;}
    if(p[i].y < 0 || p[i].y > 100 ){ p[i].vy *= -1;}

    }
}

int main()
    {
    double scale_x = WORLD_MAX_X / 100;
    double scale_y = WORLD_MAX_Y / 100;

    Mat disp(Mat(WORLD_MAX_Y, WORLD_MAX_X, CV_8UC3));
    disp = Scalar(128, 128, 128);
    int nBodies = NUM_PARTICLES;
    double dt = 0.1;
    int nIter = 100000;

    int bytes = nBodies*sizeof(Body);
    double *buf = (double*)malloc(bytes);
    Body *p = (Body*)buf;

    randomlyAllocate(p, nBodies);

    imshow("N-Particles Gravity Simulation", disp);
    for(int iter=0; iter<=nIter; iter++){
        disp = Scalar(0, 0, 0);
        calcForce(p, dt, nBodies);

        #pragma omp parallel for schedule(dynamic)
        for(int i=0; i<nBodies; i++){
                p[i].x += (int)p[i].vx*dt;
                p[i].y += (int)p[i].vy*dt;
                circle( disp, Point(round(p[i].x * scale_x), round(p[i].y * scale_y)), p[i].mass, Scalar(255,255,255), -1);
        }

        putText(disp, "Epoch: " + to_string(iter), Point(10, 30), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, LINE_8, false);
        imshow("N-Particles Gravity Simulation", disp);

        if( waitKey(30) == 'q')
            break;

    }
}
