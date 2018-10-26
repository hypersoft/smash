#!/bin/sh

package='js185-1.0.1.smash.tar.gz';
sourcedir='src/mozilla/js-1.8.5/js/src';
jsmakefile=$sourcedir/Makefile;

if [ "$1" = "clean" ]; then
  [[ -e $jsmakefile ]] && (cd $sourcedir; make --print-directory clean distclean)
 exit 0;
fi

if [ "$1" = "configuration" ]; then
 rm -rf $sourcedir/Makefile;
fi

if [ ! -d 'src/mozilla/js-1.8.5' ]; then
    echo "Unpacking monkey business..."; ( 
        cd src/mozilla; tar xzf $package;
        echo '';
    ); (
        echo "Heating up the branding iron...";        
        cd $sourcedir;
        sed -i 's/mozjs185/mozjs185c/g' mozjs185.pc.in;
        sed -i 's/8\.5/8\.5 With js-ctypes/g' mozjs185.pc.in;    
    )
fi

if [ ! -e $sourcedir/Makefile ]; then

    if test -e .debugging; then
      debug="--enable-debug=-g3 --enable-debugger-info-modules";
      build="build/debug";
    else
      debug="--enable-optimize=-O3";
      build="build/release";
    fi;
    
    echo 'Throwing monkey wrenches...'; (
        cd $sourcedir;

        autoconf-2.13 && {
            ./configure $debug --disable-tests --enable-ctypes \
            --with-system-nspr --with-pthreads --enable-thread-safe \
            --with-dist-dir="../../../../../${build}/mozjs" || exit;
        }
        
        echo '';
        echo 'Registering the brand...'; 
        sed -i 's/mozjs185/mozjs185c/g' Makefile;
        echo '';
        
    );
fi

if [ "$1" = "configuration" ]; then exit; fi

(cd $sourcedir; make)

