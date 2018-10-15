#ifndef LGP_HEADER_HPP
#define LGP_HEADER_HPP

#include <iomanip>

#include <sstream>
using std::stringstream;

#include "fs.hpp"
using std::string;
using lgp::fs::Directory;

namespace lgp
{
    class Header
    {
        public:
            Header(const Directory& __DIR);

            void setDirectory(const Directory& __DIR);
            Directory getDirectory() const;

            unsigned int getHeaderSize() const;

            stringstream write() const;
            void writeTo(const string& __FILE) const;

        private:
            Directory m_dir;
            unsigned int m_headerSize;
    };
}

#endif // LGP_HEADER_HPP
