// lzw encoder/decoder

#include "lzw.h"

int main(int argc, char** argv) {

    // now open the file containing the text
    ifstream input_file(argv[1], ios_base::in | ios_base::binary);
    if(!input_file.is_open()) {
        cout << "Cannot open input file " << argv[1] << '\n';
    }
    // open (or generate) the output file (compressed)
    ofstream out_file("out.bin", ios_base::out | ios_base::binary);
    if(!out_file.is_open()) {
        cout << "Cannot open output file\n";
    }

    cout << "encode " << argv[1] << "\n";
    encode(input_file, out_file);
    
    // close input file
    input_file.close();
    // close output file
    out_file.close();

    input_file = ifstream("out.bin", ios_base::in | ios_base::binary);
    out_file = ofstream("out.txt", ios_base::out);

    cout << "decode " << argv[1] << " in file out.txt\n";
    decode(input_file, out_file);

    return 0;
}