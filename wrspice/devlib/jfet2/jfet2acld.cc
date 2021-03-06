
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
 * WRspice Circuit Simulation and Analysis Tool:  Device Library          *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

/**********
Based on jfetacld.c
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles

Modified to add PS model and new parameter definitions ( Anthony E. Parker )
   Copyright 1994  Macquarie University, Sydney Australia.
   10 Feb 1994:  New call to PSacload() with matrix loading
**********/

#include "jfet2defs.h"

#define JFET2nextModel      next()
#define JFET2nextInstance   next()
#define JFET2instances      inst()


int
JFET2dev::acLoad(sGENmodel *genmod, sCKT *ckt)
{
    sJFET2model *model = static_cast<sJFET2model*>(genmod);
    sJFET2instance *here;

    double gdpr;
    double gspr;
    double gm;
    double gds;
    double ggs;
    double xgs;
    double ggd;
    double xgd;
    double xgm, xgds, vgd, vgs, cd;

    for( ; model != NULL; model = model->JFET2nextModel ) {
        
        for( here = model->JFET2instances; here != NULL; 
                here = here->JFET2nextInstance) {


            gdpr=model->JFET2drainConduct * here->JFET2area;
            gspr=model->JFET2sourceConduct * here->JFET2area;
            gm= *(ckt->CKTstate0 + here->JFET2gm) ;
            gds= *(ckt->CKTstate0 + here->JFET2gds) ;
            ggs= *(ckt->CKTstate0 + here->JFET2ggs) ;
            xgs= *(ckt->CKTstate0 + here->JFET2qgs) * ckt->CKTomega ;
            ggd= *(ckt->CKTstate0 + here->JFET2ggd) ;
            xgd= *(ckt->CKTstate0 + here->JFET2qgd) * ckt->CKTomega ;

            vgs = *(ckt->CKTstate0 + here->JFET2vgs);
            vgd = *(ckt->CKTstate0 + here->JFET2vgd);
            cd = *(ckt->CKTstate0 + here->JFET2cd);
            PSacload(ckt,model, here, vgs, vgd, cd, ckt->CKTomega,
                                      &gm, &xgm, &gds, &xgds);
            xgds += *(ckt->CKTstate0 + here->JFET2qds) * ckt->CKTomega ;
            *(here->JFET2drainPrimeDrainPrimePtr +1)   += xgds;
            *(here->JFET2sourcePrimeSourcePrimePtr +1) += xgds+xgm;
            *(here->JFET2drainPrimeGatePtr +1)         +=      xgm;
            *(here->JFET2drainPrimeSourcePrimePtr +1)  -= xgds+xgm;
            *(here->JFET2sourcePrimeGatePtr +1)        -=      xgm;
            *(here->JFET2sourcePrimeDrainPrimePtr +1)  -= xgds;

            *(here->JFET2drainDrainPtr ) += gdpr;
            *(here->JFET2gateGatePtr ) += ggd+ggs;
            *(here->JFET2gateGatePtr +1) += xgd+xgs;
            *(here->JFET2sourceSourcePtr ) += gspr;
            *(here->JFET2drainPrimeDrainPrimePtr ) += gdpr+gds+ggd;
            *(here->JFET2drainPrimeDrainPrimePtr +1) += xgd;
            *(here->JFET2sourcePrimeSourcePrimePtr ) += gspr+gds+gm+ggs;
            *(here->JFET2sourcePrimeSourcePrimePtr +1) += xgs;
            *(here->JFET2drainDrainPrimePtr ) -= gdpr;
            *(here->JFET2gateDrainPrimePtr ) -= ggd;
            *(here->JFET2gateDrainPrimePtr +1) -= xgd;
            *(here->JFET2gateSourcePrimePtr ) -= ggs;
            *(here->JFET2gateSourcePrimePtr +1) -= xgs;
            *(here->JFET2sourceSourcePrimePtr ) -= gspr;
            *(here->JFET2drainPrimeDrainPtr ) -= gdpr;
            *(here->JFET2drainPrimeGatePtr ) += (-ggd+gm);
            *(here->JFET2drainPrimeGatePtr +1) -= xgd;
            *(here->JFET2drainPrimeSourcePrimePtr ) += (-gds-gm);
            *(here->JFET2sourcePrimeGatePtr ) += (-ggs-gm);
            *(here->JFET2sourcePrimeGatePtr +1) -= xgs;
            *(here->JFET2sourcePrimeSourcePtr ) -= gspr;
            *(here->JFET2sourcePrimeDrainPrimePtr ) -= gds;

        }
    }
    return(OK);
}
