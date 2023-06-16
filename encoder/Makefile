all: em-encode em-encode-amo em-encode-fbc em-encode-all

em-encode: em-encode.c
	gcc em-encode.c -std=c99 -O2 -o em-encode

em-encode-amo: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DATMOSTONE -o em-encode-amo

em-encode-fbc: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DFBCOLORS -o em-encode-fbc

em-encode-all: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DATMOSTONE -DFBCOLORS -o em-encode-all

clean:
	rm em-encode em-encode-amo em-encode-fbc em-encode-all
