CPP=g++	
INCLUDES=-Icapengine
CFLAGS=  -g -DDEBUG -std=c++11 -pedantic `sdl2-config --cflags` `xml2-config --cflags` $(INCLUDES)
LIBS= `sdl2-config --cflags --libs` `xml2-config --libs` -lSDL2_image -lboost_system -lboost_filesystem capengine/libcapengine.so

objects=balloon.o start_menu_state.o arena_play_state.o player.o dumb_graphics_component.o player_input_component.o \
		player_physics_component.o pausestate.o keyboard_control_scheme.o

all: balloon

balloon: $(objects)
	$(CPP) $(CFLAGS) -o balloon $(objects) $(LIBS)

$(objects): %.o: %.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

.PHONEY: clean tests tags

clean:
	rm -f *.o
	rm balloon

tests:
	cd test && $(MAKE)

tags:
	gtags

