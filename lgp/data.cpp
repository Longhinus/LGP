#include "data.hpp"
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::string;
using lgp::fs::Directory;

lgp::Data::Data(const Directory& __DIR)
{
    m_dir = __DIR;
}

void lgp::Data::setDirectory(const Directory& __DIR)
{
    m_dir = __DIR;
}

Directory lgp::Data::getDirectory() const
{
    return m_dir;
}

stringstream lgp::Data::write() const
{
    stringstream data;

    for (unsigned int i(0); i < m_dir.getFileNumber(); i++)
    {
       data << m_dir.getFile(i).getContent(); 
    }

    return data;
}

void lgp::Data::writeTo(const string& __FILE) const
{
    ofstream output(__FILE, LGP_CONFIG_WRITETOFLAGS);

    for (unsigned int i(0); i < m_dir.getFileNumber(); i++)
    {
       output << m_dir.getFile(i).getContent(); 
    }
    output.close();
}
