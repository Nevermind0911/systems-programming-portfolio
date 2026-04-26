#include "bmp.h"

unsigned char **load_bmp(const char *filename, BITMAPINFOHEADER *InfoHeader) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    return NULL;
  }

  BITMAPFILEHEADER FileHeader;
  fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, file);
  if (FileHeader.bfType != 0x4D42) {
    fclose(file);
    return NULL;
  }
  fread(InfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

  if (InfoHeader->biHeight <= 0 || InfoHeader->biWidth <= 0 ||
      InfoHeader->biHeight > 10000 || InfoHeader->biWidth > 10000) {
    printf("Height: %d, Width: %d", InfoHeader->biHeight, InfoHeader->biWidth);
    fclose(file);
    return NULL;
  }

  int leveling = (4 - (InfoHeader->biWidth * 3) % 4) % 4;

  unsigned char **pixels =
      (unsigned char **)malloc(InfoHeader->biHeight * sizeof(unsigned char *));
  for (int i = 0; i < InfoHeader->biHeight; ++i) {
    pixels[i] = (unsigned char *)malloc(InfoHeader->biWidth * 3);
    if (!pixels[i]) {
      for (int j = 0; j < i; ++j) {
        free(pixels[j]);
      }
      free(pixels);
      fclose(file);
      return NULL;
    }
    fread(pixels[i], sizeof(unsigned char), InfoHeader->biWidth * 3, file);
    unsigned char temp[3];
    fread(temp, sizeof(unsigned char), leveling, file);
  }
  fclose(file);
  return pixels;
}

unsigned char **crop(unsigned char **pixels, int x, int y, int Width,
                     int Height, BITMAPINFOHEADER *InfoHeader) {
  if (((x + Width) > InfoHeader->biWidth) ||
      ((y + Height) > InfoHeader->biHeight)) {
    return NULL;
  }
  unsigned char **cropped_image =
      (unsigned char **)malloc(Height * sizeof(unsigned char *));
  if (cropped_image == NULL) {
    return NULL;
  }
  for (int i = 0; i < Height; ++i) {
    cropped_image[i] = (unsigned char *)malloc(Width * 3);
    if (cropped_image[i] == NULL) {
      return NULL;
    }
  }
  for (int i = 0; i < Height; ++i) {
    for (int j = 0; j < Width; ++j) {
      int xCord = x + j;
      int yCord = InfoHeader->biHeight - y - i - 1;
      cropped_image[i][j * 3] = pixels[yCord][xCord * 3];
      cropped_image[i][j * 3 + 1] = pixels[yCord][xCord * 3 + 1];
      cropped_image[i][j * 3 + 2] = pixels[yCord][xCord * 3 + 2];
    }
  }
  return cropped_image;
}

unsigned char **rotate(unsigned char **cropped_image, int Height, int Width) {
  unsigned char **rotated_image =
      (unsigned char **)malloc(Height * sizeof(unsigned char *));
  if (rotated_image == NULL) {
    return NULL;
  }
  for (int i = 0; i < Height; ++i) {
    rotated_image[i] = (unsigned char *)malloc(Width * 3);
    if (rotated_image[i] == NULL) {
      return NULL;
    }
  }
  for (int i = 0; i < Height; i++) {
    for (int j = 0; j < Width; j++) {
      int new_y = Height - i - 1;
      int new_x = Width - j - 1;
      rotated_image[i][j * 3] = cropped_image[new_x][new_y * 3];
      rotated_image[i][j * 3 + 1] = cropped_image[new_x][new_y * 3 + 1];
      rotated_image[i][j * 3 + 2] = cropped_image[new_x][new_y * 3 + 2];
    }
  }
  return rotated_image;
}

int save_bmp(const char *filename, unsigned char **image, int Height,
             int Width) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    printf("Unable to open file for writing!");
    return -1;
  }
  BITMAPFILEHEADER FileHeader;
  BITMAPINFOHEADER InfoHeader;

  // FileHeader
  FileHeader.bfType = 0x4D42;
  FileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                      Width * Height * 3 + (4 - (Width * 3) % 4) % 4 * Height;

  FileHeader.bfReserved1 = 0;
  FileHeader.bfReserved2 = 0;
  FileHeader.bfOffBits = 54;

  // InfoHeader
  InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
  InfoHeader.biWidth = Width;
  InfoHeader.biHeight = Height;
  InfoHeader.biPlanes = 1;
  InfoHeader.biBitCount = 24;
  InfoHeader.biCompression = 0;
  InfoHeader.biSizeImage =
      Height * Width * 3 + ((4 - (Width * 3) % 4) % 4) * Height;
  InfoHeader.biXPelsPerMeter = 0;
  InfoHeader.biYPelsPerMeter = 0;
  InfoHeader.biClrUsed = 0;
  InfoHeader.biClrImportant = 0;

  fwrite(&FileHeader, sizeof(BITMAPFILEHEADER), 1, file);
  fwrite(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

  int leveling = (4 - (Width * 3) % 4) % 4;

  unsigned char *leveling_buff = (unsigned char *)malloc(leveling);
  if (leveling_buff == NULL) {
    printf("can't allocate memiry for leveling_buffer\n");
    return -1;
  }
  for (int i = 0; i < leveling; i++) {
    leveling_buff[i] = 0;
  }

  for (int i = 0; i < Height; ++i) {
    fwrite(image[i], 3, Width, file);
    fwrite(leveling_buff, sizeof(unsigned char), leveling, file);
  }
  free(leveling_buff);
  fclose(file);
  return 0;
}
