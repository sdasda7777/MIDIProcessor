CXXL = g++
CXXLF = -D__LINUX_ALSA__ -Wall -pedantic -g3 -lasound -lpthread
LINKERL = g++
LINKERLF = -D__LINUX_ALSA__ -Wall ../rtmidi/RtMidi.cpp -lasound -lpthread

CXXW = x86_64-w64-mingw32-g++
CXXWF = 
LINKERW = x86_64-w64-mingw32-g++
LINKERWF = 

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:src/%.cpp=obj/%.o)

compile_linux: MIDIProcessor

compile_win: MIDIProcessor.exe

MIDIProcessor: $(OBJECTS)
	@$(LINKERL) $(OBJECTS) $(LINKERLF) -o $@

$(OBJECTS): obj/%.o : src/%.cpp
	@$(CXXL) $(CXXLF) -c $< -o $@
	
clean:
	@rm -f obj/*.o
	@rm -f MIDIProcessor