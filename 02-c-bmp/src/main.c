#include "bmp.h"

int main(int argc, char *argv[]) {
  if (argc != 8) {
    printf("Wrong command\n");
    return -1;
  }
  if (strcmp(argv[1], "crop-rotate") != 0) {
    printf("You can only use crop-rotate\n");
    return -1;
  }
  int x = atoi(argv[4]);
  int y = atoi(argv[5]);
  int Width = atoi(argv[6]);
  int Height = atoi(argv[7]);

  if ((x < 0) || (y < 0) || (Width < 0) || (Height < 0)) {
    printf("x and y cords cant be negative!\n");
    return -1;
  }

  BITMAPINFOHEADER InfoHeader;

  unsigned char **pixels = load_bmp(argv[2], &InfoHeader);
  if (pixels == NULL) {
    printf("Cant open input file!\n");
    return -1;
  }
  if ((x + Width > InfoHeader.biWidth) || (y + Height > InfoHeader.biHeight)) {
    printf("cropped immage is bigger than original\n");
    for (int i = 0; i < InfoHeader.biHeight; ++i) {
      free(pixels[i]);
    }
    free(pixels);
    return -1;
  }
  unsigned char **cropped_image =
      crop(pixels, x, y, Width, Height, &InfoHeader);

  for (int i = 0; i < InfoHeader.biHeight; ++i) {
    free(pixels[i]);
  }
  free(pixels);

  if (cropped_image == NULL) {
    printf("Cant crop the image\n");
    return -1;
  }
  unsigned char **rotated_image = rotate(cropped_image, Width, Height);

  for (int i = 0; i < Height; ++i) {
    free(cropped_image[i]);
  }
  free(cropped_image);

  if (rotated_image == NULL) {
    printf("Cant rotate image\n");
    return -1;
  }
  save_bmp(argv[3], rotated_image, Width, Height);

  for (int i = 0; i < Width; ++i) {
    free(rotated_image[i]);
  }
  free(rotated_image);
  return 0;
}
