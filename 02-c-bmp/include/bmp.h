#ifndef BMP_H
#define BMP_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct BITMAPFILEHEADER {
  uint16_t bfType;
  uint32_t bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER {
  uint32_t biSize;
  int32_t biWidth;
  int32_t biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int32_t biXPelsPerMeter;
  int32_t biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

unsigned char **load_bmp(const char *filename, BITMAPINFOHEADER *InfoHeader);
unsigned char **crop(unsigned char **pixels, int x, int y, int Width,
                     int Height, BITMAPINFOHEADER *InfoHeader);
unsigned char **rotate(unsigned char **cropped_image, int Width, int Height);
int save_bmp(const char *filename, unsigned char **image, int Width,
             int Height);

#endif  // BMP_H
