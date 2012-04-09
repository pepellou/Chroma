LIB=-L/usr/local/lib -lhighgui -lswscale -lavformat -lavcodec -ldl -lX11 -lXext -lXfixes -ljack -lasound -lSDL -lxvidcore -lx264 -lvorbisenc -lvorbis -ltheoraenc -ltheoradec -logg -lopencore-amrwb -lopencore-amrnb -lmp3lame -lfaac -lz -lavutil -lm

INC=-I/opt/local/include \
	-I/home/pepellou/workspace/igloo \
	-Iinc

SRC=src/chroma.cpp \
	src/camera.cpp

TEST=tests/test.cpp

MAIN=src/main.cpp

all:
	make croma
	make test

croma:
	test -d build || mkdir build
	g++ -pthread -Wall $(LIB) $(INC) -g $(SRC) $(MAIN) -o build/croma

test:
	test -d build || mkdir build
	g++ -pthread -Wall $(LIB) $(INC) -g $(SRC) $(TEST) -o build/test

clean:
	test -d build && rm -rf build || true
