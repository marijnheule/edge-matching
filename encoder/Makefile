all: em-encode-plain em-encode-amo em-encode-fbc em-encode-all em-encode-aux em-encode-amo-aux em-encode-fbc-aux em-encode-all-aux

em-encode-plain: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DPAIRWISE -o em-encode-plain

em-encode-amo: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DPAIRWISE -DATMOSTONE -o em-encode-amo

em-encode-fbc: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DPAIRWISE -DFBCOLORS -o em-encode-fbc

em-encode-all: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DPAIRWISE -DATMOSTONE -DFBCOLORS -o em-encode-all

em-encode-aux: em-encode.c
	gcc em-encode.c -std=c99 -O2 -o em-encode-aux

em-encode-amo-aux: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DATMOSTONE -o em-encode-amo-aux

em-encode-fbc-aux: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DFBCOLORS -o em-encode-fbc-aux

em-encode-all-aux: em-encode.c
	gcc em-encode.c -std=c99 -O2 -DATMOSTONE -DFBCOLORS -o em-encode-all-aux

clean:
	rm em-encode-plain em-encode-amo em-encode-fbc em-encode-all em-encode-aux em-encode-amo-aux em-encode-fbc-aux em-encode-all-aux
