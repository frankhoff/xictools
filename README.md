# XicTools Open-Source Release October 1, 2017

Whiteley Research is pleased to offer the XicTools programs:  Xic
(graphical editor) and WRspice (circuit simulator) as open-source. 
The two programs have sold commercially for more than 20 years, having
been updated steadily along the way.  However, a lot more development
is required to increase usage, and Stephen Whiteley, the author of most
of this, is ready to try a new approach.

It is expected that these programs will enjoy accelerated development
in an open-source environment, with the contributions of one and all
adding capability.  One day they may become widely used and known for
stability and available on-line support through user and developer
groups.  With this critical mass, the programs may become a viable
choice for foundry users, particularly those who may lack the
resources to pay for the Big Box tools.  This would never be achieved
with a proprietary software model given the limited capabilities of a
tiny company such as Whiteley Research.

The XicTools software is being released under the Apache-2.0 license,
which is actually one of the most "free" licenses available.  One is
not restricted in using this software in a commercial environment, or
in commercial products.  There is no requirement that you must share
your source code if you use this.  The code can be redistributed with
few restrictions.  See the LICENSE-2.0.txt file for the wording of the
license.  Basically, if you are commercial and this software can add
to your bottom line, go for it, and we might even help you!

However, this software, like virtually all software, has a history. 
Much of it originalted from places like the University of California,
Berkeley (e.g., WRspice is a direct descendent of Berleley Spice). 
There are bits and pieces that come from other universities.  The
university licenses are also very unrestrictive and likely highly
compatible with the Apache-2.0 license (remember that if you paid US
taxes, you almost certainly paid for the software developed at the
university).

At least one accessory (the help viewer) is tainted by the GNU library
license.  So, at the time of this initial release, there may be some
question as to whether the full terms of the Apache-2.0 license apply
in some cases.  This shall presumably be clarified in the future.  All
old headers have been retained, so users can make their own judgement
about the constraints on a given set of files.  Incorporating a better
HTML viewer, that understands CSS, should be a priority.

Incidently, the GNU licenses are an absolute scourge and should die. 
Gnu-licensed software is anything but free, its use is highly
restricted and is poison in a commercial environment.  Fortunately the
world is moving away from that nonsense, but the damage has been done. 
For years, Gnu-licensed Linux, and open-source in general, was
considered to be something far-out hippy-dippy communist, due to
"copy-lefts" and other peculiarities, by corporate IT departments. 
These people instead bought Microsoft, costing the world billions of
dollars (maybe a trillion?  who knows) as a consequence of poor
security, and need for countermeasures.  Anyway, Bill Gates owes a big
wet kiss to the GNU folks who made open-source toxic for so long.


## 1. Overview - What's Here

The distribution consists of several semi-independent programs and
libraries, rooted in subdirectories.  These are described below.

adms
This is a wrapper around the adms tool for creating run-time loadable
device models for WRspice.

fastcap
This is a customized version of the venerable capacitance extraction
program from MIT.  It is called from within Xic for capacitance
extraction of layouts, and can be used stand-alone as well.

fasthenry
This is a customized version of the venerable inductance extraction
program from MIT.  It is called from within Xic for inductance
extraction of layouts, and can be used stand-alone as well.

KLU
This is simply a wrapper around the SuiteSparse sparse matrix package
written by Timothy A. Davis, available from
http://faculty.cse.tamu.edu/davis/suitesparse.html

The provided makefile operates on the SuiteSparse distribution file,
applying needed patches, compiling and linking.  The reslt is a
run-time loadble module which provides the sparse matrix capability,
for use with WRspice (or any other application with a suitable interface).
WRspice does not require this plug-in, but will employ it when provided.
Use of the SuiteSparse package generally reduces simulation time.

mozy
The mozy package contains the help system and viewer used by Xic and
WRspice, including an http/ftp file transfer utility, and a screen
capture into image file utility.  This code (and only this program
code) is derived from GNU-licensed code.  Commercial users may wish to
build the XicTools without the mozy package, which is a top-level
option.

mrouter
The MRouter is derived from the Qrouter maze router by Tim Edwards,
available at http://www.opencircuitdesign.com.  It may be used as a
stand-alone tool similar to Qrouter, or run-time loaded into Xic as
a plug-in, in which case its functionality is available within Xic.
This integration is at a very early development stage.

secure (aka xtlserv)
This package provides the license server, client-side code, and
license-generating utilities for the licensing system used with the
commercial version of Xic and WRspice.  This, of course, is no longer
used.  This package is retained, for now at least, in the chance that
it may be of value to someone.

vl
This is a Verilog simulator, which is integrated with WRspice but can
be used as a stand-alone non-graphical simulator.

wrspice
The WRspice program is a SPICE-derived circuit simulator.

xic
The Xic program is a graphical editor, used for displaying and editing
layouts and schematics.  It provides DRC, extraction, and a lot of
other  capability.

