@rmdir /s /q build
@mkdir build
@cd build
cmake -G "MinGW Makefiles" ..
make 
@move open-world.exe ..
@cd ..
open-world.exe