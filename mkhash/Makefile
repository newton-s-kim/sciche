PROGRAMME=../tool/mkhash

all: $(PROGRAMME)

debug:
	$(MAKE) USR_CFLAGS="-g"
$(PROGRAMME):
	g++ -o $(PROGRAMME) main.cc -I ../nsl -I ../byte-code/src $(USR_CFLAGS)

clean:
	rm -rf $(PROGRAMME)
