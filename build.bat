@rmdir /s /q build
@mkdir build
@cd build
cmake -G "MinGW Makefiles" ..
make 
@cd ..
open-world.exe