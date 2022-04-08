#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include <sstream>
#include <thread>
#include <sstream>
#include <iomanip>
#include <memory>
using namespace std;

typedef unsigned int uInt;

/*
You may be wondering why an, encryption and decryption program has a compression
algorythm. The reason is quite simple. If the data going into the encrypt and decrypt 
functions is smaller, then it wont take as long, and it adds complexity to the data,
therefor making it harder to decrypt, after all the encrypt and decrypt functions are
only meant to handle binary, the compression is added onto it, plus the image 
compression that happens before it, which is what makes it faster, is squishing that
image down.
*/