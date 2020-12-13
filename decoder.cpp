// lzw decoder
#include "lzw.h"

// input is a binary file, output a text file
int decode(ifstream& input, ofstream& output)
{
    // the dictionary where strings are stored
    unordered_map<CodeType, string> dict;
    // dict is initialized to the ascii character set 
    // and its size returned
    CodeType maxcode = reset_dict_dec(dict);

    // utility variables
    string prev_buf;
    CodeType code;

    // read input file code by code (binary mode)
    // read() writes to a char*, hence the cast
    while (input.read(reinterpret_cast<char *>(&code), sizeof(code)))
    {
        // maximum value for CodeType triggers the reset of the dictionary
        if(code == numeric_limits<CodeType>::max())
        {
            //cout << "decoder: reset dictionary\n";
            dict.clear();
            maxcode = reset_dict_dec(dict);

            //cout << "prev_buf = \"" << prev_buf << "\"\n";

            // clear decoder buffer, because the encoder's buffer is resetted as well
            prev_buf.clear();
            // skip the rest of this iteration: read a new code
            continue;
        }

        /*
        * SPECIAL CASE:
        * The code read is the one that must be added in this iteration
        * so the dictionary must be updated before outputting any code
        * happens only in patterns like cScScS...
        * so the pattern to be added is prev_buf + prev_buf[0]
        */
        if(code == dict.size())
        {
            dict[code] = prev_buf + prev_buf[0];
        }

        // write (as plaintext) the string corresponding to the code read
        // stored in the dictionary
        output.write(dict[code].data(), dict[code].size());

        /*
        * the first iteration skips the rebuilding of the dict:
        * A single character is read, then it's in the dictionary's base set
        * and the dictionary then needs no updating (otherwise it will be out of sync)
        */
        if(!prev_buf.empty())
        {
            /*
            * maps the next available code to prev_buf (root) and
            * the first character of the code read (that is, the charater
            * c that caused in the encoder the addition of the entry buffer + c
            * and the write of buffer's code
            */
            dict[maxcode++] = prev_buf + dict[code][0];
        }

        // update prev_buf to the sequence that is referred by code
        prev_buf = dict[code];
    }

    cout << "file decoded\n";
    return 0;
}
