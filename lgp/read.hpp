#ifndef LGP_READ_HPP
#define LGP_READ_HPP

#include <fstream>


#include <iostream>

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <vector>
using std::vector;


#define LGP_ERR_NOT_LGPFILE 1
#define LGP_ERR_INDEX_NOT_FOUND 2

namespace lgp
{
    struct LGPFile
    {
        unsigned long long address;
        string name;
        unsigned int size;
    };

    class Read
    {
        public:
            Read(const string& __FILEPATH);

            string get(const string& __INDEX) const;
            string get(const unsigned int& __INDEX) const;

            string getFilePath() const;

            string ls() const;
            string lsSize() const;

            unsigned int getFileNumber() const;

            LGPFile getFile(const unsigned int& __ID) const;
            LGPFile getFile(const string& __INDEX) const;

            void extractTo(string __PATH) const;
            void getTo(const string& __INDEX, string __PATH) const;

        private:
            string m_filePath;
            unsigned int m_fileNumber;
            vector<LGPFile> m_fileMap;
    };
}

#endif // LGP_READ_HPP
