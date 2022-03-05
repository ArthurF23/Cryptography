#pragma once
#include <iostream>
#include <string>
#include <random>
#include <bitset>
#include <fstream>
#include <cstdlib>
#include <cstdio>
using namespace std;

#include "Compression/compressor.h"
using namespace COMPRESSION;

#include "BMP/BMP.h"
using namespace BITMAP;

typedef bitset<8> AESbyte;  
typedef bitset<32> AESword;