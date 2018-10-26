
js.print = function (variant) {
    js.libc.puts(variant.toString());
}

print = js.print;

js.exit = function (exitval) {
    if (exitval === undefined) exitval = 0;
    js.libc.exit(exitval);
}

exit = js.exit;

