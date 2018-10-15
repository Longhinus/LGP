#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#include "lgp/header.hpp"
#include "lgp/data.hpp"
#include "lgp/read.hpp"
using namespace lgp;
using std::string;

/* File structure
.LGP [BASE16]<HEADER SIZE IN LINES>
[BASE16]ADDRESS1:[ASCII]FILENAME1
[BASE16]ADDRESS2:[ASCII]FILENAME2
...
[PLAIN]FILE1[PLAIN]FILE2...
*/

void help(int argc, char * argv[])
{
    cout    << "USAGE:              " << argv[0] << " OPTIONS" << endl
            << endl
            << "Options:" << endl
            << "  -a, --archive ARCHIVE Set ARCHIVE file" << endl
            << "  -c, --create FOLDER   Create a LGP archive containing FOLDER" << endl
            << "  -x, --extract         Extract ARCHIVE" << endl
            << "  -l, --list            List all the files in ARCHIVE" << endl
            << "  -s, --size            List all the files in ARCHIVE with their respective size" << endl
            << "  -g, --get FILE        Extract FILE from ARCHIVE" << endl
            << endl
            << "Miscellaneous:" << endl
            << "  -h, --help            Show help page" << endl
            << "  -v, --version         Show LGP version" << endl
            << "  --license             Print the license" << endl
            << endl
            << "Output control:" << endl
            << "  -O, --output FILE     Redirect archiving output to FILE" << endl
            << "  -o, --outdir FOLDER   Redirect extraction output to FOLDER" << endl;
}

