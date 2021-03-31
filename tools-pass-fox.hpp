#ifndef TOOLS_PASS_FOX_H
#define TOOLS_PASS_FOX_H
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <fstream>

namespace fs = std::filesystem;

class ToolsPassFox {
  public:
    ToolsPassFox();
    std::string home, file, src, des, tmp, 
                filepass, passfirefox;
    void make_dir_files();
    std::string get_filename( std::string );
    bool fake_zlib();
    bool fake_gpgme( int, std::string );
    void save_file();
    bool export_file( std::string );
};

#endif
