OUTPUT=sciche

OBJS=src/main.o \
     src/vm/vm.o \
     src/compiler/compiler.o \
     src/compiler/token.o \
     src/compiler/scanner.o \
     src/streamer/console_streamer.o

CFLAGS=$(shell pkg-config --cflags readline) -Isrc -Isrc/compiler -Isrc/streamer -Isrc/value -Isrc/vm -Iinclude $(USR_CFLAGS)
CXXFLAGS=$(CFLAGS)
LIBS=$(shell pkg-config --libs readline)

%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all:
	$(MAKE) $(OUTPUT)

debug:
	$(MAKE) $(OUTPUT) USR_CFLAGS="-g -DDEBUG"

$(OUTPUT): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

clean:
	rm -f $(OUTPUT) $(OBJS)
