/*  LZW encoder/decoder to compress files
    Usage: lzw [-c|-d] [FILENAME]
*/

// my header that defines the encoder and decoder functions
#include "lzw.h"

// utility to replace the extension of the file with the one passed as input
string ext_replace(string &file, const char *extension);

/* Main */
int main(int argc, char** argv)
{
    // If the program recevied at least two args
    // then the second is the input file (either for encoding or decoding)
    if(argc > 2)
    {
        ifstream in_file;
        ofstream out_file;

        string in_filename = argv[2];
        string out_filename;

#ifdef DEBUG

        // open the file in binary mode
        in_file.open("testcases/plaintext/" + in_filename, ios_base::binary);
        if(!in_file.is_open())
        {
            cout << "Cannot open input file \"" << in_filename << "\"\n";
            return 1;
        }

        // generate the compressed file: the original filename with a .lzw extension
        out_filename = ext_replace(in_filename, "lzw");
        out_file.open("testcases/encoded/" + out_filename, ios_base::binary);
        if(!out_file.is_open())
        {
            cout << "Cannot open output file \"" << out_filename << "\"\n";
            return 2;
        }

        // print the operation, just a useful visual feedback
        cout << "encoding \"" << in_filename << "\"...\n";

        // encode the in_file to out_file
        if (encode(in_file, out_file) != 0) {
            cout << "Encoder error!\n";
        }

        // close input file and output file
        in_file.close();
        out_file.close();

        // open the compressed file in binary mode
        in_file.open("testcases/encoded/" + out_filename, ios_base::binary);
        // generate a .decoded file in binary mode
        in_filename = ext_replace(in_filename, "decoded");
        out_file.open("testcases/decoded/" + in_filename, ios_base::binary);

        cout << "decoding \"" << out_filename << "\" => \"" << in_filename << "\"\n";

        // decode the encoded file in the output file
        if (decode(in_file, out_file) != 0) {
            cout << "Decoder error!\n";
        }
#else
        /**********************************************************************
         *  Normal Mode. The -c flag triggers encode mode,
         *  the -d flag the decode mode. At least one filename must be specified
         **********************************************************************/

         // open in_file in binary mode or output error message
        in_file.open(in_filename, ios_base::binary);
        if(!in_file.is_open())
        {
            cout << "Cannot open input file\n";
            return 1;
        }

        string first_ln;

        if(argv[1][1] == 'c')
        {
            // output file of encoding is a .lzw file, trimmed of the path
            out_filename = ext_replace(in_filename, "lzw");
            out_filename.erase(0, out_filename.rfind('/') + 1);
            out_file.open(out_filename, ios_base::binary);
            if(!out_file.is_open())
            {
                cout << "Cannot create output file\n";
                return 2;
            }

            /*
             * the filename is actually written as the first line
             * of the compressed file, so that it can be reconstructed when decompressing
             * but trimmed from its path (that is, the path including the last / is erased)
             */
            first_ln = in_filename.substr(in_filename.rfind('/') + 1) + "\n";
            out_file.write(first_ln.data(), first_ln.size());

            cout << "encode \"" << in_filename << "\" into \"" << out_filename << "\"\n";

            // then the files are sent to the encoder
            if (encode(in_file, out_file) != 0) {
                cout << "Error while encoding \"" << in_filename << "\"\n";
                return 1;
            }
        }
        else if(argv[1][1] == 'd')
        {
            // read the original filename first: it is the first line
            char c;
            while(in_file.read(&c, sizeof(c)) && (c != '\n'))
            {
                first_ln += c;
            }

            out_filename = first_ln;

            out_file.open(out_filename, ios_base::binary);
            if(!out_file.is_open())
            {
                cout << "Cannot create output file\n";
                return 2;
            }

            cout << "decode \"" << in_filename << "\" into \"" << out_filename << "\"\n";

            // then the files are sent to the decoder
            if (decode(in_file, out_file) != 0) {
                cout << "Error while encoding \"" << in_filename << "\"\n";
                return 1;
            }
        }
        // the option specified is neither -c or -d
        else
        {
            cout << "Option " << argv[1] << " not recognized\nUsage: ./lzw [-c|-d] [file]\n";
            return 3;
        }

#endif

    }
    else
    {
        cout << "No parameters provided\nUsage: ./lzw [-c|-d] [file]\n";
        return 4;
    }
    return 0;
}

/* utility function used to replace the extension with the one provided
   as an argument (even if the original file had no extension at all)
*/
string ext_replace(string &file, const char *extension)
{
    string file_ext;

    // tries to find extension
    string::size_type extpos = file.find('.');
    if(extpos == string::npos)
    {
        // no extension, append extension
        file_ext = file + "." + extension;
    }
    else
    {
        // point found in filename
        file_ext = file;
        file_ext.replace(extpos + 1, file_ext.size() - extpos, extension);
    }

    return file_ext;
}
