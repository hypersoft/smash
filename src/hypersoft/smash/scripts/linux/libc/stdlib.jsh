// double atof ( const char * str );
glibc.atof = glibc.declare(
    "atof",
    js.ctypes.default_abi,
    js.ctypes.double,
    js.ctypes.char.ptr
);

// int atoi ( const char * str );
glibc.atoi = glibc.declare(
    "atoi",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr
);

// long int atol ( const char * str );
glibc.atol = glibc.declare(
    "atol",
    js.ctypes.default_abi,
    js.ctypes.long,
    js.ctypes.char.ptr
);

// double strtod ( const char * str, char ** endptr );
glibc.strtod = glibc.declare(
    "strtod",
    js.ctypes.default_abi,
    js.ctypes.double,
    js.ctypes.char.ptr,
    js.ctypes.PointerType(js.ctypes.char.ptr)
);

// long int strtol ( const char * str, char ** endptr, int base );
glibc.strtol = glibc.declare(
    "strtol",
    js.ctypes.default_abi,
    js.ctypes.long,
    js.ctypes.char.ptr,
    js.ctypes.PointerType(js.ctypes.char.ptr),
    js.ctypes.int
);

// unsigned long int strtoul ( const char * str, char ** endptr, int base );
glibc.strtoul = glibc.declare(
    "strtoul",
    js.ctypes.default_abi,
    js.ctypes.unsigned_long,
    js.ctypes.char.ptr,
    js.ctypes.PointerType(js.ctypes.char.ptr),
    js.ctypes.int
);

// int rand ( void );
glibc.rand = glibc.declare(
    "rand",
    js.ctypes.default_abi,
    js.ctypes.int
);

// void srand ( unsigned int seed );
glibc.srand = glibc.declare(
    "srand",
    js.ctypes.default_abi,
    js.ctypes.void_t,
    js.ctypes.unsigned_int
);

// void * calloc ( size_t num, size_t size );
glibc.calloc = glibc.declare(
    "calloc",
    js.ctypes.default_abi,
    js.ctypes.voidptr_t,
    js.ctypes.size_t,
    js.ctypes.size_t
);

// void free( void * buffer );
glibc.free = glibc.declare(
    "free",
    js.ctypes.default_abi,
    js.ctypes.void_t,
    js.ctypes.voidptr_t
);

// void * malloc ( size_t num );
glibc.malloc = glibc.declare(
    "malloc",
    js.ctypes.default_abi,
    js.ctypes.voidptr_t,
    js.ctypes.size_t
);

// void * realloc ( void * ptr, size_t size );
glibc.realloc = glibc.declare(
    "realloc",
    js.ctypes.default_abi,
    js.ctypes.voidptr_t,
    js.ctypes.voidptr_t,
    js.ctypes.size_t
);

// void abort ( void );
glibc.abort = glibc.declare(
    "abort",
    js.ctypes.default_abi,
    js.ctypes.void_t
);

// void exit ( int status );
glibc.exit = glibc.declare(
    "exit",
    js.ctypes.default_abi,
    js.ctypes.void_t,
    js.ctypes.int
);

// char * getenv ( const char * name );
glibc.getenv = glibc.declare(
    "getenv",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// int setenv(const char *name, const char *value, int overwrite);
glibc.setenv = glibc.declare(
    "setenv", js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.int
);

// int system ( const char * command );
glibc.system = glibc.declare(
    "system",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr
);

// int abs ( int n );
glibc.abs = glibc.declare(
    "abs",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.int
);

glibc.labs = glibc.declare(
    "labs",
    js.ctypes.default_abi,
    js.ctypes.long,
    js.ctypes.long
);

// -------------------------------------------------
// <NOT>


// void * bsearch ( const void * key, const void * base, size_t num, size_t size, int ( * comparator ) ( const void *, const void * ) );

// void qsort ( void * base, size_t num, size_t size, int ( * comparator ) ( const void *, const void * ) );

// TODO: declare type: div_t
// div_t div ( int numerator, int denominator );

// long int labs ( long int n );

// TODO: declare type: ldiv_t
// ldiv_t ldiv ( long int numerator, long int denominator );

//---------------------------------------------------------------------

// TODO: Maybe "fully support" this? JS already has unicode support.

// size_t mbstowcs ( wchar_t * wcstr, const char * mbstr, size_t max );

// size_t wcstombs ( char * mbstr, const wchar_t * wcstr, size_t max );


// GLIBC 2.0 Hidden?
// int atexit ( void ( * function ) (void) );
//glibc.atexit = glibc.declare(
//    "atexit",
//    js.ctypes.default_abi,
//    js.ctypes.int,
//    js.ctypes.voidptr_t //js.ctypes.FunctionType(js.ctypes.default_abi, js.ctypes.void_t).ptr
//);

// <\NOT>

// int mblen ( const char * pmb, size_t max );
glibc.mblen = glibc.declare(
    "mblen",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.size_t
);

// int mbtowc ( wchar_t * pwc, const char * pmb, size_t max );
glibc.mbtowc = glibc.declare(
    "mbtowc",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.voidptr_t,
    js.ctypes.char.ptr,
    js.ctypes.size_t
);

// int wctomb ( char * pmb, wchar_t character );
glibc.wctomb = glibc.declare(
    "wctomb",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.voidptr_t
);

