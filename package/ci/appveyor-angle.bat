call "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/vcvarsall.bat" || exit /b
set PATH=%APPVEYOR_BUILD_FOLDER%/openal/bin/Win32;%APPVEYOR_BUILD_FOLDER%\deps\bin;%PATH%

rem Build ANGLE
git clone --depth 1 git://github.com/MSOpenTech/angle.git || exit /b
cd angle\build || exit /b
msbuild angle.sln /p:Configuration=Release /t:libEGL;libGLESv2 /m /v:m || exit /b
cd ..\..\..\.. || exit /b

rem Build Corrade
git clone --depth 1 git://github.com/mosra/corrade.git || exit /b
cd corrade || exit /b
mkdir build && cd build || exit /b
cmake .. ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%APPVEYOR_BUILD_FOLDER%/deps ^
    -DWITH_INTERCONNECT=OFF ^
    -G Ninja || exit /b
cmake --build . || exit /b
cmake --build . --target install || exit /b
cd ..\.. || exit /b

rem Build
mkdir build && cd build || exit /b
cmake .. ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%APPVEYOR_BUILD_FOLDER%/deps ^
    -DCMAKE_PREFIX_PATH="%APPVEYOR_BUILD_FOLDER%/openal" ^
    -DEGL_LIBRARY=%APPVEYOR_BUILD_FOLDER%/angle/build/Release_Win32/lib/libEGL.lib ^
    -DEGL_INCLUDE_DIR=%APPVEYOR_BUILD_FOLDER%/angle/include ^
    -DOPENGLES2_LIBRARY=%APPVEYOR_BUILD_FOLDER%/angle/build/Release_Win32/lib/libGLESv2.lib ^
    -DOPENGLES2_INCLUDE_DIR=%APPVEYOR_BUILD_FOLDER%/angle/include ^
    -DOPENGLES3_LIBRARY=%APPVEYOR_BUILD_FOLDER%/angle/build/Release_Win32/lib/libGLESv2.lib ^
    -DOPENGLES3_INCLUDE_DIR=%APPVEYOR_BUILD_FOLDER%/angle/include ^
    -DWITH_AUDIO=ON ^
    -DWITH_SDL2APPLICATION=OFF ^
    -DWITH_WINDOWLESSWINDOWSEGLAPPLICATION=ON ^
    -DWITH_WGLCONTEXT=ON ^
    -DWITH_MAGNUMFONT=ON ^
    -DWITH_MAGNUMFONTCONVERTER=ON ^
    -DWITH_OBJIMPORTER=ON ^
    -DWITH_TGAIMAGECONVERTER=ON ^
    -DWITH_TGAIMPORTER=ON ^
    -DWITH_WAVAUDIOIMPORTER=ON ^
    -DWITH_DISTANCEFIELDCONVERTER=ON ^
    -DWITH_FONTCONVERTER=ON ^
    -DWITH_MAGNUMINFO=ON ^
    -DBUILD_TESTS=ON ^
    -DBUILD_GL_TESTS=ON ^
    -G Ninja || exit /b
cmake --build . || exit /b
cmake --build . --target install || exit /b

rem Test
ctest -V -E GLTest || exit /b
