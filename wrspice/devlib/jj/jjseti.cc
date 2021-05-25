
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

/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1992 Stephen R. Whiteley
****************************************************************************/

#include "jjdefs.h"


int
JJdev::setInst(int param, IFdata *data, sGENinstance *geninst)
{
#ifdef WITH_CMP_GOTO
    // Order MUST match parameter definition enum!
    static void *array[] = {
        0, // notused
        &&L_JJ_AREA, 
        &&L_JJ_ICS, 
        &&L_JJ_TEMP_K, 
#ifdef NEWLSER
        &&L_JJ_LSER, 
#endif
#ifdef NEWLSH
        &&L_JJ_LSH, 
#endif
        &&L_JJ_OFF,
        &&L_JJ_IC,
        &&L_JJ_ICP,
        &&L_JJ_ICV,
        &&L_JJ_CON,
        &&L_JJ_NOISE};

        // &&L_JJ_QUEST_V,
        // &&L_JJ_QUEST_PHS,
        // &&L_JJ_QUEST_PHSN,
        // &&L_JJ_QUEST_PHSF,
        // &&L_JJ_QUEST_PHST,
        // &&L_JJ_QUEST_TCF,
        // &&L_JJ_QUEST_VG,
        // &&L_JJ_QUEST_VL,
        // &&L_JJ_QUEST_VM,
        // &&L_JJ_QUEST_CRT,
        // &&L_JJ_QUEST_IC,
        // &&L_JJ_QUEST_IJ,
        // &&L_JJ_QUEST_IG,
        // &&L_JJ_QUEST_I,
        // &&L_JJ_QUEST_CAP,
        // &&L_JJ_QUEST_G0,
        // &&L_JJ_QUEST_GN,
        // &&L_JJ_QUEST_GS,
        // &&L_JJ_QUEST_GXSH,
        // &&L_JJ_QUEST_RXSH,
//#ifdef NEWLSH
        //&&L_JJ_QUEST_LSHVAL,
//#endif
        //&&L_JJ_QUEST_G1,
        //&&L_JJ_QUEST_G2,
        //&&L_JJ_QUEST_N1,
        //&&L_JJ_QUEST_N2,
        //&&L_JJ_QUEST_NP
//#ifdef NEWLSER
        //,
        //&&L_JJ_QUEST_NI,
        //&&L_JJ_QUEST_NB
//#endif
//#ifdef NEWLSH
        //,
        //&&L_JJ_QUEST_NSHI,
        //&&L_JJ_QUEST_NSHB
//#endif
        //};

    if ((unsigned int)param > JJ_NOISE)
        return (E_BADPARM);
#endif

    sJJinstance *inst = static_cast<sJJinstance*>(geninst);
    IFvalue *value = &data->v;

#ifdef WITH_CMP_GOTO
    void *jmp = array[param];
    if (!jmp)
        return (E_BADPARM);
    goto *jmp;
    L_JJ_AREA:
        inst->JJarea = value->rValue;
        inst->JJareaGiven = true;
        return (OK);
    L_JJ_ICS:
        inst->JJics = value->rValue;
        inst->JJicsGiven = true;
        return (OK);
    L_JJ_TEMP_K:
        inst->JJtemp_k = value->rValue;
        inst->JJtemp_kGiven = true;
        return (OK);
#ifdef NEWLSER
    L_JJ_LSER:
        inst->JJlser = value->rValue;
        inst->JJlserGiven = true;
        return (OK);
#endif
#ifdef NEWLSH
    L_JJ_LSH:
        inst->JJlsh = value->rValue;
        inst->JJlshGiven = true;
        return (OK);
#endif
    L_JJ_OFF:
        inst->JJoffGiven = true;
        return (OK);
    L_JJ_IC:
        switch(value->v.numValue) {
        case 2:
            inst->JJinitPhase = *(value->v.vec.rVec+1);
            inst->JJinitPhaseGiven = true;
            // fallthrough
        case 1:
            inst->JJinitVoltage = *(value->v.vec.rVec);
            inst->JJinitVoltGiven = true;
            data->cleanup();
            break;
        default:
            data->cleanup();
            return (E_BADPARM);
        }
        return (OK);
    L_JJ_ICP:
        inst->JJinitPhase = value->rValue;
        inst->JJinitPhaseGiven = true;
        return (OK);
    L_JJ_ICV:
        inst->JJinitVoltage = value->rValue;
        inst->JJinitVoltGiven = true;
        return (OK);
    L_JJ_CON:
        inst->JJcontrol = value->uValue;
        inst->JJcontrolGiven = true;
        return (OK);
    L_JJ_NOISE:
        inst->JJnoise = value->rValue;
        inst->JJnoiseGiven = true;
        return (OK);
#else
    switch (param) {
    case JJ_AREA:
        inst->JJarea = value->rValue;
        inst->JJareaGiven = true;
        break;
    case JJ_ICS:
        inst->JJics = value->rValue;
        inst->JJicsGiven = true;
        break;
    case JJ_TEMP_K:
        inst->JJtemp_k = value->rValue;
        inst->JJtemp_kGiven = true;
        break;
#ifdef NEWLSER
    case JJ_LSER:
        inst->JJlser = value->rValue;
        inst->JJlserGiven = true;
        break;
#endif
#ifdef NEWLSH
    case JJ_LSH:
        inst->JJlsh = value->rValue;
        inst->JJlshGiven = true;
        break;
#endif
    case JJ_OFF:
        inst->JJoffGiven = true;
        break;
    case JJ_IC:
        switch(value->v.numValue) {
        case 2:
            inst->JJinitPhase = *(value->v.vec.rVec+1);
            inst->JJinitPhaseGiven = true;
        case 1:
            inst->JJinitVoltage = *(value->v.vec.rVec);
            inst->JJinitVoltGiven = true;
            data->cleanup();
            break;
        default:
            data->cleanup();
            return (E_BADPARM);
        }
        break;
    case JJ_ICP:
        inst->JJinitPhase = value->rValue;
        inst->JJinitPhaseGiven = true;
        break;
    case JJ_ICV:
        inst->JJinitVoltage = value->rValue;
        inst->JJinitVoltGiven = true;
        break;
    case JJ_CON:
        inst->JJcontrol = value->uValue;
        inst->JJcontrolGiven = true;
        break;
    case JJ_NOISE:
        inst->JJnoise = value->uValue;
        inst->JJnoiseGiven = true;
        break;
    default:
        return (E_BADPARM);
    }
#endif
    return (OK);
}
