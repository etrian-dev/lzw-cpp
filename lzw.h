// header file lzw encoder/decoder

// standard lib header file
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <limits> // for numeric limits of codetype

// defines a codetype
typedef uint16_t CodeType;

using namespace std;

#define ASCII_MAX 256

// encoding function: returns 0 on success
int encode(ifstream& input, ofstream& out_bin);
int decode(ifstream& input, ofstream& output);
CodeType reset_dict_enc(unordered_map<string, CodeType> &dictionary);
CodeType reset_dict_dec(unordered_map<CodeType, string> &dictionary);
