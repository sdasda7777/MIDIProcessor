OS = linux

ifeq ($(OS), linux)
BACKEND = alsa
else
ifeq ($(OS), windows)
BACKEND = mm
else
$(warning Unknown OS $(OS). No default backend will be specified.)
endif
endif

OSUC = $(shell echo '$(OS)' | tr '[:lower:]' '[:upper:]')
BACKENDUC = $(shell echo '$(BACKEND)' | tr '[:lower:]' '[:upper:]')
BACKENDDEF = __$(OSUC)_$(BACKENDUC)__

ifeq ($(BACKEND), alsa)
BACKENDOPTS = -lasound -lpthread
else
ifeq ($(BACKEND), jack)
BACKENDOPTS = -ljack
else
ifeq ($(BACKEND), mm)
else
$(warning Unknown backend $(BACKEND). No extra backend specific options will be supplied to the compiler. Note that the input backend name must be all lowercase.)
endif
endif
endif

RTMIDIDIR = rtmidi

CXXL = g++
CXXLF = -D$(BACKENDDEF) -Wall -pedantic -g3 $(BACKENDOPTS)
LINKERL = g++
LINKERLF = -D$(BACKENDDEF) -Wall $(RTMIDIDIR)/RtMidi.cpp $(BACKENDOPTS)

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
	@rm -f midiprocessor
