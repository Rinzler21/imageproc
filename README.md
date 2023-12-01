imgsharp_parallel.cpp file applies the filter to the selected image to produce another image as output.
openMP is used to parallelise the process of matrix multiplication. This code can also be run using oneAPI offloader using oneAPI base toolkit for supported Intel chips which reduces the time taken to complete the process significantly.
