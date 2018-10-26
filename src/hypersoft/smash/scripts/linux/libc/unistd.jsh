glibc.getlogin = glibc.declare("getlogin", js.ctypes.default_abi, js.ctypes.char.ptr);

glibc.R_OK = js.ctypes.int(4);
glibc.W_OK = js.ctypes.int(2);
glibc.X_OK = js.ctypes.int(1);
glibc.F_OK = js.ctypes.int(0);

// int access (__const char *__name, int __type)
glibc.access = glibc.declare(
    "access",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.int
);

// int euidaccess (__const char *__name, int __type)
glibc.euidaccess = glibc.declare(
    "euidaccess",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.int
);

// An alias for the above...
glibc.eaccess = glibc.euidaccess;

