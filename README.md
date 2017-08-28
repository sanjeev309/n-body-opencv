# visualise-nbody-opencv
A Visualisation of N-body Gravitational body system in OpenCV using C++ with OpenMP parallelization

nbody.cpp contains code to implement N-Body simulation on varying number of particles and visualise it using OpenCV GUI. To improve computational speed, OpenMP parallelization has been used to distribute calculation of Force,Velocity and finally updated Co-ordinates for the particle system on multiple cores of CPU.
