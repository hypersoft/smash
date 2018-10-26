global = this;

Javascript = function Javascript() {

}

Javascript.prototype = {
	constructor: Javascript
}

js = Javascript;

js.ctypes = ctypes;

js.libc = js.ctypes.open(js.ctypes.libraryName('c') + '.6');

// int euidaccess (__const char *__name, int __type)
js.libc.euidaccess = js.libc.declare(
    "euidaccess",
    js.ctypes.default_abi,
    js.ctypes.int,
    js.ctypes.char.ptr,
    js.ctypes.int
);

// An alias for the above...
js.libc.eaccess = js.libc.euidaccess;

js.file = {};

js.file.exists = function (path) {
    if (js.libc.euidaccess(path, 0)) return false;
    return true;
}

js.loader = {};
js.loader.path = [];
js.loader.def = shellResourcePath;
js.loader.exec = include;

js.loader.addPath = function (directory) {
    js.loader.path.push(directory);
}

js.loader.addPath(js.loader.def);

js.loader.include = function (target) {
    
    if (js.file.exists(target)) return js.loader.exec(target);

    var selection, i, location = js.loader.path;
            
    for (i = 0; i < location.length; i++ ) {
        selection = location[i] + '/' + target;
        if (js.file.exists(selection)) return js.loader.exec(selection);
    }    
            
    // nothing succeeded; propogate error...
    return js.loader.exec(target);
    
}

js.loader.include('linux.jsh');

js.loader.include('libc.jsh');

js.loader.include('core.jsh');

