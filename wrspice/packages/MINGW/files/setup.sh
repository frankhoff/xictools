#! /bin/sh

inno="/c/inno-5.5.9"

appname=xictools_wrspice
appdir=wrspice.current
version=`../../version`
osname=`../../../xt_base/info.sh osname`
arch=`uname -m`
top=../root/usr/local
base=../../../xt_base
baseutil=$base/packages/util
basefiles=$base/packages/files
pkgfiles=$base/packages/pkgfiles

tifs="$IFS"
IFS="."
set $version
relnote=wrs$1.$2
IFS="$tifs"

utod=$baseutil/utod.exe
if [ ! -f $utod ]; then
    cwd=`pwd`
    cd $baseutil
    make utod.exe
    cd $cwd
fi

chmod 755 $top/xictools/$appdir/bin/wrspice.bat
$utod $top/xictools/$appdir/bin/wrspice.bat
cp -f files/postinstall.bat $top/xictools/$appdir/bin
chmod 755 $top/xictools/$appdir/bin/postinstall.bat
$utod $top/xictools/$appdir/bin/postinstall.bat
cp -f files/preinstall.bat $top/xictools/$appdir/bin
chmod 755 $top/xictools/$appdir/bin/preinstall.bat
$utod $top/xictools/$appdir/bin/preinstall.bat

examples=$top/xictools/$appdir/examples
$utod $examples/*
$utod $examples/JJexamples/*
$utod $examples/JJexamples_old/*

help=$top/xictools/$appdir/help
$utod $help/*.hlp

startup=$top/xictools/$appdir/startup
$utod $startup/*
$utod $startup/devices/README

scripts=$top/xictools/$appdir/scripts
$utod $scripts/*

docs=$top/xictools/$appdir/docs
cp $basefiles/MSWINFO.TXT $docs
$utod $docs/$relnote
$utod $docs/README
$utod $docs/MSWINFO.TXT

devkit=$top/xictools/$appdir/devkit
if [ -d $devkit ]; then
    $utod $devkit/README
    $utod $devkit/README.adms
    $utod $devkit/Makefile
    $utod $devkit/admst/*
    $utod $devkit/include/*
    foo=`cat ../util/adms_examples`
    for a in $foo; do
        $utod $devkit/$a
    done
    sed -e s/OSNAME/$osname/ -e s/VERSION/$version/ -e s/ARCH/$arch/ \
      < files/$appname.iss.in > $appname.iss
else
    sed -e s/OSNAME/$osname/ -e s/VERSION/$version/ -e s/ARCH/$arch/ \
      < files/${appname}_nodk.iss.in > $appname.iss
fi
$utod $appname.iss

$inno/iscc $appname.iss > build.log
if [ $? != 0 ]; then
    echo Compile failed!
    exit 1
fi

exfiles="$pkgfiles/$appname-$osname*.exe"
if [ -n "$exfiles" ]; then
    for a in $exfiles; do
        rm -f $a
    done
fi

pkg=Output/*.exe
if [ -f $pkg ]; then
    fn=$(basename $pkg)
    mv -f $pkg $pkgfiles
    echo ==================================
    echo Package file $fn
    echo moved to xt_base/packages/pkgfiles
    echo ==================================
fi
rmdir Output
rm $appname.iss
echo Done
