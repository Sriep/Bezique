#!/bin/bash
# Batch file for auto build

# Set project identity variables
PROJECTNAME=Bezique
GITHUB_URL=https://github.com/Sriep/Bezique.git
# WARNING rm -rf used cearful with path\
BUILD_DIR=$HOME/Documents/Build/Build_$PROJECTNAME
# Alternatly add date to directory name
# TODAY=$(date -I)
# BUILD_DIR=/home/piers/Documents/Build/Build_$PROJECTNAME.$TODAY

# Set executable locations
QT_DIR=$HOME/Qt/5.9.1/gcc_64
QT_CREATORDIR=$HOME/Qt/Tools/QtCreator
MAKE_DIR=/usr/bin
LINUXDEPLOY=$HOME/software/linuxdeployqt

# Set internal variables
SRCDIR=$BUILD_DIR/$PROJECTNAME/src
PATH=$QT_DIR/bin:$QT_CREATOR_DIR/bin:$MAKE_DIR:$PATH
RELEASE_DIR=$SRCDIR/release
QML_DIR=$SRCDIR

# Setup build directory
rm -r -f $BUILD_DIR
mkdir $BUILD_DIR

# Download from GitHub
cd $BUILD_DIR
git clone $GITHUB_URL


# Build project
cd $SRCDIR
mkdir release
cd $RELEASE_DIR
qmake  $SRCDIR/$PROJECTNAME.pro -spec linux-g++ && make qmake_all
make 
make clean 

# Build AppImage
mkdir $RELEASE_DIR/plugins
cp -r $QT_DIR/plugins/platforms $RELEASE_DIR/plugins/platforms
$LINUXDEPLOY  $RELEASE_DIR/$PROJECTNAME -qmldir=$QML_DIR -appimage -bundle-non-qt-libs -no-translations 
