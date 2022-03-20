#include "BMP_includes.h"

#include "BMP.h"
namespace BITMAP {
  void BMP::ReadImage(const char *fileName,BMPbyte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel) {
    FILE *imageFile = fopen(fileName, "rb");
    int32 dataOffset;
    fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
    fread(&dataOffset, 4, 1, imageFile);
    fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
    fread(width, 4, 1, imageFile);
    fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
    fread(height, 4, 1, imageFile);
    int16 bitsPerPixel;
    fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
    fread(&bitsPerPixel, 2, 1, imageFile);
    *bytesPerPixel = ((int32)bitsPerPixel) / 8;

    int paddedRowSize = (int)(4 * ceil((float)(*width) / 4.0f))*(*bytesPerPixel);
    int unpaddedRowSize = (*width)*(*bytesPerPixel);
    int totalSize = unpaddedRowSize*(*height);
    *pixels = (BMPbyte*)malloc(totalSize);
    int i = 0;
    BMPbyte *currentRowPointer = *pixels+((*height-1)*unpaddedRowSize);
    for (i = 0; i < *height; i++)
    {
      fseek(imageFile, dataOffset+(i*paddedRowSize), SEEK_SET);
      fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
      currentRowPointer -= unpaddedRowSize;
    }
    fclose(imageFile);
  };

  void BMP::WriteImage(const char *fileName, BMPbyte *pixels, int32 width, int32 height,int32 bytesPerPixel) {
    FILE *outputFile = fopen(fileName, "wb");
    //*****HEADER************//
    const char *BM = "BM";
    fwrite(&BM[0], 1, 1, outputFile);
    fwrite(&BM[1], 1, 1, outputFile);
    int paddedRowSize = (int)(4 * ceil((float)width/4.0f))*bytesPerPixel;
    int32 fileSize = paddedRowSize*height + HEADER_SIZE + INFO_HEADER_SIZE;
    fwrite(&fileSize, 4, 1, outputFile);
    int32 reserved = 0x0000;
    fwrite(&reserved, 4, 1, outputFile);
    int32 dataOffset = HEADER_SIZE+INFO_HEADER_SIZE;
    fwrite(&dataOffset, 4, 1, outputFile);
  
    //*******INFO*HEADER******//
    int32 infoHeaderSize = INFO_HEADER_SIZE;
    fwrite(&infoHeaderSize, 4, 1, outputFile);
    fwrite(&width, 4, 1, outputFile);
    fwrite(&height, 4, 1, outputFile);
    int16 planes = 1; //always 1
    fwrite(&planes, 2, 1, outputFile);
    int16 bitsPerPixel = bytesPerPixel * 8;
    fwrite(&bitsPerPixel, 2, 1, outputFile);
    //write compression
    int32 compression = NO_COMPRESION;
    fwrite(&compression, 4, 1, outputFile);
    //write image size (in bytes);
    int32 imageSize = width*height*bytesPerPixel;
    fwrite(&imageSize, 4, 1, outputFile);
    int32 resolutionX = 11811; //300 dpi
    int32 resolutionY = 11811; //300 dpi
    fwrite(&resolutionX, 4, 1, outputFile);
    fwrite(&resolutionY, 4, 1, outputFile);
    int32 colorsUsed = MAX_NUMBER_OF_COLORS;
    fwrite(&colorsUsed, 4, 1, outputFile);
    int32 importantColors = ALL_COLORS_REQUIRED;
    fwrite(&importantColors, 4, 1, outputFile);
    int i = 0;
    int unpaddedRowSize = width*bytesPerPixel;
    for (i = 0; i < height; i++) {
      int pixelOffset = ((height - i) - 1)*unpaddedRowSize;
      fwrite(&pixels[pixelOffset], 1, paddedRowSize, outputFile); 
      };
    fclose(outputFile);
  }
}