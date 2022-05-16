/*  LZW encoder/decoder to compress files
    Usage: lzw [-c|-d] [FILENAME]
*/

#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>

// argument parsing
#include "argparse.hpp"
// logging
#include "loguru.hpp"

// my header that defines the encoder and decoder functions
#include "lzw.h"

using std::string;
using std::vector;
namespace fs = std::filesystem;

argparse::ArgumentParser build_arg_parser(const char *progname) {
    argparse::ArgumentParser prog(progname);
    prog.add_description("An LZW compressor/decompressor");
    std::string epi = "";
    epi += "\nInput files can be arbitrary (ordinary) files.";
    epi += " \nCompressing a file creates a .lzw file with the same filename.";
    epi += " \nFiles to be decompressed must end in .lzw and have been compressed by";
    epi += " \nlzw -c <file>";
    prog.add_epilog(epi.c_str());
    prog.add_argument("-c", "--compress")
        .default_value(false)
        .implicit_value(true)
        .help("Compress the input file");
    prog.add_argument("-d", "--decompress")
        .default_value(false)
        .implicit_value(true)
        .help("Decompress the input file");
    prog.add_argument("files")
        .remaining()
        .help("File(s) to manipulate");
    return prog;
}

/* Main */
int main(int argc, char** argv)
{
    // logging
    loguru::init(argc, argv);
    string logpath = argv[0];
    logpath += ".log";
    if (!loguru::add_file(logpath.c_str(), loguru::Truncate, loguru::Verbosity_INFO)) {
        std::exit(1);
    }

    // arg parsing
    argparse::ArgumentParser parser = build_arg_parser(argv[0]);

    try {
        parser.parse_args(argc, argv);
        bool mode_compress = parser.is_used("-c");
        bool mode_decompress = parser.is_used("-d");
        if (!(mode_compress || mode_decompress)) {
            LOG_S(FATAL) << "No mode supplied. Specify one of -c or -d\n";
        }
        if(mode_compress && mode_decompress) {
            LOG_S(FATAL) << "Both -c and -d specified. Choose one\n" ;
        }

        auto files = parser.get<vector<string>>("files");
        for(auto& s : files) {
            fs::path f(s);
            LOG_S(INFO) << "processing file: " << f;
            std::uintmax_t orig_size = fs::file_size(f);
            LOG_S(INFO) << "original size: " << orig_size << " bytes\n";
            std::ifstream ifs;
            std::ofstream ofs;
            ifs.open(f, ios_base::binary);
            if(!ifs.is_open()) {
                LOG_S(ERROR) << "Cannot open file " << f << "\n";
                continue;
            }

            if (mode_compress) {
                fs::path out_filename(f.filename());
                out_filename.replace_extension(".lzw");
                ofs.open(out_filename, ios_base::binary);
                if(!ofs.is_open())
                {
                    LOG_S(ERROR) << "Cannot create output file " << out_filename << "\n";
                    continue;
                }

                /*
                 * the filename is actually written as the first line
                 * of the compressed file, so that it can be reconstructed when decompressing
                 * but trimmed from its path (that is, the path including the last / is erased)
                 */
                ofs << f.filename().string() << '\n';

                LOG_S(INFO) << "Compressing " << f << " to " << out_filename << "\n";

                // then the files are sent to the encoder
                if (encode(ifs, ofs) != 0) {
                    LOG_S(INFO) << "Error while encoding " << f << "\n";
                    return 1;
                }
                ifs.close();
                ofs.flush();
                ofs.close();

                std::uintmax_t compr_size = fs::file_size(out_filename);
                double ratio = 100.0 * (double)compr_size / orig_size;
                LOG_S(INFO) << "Compressed size: " << compr_size << " bytes (ratio " << ratio << "%)\n";
            } else {
                // read the original filename first: it is the first line
                string orig_file;
                char c;
                while(ifs.read(&c, 1) && (c != '\n'))
                {
                    orig_file += c;
                }

                fs::path out_filename(orig_file);
                ofs.open(out_filename, ios_base::binary);
                if(!ofs.is_open())
                {
                    LOG_S(INFO) << "Cannot create output file " << out_filename << "\n";
                    return 2;
                }

                LOG_S(INFO) << "Decompressing " << f << " into " << out_filename << "\n";

                // then the files are sent to the decoder
                if (decode(ifs, ofs) != 0) {
                    LOG_S(INFO) << "Error while encoding " << f << "\n";
                    return 1;
                }
                ifs.close();
                ofs.flush();
                ofs.close();
            }
            
        }
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << "\n";
        std::cerr << parser;
        std::exit(1);
    }
}
