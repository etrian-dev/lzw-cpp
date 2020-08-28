// lzw encoder/decoder

#include "lzw.h"

string ext_replace(string &file, const char *extension);

int main(int argc, char** argv) {

    if(argc > 1) {
        string in_filename = argv[1];

        // open the file containing in binary mode
        ifstream in_file("testcases/plaintext/" + in_filename, ios_base::binary);
        if(!in_file.is_open()) {
            cout << "Cannot open input file \"" << in_filename << "\"\n";
            return 2;
        }

        // generate the compressed file with a .lzw extension
        string compressed_file = ext_replace(in_filename, "lzw");
        ofstream out_file("testcases/encoded/" + compressed_file, ios_base::binary);
        if(!out_file.is_open()) {
            cout << "Cannot open output file \"" << compressed_file << "\"\n";
            return 3;
        }

        cout << "encoding \"" << in_filename << "\"...\n";
        encode(in_file, out_file);

        // close input file and output file
        in_file.close();
        out_file.close();

        // open compressed file  and decoded output files in binary mode
        in_file.open("testcases/encoded/" + compressed_file, ios_base::binary);

        in_filename = ext_replace(in_filename, "decoded");
        out_file.open("testcases/decoded/" + in_filename, ios_base::binary);

        cout << "decoding \"" << compressed_file << "\" => \"" << in_filename << "\"\n";
        decode(in_file, out_file);
    }
    else {
        cout << "No parameters provided\nUsage: ./lzw [file]\n";
        return 1;
    }
    return 0;
}

string ext_replace(string &file, const char *extension) {
    string file_ext;

    // tries to find extension
    string::size_type extpos = file.find('.');
    if(extpos == string::npos) {
        // no extension, append extension
        file_ext = file + "." + extension;
    }
    else {
        // point found in filename
        file_ext = file;
        file_ext.replace(extpos + 1, file_ext.size() - extpos, extension);
    }

    return file_ext;
}
