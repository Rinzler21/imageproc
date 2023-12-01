//command for openMP !g++ imgsharp.cpp -fopenmp -o ser


#include <vector>
#include "bitmap.h"
#include<omp.h>
#include<iomanip>
using namespace std;


#define filterWidth 5
#define filterHeight 5

double filter[filterHeight][filterWidth] =
{
  -1, -1, -1, -1, -1,
  -1,  2,  2,  2, -1,
  -1,  2,  8,  2, -1,
  -1,  2,  2,  2, -1,
  -1, -1, -1, -1, -1,
};

double factor = 1.0 / 8.0;
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

    // SHARPENING!
    double st = omp_get_wtime();
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
    // END SHARPENING!

    std::cout<<std::setprecision(15)<<"Time : "<<end - st;

    image.fromPixelMatrix(bmp);
    image.save("output_ser.bmp");
  }

  return 0;
}