xt_base
This contains base libraries and configuration scripts for use with
the other packages.


## 2.  Dependencies

- wrspice
  - mozy (optional)
  - xt_base
  - KLU
  - vl

- xic
  - mozy (optional)
  - xt_base
  - mrouter

- mozy
  - xt_base

- vl
  - xt_base

- adms
- fastcap
- fasthenry
  - xt_base


## 3.  Building the XicTools

### 3.1.  Prerequisites

CentOS/RedHat Linux is the reference platform.  You should have
success building the tools on a RPM-based Linux, including OpenSuSE
and Fedora.  Other Linux distributions will provide challenges, for
now.

You will need to be familiar with makefiles, compilers, and the tools
needed to build Linux programs.  Unless you are very lucky, something
won't work right, and you'll need to know how to fix it. 
User-friendliness varies from nonexistant to execrable, meaning that
the code itself is the documentation.  This will all change over time,
but for now, this is probably for experts only.

You will need to have the development versions of lots of different
libraries installed, including GTK.  As you encounter missing headers,
etc., these should be installed on your system from your operating
system source repositories.  You of course need the basic software
development tools installed.  It can be a bit of a challenge to
figure out which packages to install, Google is your friend.

Here's a probably incomplete list of things to add, based on the port to
Ubuntu-17.10.  This works for Debian-9 as well.

Programs and Utilities:
These were missing from the basic operating system after a fresh
install.  These can be installed with "sudo apt-get install <progname>".
  git, autoconf, flex, bison, pkg-config

Libraries and Development Packages:
Often, the basic libraries are provided, but a separate "devel"
package contains the include files needed to compile programs with the
libraries.  If a build fails with a missing header (.h) file, the game
is to figure out which development package provides it, and install
the package.  In other cases, the library may be wholly missing, so
both the library and development packages should be installed.  For
Ubuntu/Debian, I installed gtk and all its dependents with
  sudo apt-get install build-essential libgtk2.0-dev
This brings in lot of dependent packages, on other systems, the dependents
may require separate installation.
Also needed, libncurses using
  sudo apt-get install libncurses5-dev libncursesw5-dev
and libjpeg, libtiff using
  sudo apt-get install libjpeg-dev libtiff-dev

### 3.2.  Build Procedure for Linux

The procedure is as follows.

    1.  Copy Makefile.sample into Makefile, edit Makefile to set the
        configuration flags, as described in the Makefile.
    2.  "make config" will configure the source tree to your build
        environment.
    3.  "make all" will build all of the programs.
    4.  Thereafter, after changes, do "make reconfig" followed by
        "make all".

The intended installation procedure is to first build a binary package
(e.g., RPM) for each program, then install the packages on your system
using the wr_install script.  The Makefiles will actually prevent
running "make install" to the default location under /usr/local. 
However, you can modify the Makefiles to allow this, and skip the
packaging step entirely if you wish.  However, if you don't install
from the packages, you won't have the "Safe Install" feature, see
http://wrcad.com for info about the packages.  Basically, if you are a
"user", you probably want to install with packages.  If you are a
"developer", you may wish to enable direct installation.  If you are
building on a non-rpm Linux, the packaging system is not yet
supported, so direct installation is your sole option.

    4. "make packages" will create all of the package files.
       Packages are left in xt/xt_base/packages/pkgfiles.

    5. cd xt_base/packages/pkgfiles; ../util/wr_install all
       This will install the programs from the package files.


### 3.3  Windows

