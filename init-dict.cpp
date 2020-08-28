// initializes the dictionary
#include "lzw.h"

CodeType reset_dict_enc(unordered_map<string, CodeType> &dictionary) {
    CodeType code;
    string buffer;

    // builds basic dict with all ASCII (extended) characters
    for (code = 0; code < ASCII_MAX; code++)
    {
        buffer = (char)code;
        // inserts the string buffer at the end of the dictionary
        dictionary[buffer] = code;
    }

    // emit size of the basic dict
    return code;
}

CodeType reset_dict_dec(unordered_map<CodeType, string> &dictionary) {

    CodeType code;
    string buffer;

    // builds basic dict with all ASCII (extended) characters
    for (code = 0; code < ASCII_MAX; code++)
    {
        buffer = (char)code;
        // inserts the string buffer at the end of the dictionary
        dictionary[code] = buffer;
    }

    // emit size of the basic dict
    return code;
}
