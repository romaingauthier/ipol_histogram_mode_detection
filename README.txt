###############################################################################
###############################################################################
#############                                                     #############
#############          HISTOGRAM MODE DETECTION version O.1       #############
#############                                                     #############
###############################################################################
###############################################################################


1. PRESENTATION

The project aims at detecting the modes of a histogram as described in the 7th 
chapter: "Modes of a histogram" of the book "From Gestalt theory to image 
analysis: a probabilistic approach" by Desolneux, Moisan & Morel, 2008
(publisher: Springer).


-------------------------------------------------------------------------------
2. DESCRIPTION

The project is composed of three files :
    hist_modes.h
    hist_modes.c
    hist_modes_cmd.c

No external library is needed.

-------------------------------------------------------------------------------
3. HOW TO INSTALL & RUN THE PROGRAM

* For Linux users, a makefile is provided to build the project.

* To run the program, several parameters can be specified as arguments :

    @param1 input_filename : name of the input file ; each line of the file must
                            contain only one value. If a line is empty or does
                            not contain any integer value, it will be skipped.

    @param2 output_filename : name of the output file.

    @param3(optional) epsilon : value of epsilon used in the detection.

    @param4(optional) circular : to decide wether the histogram is circular 
                                 (=1) or not (=0). (default = 0)

 Working command-line examples :
    @example$ .\hist_modes example.txt results.txt
    @example$ .\hist_modes example.txt results.txt 0.1
    @example$ .\hist_modes example.txt results.txt 0.5 1

-------------------------------------------------------------------------------
4. AUTHORS

Romain Gauthier               <rj.gauthier@laposte.net>	
Rafael Grompone von Gioi      <grompone@gmail.com>
    
-------------------------------------------------------------------------------
5. LICENSE

Please refer to the AGPLv3 (agpl-3.0.txt).
