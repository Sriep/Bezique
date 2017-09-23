REM Batch file for auto build

REM Set project identity variables
set PROJECTNAME=Bezique
set GITHUB_URL=https://github.com/Sriep/Bezique.git
set BUILD_DIR=C:\Data\Build_%PROJECTNAME%

REM Set executable locations
set QT_DIR=C:\Qt\5.9.1\msvc2015_64
set QT_CREATORDIR=C:\Qt\Tools\QtCreator

REM Set internal variables
set SRCDIR=%BUILD_DIR%\%PROJECTNAME%\Src
set PATH=%QT_DIR%\bin;%QT_CREATOR_DIR%/bin;%PATH%

REM Setup build directory
mkdir %BUILD_DIR%
cd %BUILD_DIR%

REM Download from GitHub
git clone %GITHUB_URL%

REM Initialise enviormental varables for MSVC compliers
call C:\"Program Files (x86)\Microsoft Visual Studio 14.0\VC"\vcvarsall.bat amd64

REM Build project
cd %SRCDIR%
qmake.exe %SRCDIR%\%PROJECTNAME%.pro -spec win32-msvc "CONFIG+=qtquickcompiler" && jom.exe qmake_all
jom.exe 
jom.exe clean

REM Build release
set WINDEPLOYQT_EXE=%QT_DIR%\bin\windeployqt.exe
set RELEASE_DIR=%SRCDIR%\release
set QML_DIR=%SRCDIR%
%WINDEPLOYQT_EXE% --qmldir %QML_DIR% %RELEASE_DIR%\%PROJECTNAME%.exe

REM Create install file
set INNO_DIR="C:\Program Files (x86)\Inno Setup 5"
cd %INNO_DIR%
iscc /Q %SRCDIR%\deploy\%PROJECTNAME%.iss
