#!/bin/bash

#assumes icu, boost and exodus are in the home directory

export EXO_CONFIGMODE=$1

#-------------------
#--- Run options ---
#-------------------
export EXO_ICU_REUSE_DOWNLOAD=YES
export EXO_BOOST_REUSE_DOWNLOAD=YES
export EXO_EXODUS_REUSE_DOWNLOAD=YES

export EXO_PREFIX=$HOME/exo

#-------------
#--- Boost ---
#-------------
export EXO_BOOST_VER=1_46_1
export EXO_BOOST_VERNO=1.46.1
export EXO_BOOST_DIR=boost_${EXO_BOOST_VER}
export EXO_BOOST_FILE=boost_${EXO_BOOST_VER}.tar.gz
export EXO_BOOST_URL=http://sourceforge.net/projects/boost/files/boost/${EXO_BOOST_VERNO}/${EXO_BOOST_FILE}

#---------------
#--- Toolset ---
#---------------

export EXO_UNAME=`uname`

if [ $EXO_UNAME = Darwin ]; then

export EXO_BUILDING="OSX10.6"

export EXO_LIBS_ICU="-licudata -licui18n -licutu -licuuc"
export EXO_LIBS_BOOST="-lboost_date_time -lboost_filesystem -lboost_regex -lboost_system -lboost_thread"

if [ "$EXO_BUILDING" = "OSX10.6" ]; then
echo --- Found $EXO_BUILDING ---
 #export EXO_SDK=/Developer/SDKs/MacOSX10.4u.sdk #doesnt have backtrace
 #export EXO_SDK=/Developer/SDKs/MacOSX10.5.sdk
 export EXO_SDK=/Developer/SDKs/MacOSX10.6.sdk
 export EXO_MINVER=10.6
 export EXO_ARCH=i686
 export EXO_BUILD=x86_64-apple-darwin10.0.0
 export EXO_CC=gcc-4.2
 export EXO_CXX=g++-4.2

 export EXO_BOOST_JAM_USING="darwin : 10.0 : /usr/bin/g++-4.2 :"
 export EXO_BOOST_JAM_ARCHITECTURE=combined
 export EXO_BOOST_JAM_ADDRESS_MODEL=64
 #export EXO_BOOST_JAM_ADDRESS_MODEL=32_64
fi

if [ "$EXO_BUILDING" = "OSX10.4" ]; then
echo --- Found $EXO_BUILDING ---
 export EXO_SDK=/Developer/SDKs/MacOSX10.4u.sdk #doesnt have backtrace
 #export EXO_SDK=/Developer/SDKs/MacOSX10.5.sdk
 #export EXO_SDK=/Developer/SDKs/MacOSX10.6.sdk
 export EXO_MINVER=10.4
 export EXO_ARCH=i386
 export EXO_BUILD=i386-apple-darwin8.11.0
 export EXO_CC=gcc-4.0
 export EXO_CXX=g++-4.0

 export EXO_BOOST_JAM_USING="darwin : 8.11 : /usr/bin/g++-4.0 :"
 export EXO_BOOST_JAM_ARCHITECTURE=combined
 export EXO_BOOST_JAM_ADDRESS_MODEL=32
 #export EXO_BOOST_JAM_ADDRESS_MODEL=32_64
fi

#determine which include files and libs are used from /Developer/SDKs
export EXO_OSX_FLAGS="-arch $EXO_ARCH -mmacosx-version-min=$EXO_MINVER -march=prescott -isysroot $EXO_SDK"

#see XCODE's Cross-Development Programming Guide:Configuring a Makefile-Based Project
export MACOSX_DEPLOYMENT_TARGET=$EXO_MINVER

fi

# --- common to all tools ---

export EXO_EPREFIX=$EXO_PREFIX/$EXO_ARCH-$EXO_MINVER

export EXO_FLAGS="-I$EXO_PREFIX/include -I$HOME/$EXO_BOOST_DIR $EXO_OSX_FLAGS"
export EXO_LDFLAGS="-Bstatic -L$EXO_EPREFIX/lib"

#-----------
#--- Icu ---
#-----------
export EXO_ICU_DOWNLOADER=SUBVERSION
export EXO_ICU_VER=4_8
export EXO_ICU_URL_SUBVERSION=http://source.icu-project.org/repos/icu/icu/tags/release-4-8/
export EXO_ICU_URL_CURL=http://download.icu-project.org/files/icu4c/4.8/icu4c-4_8-src.tgz

