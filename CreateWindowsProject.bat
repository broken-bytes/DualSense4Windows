mkdir %CD%./build
cd %CD%./build
start cmd /k "cmake -B ./build/win64 -S ./ -G "Visual Studio 16 2019""