int main(int argc, char * argv[])
{
    string outputFile("");
    string outputPath("");
    string archive("");

    /*Directory dir("test");
    Header(dir).writeTo("out");
    Data(dir).writeTo("out");*/

    if ( argc == 1 )
    {
        cout    << "USAGE: " << argv[0] << " OPTIONS" << endl
                << "See " << argv[0] << " --help for help" << endl;
        exit(0);
    }

    //First loop: settings
    for (int i(1); i < argc; i++)
    {
        //Help
        if ( argv[i] == (string)"--help" || argv[i] == (string)"-h" )
        {
            help(argc, argv);
            exit(0);
        }
        if ( argv[i] == (string)"--version" || argv[i] == (string)"-v" )
        {
            cout << "LGP version: " << LGP_sVERSION << endl;
            exit(0);
        }
        if ( argv[i] == (string)"--license")
        {
            cout << "LGP is licensed under the GNU GPLv2." << endl;
            exit(0);
        }
        //Output
        if ( argv[i] == (string)"--output" || argv[i] == (string)"-O" )
        {
            if ( argc > i+1 )
            {
                string arg = string(argv[i+1]);

                //Check if filename is a dir
                if ( fs::directoryExists(arg) )
                {
                    cerr << "Error, you must specify a filename after the path" << endl;
                    exit(1);
                }
                //Direct filename
                else if ( arg.find('/') == std::string::npos )
                {
                    outputFile = arg;
                    i++;
                }
                //PATH + filename
                else if ( fs::directoryExists( arg.substr(0, arg.rfind('/')) ) ) //Normal case
                {
                    outputFile = arg;
                    i++;
                }
                else //Normal case but folder is non existant
                {
                    cerr << "Error, the specified path for the filename does not exist." << endl;
                    exit(1);
                }

            }
            else
            {
                cerr << "Error, you must specify an output file." << endl;
                exit(1);
            }
        }
        //Outdir
        if ( argv[i] == (string)"--outdir" || argv[i] == (string)"-o" )
        {
            if ( argc > i+1 )
            {
                if ( ! fs::directoryExists(argv[i+1]) )
                {
                    cerr << "Error, the specified path does not exists." << endl;
                    exit(1);
                }

                outputPath = argv[i+1];
                i++;
            }
            else
            {
                cerr << "Error, you must specify a valid output path." << endl;
                exit(1);
            }
        }
        //Archive
        if ( argv[i] == (string)"--archive" || argv[i] == (string)"-a" )
        {
            if ( argc > i+1 )
            {
                if ( ! fs::fileExists(argv[i+1]) )
                {
                    cerr << "Error, the specified archive does not exists." << endl;
                    exit(1);
                }

                archive = argv[i+1];
                i++;
            }
            else
            {
                cerr << "Error, you must specify a valid archive filename." << endl;
                exit(1);
            }
        }
    }

    //Second loop: actions
    for (int i(1); i < argc; i++)
    {
        //List
        if ( argv[i] == (string)"--list" || argv[i] == (string)"-l" )
        {
            //Check if archive is defined
            if ( archive == "" )
            {
                cerr << "Error, you must first specify a LGP archive with -a or --archive." << endl;
                exit(1);
            }

            try { cout << Read(archive).ls(); }
            catch (int &e)
            {
                if (e == LGP_ERR_NOT_LGPFILE)
                    cerr << "Error, the specified archive is not a valid LGP file." << endl;
                else
                    cerr << "Unknown error" << endl;
                exit(1);
            }
            exit(0);
        }
        //Size
        else if ( argv[i] == (string)"--size" || argv[i] == (string)"-s" )
        {
            //Check if archive is defined
            if ( archive == "" )
            {
                cerr << "Error, you must first specify a LGP archive with -a or --archive." << endl;
                exit(1);
            }

            try { cout << Read(archive).lsSize(); }
            catch (int &e)
            {
                if (e == LGP_ERR_NOT_LGPFILE)
                    cerr << "Error, the specified archive is not a valid LGP file." << endl;
                else
                    cerr << "Unknown error" << endl;
                exit(1);
            }
            exit(0);
        }
        //Extract
        else if ( argv[i] == (string)"--extract" || argv[i] == (string)"-x" )
        {
            //Check if outdir is defined
            if ( outputPath == "" )
            {
                cerr << "Error, you must first specify a valid output folder with -o or --outdir." << endl;
                exit(1);
            }
            //Check if archive is defined
            if ( archive == "" )
            {
                cerr << "Error, you must first specify a LGP archive with -a or --archive." << endl;
                exit(1);
            }


            try { Read(archive).extractTo(outputPath); }
            catch (int &e)
            {
                cerr << "Unknown error" << endl;
                exit(1);
            }

            cout << "Extraction done!" << endl;

            exit(0);

        }
        //Get
        else if ( argv[i] == (string)"--get" || argv[i] == (string)"-g" )
        {
            if ( argc + 1 > i )
            {
                //Check if outdir is defined
                if ( outputPath == "" )
                {
                    cerr << "Error, you must first specify a valid output folder with -o or --outdir." << endl;
                    exit(1);
                }
                //Check if archive is defined
                if ( archive == "" )
                {
                    cerr << "Error, you must first specify a LGP archive with -a or --archive." << endl;
                    exit(1);
                }


                try { Read(archive).getTo(string(argv[i+1]), outputPath); }
                catch (int &e)
                {
                    if (e == LGP_ERR_INDEX_NOT_FOUND)
                        cerr << "Error, the file you specified is not part of this archive." << endl;
                    else
                        cerr << "Unknown error" << endl;
                    exit(1);
                }

                cout << "File extracted." << endl;

                exit(0);
            }
            else
            {
                cerr << "Error, you must specify the filename of the file you want to extract." << endl;
                exit(1);
            }
        }
        //Create
        else if ( argv[i] == (string)"--create" || argv[i] == (string)"-c" )
        {
            if ( argc + 1 > i )
            {
                //Check if outdir is defined
                if ( outputFile == "" )
                {
                    cerr << "Error, you must first specify a valid output file with -O or --output." << endl;
                    exit(1);
                }
                //Check if direcory exists
                if ( ! fs::directoryExists(argv[i+1]) )
                {
                    cerr << "Error, the specified directory does not exist." << endl;
                    exit(1);
                }

                try
                {
                    Directory dir(argv[i+1]);

                    Header(dir).writeTo(outputFile);
                    Data(dir).writeTo(outputFile);
                }
                catch (int &e)
                {
                    if (e == LGP_ERR_CANNOT_OPEN_DIRECTORY)
                        cerr << "Error, unable to open the specified directory." << endl;
                    else
                        cerr << "Unknown error" << endl;
                    exit(1);
                }

                cout << "Archive created." << endl;

                exit(0);
            }
            else
            {
                cerr << "Error, you must specify a valid directory path." << endl;
                exit(1);
            }
        }

    }

    return 0;
}
