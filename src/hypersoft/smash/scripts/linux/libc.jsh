
js.loader.addPath(js.loader.def + '/linux/libc');

glibc = js.libc;

glibc.libName = 'glibc';

glibc.puts = glibc.declare(
    "puts",
    js.ctypes.default_abi,
    js.ctypes.int32_t,
    js.ctypes.char.ptr
);

// int system ( const char * command );
glibc.system = glibc.declare(
    "system",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr
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

// int putenv ( const char * var );
glibc.putenv = glibc.declare(
    "putenv",
    js.ctypes.default_abi,
    js.ctypes.int,
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

