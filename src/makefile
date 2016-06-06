wincc = i686-w64-mingw32-g++
winext = .exe 
linuxcc = g++
name = AapeliBlox
optimize =-O2
errors = # Change to -Wall for all warnings
hcon = -mwindows # Hide console

windows_i686:
	$(wincc) *.cpp -o release/$(name)$(winext) -lsfml-graphics -lsfml-window -lsfml-system $(errors) -std=c++11 $(optimize) -static $(hcon)
linux_release:
	$(linuxcc) *.cpp -o release/$(name) -lsfml-graphics -lsfml-window -lsfml-system $(errors) -std=c++11 $(optimize)
linux_debug:
	$(linuxcc) *.cpp -o release/$(name).debug -lsfml-graphics -lsfml-window -lsfml-system $(errors) -std=c++11 -ggdb
