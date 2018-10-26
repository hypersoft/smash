
// Managed stdio structure pointers
glibc.FILE = new js.ctypes.StructType("glibc.FILE").ptr;
glibc.fpos_t = new js.ctypes.StructType('glibc.fpos_t').ptr;

/*  
    Managed structures are created for you, to be shared 
    with and by you.
*/

glibc.EOF = -1;

// MODES FOR stdio.fseek
glibc.SEEK_SET = 0;
glibc.SEEK_CUR = 1;
glibc.SEEK_END = 2;

// MODES FOR stdio.setvbuf
glibc._IOFBF = 0;		// Fully buffered.
glibc._IOLBF = 1;		// Line buffered.
glibc._IONBF = 2;		// No buffering.

glibc.BUFSIZ = 8192;  // default io buffer size
glibc.L_tmpnam = 100; // default tmpnam recieve buffer size
glibc.FOPEN_MAX = 16; // Minimum number of files that can be open at once.

glibc.stdin  = glibc.declare("stdin", glibc.FILE);
glibc.stdout = glibc.declare("stdout", glibc.FILE);
glibc.stderr = glibc.declare("stderr", glibc.FILE);

// operations on files

    // int remove ( const char * filename );
    glibc.remove = glibc.declare(
        "remove",
        js.ctypes.default_abi,
        js.ctypes.int32_t,
        js.ctypes.char.ptr
    );

    // int rename ( const char * oldname, const char * newname );
    glibc.rename = glibc.declare(
        "rename",
        js.ctypes.default_abi,
        js.ctypes.int32_t,
        js.ctypes.char.ptr,
        js.ctypes.char.ptr
    );

    // FILE * tmpfile ( void )
    glibc.tmpfile = glibc.declare(
        "tmpfile",
        js.ctypes.default_abi,
        glibc.FILE
    ); 

    // char * tmpnam ( char * str );
    glibc.tmpnam = glibc.declare(
        "tmpnam",
        js.ctypes.default_abi,
        js.ctypes.char.ptr,
        js.ctypes.char.ptr
    );

// *

// file access

    // int fclose ( FILE * stream );
    glibc.fclose = glibc.declare(
        "fclose",
        js.ctypes.default_abi,
        js.ctypes.int32_t,
        glibc.FILE
    );

    // int fflush ( FILE * stream );
    glibc.fflush = glibc.declare(
        "fflush",
        js.ctypes.default_abi,
        js.ctypes.int32_t,
        glibc.FILE
    );

    // FILE * fopen ( const char * filename, const char * mode );
    glibc.fopen = glibc.declare(
        "fopen",
        js.ctypes.default_abi,
        glibc.FILE,
        js.ctypes.char.ptr,
        js.ctypes.char.ptr
    );

    // FILE * freopen ( const char * filename, const char * mode, FILE * stream );
    glibc.freopen = glibc.declare(
        "freopen",
        js.ctypes.default_abi,
        glibc.FILE,
        js.ctypes.char.ptr,
        js.ctypes.char.ptr,
        glibc.FILE
    );

    // void setbuf ( FILE * stream, char * buffer );
    glibc.setbuf = glibc.declare(
        "setbuf",
        js.ctypes.default_abi,
        js.ctypes.void_t,
        glibc.FILE,
        js.ctypes.char.ptr
    );

    // int setvbuf ( FILE * stream, char * buffer, int mode, size_t size );
    glibc.setvbuf = glibc.declare(
        "setvbuf",
        js.ctypes.default_abi,
        js.ctypes.int32_t,
        glibc.FILE,
        js.ctypes.char.ptr,
        js.ctypes.int32_t,
        js.ctypes.size_t
    );

// *

//  Formatted input/output

    /*
        A note on variadic functions with js-js.ctypes.

        "contrary to varargs in C, we (js-ctypes) actually need
        to know all the types of the arguments in order for 
        libffi to assemble them onto the stack correctly. Thus 
        we would require, at call time, that each argument be a 
        CData proper -- no ImplicitConvert." -- Dan Witte

        Example: create a ctype: null terminated 8 bit unicode 
        "string".

        myString = js.ctypes.char.array()('hello world!');
    */

    // int fprintf ( FILE * stream, const char * format, ... );
    glibc.fprintf = glibc.declare(
        "fprintf", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE,
        js.ctypes.char.ptr,
        '...'
    );

    // int fscanf ( FILE * stream, const char * format, ... );
    glibc.fscanf = glibc.declare(
        "fscanf", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE,
        js.ctypes.char.ptr,
        '...'
    );

    // int printf ( const char * format, ... );
    glibc.printf = glibc.declare(
        "printf", js.ctypes.default_abi, js.ctypes.int32_t,
        js.ctypes.char.ptr,
        '...'
    );

    // int  scanf ( const char * format, ... );
    glibc.scanf = glibc.declare(
        "scanf", js.ctypes.default_abi, js.ctypes.int32_t,
        js.ctypes.char.ptr,
        '...'
    );

    // int sprintf ( char * str, const char * format, ... );
    glibc.sprintf = glibc.declare(
        "sprintf", js.ctypes.default_abi, js.ctypes.int32_t,
        js.ctypes.char.ptr,
        js.ctypes.char.ptr,
        '...'
    );

    // int sscanf ( const char * str, const char * format, ... );
    glibc.sscanf = glibc.declare(
        "sscanf", js.ctypes.default_abi, js.ctypes.int32_t,
        js.ctypes.char.ptr,
        js.ctypes.char.ptr,
        '...'
    );

    /*

    These functions behave exactly as their non 'v' types except,
    the variable argument list is passed as a va_list instead of 
    a succession of arguments ('...').

    They cannot be implemented until the C macros: va_start,
    and va_end have been translated to javascript. These macros
    are responsible for managing va_lists.

    // int vfprintf ( FILE * stream, const char * format, va_list arg );
    glibc.vfprintf

    // int vprintf ( const char * format, va_list arg );
    glibc.vprintf

    // int vsprintf (char * str, const char * format, va_list arg );
    glibc.vsprintf

    */

