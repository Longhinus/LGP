#include "header.hpp"
using std::string;
using std::stringstream;
using std::ofstream;
using std::setfill;
using std::setw;
using std::hex;
using std::endl;

lgp::Header::Header(const Directory& __DIR)
{
    m_dir = __DIR;
    m_headerSize = __DIR.getFileNumber();
}

void lgp::Header::setDirectory(const Directory& __DIR)
{
    m_dir = __DIR;
}

Directory lgp::Header::getDirectory() const
{
    return m_dir;
}

unsigned int lgp::Header::getHeaderSize() const
{
    return m_headerSize;
}

stringstream lgp::Header::write() const
{
    unsigned int offset;
    stringstream header;

    //Calculating header size in byte
    offset = 21 + m_headerSize * 18; //".LGP " + "\n" + NUMBERS ((-1 to trick the seekg function later))+ n*("16xBASE16: ... \n")
    for (unsigned int i(0); i < m_headerSize; i++)
        offset += m_dir.getFile(i).name.size(); //Size of files names

    //Writing the header
    header << ".LGP " << setfill('0') << setw(16) << hex << m_headerSize << endl;
    for (unsigned int i(0); i < m_headerSize; i++)
    {
        //Determininf the file position
        unsigned long long pos(1);
        if (i > 0)
            for (unsigned int j(i); j >= 1; j--)
                pos += m_dir.getFile(j-1).size;

        header << setfill('0') << setw(16) << hex << pos + offset << ':' << m_dir.getFile(i).name << endl;
    }

    return header;
}

void lgp::Header::writeTo(const string& __FILE) const
{
    unsigned int offset;
    ofstream output(__FILE, LGP_CONFIG_WRITETOFLAGS);

    //Calculating header size in byte
    offset = 21 + m_headerSize * 18; //".LGP " + "\n" + NUMBERS + n*("16xBASE16: ... \n")
    for (unsigned int i(0); i < m_headerSize; i++)
        offset += m_dir.getFile(i).name.size(); //Size of files names

    //Writing the header
    output << ".LGP " << setfill('0') << setw(16) << hex << m_headerSize << endl;
    for (unsigned int i(0); i < m_headerSize; i++)
    {
        //Determininf the file position
        unsigned long long pos(1);
        if (i > 0)
            for (unsigned int j(i); j >= 1; j--)
                pos += m_dir.getFile(j-1).size;

        output << setfill('0') << setw(16) << hex << pos + offset << ':' << m_dir.getFile(i).name << endl;
    }

    output.close();
}
