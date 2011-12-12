/*
    The following program implements a histogram mode detection as
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

#include <stdio.h>  // to read the histogram from a file
#include <stdlib.h> // to manage memory
#include <math.h>   // to compute the logarithm
#include "hist_modes.h"

#ifndef FALSE
#define FALSE 0
#endif /* !FALSE */

#ifndef TRUE
#define TRUE 1
#endif /* !TRUE */

static void error(char * msg)
{
  fprintf(stderr,"error: %s\n",msg);
  exit(EXIT_FAILURE);
}

static int mod(int a, unsigned int b)
{
  if( b == 0 ) error("b can't be zero in mod operator");
  while( a < 0 ) a += b;
  while( a >= b ) a -= b;
  return a;
}

int * histogram_mode_detection( float * hist, int size, int circular, float eps,
                                int * num_modes )
{
    float * cumulated;          // cumulated histogram
    float * entropy;            // entropy table
    int * gap;                  // meaningful gap table
    int * mode;                 // meaningful mode table
    int * max_meaningful_mode;  // maximum meaningful mode table
    int * modes_list;           // list of maximal meaningful modes found
    float total,r,p,h,threshold;
    int i,j,a,b,n,found,mode_count;

    // allocate memory
    cumulated = (float *) malloc( size * sizeof(float) );
    entropy = (float *) malloc( size*size * sizeof(float) );
    gap = (int *) malloc( size*size * sizeof(int) );
    mode = (int *) malloc( size*size * sizeof(int) );
    max_meaningful_mode = (int *) malloc( size*size* sizeof(int) );
    if( cumulated==NULL || entropy==NULL || gap==NULL || mode==NULL || max_meaningful_mode==NULL )
        error("not enough memory");

    // compute cumulated histogram
    cumulated[0] = hist[0];
    for(i=1; i<size; i++) cumulated[i] = cumulated[i-1] + hist[i];
    total = cumulated[size-1];

    // detection threshold
    threshold = (1.0/total) * log( size * (size+1.0) / (2.0*eps) );

    // compute entropy of intervals [i,j]
    for(i=0; i<size; i++)
    for(j=(circular?0:i); j<size; j++){
        // compute r and p
        if(i<=j) r = (cumulated[j]-cumulated[i]+hist[i]) / total;
        else     r = ( total - (cumulated[i-1] - cumulated[j]) ) / total;
        p = (float)( (i<=j)?(j-i+1):size-(i-j-1) ) / size;

        // computation of the entropy
        if( r == 0.0 ) h = -log(1.0-p);
        else if( r == 1.0 ) h = -log(p);
        else h = r*log(r/p) + (1.0-r)*log((1.0-r)/(1.0-p));
        entropy[i*size+j] = h;

        // meaningful intervals & meaningful gaps
        if( r>p && h>threshold ) mode[i*size+j] = TRUE;
        else                     mode[i*size+j] = FALSE;
        if( r<p && h>threshold ) gap[i*size+j]  = TRUE;
        else                     gap[i*size+j]  = FALSE;
    }

    // compute meaningful modes
    for(i=0; i<size; i++)
    for(j=(circular?0:i); j<size; j++){
        if( mode[i*size+j] ){
            found = FALSE;
            if(i<=j){
                for(a=i; a<=j && !found; a++)
                for(b=a; b<=j && !found; b++)
                    if( gap[a*size+b] ) found = TRUE;
            }else{
                for(a=i; (a>=i || a<=j) && !found; a=mod(a+1,size))
                for(b=a; (b>=i || b<=j) && !found; b=mod(b+1,size))
                    if( gap[a*size+b] ) found = TRUE;
            }
            if(found) mode[i*size+j] = FALSE;
        }
    }
    
    // 
    for(i=0; i<size; i++)
    for(j=(circular?0:i); j<size; j++)
        max_meaningful_mode[i*size+j] = mode[i*size+j];

    // compute maximal meaningful modes
    for(i=0; i<size; i++)
    for(j=(circular?0:i); j<size; j++){
        if( mode[i*size+j] ){
            h = entropy[i*size+j];
            found = FALSE;

            // check the presence of an interior higher entropy interval
            if(i<=j){
                for(a=i; a<=j && !found; a++)
                for(b=a; b<=j && !found; b++)
                    if( mode[a*size+b] && entropy[a*size+b] > h ) found = TRUE;
            }else{
                for(a=i; (a>=i || a<=j) && !found; a=mod(a+1,size))
                for(b=a; (b>=i || b<=j) && !found; b=mod(b+1,size))
                    if( mode[a*size+b] && entropy[a*size+b] > h ) found = TRUE;
            }

            // check the presence of an exterior higher entropy interval
            if(circular){
                // if i=j, then all the bins can be the initial bin 'a'
                // of an external interval [a,b]. in this case, the only
                // way to control the loop is counting the number of bins
                // tested. the values of 'a' to test are all the bins
                // outside the inteval, plus the bin 'i'.
                // so the total number of cases n is:
                // i<=j:  n = size - (j-i)
                // i>j:   n = size - (i-j)
                n = i<=j ? size-j+i : size-i+j;
                for(a=i; n>0  && !found; a=mod(a-1,size),n--)
                for(b=j; b!=a && !found; b=mod(b+1,size))
                    if( mode[a*size+b] && entropy[a*size+b] > h ) found = TRUE;
            }else{ // if the histogram is not circular, then i<=j
                for(a=i; a>=0 && !found; a--)
                for(b=j; b<size  && !found; b++)
                    if( mode[a*size+b] && entropy[a*size+b] > h ) found = TRUE;
            }

            // if an internal mode has entropy > h
            // or an external mode has entropy >= h,
            // then this is not a maximal meaningful interval
            if(found) max_meaningful_mode[i*size+j] = FALSE;
        }
    }

    // count the number of maximal meaningful modes
    mode_count = 0;
    for(i=0; i<size; i++)
    for(j=(circular?0:i); j<size; j++)
    if( max_meaningful_mode[i*size+j] )
        ++mode_count;

    // allocate the memory to store the maximal meaningful modes
    modes_list = (int *) malloc( 2 * mode_count * sizeof(int) );
    if( modes_list == NULL ) error("not enough memory");

    // store modes
    n=0;
    for(i=0; i<size; i++)
    for(j=(circular?0:i); j<size; j++)
    if( max_meaningful_mode[i*size+j] ){
        modes_list[2*n+0] = i;
        modes_list[2*n+1] = j;
        ++n;
    }

    // free memory
    free(cumulated);
    free(entropy);
    free(gap);
    free(mode);
    free(max_meaningful_mode);

    // return result
    *num_modes = mode_count;
    return modes_list;
}