// *

// Character input/output

    // int fgetc ( FILE * stream );
    glibc.fgetc = glibc.declare(
        "fgetc", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE
    );

    // char * fgets ( char * str, int num, FILE * stream );
    glibc.fgets = glibc.declare(
        "fgets", js.ctypes.default_abi, js.ctypes.char.ptr,
        js.ctypes.char.ptr,
        js.ctypes.int32_t,
        glibc.FILE
    );

    // int fputc ( int character, FILE * stream );
    glibc.fputc = glibc.declare(
        "fputc", js.ctypes.default_abi, js.ctypes.int32_t,
        js.ctypes.int32_t,
        glibc.FILE
    );

    // int fputs ( const char * str, FILE * stream );
    glibc.fputs = glibc.declare(
        "fputs", js.ctypes.default_abi, js.ctypes.int32_t,
      js.ctypes.char.ptr,
      glibc.FILE
    );

    // int getc ( FILE * stream );
    glibc.getc = glibc.declare(
        "getc", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE
    );

    // int getchar ( void );
    glibc.getchar = glibc.declare(
        "getchar", js.ctypes.default_abi, js.ctypes.int32_t
    );

    // char * gets ( char * str );
    glibc.gets = glibc.declare(
        "gets", js.ctypes.default_abi, js.ctypes.char.ptr,
        js.ctypes.char.ptr
    );

    // int putc ( int character, FILE * stream );
    glibc.putc = glibc.declare(
        "putc", js.ctypes.default_abi, js.ctypes.int32_t,
        js.ctypes.int32_t,
        glibc.FILE
    );

    // int putchar ( int character );
    glibc.putchar = glibc.declare(
        "putchar", js.ctypes.default_abi, js.ctypes.int32_t,
        js.ctypes.int32_t
    );

    // int puts ( const char * str );
    glibc.puts = glibc.declare(
        "puts",
        js.ctypes.default_abi,
        js.ctypes.int32_t,
        js.ctypes.char.ptr
    ); 

    // int ungetc ( int character, FILE * stream );
    glibc.ungetc = glibc.declare(
        "ungetc", js.ctypes.default_abi, js.ctypes.int32_t,
        js.ctypes.int32_t,
        glibc.FILE
    );

// *

// Direct input/output

    // size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
    glibc.fread = glibc.declare(
        "fread", js.ctypes.default_abi, js.ctypes.size_t,
        js.ctypes.voidptr_t,
        js.ctypes.size_t,
        js.ctypes.size_t,
        glibc.FILE
    );

    // size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
    glibc.fwrite = glibc.declare(
        "fwrite", js.ctypes.default_abi, js.ctypes.size_t,
        js.ctypes.voidptr_t,
        js.ctypes.size_t,
        js.ctypes.size_t,
        glibc.FILE
    );

// *

// File positioning

    // int fgetpos ( FILE * stream, fpos_t * position );
    glibc.fgetpos = glibc.declare(
        "fgetpos", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE,
        glibc.fpos_t
    );

    // int fseek ( FILE * stream, long int offset, int origin );
    glibc.fseek = glibc.declare(
        "fseek", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE,
        js.ctypes.int32_t,
        js.ctypes.int32_t
    );

    // int fsetpos ( FILE * stream, const fpos_t * pos );
    glibc.fsetpos = glibc.declare(
        "fsetpos", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE,
        glibc.fpos_t
    );

    // long int ftell ( FILE * stream );
    glibc.ftell = glibc.declare(
        "ftell", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE
    );

    // void rewind ( FILE * stream );
    glibc.rewind = glibc.declare(
        "rewind", js.ctypes.default_abi, js.ctypes.void_t,
        glibc.FILE
    );

// *

// error handling

    // void clearerr ( FILE * stream );
    glibc.clearerr = glibc.declare(
        "clearerr", js.ctypes.default_abi, js.ctypes.void_t,
        glibc.FILE
    );

    // int feof ( FILE * stream );
    glibc.feof = glibc.declare(
        "feof", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE
    );

    // int ferror ( FILE * stream );
    glibc.ferror = glibc.declare(
        "ferror", js.ctypes.default_abi, js.ctypes.int32_t,
        glibc.FILE
    );

    // void perror ( const char * str );
    glibc.perror = glibc.declare(
        "perror",
        js.ctypes.default_abi,
        js.ctypes.void_t,
        js.ctypes.char.ptr
    );

// *
