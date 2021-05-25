
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
 * WRspice Circuit Simulation and Analysis Tool                           *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

#ifndef UIDHASH_H
#define UIDHASH_H

#include "circuit.h"
#include "miscutil/symtab.h"


// A uid hash table for mapping IFuid names to device instance
// structs, for sGENmodel.  Hashing speeds up construction for large
// circuits.

struct sGENinstTable
{
    sGENinstTable()
        {
            inst_tab = 0;
        }

    ~sGENinstTable()
        {
            delete inst_tab;
        }

    sGENinstance *find(IFuid n)
        {
            if (!inst_tab || !n)
                return (0);
            return (inst_tab->find((uintptr_t)n));
        }

    sGENinstance *remove(IFuid n)
        {
            if (!inst_tab || !n)
                return (0);
            return (inst_tab->remove((uintptr_t)n));
        }

    void link(sGENinstance *inst)
        {
            if (!inst_tab)
                inst_tab = new itable_t<sGENinstance>;
            inst_tab->link(inst, true);
            inst_tab = inst_tab->check_rehash();
        }

private:
    itable_t<sGENinstance> *inst_tab;
};

#endif

