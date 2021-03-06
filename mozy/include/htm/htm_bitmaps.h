
/*========================================================================*
 *                                                                        *
 *  Distributed by Whiteley Research Inc., Sunnyvale, California, USA     *
 *                       http://wrcad.com                                 *
 *  Copyright (C) 2017 Whiteley Research Inc., all rights reserved.       *
 *  Author: Stephen R. Whiteley, except as indicated.                     *
 *                                                                        *
 *  As fully as possible recognizing licensing terms and conditions       *
 *  imposed by earlier work from which this work was derived, if any,     *
 *  this work is released under the Apache License, Version 2.0 (the      *
 *  "License").  You may not use this file except in compliance with      *
 *  the License, and compliance with inherited licenses which are         *
 *  specified in a sub-header below this one if applicable.  A copy       *
 *  of the License is provided with this distribution, or you may         *
 *  obtain a copy of the License at                                       *
 *                                                                        *
 *        http://www.apache.org/licenses/LICENSE-2.0                      *
 *                                                                        *
 *  See the License for the specific language governing permissions       *
 *  and limitations under the License.                                    *
 *                                                                        *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      *
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-        *
 *   INFRINGEMENT.  IN NO EVENT SHALL WHITELEY RESEARCH INCORPORATED      *
 *   OR STEPHEN R. WHITELEY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER     *
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,      *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE       *
 *   USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                        *
 *========================================================================*
 *               XicTools Integrated Circuit Design System                *
 *                                                                        *
 * MOZY html help viewer files                                            *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

/*------------------------------------------------------------------------*
 * This file is part of the gtkhtm widget library.  The gtkhtm library
 * was derived from the gtk-xmhtml library by:
 *
 *   Stephen R. Whiteley  <srw@wrcad.com>
 *   Whiteley Research Inc.
 *------------------------------------------------------------------------*
 *  The gtk-xmhtml widget was derived from the XmHTML library by
 *  Miguel de Icaza  <miguel@nuclecu.unam.mx> and others from the GNOME
 *  project.
 *  11/97 - 2/98
 *------------------------------------------------------------------------*
 * Author:  newt
 * (C)Copyright 1995-1996 Ripley Software Development
 * All Rights Reserved
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *------------------------------------------------------------------------*/

// XPM
static const char * const boomerang_xpm[] = {
// width height ncolors chars_per_pixel
"32 32 16 1",
// colors
" 	s None	c None",
".	c #FFCC9A",
"X	c #DDDDDD",
"o	c #6699FF",
"O	c #FF9966",
"+	c white",
"@	c #0000EF",
"#	c #9A6633",
"$	c #CB6532",
"%	c #FF6666",
"&	c #FE0000",
"*	c #890000",
"=	c #BBBBBB",
"-	c #663300",
";	c #666666",
":	c #000066",
// pixels
"               .......XXo       ",
"           ....OOOOOOO++@oXX    ",
"        ...OOO###$$##$++@@++%%% ",
"      ..OO##$##$$#$#$$XX@@++&&&*",
"     .O####$#$###$#$#$==@@==&&**",
"    .O#$####$###------;;::;;*** ",
"   .O#$#######---:::-#          ",
"   .O$######---:#               ",
"  .O#######--:#                 ",
"  .O######--:                   ",
"  .######--:                    ",
" .O#####--:                     ",
" .O####--:                      ",
" .O####-:                       ",
" .####--#                       ",
".O#$##-:                        ",
".O#$##-#                        ",
".O$##-:                         ",
".O$##-:                         ",
".O###-:                         ",
".O$##--                         ",
".O$$$$#                         ",
"X++X=;                          ",
"X++X=;                          ",
"o@@@@:                          ",
" o@@@:                          ",
" X++=;                          ",
" X++=;                          ",
"  %&&*                          ",
"  %&&*                          ",
"  %&**                          ",
"   **                           "};


