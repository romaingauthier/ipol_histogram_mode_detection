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

#ifndef DEF_HISTOGRAM
#define DEF_HISTOGRAM

int * histogram_mode_detection( float * hist, int L, int circular, float eps,
                                int * num_modes );

#endif
