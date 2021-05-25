
/*========================================================================*
 *                                                                        *
 *  Distributed by Whiteley Research Inc., Sunnyvale, California, USA     *
 *                       http://wrcad.com                                 *
 *  Copyright (C) 2019 Whiteley Research Inc., all rights reserved.       *
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

#include "tjmdefs.h"


#ifndef M_PI_4
#define M_PI_4      0.785398163397448309615660845819875721  // pi/4
#endif

// MIT-LL SFQ5EE process, parameters from:
// Tolpygo et al., IEEE J. Appl. Superconductivity, 26, 1100110, (2016)
// Tolpygo et al., IEEE J. Appl. Superconductivity, 27, 1100815, (2017)
#define C_PER_A_10000  70.0     // ff/um2
#define I_PER_A_10000 100.0     // uA/um2
#define Vm_LL          16.5     // mV, critical current * subgap resistance
#define IcR_LL          1.65    // mV, critical current * normal resistance

// Various Hypres foundry processes.
#define C_PER_A_4500   59.0     // ff/um2
#define I_PER_A_4500   45.0     // uA/um2
#define C_PER_A_1000   50.0     // ff/um2
#define I_PER_A_1000   10.0     // uA/um2
#define C_PER_A_30     37.0     // ff/um2
#define I_PER_A_30      0.3     // uA/um2
#define Vm_HYP         30.0     // mV, critical current * subgap resistance
#define IcR_HYP         1.7     // mV, critical current * normal resistance

// Hard-wire defaults for MIT-LL process for SuperTools.
#define C_PER_A C_PER_A_10000
#define I_PER_A I_PER_A_10000
#define Vm      (Vm_LL*1e-3)
#define IcR     (IcR_LL*1e-3)

#define VmMin   0.008       // Min Vm V
#define VmMax   0.1         // Max Vm V
#define IcRmin  0.5e-3      // Min IcR V
#define Ic      1e-3        // Assumed Ic of reference, A
#define IcMin   1e-9        // Min reference Ic, A
#define IcMax   1e-1        // Max referenct Ic, A
#define IcsMin  Icrit/50    // Min instance Ic, A
#define IcsMax  Icrit*50    // Max instance Ic, A
#define Vg      2.6e-3      // Assumed Vgap of reference, V
#define VgMin   0.1e-3      // Min Vgap, V
#define VgMax   10.0e-3     // Max Vgap, V

#define RTMAX   1           // Max rtype, integer
#define ITMAX   1           // Max ictype, integer
#define CCsens  1e-2        // Assumed magnetic sens. of reference
#define CCsensMin 1e-4      // Min magnetic sens.
#define CCsensMax 1.0       // Max magnetic sens.
#define CPIC    (1e-9*C_PER_A/I_PER_A)  // Reference capacitance, F/A
#define CPICmin 0.0         // Minimum CPIC F/A
#define CPICmax 1e-6        // Maximum CPIC F/A
#define CAP     CPIC*Icrit  // Reference capacitance, F
#define ICfct   M_PI_4      // Igap/Ic factor for reference
#define ICfctMin 0.5        // Min factor
#define ICfctMax M_PI_4     // Max factor
#define NOI     1.0         // Noise scale for reference
#define NOImin  0.0         // Min noise scale
#define NOImax  10.0        // Max noise scale

#ifdef NEWLSH
#define LSH_MAX 1e-10
#define LSH0_MAX 2e-12
#define LSH1_MAX 1e-11
#endif

namespace {
    int get_node_ptr(sCKT *ckt, sTJMinstance *inst)
    {
        TSTALLOC(TJMposPosPtr, TJMposNode, TJMposNode)
        TSTALLOC(TJMnegNegPtr, TJMnegNode, TJMnegNode)
        TSTALLOC(TJMnegPosPtr, TJMnegNode, TJMposNode)
        TSTALLOC(TJMposNegPtr, TJMposNode, TJMnegNode)
#ifdef NEWLSER
        // The series inductance is from TJMrealPosNode (device contact)
        // to TJMposNode (an internal node when lser is nonzero).
        TSTALLOC(TJMlserPosIbrPtr, TJMrealPosNode, TJMlserBr)
        TSTALLOC(TJMlserNegIbrPtr, TJMposNode, TJMlserBr)
        TSTALLOC(TJMlserIbrPosPtr, TJMlserBr, TJMrealPosNode)
        TSTALLOC(TJMlserIbrNegPtr, TJMlserBr, TJMposNode)
        TSTALLOC(TJMlserIbrIbrPtr, TJMlserBr, TJMlserBr)
#endif

#ifdef NEWLSH
        // The shunt series inductance is from device top contact to
        // TJMlshIntNode.
#ifdef NEWLSER
        TSTALLOC(TJMlshPosIbrPtr, TJMrealPosNode, TJMlshBr)
        TSTALLOC(TJMlshIbrPosPtr, TJMlshBr, TJMrealPosNode)
#else
        TSTALLOC(TJMlshPosIbrPtr, TJMposNode, TJMlshBr)
        TSTALLOC(TJMlshIbrPosPtr, TJMlshBr, TJMposNode)
#endif
        TSTALLOC(TJMlshNegIbrPtr, TJMlshIntNode, TJMlshBr)
        TSTALLOC(TJMlshIbrNegPtr, TJMlshBr, TJMlshIntNode)
        TSTALLOC(TJMlshIbrIbrPtr, TJMlshBr, TJMlshBr)
#endif

#ifdef NEWLSH
        // Rshunt:  internal node to device contact 2.
        TSTALLOC(TJMrshPosPosPtr, TJMlshIntNode, TJMlshIntNode);
        TSTALLOC(TJMrshPosNegPtr, TJMlshIntNode, TJMnegNode);
        TSTALLOC(TJMrshNegPosPtr, TJMnegNode, TJMlshIntNode);
        TSTALLOC(TJMrshNegNegPtr, TJMnegNode, TJMnegNode);
#else
#ifdef NEWLSER
        // Rshunt:  device contact 1 to device contact 2.
        TSTALLOC(TJMrshPosPosPtr, TJMrealPosNode, TJMrealPosNode);
        TSTALLOC(TJMrshPosNegPtr, TJMrealPosNode, TJMnegNode);
        TSTALLOC(TJMrshNegPosPtr, TJMnegNode, TJMrealPosNode);
        TSTALLOC(TJMrshNegNegPtr, TJMnegNode, TJMnegNode);
#else
        // Rshunt:  device contact 1 to device contact 2.
        TSTALLOC(TJMrshPosPosPtr, TJMposNode, TJMposNode);
        TSTALLOC(TJMrshPosNegPtr, TJMposNode, TJMnegNode);
        TSTALLOC(TJMrshNegPosPtr, TJMnegNode, TJMposNode);
        TSTALLOC(TJMrshNegNegPtr, TJMnegNode, TJMnegNode);
#endif
#endif

        if (inst->TJMphsNode > 0) {
            TSTALLOC(TJMphsPhsPtr, TJMphsNode, TJMphsNode)
        }
        return (OK);
    }
}


int
TJMdev::setup(sGENmodel *genmod, sCKT *ckt, int *states)
{
    sTJMmodel *model = static_cast<sTJMmodel*>(genmod);
    for ( ; model; model = model->next()) {


        if (!model->TJMrtypeGiven)
            model->TJMrtype = 1;
        else {
            if (model->TJMrtype < 0 || model->TJMrtype > RTMAX) {
                DVO.textOut(OUT_WARNING,
                    "%s: RTYPE=%d out of range [0-%d], reset to 1.\n",
                    model->GENmodName, model->TJMrtype, RTMAX);
                model->TJMrtype = 1;
            }
        }
        if (!model->TJMictypeGiven)
            model->TJMictype = 1;
        else {
            if (model->TJMictype < 0 || model->TJMictype > ITMAX) {
                DVO.textOut(OUT_WARNING,
                    "%s: CCT=%d out of range [0-%d], reset to 1.\n",
                    model->GENmodName, model->TJMictype, ITMAX);
                model->TJMictype = 1;
            }
        }
        if (!model->TJMvgGiven)
            model->TJMvg = Vg;
        else {
            if (model->TJMvg < VgMin || model->TJMvg > VgMax) {
                DVO.textOut(OUT_WARNING,
                    "%s: VG=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMvg, VgMin, VgMax, Vg);
                model->TJMvg = Vg;
            }
        }

        if (!model->TJMcritiGiven)
            model->TJMcriti = Ic;
        else {
            if (model->TJMcriti < IcMin || model->TJMcriti > IcMax) {
                DVO.textOut(OUT_WARNING,
                    "%s: ICRIT=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMcriti, IcMin, IcMax, Ic);
                model->TJMcriti = Ic;
            }
        }
        if (!model->TJMcpicGiven)
            model->TJMcpic = CPIC;
        else {
            if (model->TJMcpic < CPICmin || model->TJMcpic > CPICmax) {
                DVO.textOut(OUT_WARNING,
                    "%s: CPIC=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMcpic, CPICmin, CPICmax, CPIC);
                model->TJMcpic = CPIC;
            }
        }
        if (!model->TJMcapGiven)
            model->TJMcap = model->TJMcpic * model->TJMcriti;
        else {
            double Icrit = model->TJMcriti;
            double cmin = CPICmin * Icrit;
            double cmax = CPICmax * Icrit;
            if (model->TJMcap < cmin || model->TJMcap > cmax) {
                double cap = model->TJMcpic * model->TJMcriti;
                DVO.textOut(OUT_WARNING,
                    "%s: CAP=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMcap, cmin, cmax, cap);
                model->TJMcap = cap;
            }
        }
        if (!model->TJMicfGiven)
            model->TJMicFactor = ICfct;
        else {
            if (model->TJMicFactor < ICfctMin || model->TJMicFactor > ICfctMax) {
                DVO.textOut(OUT_WARNING,
                    "%s: ICFACT=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMicFactor, ICfctMin, ICfctMax,
                    ICfct);
                model->TJMicFactor = ICfct;
            }
        }
        if (!model->TJMvmGiven) {
            model->TJMvm = Vm;
        }
        else {
            if (model->TJMforceGiven) {
                if (model->TJMvm <= 0.0) {
                    DVO.textOut(OUT_WARNING,
                        "%s: VM=%g zero or negative, reset to %g.\n",
                        model->GENmodName, model->TJMvm, Vm);
                    model->TJMvm = Vm;
                }
            }
            else {
                if (model->TJMvm < VmMin || model->TJMvm > VmMax) {
                    DVO.textOut(OUT_WARNING,
                        "%s: VM=%g out of range [%g-%g], reset to %g.\n",
                        model->GENmodName, model->TJMvm, VmMin, VmMax, Vm);
                    model->TJMvm = Vm;
                }
            }
        }

        if (!model->TJMr0Given) {
            double i = model->TJMcriti > 0.0 ? model->TJMcriti : 1e-3;
            model->TJMr0 = model->TJMvm/i;
        }
        else {
            if (model->TJMforceGiven) {
                if (model->TJMr0 <= 0.0) {
                    double i = model->TJMcriti > 0.0 ? model->TJMcriti : 1e-3;
                    double R0 = model->TJMvm/i;
                    DVO.textOut(OUT_WARNING,
                        "%s: RSUB=%g zero or negative, reset to %g.\n",
                        model->GENmodName, model->TJMr0, R0);
                    model->TJMr0 = R0;
                }
            }
            else {
                double i = model->TJMcriti > 0.0 ? model->TJMcriti : 1e-3;
                double R0min = VmMin/i;
                double R0max = VmMax/i;
                if (model->TJMr0 < R0min || model->TJMr0 > R0max) {
                    double R0 = model->TJMvm/i;
                    DVO.textOut(OUT_WARNING,
                        "%s: RSUB=%g out of range [%g-%g], reset to %g.\n",
                        model->GENmodName, model->TJMr0, R0min, R0max, R0);
                    model->TJMr0 = R0;
                }
            }
        }


        if (model->TJMvShuntGiven) {
            if (model->TJMvShunt < 0.0 || model->TJMvShunt > model->TJMvg) {
                DVO.textOut(OUT_WARNING,
                    "%s: VSHUNT=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMvShunt, 0.0, model->TJMvg, 0.0);
                model->TJMvShunt = 0.0;
            }
        }
#ifdef NEWLSH
        if (model->TJMlsh0Given) {
            if (model->TJMlsh0 < 0.0 || model->TJMlsh0 > LSH0_MAX) {
                DVO.textOut(OUT_WARNING,
                    "%s: LSH0=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMlsh0, 0.0, LSH0_MAX, 0.0);
                model->TJMlsh0 = 0.0;
            }
        }
        if (model->TJMlsh1Given) {
            if (model->TJMlsh1 < 0.0 || model->TJMlsh1 > LSH1_MAX) {
                DVO.textOut(OUT_WARNING,
                    "%s: LSH1=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMlsh1, 0.0, LSH1_MAX, 0.0);
                model->TJMlsh1 = 0.0;
            }
        }
#endif

        if (!model->TJMnoiseGiven)
            model->TJMnoise = NOI;
        else {
            if (model->TJMnoise < NOImin || model->TJMnoise > NOImax) {
                DVO.textOut(OUT_WARNING,
                    "%s: NOISE=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMnoise, NOImin, NOImax, NOI);
                model->TJMnoise = NOI;
            }
        }

        if (!model->TJMtsfactGiven)
            model->TJMtsfact = ckt->CKTcurTask->TSKdphiMax/(2*M_PI);
        else {
            if (model->TJMtsfact < 0.001 || model->TJMtsfact > 1) {
                DVO.textOut(OUT_WARNING,
                    "%s: TSFACTOR=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMtsfact, 0.001, 1.0,
                    ckt->CKTcurTask->TSKdphiMax/(2*M_PI));
                model->TJMtsfact = ckt->CKTcurTask->TSKdphiMax/(2*M_PI);
            }
        }

        if (!model->TJMtsacclGiven)
            model->TJMtsaccl = 1.0;
        else {
            if (model->TJMtsaccl < 1.0 || model->TJMtsaccl > 20) {
                DVO.textOut(OUT_WARNING,
                    "%s: TSACCEL=%g out of range [%g-%g], reset to %g.\n",
                    model->GENmodName, model->TJMtsaccl, 1.0, 20.0, 1.0);
                model->TJMtsaccl = 1.0;
            }
        }

        double halfvg = model->TJMvg/2;
        if (model->TJMcap > 0.0) {
            model->TJMvdpbak = sqrt(PHI0_2PI/model->TJMcpic);
            if (model->TJMvdpbak > halfvg)
                model->TJMvdpbak = halfvg;
            model->TJMomegaJ = sqrt(1.0/(model->TJMcpic*PHI0_2PI));
        }
        else
            model->TJMvdpbak = halfvg;

        int rval = model->tjm_init();
        if (rval != OK)
            return (rval);

        sTJMinstance *inst;
        for (inst = model->inst(); inst; inst = inst->next()) {

            if (inst->TJMicsGiven) {
                if (inst->TJMareaGiven) {
                    DVO.textOut(OUT_WARNING,
                        "%s: ICS and AREA both given, AREA ignored.\n",
                        inst->GENname);
                }
                double Icrit = model->TJMcriti;
                if (inst->TJMics < IcsMin || inst->TJMics > IcsMax) {
                    DVO.textOut(OUT_WARNING,
                        "%s: ICS=%g out of range [%g-%g], reset to %g.\n",
                        inst->GENname, inst->TJMics, IcsMin, IcsMax,
                        model->TJMcriti);
                    inst->TJMics = model->TJMcriti;
                }
                inst->TJMarea = inst->TJMics/model->TJMcriti;
            }
            else if (!inst->TJMareaGiven)
                inst->TJMarea = 1;

            // ics is the input parameter, criti is the actual critical
            // current to use.
            inst->TJMcriti = model->TJMcriti * inst->TJMarea;

#ifdef NEWLSER
            if (inst->TJMlser > 0.0 && inst->TJMlser < 1e-14) {
                DVO.textOut(OUT_WARNING,
                    "%s: LSER less than 0.01pH, reset to 0.\n", inst->GENname);
                inst->TJMlser = 0.0;
            }
            if (inst->TJMlser > 0.0) {
                sCKTnode *node;
                int err = ckt->mkVolt(&node, inst->GENname, "jpos");
                if (err)
                    return (err);
                inst->TJMposNode = node->number();
                err = ckt->mkCur(&node, inst->GENname, "branch");
                if (err)
                    return (err);
                inst->TJMlserBr = node->number();
            }
            else {
                inst->TJMposNode = inst->TJMrealPosNode;
            }
#endif
            inst->TJMgqp = inst->TJMg0;
            if (model->TJMvShuntGiven && model->TJMvShunt > 0.0) {
                double gshunt = inst->TJMcriti/model->TJMvShunt - inst->TJMgqp;
                if (gshunt > 0.0)
                    inst->TJMgshunt = gshunt;
            }

#ifdef NEWLSH
            if (inst->TJMlshGiven) {
                if (inst->TJMlsh < 0.0 || inst->TJMlsh > LSH_MAX) {
                    DVO.textOut(OUT_WARNING,
                        "%s: LSH=%g out of range [%g-%g], reset to %g.\n",
                        inst->GENname, inst->TJMlsh, 0.0, LSH_MAX, 0.0);
                    inst->TJMlsh = 0.0;
                }
            }
            else if (inst->TJMgshunt > 0.0 &&
                    (model->TJMlsh0Given || model->TJMlsh1Given))
                inst->TJMlsh = model->TJMlsh0 + model->TJMlsh1/inst->TJMgshunt;
            if (inst->TJMgshunt > 0.0) {
                if (inst->TJMlsh > 0) {
                    sCKTnode *node;
                    int err = ckt->mkVolt(&node, inst->GENname, "sh");
                    if (err)
                        return (err);
                    inst->TJMlshIntNode = node->number();
                    err = ckt->mkCur(&node, inst->GENname, "shbr");
                    if (err)
                        return (err);
                    inst->TJMlshBr = node->number();
                }
                else {
#ifdef NEWLSER
                    inst->TJMlshIntNode = inst->TJMrealPosNode;
#else
                    inst->TJMlshIntNode = inst->TJMposNode;
#endif
                }
            }
#endif

#ifdef NEWJJDC
            if (model->TJMictype > 0 && !ckt->CKTcurTask->TSKnoPhaseModeDC) {
                // Set the phase flag of connected nodes for
                // phase-mode DC analysis, if critical current is
                // turned on and phase mode DC is not disabled.

#ifdef NEWLSER
                if (inst->TJMrealPosNode > 0) {
                    sCKTnode *node = ckt->CKTnodeTab.find(inst->TJMrealPosNode);
                    if (node)
                        node->set_phase(true);
                }
#endif
                if (inst->TJMposNode > 0) {
                    sCKTnode *node = ckt->CKTnodeTab.find(inst->TJMposNode);
                    if (node)
                        node->set_phase(true);
                }
                if (inst->TJMnegNode > 0) {
                    sCKTnode *node = ckt->CKTnodeTab.find(inst->TJMnegNode);
                    if (node)
                        node->set_phase(true);
                }
            }
#endif

            double sqrta = sqrt(inst->TJMarea);
            inst->TJMcap = model->TJMcap*(inst->TJMarea*(1.0 - model->TJMcmu) +
                sqrta*model->TJMcmu);

            double gfac = inst->TJMarea*(1.0 - model->TJMgmu) +
                sqrta*model->TJMgmu;
            inst->TJMg0 = gfac / model->TJMr0;

            if (!inst->TJMnoiseGiven)
                inst->TJMnoise = model->TJMnoise;
            else {
                if (inst->TJMnoise < NOImin || inst->TJMnoise > NOImax) {
                    DVO.textOut(OUT_WARNING,
                        "%s: NOISE out of range [%g-%g], reset to %g.\n",
                        inst->GENname, NOImin, NOImax, NOI);
                    inst->TJMnoise = NOI;
                }
            }

            inst->GENstate = *states;
            *states += TJMnumStates;

            int error = get_node_ptr(ckt, inst);
            if (error != OK)
                return (error);

#ifdef USE_PRELOAD
            if (ckt->CKTpreload) {
                if (inst->TJMphsNode > 0) {
                    ckt->preldset(inst->TJMphsPhsPtr, 1.0);
                }
#ifdef NEWLSER
                if (inst->TJMrealPosNode) {
                    ckt->preldset(inst->TJMlserPosIbrPtr, 1.0);
                    ckt->preldset(inst->TJMlserIbrPosPtr, 1.0);
                }
                if (inst->TJMposNode) {
                    ckt->preldset(inst->TJMlserNegIbrPtr, -1.0);
                    ckt->preldset(inst->TJMlserIbrNegPtr, -1.0);
                }
#endif
#ifdef NEWLSH
                if (inst->TJMlsh > 0.0) {
                    ckt->ldset(inst->TJMlshPosIbrPtr, 1.0);
                    ckt->ldset(inst->TJMlshIbrPosPtr, 1.0);
                    ckt->ldset(inst->TJMlshNegIbrPtr, -1.0);
                    ckt->ldset(inst->TJMlshIbrNegPtr, -1.0);
                }
#endif
            }
#endif
            // Make sure that the arrays are built now.
            inst->tjm_init(0.0);
        }
    }

    return (OK);
}


// Reset the matrix element pointers.
//
int
TJMdev::resetup(sGENmodel *inModel, sCKT *ckt)
{
    for (sTJMmodel *model = (sTJMmodel*)inModel; model;
            model = model->next()) {
        for (sTJMinstance *here = model->inst(); here;
                here = here->next()) {
            int error = get_node_ptr(ckt, here);
            if (error != OK)
                return (error);
        }
    }
    return (OK);
}
// End of TJMdev functions.


int
sTJMmodel::tjm_init()
{
    if (!tjm_coeffsGiven)
        tjm_coeffs = strdup("tjm1");

    TJMcoeffSet *cs = TJMcoeffSet::getTJMcoeffSet(tjm_coeffs);
    if (!cs) {
        // ERROR
        return (E_PANIC);
    }
    tjm_narray = cs->size();
    tjm_p = new IFcomplex[3*tjm_narray];
    tjm_A = tjm_p + tjm_narray;
    tjm_B = tjm_A + tjm_narray;
    for (int i = 0; i < tjm_narray; i++) {
        tjm_p[i] = cs->p()[i];
        tjm_A[i] = cs->A()[i];
        tjm_B[i] = cs->B()[i];
    }

    double omega_g = 0.5*TJMvg/PHI0_2PI;  // e*Vg = hbar*omega_g
    tjm_kgap = omega_g/TJMomegaJ;

    double rejpt = 0.0;
    for (int i = 0; i < tjm_narray; i++)
        rejpt -= (tjm_A[i]/tjm_p[i]).real;
    rejpt *= TJMicFactor;
    tjm_kgap_rejpt = tjm_kgap/rejpt;
    tjm_alphaN = 1.0/(2*rejpt*tjm_kgap);

    // Renormalize.  Here we would rotate to C and D vectors, however
    // we want to preserve the pair and qp amplitudes for separate
    // access.
    for (int i = 0; i < tjm_narray; i++) {
        tjm_A[i] = (TJMicFactor*tjm_A[i]) / (-tjm_kgap*tjm_p[i]);
        tjm_B[i] = (tjm_B[i]) / (-tjm_kgap*tjm_p[i]);
    }
    return (OK);
}
// End of sTJMmodel functions.

