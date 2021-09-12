CXXL = g++
CXXLF = -D__LINUX_ALSA__ -Wall -pedantic -g3 -lasound -lpthread
LINKERL = g++
LINKERLF = -D__LINUX_ALSA__ -Wall ../rtmidi/RtMidi.cpp -lasound -lpthread

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:src/%.cpp=obj/%.o)

.PHONY: compile_linux
compile_linux: midiprocessor

midiprocessor: $(OBJECTS)
	@$(LINKERL) $(OBJECTS) $(LINKERLF) -o $@

$(OBJECTS): obj/%.o : src/%.cpp
	@$(CXXL) $(CXXLF) -c $< -o $@
	
.PHONY: clean
clean:
	@rm -f obj/*.o
	@rm -f MIDIProcessor
