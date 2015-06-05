#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <string>
#include <sstream>
#include <fstream>
#include <utils/Assert.h>
#include <sys/stat.h>

namespace ce {

    /// \brief Check whether a file of the given name exists.
    inline bool FileExists( const std::string& file ) {
        struct stat buffer;
        return ( stat( file.c_str(), &buffer) == 0 );
    }

    /// \brief Get the contents of a file as a string.
    inline std::string FileToString( const std::string& file ) {
        std::ifstream fin;
        fin.open( file, std::ios::in );
        std::string msg = "error: no such file ";
        msg += file;
        ASSERT( FileExists( file ), msg.c_str() );

        std::stringstream buffer;
        buffer << fin.rdbuf();
        return buffer.str();
    }

}

#endif // FILE_H_INCLUDED
