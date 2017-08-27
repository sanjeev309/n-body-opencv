#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

#include <omp.h>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#define MAX_X 600
#define MAX_Y 600

#define SOFTENING 3

using namespace std;
using namespace cv;

typedef struct{int x,y ; float vx,vy;} Body;

void randomlyAllocate(Body *data,int n)
{
for (int i=0;i<n;i++){
    data[i].x = std::rand() % ( MAX_X + 1 );
    data[i].y = std::rand() % ( MAX_Y + 1 );
    }

}

void calcForce(Body *p,float dt, int numBodies)
{
#pragma omp parallel for schedule(dynamic)
for (int i=0;i<numBodies;i++){
    float fx=0.0f;
    float fy=0.0f;

    for(int j=0;j<numBodies;j++){
        int dx= p[j].x - p[i].x;
        int dy= p[j].y - p[i].y;
        int sqdist=dx*dx + dy*dy +SOFTENING;
        float InvDist = 1.0f/(sqrtf(sqdist));
        float InvDist3= InvDist*InvDist*InvDist;
        fx+= dx*InvDist3;
        fy+= dy*InvDist3;
    }

    p[i].vx+=dt*fx;
    p[i].vy+=dt*fy;

}
}

int main()
{
Mat disp;
disp= Mat::zeros( MAX_Y, MAX_X, CV_8UC3 );
int nBodies=5000;
int dt=1;
int nIter=1000;

int bytes = nBodies*sizeof(Body);
float *buf = (float*)malloc(bytes);
Body *p = (Body*)buf;

randomlyAllocate(p,nBodies);

for(int iter=0;iter<=nIter;iter++){

    disp= Mat::zeros( MAX_Y, MAX_X, CV_8UC3 );
    calcForce(p,dt,nBodies);

    for(int i=0;i<nBodies;i++){
            p[i].x+=(int)p[i].vx*dt;
            p[i].y+=(int)p[i].vy*dt;
            circle(disp,Point(p[i].x,p[i].y),0,Scalar(255,255,255),1);
    }

    putText(disp,"Iterations: "+to_string(iter),Point(MAX_X - 200,30),FONT_HERSHEY_COMPLEX,0.5,Scalar(255,255,255),1,LINE_8,false);
    imshow("Nbody",disp);
    if( waitKey(30)==0)
        break;

}
}
