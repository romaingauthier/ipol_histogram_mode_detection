all: hist_modes clean

hist_modes: hist_modes_cmd.o hist_modes.o
	cc -Wall -o hist_modes hist_modes.o hist_modes_cmd.o -lm

hist_modes_cmd.o: hist_modes.h
	cc -Wall -c hist_modes_cmd.c

hist_modes.o: hist_modes.h
	cc -Wall -c hist_modes.c 

clean:
	rm hist_modes.o hist_modes_cmd.o