export EXO_ICU_FILE=icu4c-4_8-src.tgz
export EXO_ICU_DIR=icu

#----------------
#--- Postgres ---
#----------------
export EXO_POSTGRES_BIN32=/Library/PostgreSQL/9.0/bin
export EXO_POSTGRES_BIN=$EXO_POSTGRES_BIN32

#--------------
#--- Exodus ---
#--------------
export EXO_EXODUS_REUSE_DOWNLOAD=YES
export EXO_EXODUS_DOWNLOADER=SUBVERSION
export EXO_EXODUS_URL_SUBVERSION=HTTPS://exodusdb.googlecode.com/svn/trunk/
export EXO_EXODUS_DIR=exodus
export EXO_EXODUS_FILE=
export EXO_EXODUS_URL=

export EXO_CODENAME=exodus

export PATH=/Library/PostgreSQL/9.0/bin:$PATH

test -f ./version.sh     && source ./version.sh
test -f ./configlocal.sh && source ./configlocal.sh

test "$EXO_BATCH_MAJOR_VER" = "" || export EXO_MAJOR_VER=$EXO_BATCH_MAJOR_VER
test "$EXO_BATCH_MINOR_VER" = "" || export EXO_MINOR_VER=$EXO_BATCH_MINOR_VER
test "$EXO_BATCH_MICRO_VER" = "" || export EXO_MICRO_VER=$EXO_BATCH_MICRO_VER
test "$EXO_BATCH_BUILD_VER" = "" || export EXO_BUILD_VER=$EXO_BATCH_BUILD_VER

# --------------------
# --- CLEAN COMMAND ---
# --------------------
  export EXO_CLEAN_CMD="make clean"
  export EXO_CLEAN_OPT=
  export EXO_DISTCLEAN_CMD="make distclean"
  export EXO_DISTCLEAN_OPT=

# --------------------
# --- MAKE COMMAND ---
# --------------------
  export EXO_CONFIGURE_CMD=./configure
  export EXO_CONFIGURE_OPT=
  export EXO_MAKE_CMD=make
  export EXO_MAKE_OPT=

# ------------------------
# --- INSTALLFILE NAME ---
# ------------------------
if [ "`uname`" = "Darwin" ]; then
  export EXO_INSTALLFILENAME="$EXO_CODENAME-$EXO_MAJOR_VER.$EXO_MINOR_VER.$EXO_MICRO_VER-osx-installer.app.zip"
else
  export EXO_INSTALLFILENAME="$EXO_CODENAME-$EXO_MAJOR_VER.$EXO_MINOR_VER.$EXO_MICRO_VER.tar.gz"
fi

#if [ "$EXO_CONFIGMODE" -eq "CLEAN" ] goto afteruploader
#if [ "$EXO_CONFIGMODE" -eq "MAKE"  ] goto afteruploader
#if [ "$EXO_CONFIGMODE" -eq "DEV"   ] goto afteruploader

# -------------------------
# --- COMMAND PACKER UI ---
# -------------------------
  export EXO_PACKER_CMD=/Applications/BitRock*/bin/Builder.app/Contents/MacOS/installbuilder.sh
  export EXO_PACKER_OPT="--project bitrock_all.xml"

# -----------------------
# --- COMMAND TO PACK ---
# -----------------------
if [ $EXO_UNAME = Darwin ]; then
  export EXO_PACK_CMD=./bitrock_all_osx.sh
  export EXO_PACK_OPT=
else
  export EXO_PACK_CMD=make dist
  export EXO_PACK_OPT=
fi

# needs EXO_UPLOADUSER UPLOADPASS_EXO EXO_INSTALLFILENAME
# -------------------------
# --- COMMAND TO UPLOAD ---
# -------------------------
# NOTE GOODLE CODE HAS NO OVERWRITE OPTION OR DELETE OPTION
  export EXO_UPLOAD_CMD=./googlecode_upload.py
  export EXO_UPLOAD_OPT=" \
 --summary=Mac_OSX_${TARGET_CPU}_Installer \
 --project=exodusdb \
 --user=$EXO_UPLOADUSER \
 --password=$UPLOADPASS_EXO \
 --labels=Type-Installer,OpSys-OSX,Featured \
 $EXO_INSTALLFILENAME"


#afteruploader:

#----------
#-- Log ---
#----------
set | grep EXO_



