// lzw decoder
#include "lzw.h"

void decode(ifstream &input, ofstream &output) // input is a binary file, output a text file
{
    // the dictionary where strings are stored
    unordered_map<CodeType, string> dict;

    string prev_buf;
    CodeType code;
    // dict is initialized and its size returned
    CodeType maxcode = reset_dict_dec(dict);

    // read input file code by code
    while (input.read(reinterpret_cast<char *>(&code), sizeof(code)))
    {
        // maximum value for CodeType triggers reset of the dictionary
        if(code == numeric_limits<CodeType>::max()) {
            cout << "decoder: reset dictionary\n";
            dict.clear();
            maxcode = reset_dict_dec(dict);
        }

        /*
        * special case:
        the code read is the one that must be added
        * so the dictionary must be updated before outputting any code
        * happens only in patterns like cScScS
        * so the pattern to be added is prev_buf + prev_buf[0]
        */
        if(code == dict.size()) {
            dict[code] = prev_buf + prev_buf[0];
        }

        // output (as plaintext) the string corresponding to the code read
        output.write(dict[code].data(), dict[code].size());

        /*
        * the first iteration skips the rebuilding of the dict:
        * only one char is read and it MUST be in the dict
        * so it MUST NOT be updated when decoding
        */
        if(!prev_buf.empty()) {
            /*
            * maps the next available code to prev_buf (root) and
            * the first character of the code read (that is, the charater
            * c that caused in the encoder the addition of the entry buffer + c
            * and the write of buffer's code
            */
            dict[maxcode++] = prev_buf + dict[code][0];
        }

        // update prev_buf to the correspondent sequence matched by the encoder
        prev_buf = dict[code];
    }

    cout << "file decoded\n";
}