// XPM
static const char * const img_xpm[] = {
"32 32 7 1",
" 	c #AEBAAEBAAEBA",
".	c #FFFFFFFFFFFF",
"X	c #000000000000",
"o	c #AEBA8E384103",
"O	c #0000FFFFFFFF",
"+	c #FFFFFFFF0000",
"@	c #00000000FFFF",
"                                ",
" .......X.......X.......X...... ",
" .......X.......X.......X...... ",
" .......X.......X.......X...... ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" ...........X.......X.......... ",
" ....XXXXXXXXXXXXXXXXXXXXXX.... ",
" ....XooooooooooooooooooooX.... ",
" XXXXXooooooooooooooooooooXXXXX ",
" ....XooXXXXXXXXXXXXXXXXooX.... ",
" ....XooXOOOOOOOOO+++++XooX.... ",
" ....XooXOOOOO.OOO+++++XooX.... ",
" XXXXXooXOOOO..OOO+++++XooXXXXX ",
" ....XooXOOO....OO+++++XooX.... ",
" ....XooXOOO....OOO++++XooX.... ",
" ....XooXOO.....OOOOOOOXooX.... ",
" XXXXXooXOXXXXXXXOOOOOOXooXXXXX ",
" ....XooX@XXXXXXX@@@@@@XooX.... ",
" ....XooX@@XXXXX@@@@@@@XooX.... ",
" ....XooX@@@@@@@@@@@@@@XooX.... ",
" XXXXXooX@@@@@@@@@@@@@@XooXXXXX ",
" ....XooX@@@@@@@@@@@@@@XooX.... ",
" ....XooXXXXXXXXXXXXXXXXooX.... ",
" ....XooooooooooooooooooooX.... ",
" XXXXXooooooooooooooooooooXXXXX ",
" ....XXXXXXXXXXXXXXXXXXXXXX.... ",
" .......X.......X.......X...... ",
" .......X.......X.......X...... ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" ...X.......X.......X.......X.. ",
" ...X.......X.......X.......X.. ",
"                                "};

// XPM
static const char * const noboomerang_xpm[] = {
// width height ncolors chars_per_pixel
"32 32 15 1",
// colors
" 	s None	c None",
".	c #FE0000",
"X	c #FF6666",
"o	c #FFCC9A",
"O	c #DDDDDD",
"+	c #6699FF",
"@	c #890000",
"#	c #FF9966",
"$	c #0000EF",
"%	c #9A6633",
"&	c #CB6532",
"*	c #BBBBBB",
"=	c #653232",
"-	c #666666",
";	c black",
// pixels
"..X            oooooooOO+    X..",
"@..X       oooo#######  $+OOX...",
"@@..X   ooo###%%%&&%%&  $$ X...@",
" @@..Xoo##%%&%%&&%&%&&OO$$X...@@",
"  @@..X%%%%&%&%%%&%&%&**$X...@@@",
"   @@..X%%%%&%%%======--X...@@@ ",
"   o@@..X%%%%%===;;;=% X...@    ",
"   o#@@..X%%===;%     X...@     ",
"  o#%%@@..X==;%      X...@      ",
"  o#%%%@@..X;       X...@       ",
"  o%%%%%@@..X      X...@        ",
" o#%%%%%=@@..X    X...@         ",
" o#%%%%==;@@..X  X...@          ",
" o#%%%%=;  @@..XX...@           ",
" o%%%%==%   @@.....@            ",
"o#%&%%=;     @@..@@             ",
"o#%&%%=%     X....X             ",
"o#&%%=;     X..@@..X            ",
"o#&%%=;    X...@@@..X           ",
"o#%%%=;   X...@  @@..X          ",
"o#&%%==  X...@    @@..X         ",
"o#&&&&% X...@      @@..X        ",
"O  O*- X...@        @@..X       ",
"O  O*-X...@          @@..X      ",
"+$$$$X...@            @@..X     ",
" +$$X...@              @@..X    ",
" O X...@                @@..X   ",
" OX...@                  @@..X  ",
" X...@                    @@..X ",
"X...@@                     @@..X",
"...@@@                      @@@@",
"..@@@                        @@@"};


// XPM
static const char * const noimg_xpm[] = {
"32 32 4 1",
" 	c #AEBAAEBAAEBA",
".	c #FFFFFFFFFFFF",
"X	c #000000000000",
"o	c #CD00CD00CD00",
"                                ",
" .......X.......X.......X...... ",
" .......X.......X.......X...... ",
" .......X.......X.......X...... ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" ...X.......X.......X.......X.. ",
" ...X...ooooXoooooooXooo....X.. ",
" ...X...ooooXoooooooXooo....X.. ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" .......XoooooooXoooooooX...... ",
" .......XoooooooXoooooooX...... ",
" .......XoooooooXoooooooX...... ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" ...X...ooooXoooooooXooo....X.. ",
" ...X...ooooXoooooooXooo....X.. ",
" ...X...ooooXoooooooXooo....X.. ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" .......XoooooooXoooooooX...... ",
" .......XoooooooXoooooooX...... ",
" .......XoooooooXoooooooX...... ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" ...X...ooooXoooooooXooo....X.. ",
" ...X...ooooXoooooooXooo....X.. ",
" ...X...ooooXoooooooXooo....X.. ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" .......X.......X.......X...... ",
" .......X.......X.......X...... ",
" .......X.......X.......X...... ",
" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
" ...X.......X.......X.......X.. ",
" ...X.......X.......X.......X.. ",
"                                "};

// XPM
static const char * const onepix_xpm[] = {
// width height ncolors chars_per_pixel
"1 1 1 1",
// colors
" 	s None	c None",
// pixels
" "};

