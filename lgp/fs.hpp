#ifndef FS_HPP
#define FS_HPP

#include <iostream>
#include <dirent.h>
#include <fstream>

#include <string.h>
using std::string;

#include <vector>
using std::vector;

#include "config.hpp"


#define LGP_ERR_CANNOT_OPEN_DIRECTORY 1
#define LGP_ERR_NO_FILE_WITH_ID 2

namespace lgp
{
    namespace fs
    {
        struct File
        {
            File(const string& __NAME, const string& __PATH, const unsigned long long& __SIZE): 
                name(__NAME), path(__PATH), size(__SIZE) {  }
            string name;
            string path;
            unsigned long long size;

            string getContent() const;
        };

        class Directory
        {
            public:
                Directory(const string& __PATH="./");

                unsigned int getFileNumber() const;
                File getFile(const unsigned int& __ID) const;

            private:
                string m_path;
                vector<File>m_files;
                unsigned int m_fileNumber;
        };

        bool directoryExists(string __PATH);
        bool fileExists(string __FILE);
    }
}

#endif // FS_HPP
