
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
Copyright 1990 Regents of the University of California.  All rights reserved.
Author:  1988 Jaijeet S Roychowdhury
Modified by Dietmar Warning 2003 and Paolo Nenzi 2003
**********/

#define DISTO
#include "diodefs.h"
#include "distdefs.h"

#define P33 .333333333

#define DIOnextModel      next()
#define DIOnextInstance   next()
#define DIOinstances      inst()
#define CKTgmin  CKTcurTask->TSKgmin

int
DIOdev::dSetup(sDIOmodel *model, sCKT *ckt)
{
    sDIOinstance *here;

        double arg;
        double csat;    /* area-scaled saturation current */
        double czero;
        double czof2;
        double evd;
        double evrev;
        double gd;
        double sarg;
        double vd;      /* current diode voltage */
        double vt;      /* K t / Q */
        double vte;
        double g2,g3;
        double cdiff2,cdiff3;
        double cjunc1,cjunc2,cjunc3;
//        double cd;
        double czeroSW;
        double cjunc1SW,cjunc2SW,cjunc3SW;

        /*  loop through all the diode models */
        for( ; model != NULL; model = model->DIOnextModel ) {

                /* loop through all the instances of the model */
                for (here = model->DIOinstances; here != NULL ;
                here=here->DIOnextInstance) {
//                    if (here->DIOowner != ARCHme) continue;

                      /*
                *  this routine loads diodes for dc and transient analyses.
                *  PN 2003: High level injection is not taken into
                *           account, since resulting equations are
                *           very complex to deal with.
                *
                *           This is an old analysis anyway....
                */

                        csat=(here->DIOtSatCur*here->DIOarea+here->DIOtSatSWCur*here->DIOpj)*here->DIOm;
                        vt = CONSTKoverQ * here->DIOtemp;
                        vte=model->DIOemissionCoeff * vt;
                        vd = *(ckt->CKTrhsOld + (here->DIOposPrimeNode)) -
                        *(ckt->CKTrhsOld + (here->DIOnegNode));

                        /*
                 *   compute derivatives
                 * Note: pn 2003 changed the code to the new spice 3 code 
                 */
                        if (vd >= -3*vte) { /* forward */
                                evd = exp(vd/vte);
//                                cd = csat*(evd-1);
                                gd = csat*evd/vte;
                                
                                g2 = 0.5 * gd / vte;
                                cdiff2 = g2 * here->DIOtTransitTime;
                                g3 = g2 / 3 / vte;
                                cdiff3 = g3 * here->DIOtTransitTime;
                                
                                gd = gd + ckt->CKTgmin;
                        } 
                        else if((!(here->DIOtBrkdwnV))|| 
                            (vd >= -here->DIOtBrkdwnV)) { /* reverse */
                            
                                arg=3*vte/(vd*CONSTe);
                                arg = arg * arg * arg;
                                
//                                cd = -csat * (1 + arg);
                                gd = csat * 3 * arg / vd;
                                g2 = -4 * gd / vd;
                                g3 = 5 * g2 / vd;
                                cdiff2 = cdiff3 = 0.0;
                                
                                gd = gd + ckt->CKTgmin;
                        } 
                        else { /* breakdown*/
                                /* why using csat instead of breakdowncurrent? */
                                evrev=exp(-(here->DIOtBrkdwnV+vd)/vt);
//                                cd = -csat*evrev;
                                gd = csat*evrev/vte;
                                /*
                      * cd = -csat*(evrev-1+here->DIOtBrkdwnV/vt);
                      */
                                /* should there be a minus here above? 
                      */
                                gd=csat*evrev/vt;
                                g2 = -gd/2/vt;
                                g3 = -g2/3/vt;
                                cdiff3 = cdiff2 = 0;
                        }
                        /*
                                         *   junction charge storage elements
                                         */
                        czero=here->DIOtJctCap*here->DIOarea*here->DIOm;
                        if (czero != 0.0) {
                          if (vd < here->DIOtDepCap){
                                arg=1-vd/model->DIOjunctionPot;
                                sarg=exp(-here->DIOtGradingCoeff*log(arg));
                                /* the expression for depletion charge 
                                                        model->DIOjunctionPot*czero*
                              (1-arg*sarg)/(1-here->DIOtGradingCoeff);
                                                    */
                                cjunc1 = czero*sarg;
                                cjunc2 = cjunc1/2/model->DIOjunctionPot*here->DIOtGradingCoeff/arg;
                                cjunc3 = cjunc2/3/model->DIOjunctionPot/arg*(here->DIOtGradingCoeff + 1);
                          } 
                          else {
                                czof2=czero/here->DIOtF2;
                                /* depletion charge equation 
                                                        czero*here->DIOtF1+czof2*
                          (model->DIOtF3*(vd-here->DIOtDepCap)+
                                                        (here->DIOtGradingCoeff/(model->DIOjunctionPot+
                                                        model->DIOjunctionPot))*(vd*vd-here->DIOtDepCap*
                          here->DIOtDepCap));
                                                        */
                                cjunc2 = czof2/2/model->DIOjunctionPot*here->DIOtGradingCoeff;
                                cjunc3 =0.0;
                          }
                        } 
                        else 
                        {
                          cjunc1 = cjunc2 = cjunc3 = 0.0;
                        }
                        czeroSW=+here->DIOtJctSWCap*here->DIOpj*here->DIOm;
                        if (czeroSW != 0.0) {
                          if (vd < here->DIOtDepCap){
                                arg=1-vd/model->DIOjunctionSWPot;
                                sarg=exp(-model->DIOgradingSWCoeff*log(arg));
                                cjunc1SW = czeroSW*sarg;
                                cjunc2SW = cjunc1SW/2/model->DIOjunctionSWPot*model->DIOgradingSWCoeff/arg;
                                cjunc3SW = cjunc2SW/3/model->DIOjunctionSWPot/arg*(model->DIOgradingSWCoeff + 1);
                          } 
                          else {
                                czof2=czeroSW/here->DIOtF2SW;
                                cjunc2SW = czof2/2/model->DIOjunctionSWPot*model->DIOgradingSWCoeff;
                                cjunc3SW = 0.0;
                          }
                        } 
                        else 
                        {
                          cjunc1SW = cjunc2SW = cjunc3SW = 0.0;
                        }

                        /*
                                         *   store small-signal parameters
                                         */

                        here->id_x2 = g2;
                        here->id_x3 = g3;
                        here->cdif_x2 = cdiff2;
                        here->cdif_x3 = cdiff3;
                        here->cjnc_x2 = cjunc2+cjunc2SW;
                        here->cjnc_x3 = cjunc3+cjunc3SW;
                }
        }
                return(OK);
}
