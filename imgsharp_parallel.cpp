//command for openMP !g++ imgsharp.cpp -fopenmp -o omp
//command for oneAPI !icx /Qiopenmp /Qopenmp-targets=spir64 imgsharp.cpp -o imgsharpone

#include <vector>
#include "bitmap.h"
#include<omp.h>
#include<iomanip>
using namespace std;


#define filterWidth 5
#define filterHeight 5

// for sharpening
// double filter[filterHeight][filterWidth] =
// {
//   -1, -1, -1, -1, -1,
//   -1,  2,  2,  2, -1,
//   -1,  2,  8,  2, -1,
//   -1,  2,  2,  2, -1,
//   -1, -1, -1, -1, -1,
// };


// for blur
double filter[filterHeight][filterWidth] =
{
  1/256.0, 4/256.0, 6/256.0, 4/256.0, 1/256.0,
  4/256.0, 16/256.0, 24/256.0, 16/256.0, 4/256.0,
  6/256.0, 24/256.0, 36/256.0, 24/256.0, 6/256.0,
  4/256.0, 16/256.0, 24/256.0, 16/256.0, 4/256.0,
  1/256.0, 1/256.0, 1/256.0, 1/256.0, 1/256.0,
};

// for sharpen 
// double factor = 1.0 / 8.0;

//for blur
double factor = 1.0;
double bias = 0.0;

int main () {
  Bitmap image;
  vector <vector <Pixel> > bmp;

  image.open("sample.bmp");

  bool validBmp = image.isImage();

  if (validBmp == true) {
    bmp = image.toPixelMatrix();
    int h = bmp.size();
    int w = bmp[0].size();

    // Apply filter
    double st = omp_get_wtime();
    
    #pragma omp parallel for collapse (2) 
    for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
        double red = 0.0;
        double green = 0.0;
        double blue = 0.0;

        for (int filterY = 0; filterY < filterHeight; filterY++) {
          for (int filterX = 0; filterX < filterWidth; filterX++) {
            int imageX = (x - filterWidth / 2 + filterX + w) % w;
            int imageY = (y - filterHeight / 2 + filterY + h) % h;
            red += bmp[imageY][imageX].red * filter[filterY][filterX];
            green += bmp[imageY][imageX].green * filter[filterY][filterX];
            blue += bmp[imageY][imageX].blue * filter[filterY][filterX];
          }
        }
        
        bmp[y][x].red =  min(max(int(factor * red + bias), 0), 255);
        bmp[y][x].green =  min(max(int(factor * green + bias), 0), 255);
        bmp[y][x].blue =  min(max(int(factor * blue + bias), 0), 255);
      }
    }
    double end = omp_get_wtime();
    // End 

    std::cout<<std::setprecision(15)<<"Time : "<<end - st;

    image.fromPixelMatrix(bmp);
    image.save("output_parallel_sharp.bmp");
  }

  return 0;
}