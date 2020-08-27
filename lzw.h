// header file lzw encoder/decoder

// standard lib header file
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

#define ASCII_MAX 256

void encode(ifstream &input, ofstream &out_bin);
void decode(ifstream& input, ofstream& output);