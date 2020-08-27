// lzw encoder

#include "lzw.h" // defines ASCII_MAX to agree upon initial dict size

void encode(ifstream &input, ofstream &out_bin) // input is a text file, output a binary file
{
    // an unordered map of key:val used as a dictionary
    unordered_map<string, uint16_t> dict;

    // a buffer string to hold characters
    string buffer;

    // initialize the dictionary that maps the symbols in the ascii set
    // with codes in [0, ASCII_MAX] (either 127 or 255)
    uint16_t i;
    for (i = 0; i < ASCII_MAX; i++)
    {
        buffer = (char)i;
        // insert the key-value pair where buffer is the ascii symbol and i is the code
        dict[buffer] = i;
    }
    
    // declare a code, initialized to the next available code after initialization
    uint16_t code = i;
    char c;
    // iterator, just useful when searching for elements in the dict
    unordered_map<string, uint16_t>::iterator element;

    /** COMPRESSION LOOP
     * for each character in input:
     * 1) if buffer + c is in dict, keep reading
     * 2) otherwise a new sequence is encountered, so it gets added to the dict
     * then remove the last character (c) to write in out its code
     * then reset buffer to just c
     */
    buffer.clear();
    while (input.get(c))
    {
        // concatenate c to buffer
        buffer += c;

        // try finding buffer in dict
        element = dict.find(buffer);

        // if the current buffer is not in dict, then add it
        if (element == dict.end())
        {
            // creates a new record key:val -> buffer:code
            dict[buffer] = code;

            // then increment the next available code
            ++code;

            // erase the last character added to the buffer
            buffer.erase(buffer.length() - 1);
            // and then write the code to the output file
            // it gets reinterpreted as a char* as that's the signature
            // and the size is that used for code (uint16_t is 2 bytes)
            out_bin.write(reinterpret_cast<char *>(&dict[buffer]), sizeof(code));

            // output the code to stdout as well
            // cout << dict[buffer] << ' ';

            // then reset the buffer an empty string
            buffer.clear();
            // and initialize with the last character read
            buffer = c;
        }
    }

    // all characters have been read, thus the buffer's code must be copied to out
    out_bin.write(reinterpret_cast<char *>(&dict[buffer]), sizeof(code));
    //cout << dict[buffer] << '\n';

    cout << "file encoded\n";
}