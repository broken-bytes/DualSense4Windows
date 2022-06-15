cmake -G "Visual Studio 16 2019" -B "projects/Visual Studio 2019" -S ./ -DCMAKE_PREFIX_PATH=E:/libs/Qt/6.2.4/msvc2019_64;
cmake --build "projects/Visual Studio 2019" --config Debug
