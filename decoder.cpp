// lzw decoder

#include "lzw.h"
#include <cstdlib>

void decode(ifstream &input, ofstream &output) // input is a binary file, output a text file
{
    // the dictionary where strings are stored
    unordered_map<uint16_t, string> dict;

    // builds basic dict
    uint16_t i;
    string buffer;
    for (i = 0; i < ASCII_MAX; i++)
    {
        buffer = (char)(i);
        // inserts the string buffer at the end of the dictionary
        dict[i] = buffer;
    }

    // variables to hold codes read
    uint16_t code, maxcode = i;
    // the decoder stores the previous string (one step behind the encoder)
    string prev_buf;

    // read input file code by code
    while (input.read(reinterpret_cast<char *>(&code), sizeof(code)))
    {

        // special case: the code read is the one that must be added
        // so the dictionary must be updated before outputting any code
        // happens only in patterns like cScScS
        // so add to dict the pattern cS(prev_buf) + c(prev_buf[0]) 
        if(code == dict.size()) {
            //cout << "added " << prev_buf + prev_buf[0] << "\n";
            dict[code] = prev_buf + prev_buf[0];
        }

        // output the string corresponding to the code read
        //cout << "write " << dict[code] << "\n";
        output << dict[code];

        /* DECODER LOGIC -> https://marknelson.us/posts/2011/11/08/lzw-revisited.html
        * The important thing is to understand the logic behind the decoder. 
        * When the encoder encounters a string that isn’t in the dictionary, 
        * it breaks it into two pieces: a root string and an appended character. 
        * It outputs the code for the root string, and adds 
        * the root string + appended character to the dictionary.
        * It then starts building a new string that starts with the appended character.
        * 
        * So every time the decoder accesses the dictionary with a code, 
        * it knows that the first character in that string was the appended character 
        * of the string just added to the dictionary by the encoder (dict[code][0]).
        * And the root of the string added to the dictionary? That was the previously decoded string,
        * because the decoder is one step behind the encoder.
        */
        // rebuilds dict, skips on empty buffer
        if(!prev_buf.empty()) {
            // adds a record with the previously decoded string (prev_buf)
            // which is the root of the new entry
            // and adds the first character of the currently decoded string
            //cout << "prev_buf = " << prev_buf << "\nadded " << prev_buf + dict[maxcode][0] << "\n";
            dict[maxcode++] = prev_buf + dict[code][0];
        }

        // then set prev_buf to the string just entered 
        // or single char on the fist iteration
        prev_buf = dict[code];
    }

    // outputs the message file decoded
    cout << "file decoded";
}