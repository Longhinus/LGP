# LGP: A Lightweight Game Packager

## Standalone Usage

#### Creating an archive
`lgp -c FOLDER -O OUT` Creates an archive containing FOLDER

#### Listing
`lgp -a ARCHIVE -l` Lists the content of archive ARCHIVE

`lgp -a ARCHIVE -s` Lists the content of archive ARCHIVE and the size of each files

### Extracting
`lgp -a ARCHIVE -O FOLDER -x` Extract the whole archive to FOLDER

`lgp -a ARCHIVE -O FOLDER -g FILENAME` Extract FILENAME to FOLDER


## Usage as a library
Move the `lgp` folder to your project source directory.

#### Creating an archive
```[c++]
lgp::Directory dir("res/Textures"); //Create an archive containing the textures

lgp::Header header(dir); //Create the header of the texture archive
header.writeTo("pkg/textures.lgp"); //Write the header of the texture archive to `pkg/textures.lgp`

lgp::Data data(dir); //Create the data container of the texture archive
data.writeTo("pkg/textures.lgp"); //Write the data of the texture archive to `pkg/textures.lgp`
```

#### Reading an archive
```[c++]
lgp::Read archive("pkg/textures.lgp");

Texture::Wall = archive.get("Wall.png"); //Loads the wall texture in a string
Texture::Wall = archive.get(5); //You can work with indexes instead of names (corresponds to the id of the file in the archive)
```

## Informations
* When writing a file to an archive, the data of the file is stored in the memory.

## Limitations
* Only supports files with the UNIX type: **file**.
* Supports up to 4 GiB (2^32 bytes) per files.
* Supports up to archives of size 16TiB (2^64 bytes).
