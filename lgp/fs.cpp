#include "fs.hpp"

using std::string;
using std::ifstream;
using std::istreambuf_iterator;
using lgp::fs::File;


string lgp::fs::File::getContent() const
{
    ifstream input(path + name);

    string content
    (
        (istreambuf_iterator<char>(input)),
        (istreambuf_iterator<char>())
    );
    input.close();

    return content;
}

lgp::fs::Directory::Directory(const string& __PATH)
{
    m_path = __PATH;
    if ( m_path[m_path.size()-1] != '/' )
        m_path += '/';

    DIR *dir;
    struct dirent *entry;

    if ( (dir = opendir( m_path.c_str() )) != NULL )
    {
        while( (entry = readdir(dir)) != NULL )
        {
            //Ignore "." and ".."
            if ( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") )
                continue;


            //Ignore hidden
            if ( LGP_CONFIG_bIGNOREHIDDEN && entry->d_name[0] == '.' ) 
                continue;

            //Only take files (type == 8)
            if ( entry->d_type != 8 )
                continue;

            //Filename with path
            string filename = m_path + entry->d_name;
            ifstream in(filename, std::ifstream::ate | std::ifstream::binary);

            m_files.push_back( File(entry->d_name, m_path, in.tellg()) );
            in.close();
        }
        closedir(dir);

        m_fileNumber = m_files.size();
    }
    else
    {
        throw LGP_ERR_CANNOT_OPEN_DIRECTORY;
    }
}

unsigned int lgp::fs::Directory::getFileNumber() const
{
    return m_fileNumber;
}

File lgp::fs::Directory::getFile(const unsigned int& __ID) const
{
    if ( __ID > m_fileNumber - 1 )
        throw LGP_ERR_NO_FILE_WITH_ID;

    return m_files[__ID];
}

bool lgp::fs::directoryExists(string __PATH)
{
    if (__PATH.c_str() == NULL)
        return false;

    DIR *dir;

    if ( (dir = opendir( __PATH.c_str() )) != NULL )
    {
        closedir(dir);

        return true;
    }
    else
    {
        return false;
    }
}

bool lgp::fs::fileExists(string __FILE)
{
    ifstream input(__FILE.c_str());
    bool exists = input.good();
    input.close();

    return exists;
}
