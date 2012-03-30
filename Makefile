LIB=-L/usr/local/lib -lhighgui -lswscale -lavformat -lavcodec -ldl -lX11 -lXext -lXfixes -ljack -lasound -lSDL -lxvidcore -lx264 -lvorbisenc -lvorbis -ltheoraenc -ltheoradec -logg -lopencore-amrwb -lopencore-amrnb -lmp3lame -lfaac -lz -lavutil -lm
INC=-I/opt/local/include

default:
	make all

all:
	test -d build || mkdir build
	g++ -pthread -Wall $(LIB) $(INC) -g main.c -o build/croma

clean:
	test -d build && rm -rf build || true
