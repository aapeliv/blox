wincc = i686-w64-mingw32-g++
winext = .exe 
linuxcc = g++
maccc = c++
name = AapeliBlox
optimize =-O2
hcon = -mwindows # Hide console
version = `cat version`
cflags = -lsfml-graphics -lsfml-window -lsfml-system -std=c++11 -Wall

all: win linux mac

win: win-pkg

linux: linux-pkg

mac: mac-pkg

win-build:
	$(wincc) src/*.cpp -o bin/$(name)Windows$(winext) $(cflags) -std=c++11 $(optimize) -static $(hcon)

linux-build:
	$(linuxcc) src/*.cpp -o bin/$(name)Linux $(cflags) -std=c++11 $(optimize)

mac-build:
	$(maccc) src/*.cpp -o bin/$(name)Mac $(cflags) -std=c++11

builds: win-build linux-build mac-build

win-pkg: win-build
	mkdir windows
	cp skel/licenses/* windows
	cp skel/dlls/* windows
	cp bin/$(name)Windows$(winext) windows/$(name)$(winext)
	sed -e 's/{{version}}/'"$(version)"'/g' skel/readmes/Readme.txt > windows/Readme.txt
	cd windows && zip $(name)Windows.zip * && cp $(name)Windows.zip ../pkg

linux-pkg: linux-build
	mkdir linux
	cp skel/licenses/* linux
	cp bin/$(name)Linux linux/$(name)
	sed -e 's/{{version}}/'"$(version)"'/g' skel/readmes/Readme > linux/Readme
	cd linux && tar cvf $(name)Linux.tar.gz * && cp $(name)Linux.tar.gz ../pkg

mac-pkg: mac-build
	mkdir mac 
	cp skel/licenses/* mac
	cp bin/$(name)Mac mac/$(name)
	sed -e 's/{{version}}/'"$(version)"'/g' skel/readmes/Readme > mac/Readme
	cd mac && tar cvf $(name)Mac.tar.gz * && cp $(name)Mac.tar.gz ../pkg

pkg: win-pkg linux-pkg mac-pkg

clean:
	rm -rf linux
	rm -rf windows
	rm -rf mac
	rm -rf bin/*
	rm -rf pkg/*
