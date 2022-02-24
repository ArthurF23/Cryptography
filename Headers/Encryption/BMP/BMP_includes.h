#pragma once
#include <fstream>
#include <math.h>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0
 
typedef unsigned int int32;
typedef short int16;
typedef unsigned char BMPbyte;