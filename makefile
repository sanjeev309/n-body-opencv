build:  main.cpp
	g++ -I/headers -L/libs/ -fopenmp -g -o n_body_opencv main.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui
	@[ -f n_body_opencv ] && echo "Compilation successful!" || echo "Compilation failed"

clean: 
	rm -f n_body_opencv