This is set up for a hybrid build environment consisting of Cygwin
(www.cygwin.org, I use 64-bit version), win-builds-1.5
(win-builds.org, presently only the 32-bit version).  Also used is the
inno-setup 5.5.9 packager (http://www.jrsoftware.org/).  Cygwin
provides bash, make, and similar.  win-builds provides the compiler
tool chain, and the gtk libraries and prerequisites.

The procedure below will give you the "mingw" tool chain for building
the programs, and the gtk-2 libraries and all dependencies.  The
latter are resolved in gtk2-bundle, which is necessary for exported
packages.

1.  Go to http://cygwin.org, and download the 64-bit setup
application.

BEWARE:  there is a cygwin.com site that looks the same but may be
unsafe, DON'T download from there.

Run the installer.  Specify c:\cygwin64 as the installation location.
The basic tools are already selected for initial installation, but you
will need a few additional things, including:
  git, autoconf-2.69, make, flex, bison, patch
All of these are in category "Devel", the autoconf is listed as
"autoconf2.5" for some reason last I looked.

You will run the installer in the future to update your packages, and
to add packages needed later.

Once installed, you can start bash terminal windows from an icon on
the desktop.  You will be building and probably running the XicTools
programs from these windows (There are issues running the programs,
though.  See the Xic or WRspice release notes).

2.  From a bash window, edit your .bash_profile file.  Add the
 following lines (between the dashed lines) to the end of this file. 
 This will set your search path, among other things.

----------------------------------------
mount c:/usr/local  /usr/local
mount -f c:/inno-5.5.9 /inno-5.5.9

PATH="$PATH:/usr/local/xictools/bin"
if [ -z "$CYGWIN_BUILD" ]; then
    PATH="/usr/local/gtk2-bundle/bin:/usr/local/win-builds/bin:$PATH"
fi
PATH="${HOME}/bin:$PATH"
export PATH
----------------------------------------

3.  Go to http:/win-builds.org, and download the installer
application.  Run the installer, with the location set to

      c:\usr\local\win-builds

Choose 32 bits (for now, will support 64-bits at some point), choose
native installation (NOT MSYS/Cygwin).  Install everything, it isn't
worth the effort to pick and choose.

4.  We need to adjust the location of the png include files so that
the build script will find them.  The configuration script will attempt
to do this automatically if needed, so you really only need to do
the procedure below if that fails.

    a.  Click on the Cygwin terminal icon with the right mouse button,
        click Run as Administrator from the menu.
    b.  Enter these commands into the new Cygwin bash window:
          cd c:/usr/local/win-builds/include/libpng14
          cp *.h ..
          (exit the window)

5.  If you do not already have the gtk2-bundle package installed, go
to http://wrcad.com/xictools/Win32 and get the gtk2-bundle package and
install it (execute it like a program).

6.  Go to http://www.jrsoftware.org and grab inno-setup 5.5.9. 
Install it in c:\inno-5.5.9 This location, and the version number, are
built into the packaging system so should not be changed.

7.  Open a new bash window, you should be able to build xictools from
this window.


### 3.4  OS X

You will need xcode, the X-Quartz X-Windows server, and the MacPorts
gtk2 libraries.

There is provision for creating a "gtk2-bundle" from the MacPorts, which
is deployed with packages.  Again, a local user can likely skip this
and link directly.

Here's a step-by-step procedure for building the X11 version of the
programs.  It is also possible to build a native version, but alas,
the last time I looked at this, the graphics support in the native gtk
had very serious shortcomings and the graphics display, notably
ghosting, doesn't work correctly.

Overall, we install a special version of macports/gtk in
/usr/local/gtk2-bundle-x11.  This is used to build and run the
programs on the local machine.  One can build a "gtk2-bundle-x11"
package which contains only the run-time elements needed, and export
this.  This package will install a small run-time only
/usr/local/gtk2-bundle-x11, allowing the program packages to be
installed and run on other Macs.  There are other ways to obtain these
libraries and build the programs for local use, should you prefer, but
you're on your own in this case.

1.  Install xcode
    This is Apple's development package, available as a download from the
    App store.

2.  Install the "command line tools"
    Install this by giving the following command in a terminal window:
      xcode-select --install

3.  Run xcode to sign off on the license agreement.
    Give the following command and agree to the terms:
      xcodebuild -license

4.  To run the programs you will need an X11 server.  The recommended
    server is XQuartz (www.xquartz.org).  This is the "official" X
    server for the Mac, however others are available, including one
    through macports.  Follow the instructions to install and start
    the server.

5.  Download the latest source code for the Macports installer from
    macports.org.  Presently, this file is named
    Macports-2.4.2.tar.gz, but the release number will likely be
    different (get the gzip (.gz) version).  You are going to be
    building a special macports from source, don't bother downloading
    the pre-built package manager.

6.  Move the Macports file you just downloaded into a work area where
    you have all permissions.  Copy the file xt_base/packages/util/MPbuild
    from the xictools source tree into this same work area.

7.  Edit the MPbuild script copy with a text editor.  Read the comment
    and perform configuration as directed.

8.  Move any existing /usr/local/gtk2-bundle-x11 directory out of the way.
    You will have this directory if you have installed the gtk2-bundle-x11
    package from wrcad.com.  It is not likely that you will need this
    anymore, so it can be deleted.

9.  Run the script by, in a Terminal window in the work area, giving the
    command

      ./MPbuild

    Respond to the prompts.  You should answer "n" to the first prompt
    to build the standard x11 version, but you can choose to build a
    quartz native version.  However, this may not work correctly yet.

    The script takes a long time to run, several hours on my old
    mid-2010 macbook pro.  Make sure to use the ac adaptor!

10.  Make sure that /usr/local/gtk2-bundle-x11/bin is in your search
    path, ahead of /opt/local/bin (if this exists).

    The shell search path is likely set in your .bash_profile file
    in your home directory (PATH variable).  After adding the directory
    to the search path assignment with a text editor, save the file.
    Exit the Terminal window, and start a new one.  The new window will
    have the new path set.

You should now be able to build the graphical xictools programs (mozy,
xic, wrspice).

