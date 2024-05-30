CC = gcc
INCLUDES = -I include
CFLAGS = -lm

VPATH = include lib obj src

pb : pb.o libpersonal.o
	$(CC) $^ -o $@ $(CFLAGS)
	mv *.o obj
	mv $@ build
pb.o : pb.c libpersonal.h
	$(CC) $(INCLUDES) -c $<



multi_free : multi_free.o libpersonal.o
	$(CC) $^ -o $@ $(CFLAGS)
	mv *.o obj
	mv $@ build
multi_free.o : multi_free.c libpersonal.h
	$(CC) $(INCLUDES) -c $<



bit_extend_example : bit_extend_example.o libpersonal.o
	$(CC) $^ -o $@ $(CFLAGS)
	mv *.o obj
	mv $@ build
bit_extend_example.o : bit_extend_example.c libpersonal.h
	$(CC) $(INCLUDES) -c $<



libpersonal.o : libpersonal.c libpersonal.h
	$(CC) $(INCLUDES) -c $<



clean : 
	rm -f obj/*
