// the header that declares all the functions
#include "lzw.h"

/********************************************************************
* COMPRESSION LOOP
* for each character in input:
* 1) if buffer + c is in dict, keep reading characters
* 2) otherwise a new sequence is encountered, so it must be added
* to the ditionary and the code corresponding to buffer - c is sent
* to out_bin.
* 3) if the dictionary has reached the maximum size (max value of CodeType)
* the function to reset dictionary is called
*********************************************************************/

int encode(ifstream& input, ofstream& out_bin) // input is a text file, output a binary file
{
    // a dictionary that maps strings to CodeType values (ints)
    unordered_map<string, CodeType> dict;
    // an iterator for the dictionary
    unordered_map<string, CodeType>::iterator element;
    // the dictionary is initialized with ascii characters
    // and its size is returned
    CodeType code = reset_dict_enc(dict);

    // utility variables
    string buffer;
    char c;

    buffer.clear(); // start with an empty buffer
    // loops as long as there are characters to be read in the file
    while (input.get(c))
    {
        // add c to the buffer and try to find buffer in the dict
        buffer += c;
        element = dict.find(buffer);

        // buffer not found => add a new entry to the dictionary
        if (element == dict.end())
        {
            // the entry buffer -> code is created
            dict[buffer] = code;
            ++code;

            /*
            * output buffer - 'c'
            * Buffer's code (int) is reinterpreted as a char* because of
            * write(), but the value is the same. Just a workaround
            */
            buffer.erase(buffer.length() - 1);
            out_bin.write(reinterpret_cast<char *>(&dict[buffer]), sizeof(CodeType));

            // reset the buffer as the single character c
            buffer = c;
        }

        // if the max dictionary size is reached, rebuild it
        // this happens when the maximum value representable by CodeType is reached
        if(static_cast<CodeType>(dict.size()) == numeric_limits<CodeType>::max())
        {
            //cout << "encoder: reset dictionary\n";

            /* Writes the max value for codetype to out_bin
             * This is used to tell the decoder to do the same when
             * reading the compressed file, otherwise the process goes out of sync
             */
            code = numeric_limits<CodeType>::max();
            out_bin.write(reinterpret_cast<char *>(&code), sizeof(CodeType));

            dict.clear();
            code = reset_dict_enc(dict);
            //cout << "buffer = \"" << buffer << "\"\n";

            // reset the buffer to the current character (as if after adding an entry)
            buffer = c;
        }
    }

    // EOF reached, buffer is certainly in dict 
    // (at worst it was added in the last iteration)
    out_bin.write(reinterpret_cast<char *>(&dict[buffer]), sizeof(CodeType));

    cout << "file encoded.\n";
    return 0;
}
