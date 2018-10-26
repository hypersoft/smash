
glibc.NULL = js.ctypes.voidptr_t(0);

// void * memcpy ( void * destination, const void * source, size_t num );
glibc.memcpy = glibc.declare(
    "memcpy",
    js.ctypes.default_abi,
    js.ctypes.voidptr_t,
    js.ctypes.voidptr_t,
    js.ctypes.voidptr_t,
    js.ctypes.size_t
);

// void * memmove ( void * destination, const void * source, size_t num );
glibc.memmove = glibc.declare(
    "memmove",
    js.ctypes.default_abi,
    js.ctypes.voidptr_t,
    js.ctypes.voidptr_t,
    js.ctypes.voidptr_t,
    js.ctypes.size_t
);

// char * strcpy ( char * destination, const char * source );
glibc.strcpy = glibc.declare(
    "strcpy",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// char * strncpy ( char * destination, const char * source, size_t num );
glibc.strncpy = glibc.declare(
    "strncpy",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.size_t
);

// char * strcat ( char * destination, const char * source );
glibc.strcat = glibc.declare(
    "strcat",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// char * strncat ( char * destination, char * source, size_t num );
glibc.strncat = glibc.declare(
    "strncat",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.size_t
);

// int memcmp ( const void * ptr1, const void * ptr2, size_t num );
glibc.memcmp = glibc.declare(
    "memcmp",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.voidptr_t,
    js.ctypes.voidptr_t,
    js.ctypes.size_t
);

// int strcmp ( const char * str1, const char * str2 );
glibc.strcmp = glibc.declare(
    "strcmp",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// int strcoll ( const char * str1, const char * str2 );
glibc.strcoll = glibc.declare(
    "strcoll",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// int strncmp ( const char * str1, const char * str2, size_t num );
glibc.strncmp = glibc.declare(
    "strncmp",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.size_t
);

// size_t strxfrm ( char * destination, const char * source, size_t num );
glibc.strxfrm = glibc.declare(
    "strxfrm",
    js.ctypes.default_abi,
    js.ctypes.size_t,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.size_t
);

// const void * memchr ( const void * ptr, int value, size_t num );
glibc.memchr = glibc.declare(
    "memchr",
    js.ctypes.default_abi,
    js.ctypes.voidptr_t,
    js.ctypes.voidptr_t,
    js.ctypes.int,
    js.ctypes.size_t
);

// const char * strchr ( const char * str, int character );
glibc.strchr = glibc.declare(
    "strchr",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.int
);

// size_t strcspn ( const char * str1, const char * str2 );
glibc.strcspn = glibc.declare(
    "strcspn",
    js.ctypes.default_abi,
    js.ctypes.size_t,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// const char * strpbrk ( const char * str1, const char * str2 );
glibc.strpbrk = glibc.declare(
    "strpbrk",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// const char * strrchr ( const char * str, int character );
glibc.strrchr = glibc.declare(
    "strrchr",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.int
);

// size_t strspn ( const char * str1, const char * str2 );
glibc.strspn = glibc.declare(
    "strspn",
    js.ctypes.default_abi,
    js.ctypes.size_t,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// const char * strstr ( const char * str1, const char * str2 );
glibc.strstr = glibc.declare(
    "strstr",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// char * strtok ( char * str, const char * delimiters );
glibc.strtok = glibc.declare(
    "strtok",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr,
    js.ctypes.char.ptr
);

// void * memset ( void * ptr, int value, size_t num );
glibc.memset = glibc.declare(
    "memset",
    js.ctypes.default_abi,
    js.ctypes.voidptr_t,
    js.ctypes.voidptr_t,
    js.ctypes.int,
    js.ctypes.size_t
);

// char * strerror ( int errnum );
glibc.strerror = glibc.declare(
    "strerror",
    js.ctypes.default_abi,
    js.ctypes.char.ptr,
    js.ctypes.int
);

// size_t strlen ( const char * str );
glibc.strlen = glibc.declare(
    "strlen",
    js.ctypes.default_abi,
    js.ctypes.size_t,
    js.ctypes.char.ptr
);

