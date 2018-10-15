#include "read.hpp"
using std::string;
using std::stol;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::istreambuf_iterator;
using std::to_string;
using lgp::LGPFile;

lgp::Read::Read(const string& __FILEPATH)
{
    string temp;

    m_filePath = __FILEPATH;

    //Reading in file
    ifstream input(m_filePath);
    getline(input, temp);

    //Checking if file is LGP
    if ( temp.substr(0,4) != ".LGP" )
        throw LGP_ERR_NOT_LGPFILE;

    //parsing base16 to base10
    m_fileNumber = stol
    (
        temp.substr( temp.find(' ')+1 ),
        0,
        16
    );

    for (unsigned int i(0); i < m_fileNumber; i++)
    {
        //Reading a file
        getline(input, temp);

        LGPFile File;

        File.address = stol
        (
            temp.substr
            (
                0,
                16
            ),
            0,
            16
        );

        File.name = temp.substr( 17 );

        File.size = 0;

        m_fileMap.push_back( File );
    }

    //Defining file size
    for (unsigned int i(0); i < m_fileNumber; i++)
    {
        if ( i + 1 >= m_fileNumber )
        {
            ifstream input(__FILEPATH, std::ios::ate);
            m_fileMap[i].size = (long long)input.tellg() - m_fileMap[i].address;
            input.close();

            continue;
        }

        m_fileMap[i].size = m_fileMap[i+1].address - m_fileMap[i].address;
    }

    input.close();
}

string lgp::Read::get(const string& __INDEX) const
{
    unsigned int index;
    bool checked = false;

    //Get LGPFile index for requested name
    for (unsigned int i(0); i < m_fileNumber; i++)
    {
        if ( m_fileMap[i].name == __INDEX )
        {
            index = i;
            checked = true;
            break;
        }
    }
    //Check if index has been initialized
    if ( !checked )
        throw LGP_ERR_INDEX_NOT_FOUND;

    //Reading content from file
    ifstream input(m_filePath);

    input.seekg(m_fileMap[index].address);
    if ( index + 1 == m_fileNumber ) //+1 Because index is m_fileNumber
    {
        //Last file, return everything left
        string content
        (
            (istreambuf_iterator<char>(input)),
            (istreambuf_iterator<char>())
        );

        input.close();
        return content;
    }
    else
    {
        string content;
        content.resize( m_fileMap[index+1].address - m_fileMap[index].address );

        input.read(&content[0], m_fileMap[index+1].address - m_fileMap[index].address);

        input.close();
        return content;
    }
}

string lgp::Read::get(const unsigned int& __INDEX) const
{
    //Reading content from file
    ifstream input(m_filePath);

    input.seekg(m_fileMap[__INDEX].address);
    if ( __INDEX + 1 == m_fileNumber ) //+1 Because index is m_fileNumber
    {
        //Last file, return everything left
        string content
        (
            (istreambuf_iterator<char>(input)),
            (istreambuf_iterator<char>())
        );

        input.close();
        return content;
    }
    else
    {
        string content;
        content.resize( m_fileMap[__INDEX+1].address - m_fileMap[__INDEX].address );

        input.read(&content[0], m_fileMap[__INDEX+1].address - m_fileMap[__INDEX].address);

        input.close();
        return content;
    }
}

string lgp::Read::getFilePath() const
{
    return m_filePath;
}

string lgp::Read::ls() const
{
    string content;

    for (unsigned int i(0); i < m_fileMap.size(); i++)
        content += m_fileMap[i].name + "\n";

    return content;
}

string lgp::Read::lsSize() const
{
    string content;

    for (unsigned int i(0); i < m_fileMap.size(); i++)
    {
        content += to_string(m_fileMap[i].size) + "\t\t" + m_fileMap[i].name + "\n";
    }

    return content;
}

unsigned int lgp::Read::getFileNumber() const
{
    return m_fileNumber;
}

LGPFile lgp::Read::getFile(const unsigned int& __ID) const
{
    return m_fileMap[__ID];
}

LGPFile lgp::Read::getFile(const string& __INDEX) const
{
    for (unsigned int i(0); i < m_fileNumber; i++)
    {
        if ( m_fileMap[i].name == __INDEX )
            return m_fileMap[i];
    }

    throw LGP_ERR_INDEX_NOT_FOUND;
}

void lgp::Read::extractTo(string __PATH) const
{
    //Adding a slash at the end
    if ( __PATH[__PATH.size()-1] != '/' )
        __PATH += "/";

    for (unsigned int i(0); i < m_fileNumber; i++)
    {
        ofstream output(__PATH + m_fileMap[i].name);
        output << get(i);
        output.close();
    }
}

void lgp::Read::getTo(const string& __INDEX, string __PATH) const
{
    //Adding a slash at the end
    if ( __PATH[__PATH.size()-1] != '/' )
        __PATH += "/";

    ofstream output(__PATH + __INDEX);
    output << get(__INDEX);
    output.close();

}
