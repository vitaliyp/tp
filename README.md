### Description
Simple FreeBSD utility for testing user's access permission for specified file or directory.

### Usage
Use
    $ tp file
to check curent user's access permissions for file.
If you have root access you can use following syntax to get file access permissions for specific user:
    # tp [-u (username|uid)] file

### Installation
    make install
