#!/bin/bash
set -e

export SWIG_SYNTAX="Syntax is ./install.sh "

export SWIG_TARGET=$1

source config.sh

if [ "$SWIG_TARGET" == "all" ]; then
	for  SWIG_TARGET in $SWIG_ALL_TARGETS; do
		./install.sh $SWIG_TARGET
	done
        echo "all done"
        exit 0
fi

#------------
#--- "cd" ---
#------------
test -d $SWIG_TARGET || ( echo "Make $SWIG_TARGET first" && exit 1 )
cd $SWIG_TARGET

echo
echo --------------------
echo $SWIG_TARGET install
echo --------------------
echo Entering: `pwd`

#-----------------
#--- "Install" ---
#-----------------

if [ "$SWIG_MODULE_INSTALL" != "" ]; then
	echo
	echo -ne "Installing: "
	echo eval $SWIG_MODULE_INSTALL
	     eval $SWIG_MODULE_INSTALL
fi

if [ "$SWIG_TARGET_LIBDIR" != "" ]; then
	echo
	echo -ne "Installing $SWIG_TARGET library: "
	echo cp -f $SWIG_TARGET_LIBFILE $SWIG_TARGET_LIBDIR/
	     cp -f $SWIG_TARGET_LIBFILE $SWIG_TARGET_LIBDIR/
fi

if [ "$SWIG_TARGET_MODDIR" != "" ]; then
	echo
	echo -ne "Installing $SWIG_TARGET module: "
	echo cp -f $SWIG_TARGET_MODFILE $SWIG_TARGET_MODDIR/
	     cp -f $SWIG_TARGET_MODFILE $SWIG_TARGET_MODDIR/
fi
sleep 1
