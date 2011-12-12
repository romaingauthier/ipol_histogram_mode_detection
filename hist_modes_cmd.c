/*  The following program implements a histogram mode detection as
    described in the 7th chapter: "Modes of a histogram" of the book
    "From Gestalt theory to image analysis: a probabilistic approach",
    Desolneux, Moisan & Morel, 2008, Springer.

    Copyright (c)2011 Romain Gauthier <rj.gauthier@laposte.net>
                      Rafael Grompone von Gioi <grompone@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>     // to read the histogram from a file
#include <stdlib.h>    // to manage memory
#include "hist_modes.h"

static float * read_histogram(const char * filename, int * size){
    FILE * file = NULL;
    float * histogram;
    float num;
    int i;

    // count histrogram size
    file = fopen(filename,"r");
    *size = 0;
    while( fscanf(file,"%g",&num) == 1 ) (*size)++;
    fclose(file);

    // initialization of histogram
    histogram = malloc( *size * sizeof(float) );

    // read histogram
    file = fopen(filename,"r");
    for(i=0; i<*size; i++){
        fscanf(file,"%f",&num);
        histogram[i] = num;
    }
    fclose(file);

    return histogram;
}

static void write_modes(int * modes, int num_modes, char * filename){
    FILE * file = NULL;
    int i;

    file = fopen(filename,"w");
    for(i=0; i<num_modes; i++)
        fprintf(file,"%d %d\n", modes[2*i+0], modes[2*i+1]);
    fclose(file);
}

int main(int argc, char *argv[])
{
    float * histogram;
    int * modes;
    int num_modes,size;
    int circular = 0;
    float epsilon = 1.0;

    switch(argc){
        default:
            printf("use: hist <hist.txt> <output.txt> [epsilon [circular]]\n");
            exit(0);
        break;
        case 5:
            circular = atoi(argv[4]);
        case 4:
            epsilon = atof(argv[3]);
        case 3:
            histogram = read_histogram(argv[1],&size);
            modes = histogram_mode_detection(histogram,size,
                                             circular,epsilon,&num_modes);
            write_modes(modes,num_modes,argv[2]);
        break;
    }

    return 0;
}
