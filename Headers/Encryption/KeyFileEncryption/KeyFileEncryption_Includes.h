#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

/*
This header is for encrypting the keyfile of any encryted file. It uses a 
ripoff RSA encryption style, using a custom random number generation method
and only even numbers, it should be strong but it does use rand() and srand(),
but the seeds for srand() and the times rand() is used is randomized so the 
output should be unpredictable although consistant with the correct public key
inserted. 

It is reccomended you set your own private key & entropy values, that way your private key & entropy isnt 
publicly availible on github.
*/