all:
	mkdir build
	gcc -I/opt/local/include -pthread -L/usr/local/lib -Wall -g main.c -o build/croma -lswscale -lavformat -lavcodec -ldl -lX11 -lXext -lXfixes -ljack -lasound -lSDL -lxvidcore -lx264 -lvorbisenc -lvorbis -ltheoraenc -ltheoradec -logg -lopencore-amrwb -lopencore-amrnb -lmp3lame -lfaac -lz -lavutil -lm
clean:
	rm -rf build
