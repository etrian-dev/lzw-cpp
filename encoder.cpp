// lzw encoder
#include "lzw.h"

void encode(ifstream &input, ofstream &out_bin) // input is a text file, output a binary file
{
    // an unordered map of key:val used as a dictionary
    unordered_map<string, CodeType> dict;

    // the dictionary is initialized and its size is returned
    CodeType code = reset_dict_enc(dict);

    string buffer;
    char c;

    // iterator, just useful when searching for elements in the dict
    unordered_map<string, CodeType>::iterator element;

    /*
    * COMPRESSION LOOP
    * for each character in input:
    * 1) if buffer + c is in dict, keep reading characters
    * 2) otherwise a new sequence is encountered, so it must be added
    * to the ditionary and the code corresponding to buffer - c is sent
    * to out_bin.
    * 3) if the dictionary has reached the maximum size (max value of CodeType)
    * the function to reset dictionary is called
    */
    buffer.clear(); // start with an empty buffer
    while (input.get(c))
    {
        buffer += c;
        // try finding buffer in dict
        element = dict.find(buffer);
        if (element == dict.end())
        {
            // then add buffer as a new entry with the next available code
            dict[buffer] = code;
            ++code;

            // output buffer - c
            buffer.erase(buffer.length() - 1);
            /*
            * buffer's code gets reinterpreted as a char*
            * because of the prototype of write()
            */
            out_bin.write(reinterpret_cast<char *>(&dict[buffer]), sizeof(CodeType));

            buffer = c;
        }

        // if needed rebuild the dictionary
        if(static_cast<CodeType>(dict.size()) == numeric_limits<CodeType>::max()) {
            cout << "encoder: reset dictionary\n";
            code = numeric_limits<CodeType>::max();
            // writes the max value for codetype to out_bin
            out_bin.write(reinterpret_cast<char *>(&code), sizeof(CodeType));

            dict.clear();
            code = reset_dict_enc(dict);
        }
    }

    // EOF reached, the buffer is certainly in dict
    out_bin.write(reinterpret_cast<char *>(&dict[buffer]), sizeof(CodeType));

    cout << "file encoded\n";
}
