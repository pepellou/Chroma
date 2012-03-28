all:
	test -d build || mkdir build
	g++ -I/opt/local/include -pthread -L/usr/local/lib -Wall -g main.c -o build/croma -lhighgui -lswscale -lavformat -lavcodec -ldl -lX11 -lXext -lXfixes -ljack -lasound -lSDL -lxvidcore -lx264 -lvorbisenc -lvorbis -ltheoraenc -ltheoradec -logg -lopencore-amrwb -lopencore-amrnb -lmp3lame -lfaac -lz -lavutil -lm
clean:
	test -d build && rm -rf build || true
