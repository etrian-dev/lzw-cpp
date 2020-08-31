// lzw encoder/decoder entry point

#include "lzw.h"

string ext_replace(string &file, const char *extension);

int main(int argc, char** argv)
{

    if(argc > 2)
    {
        ifstream in_file;
        ofstream out_file;

        string in_filename = argv[2];
        string out_filename;

#ifdef DEBUG

        // open the file containing in binary mode
        in_file.open("testcases/plaintext/" + in_filename, ios_base::binary);
        if(!in_file.is_open())
        {
            cout << "Cannot open input file \"" << in_filename << "\"\n";
            return 1;
        }

        // generate the compressed file with a .lzw extension
        out_filename = ext_replace(in_filename, "lzw");
        out_file.open("testcases/encoded/" + out_filename, ios_base::binary);
        if(!out_file.is_open())
        {
            cout << "Cannot open output file \"" << out_filename << "\"\n";
            return 2;
        }

        cout << "encoding \"" << in_filename << "\"...\n";
        encode(in_file, out_file);

        // close input file and output file
        in_file.close();
        out_file.close();

        // open compressed file  and decoded output files in binary mode
        in_file.open("testcases/encoded/" + out_filename, ios_base::binary);

        in_filename = ext_replace(in_filename, "decoded");
        out_file.open("testcases/decoded/" + in_filename, ios_base::binary);

        cout << "decoding \"" << out_filename << "\" => \"" << in_filename << "\"\n";
        decode(in_file, out_file);

#else

        // open in_file and encode it in out_file
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
            encode(in_file, out_file);
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
            decode(in_file, out_file);
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
