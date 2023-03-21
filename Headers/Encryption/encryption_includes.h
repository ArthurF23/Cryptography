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

#include "FileOP/FileOP.h"
using namespace FileOperations;

#include "KeyFileEncryption/KeyFileEncryption.h"
using namespace asymmetric;

#include "VersionInfo/VersionInfo.h"

#include "PNG/PNG.h"
using namespace PNG_H;

typedef bitset<8> AESbyte;  
typedef bitset<32> AESword;