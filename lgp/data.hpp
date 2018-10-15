#ifndef LGP_DATA_HPP
#define LGP_DATA_HPP

#include <sstream>
using std::stringstream;

#include "fs.hpp"
using std::string;
using lgp::fs::Directory;

namespace lgp
{
    class Data
    {
        public:
            Data(const Directory& __DIR);

            void setDirectory(const Directory& __DIR);
            Directory getDirectory() const;

            stringstream write() const;
            void writeTo(const string& __FILE) const;

        private:
            Directory m_dir;
    };
}

#endif // LGP_DATA_HPP
