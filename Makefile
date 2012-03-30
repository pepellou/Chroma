LIB=-L/usr/local/lib -lhighgui -lswscale -lavformat -lavcodec -ldl -lX11 -lXext -lXfixes -ljack -lasound -lSDL -lxvidcore -lx264 -lvorbisenc -lvorbis -ltheoraenc -ltheoradec -logg -lopencore-amrwb -lopencore-amrnb -lmp3lame -lfaac -lz -lavutil -lm
INC=-I/opt/local/include -I/home/pepellou/workspace/igloo

all:
	make croma
	make test

croma:
	test -d build || mkdir build
	g++ -pthread -Wall $(LIB) $(INC) -g main.cpp -o build/croma

test:
	test -d build || mkdir build
	g++ -pthread -Wall $(LIB) $(INC) -g chroma.cpp test.cpp -o build/test

clean:
	test -d build && rm -rf build || true
