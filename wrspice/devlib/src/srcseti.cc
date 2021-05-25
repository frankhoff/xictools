
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
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1992 Stephen R. Whiteley
****************************************************************************/

#include "srcdefs.h"
#include "input.h"


int
SRCdev::setInst(int param, IFdata *data, sGENinstance *geninst)
{
#ifdef WITH_CMP_GOTO
    // Order MUST match parameter definition enum!
    static void *array[] = {
        0, // notused
        &&L_SRC_I,
        &&L_SRC_V,
        &&L_SRC_PWRMODE,
        &&L_SRC_DEP,
        &&L_SRC_DC,
        &&L_SRC_AC,
        &&L_SRC_AC_MAG,
        &&L_SRC_AC_PHASE,
        0, // &&L_SRC_AC_REAL,
        0, // &&L_SRC_AC_IMAG,
        &&L_SRC_FUNC,
        &&L_SRC_D_F1,
        &&L_SRC_D_F2,
        &&L_SRC_GAIN,
        &&L_SRC_CONTROL,
        &&L_SRC_PRM1,
        &&L_SRC_PRM2,
        &&L_SRC_PRM3,
        &&L_SRC_PRM4,
        &&L_SRC_PRM5,
        &&L_SRC_PRM6,
        &&L_SRC_PRM7,
        &&L_SRC_PRM8,
        0, // &&L_SRC_VOLTAGE,
        0, // &&L_SRC_CURRENT,
        0, // &&L_SRC_POWER,
        0, // &&L_SRC_POS_NODE,
        0, // &&L_SRC_NEG_NODE,
        0, // &&L_SRC_CONT_P_NODE,
        0, // &&L_SRC_CONT_N_NODE,
        0};// &&L_SRC_BR_NODE};

    if ((unsigned int)param > SRC_BR_NODE)
        return (E_BADPARM);
#endif

    sSRCinstance *inst = static_cast<sSRCinstance*>(geninst);
    IFvalue *value = &data->v;

#ifdef WITH_CMP_GOTO
    void *jmp = array[param];
    if (!jmp)
        return (E_BADPARM);
    goto *jmp;
    L_SRC_I:
        if (inst->SRCtype) {
            if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
                delete value->tValue;
                value->tValue = 0;
            }
            return (E_BADPARM);
        }
        inst->SRCtype = SRC_I;
        if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
            inst->SRCtree = value->tValue;
            inst->SRCtree->differentiate();
            value->tValue = 0;
        }
        return (OK);
    L_SRC_V:
        if (inst->SRCtype) {
            if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
                delete value->tValue;
                value->tValue = 0;
            }
            return (E_BADPARM);
        }
        inst->SRCtype = SRC_V;
        if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
            inst->SRCtree = value->tValue;
            inst->SRCtree->differentiate();
            value->tValue = 0;
        }
        return (OK);
    L_SRC_PWRMODE:
        // This is undocumented, for HSPICE coexistence.  A
        // value 0 is a normal source, nonzero is something weird and
        // not supported.

        if (value->iValue != 0)
            return (E_BADPARM);
        return (OK);
    L_SRC_DEP:
        if (inst->SRCdep)
            return (E_BADPARM);
        switch (value->iValue) {
            case 'A':
                break;
            case 'I':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_I;
                break;
            case 'V':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_V;
                break;
            case 'E':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_V;
                inst->SRCdep = SRC_VC;
                break;
            case 'F':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_I;
                inst->SRCdep = SRC_CC;
                break;
            case 'G':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_I;
                inst->SRCdep = SRC_VC;
                break;
            case 'H':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_V;
                inst->SRCdep = SRC_CC;
                break;
            default:
                return (E_BADPARM);
        }
        return (OK);
    L_SRC_DC:
        inst->SRCdcValue = value->rValue;
        inst->SRCdcGiven = true;
        return (OK);
    L_SRC_AC:
        if ((data->type & IF_VARTYPES) == IF_FLAG) {
            inst->SRCacGiven = true;
            return (OK);
        }
        else if ((data->type & IF_VARTYPES) == IF_REALVEC) {
            switch (value->v.numValue) {
            case 2:
                inst->SRCacPhase = *(value->v.vec.rVec+1);
                inst->SRCacPGiven = true;
                // fallthrough
            case 1:
                inst->SRCacMag = *(value->v.vec.rVec);
                inst->SRCacMGiven = true;
                // fallthrough
            case 0:
                inst->SRCacGiven = true;
                data->cleanup();
                break;
            default:
                data->cleanup();
                return (E_BADPARM);
            }
        }
        else if ((data->type & IF_VARTYPES) == IF_STRING) {
            // given AC table
            inst->SRCacTabName = value->sValue;
            inst->SRCacGiven = true;
        }
        return (OK);
    L_SRC_AC_MAG:
        inst->SRCacMag = value->rValue;
        inst->SRCacMGiven = true;
        inst->SRCacGiven = true;
        return (OK);
    L_SRC_AC_PHASE:
        inst->SRCacPhase = value->rValue;
        inst->SRCacPGiven = true;
        inst->SRCacGiven = true;
        return (OK);
    L_SRC_FUNC:
        if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
            delete inst->SRCtree;
            inst->SRCtree = value->tValue;
            inst->SRCtree->differentiate();
            value->tValue = 0;
        }
        return (OK);
    L_SRC_D_F1:
        inst->SRCdF1given = true;
        inst->SRCdGiven = true;
        switch (value->v.numValue) {
        case 2:
            inst->SRCdF1phase = *(value->v.vec.rVec+1);
            inst->SRCdF1mag = *(value->v.vec.rVec);
            break;
        case 1:
            inst->SRCdF1mag = *(value->v.vec.rVec);
            inst->SRCdF1phase = 0.0;
            break;
        case 0:
            inst->SRCdF1mag = 1.0;
            inst->SRCdF1phase = 0.0;
            data->cleanup();
            break;
        default:
            data->cleanup();
            return (E_BADPARM);
        }
        return (OK);
    L_SRC_D_F2:
        inst->SRCdF2given = true;
        inst->SRCdGiven = true;
        switch (value->v.numValue) {
        case 2:
            inst->SRCdF2phase = *(value->v.vec.rVec+1);
            inst->SRCdF2mag = *(value->v.vec.rVec);
            break;
        case 1:
            inst->SRCdF2mag = *(value->v.vec.rVec);
            inst->SRCdF2phase = 0.0;
            break;
        case 0:
            inst->SRCdF2mag = 1.0;
            inst->SRCdF2phase = 0.0;
            data->cleanup();
            break;
        default:
            data->cleanup();
            return (E_BADPARM);
        }
        return (OK);
    L_SRC_GAIN:
        if (inst->SRCccCoeffGiven || inst->SRCvcCoeffGiven)
            return (E_BADPARM);
        if ((data->type & IF_VARTYPES) == IF_REAL)
            inst->SRCcoeff.real = value->rValue;
        else if ((data->type & IF_VARTYPES) == IF_COMPLEX)
            inst->SRCcoeff = value->cValue;
        else
            return (E_BADPARM);
        if (inst->SRCdep == SRC_CC)
            inst->SRCccCoeffGiven = true;
        else if (inst->SRCdep == SRC_VC)
            inst->SRCvcCoeffGiven = true;
        else
            return (E_BADPARM);
        return (OK);
    L_SRC_CONTROL:
        inst->SRCcontName = value->uValue;
        return (OK);
    L_SRC_PRM1:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 0);
        return (OK);
    L_SRC_PRM2:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 1);
        return (OK);
    L_SRC_PRM3:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 2);
        return (OK);
    L_SRC_PRM4:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 3);
        return (OK);
    L_SRC_PRM5:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 4);
        return (OK);
    L_SRC_PRM6:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 5);
        return (OK);
    L_SRC_PRM7:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 6);
        return (OK);
    L_SRC_PRM8:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 7);
        return (OK);
#else
    switch (param) {
    case SRC_I:
        if (inst->SRCtype) {
            if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
                delete value->tValue;
                value->tValue = 0;
            }
            return (E_BADPARM);
        }
        inst->SRCtype = SRC_I;
        if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
            inst->SRCtree = value->tValue;
            inst->SRCtree->differentiate();
            value->tValue = 0;
        }
        break;
    case SRC_V:
        if (inst->SRCtype) {
            if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
                delete value->tValue;
                value->tValue = 0;
            }
            return (E_BADPARM);
        }
        inst->SRCtype = SRC_V;
        if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
            inst->SRCtree = value->tValue;
            inst->SRCtree->differentiate();
            value->tValue = 0;
        }
        break;
    case SRC_PWRMODE:
        // This is undocumented, for HSPICE coexistence.  A
        // value 0 is a normal source, nonzero is something weird and
        // not supported.

        if (value->iValue != 0)
            return (E_BADPARM);
        break;
    case SRC_DEP:
        if (inst->SRCdep)
            return (E_BADPARM);
        switch (value->iValue) {
            case 'A':
                break;
            case 'I':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_I;
                break;
            case 'V':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_V;
                break;
            case 'E':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_V;
                inst->SRCdep = SRC_VC;
                break;
            case 'F':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_I;
                inst->SRCdep = SRC_CC;
                break;
            case 'G':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_I;
                inst->SRCdep = SRC_VC;
                break;
            case 'H':
                if (inst->SRCtype)
                    return (E_BADPARM);
                inst->SRCtype = SRC_V;
                inst->SRCdep = SRC_CC;
                break;
            default:
                return (E_BADPARM);
        }
        break;
    case SRC_DC:
        inst->SRCdcValue = value->rValue;
        inst->SRCdcGiven = true;
        break;
    case SRC_AC:
        if ((data->type & IF_VARTYPES) == IF_FLAG) {
            inst->SRCacGiven = true;
            break;
        }
        else if ((data->type & IF_VARTYPES) == IF_REALVEC) {
            switch (value->v.numValue) {
            case 2:
                inst->SRCacPhase = *(value->v.vec.rVec+1);
                inst->SRCacPGiven = true;
            case 1:
                inst->SRCacMag = *(value->v.vec.rVec);
                inst->SRCacMGiven = true;
            case 0:
                inst->SRCacGiven = true;
                data->cleanup();
                break;
            default:
                data->cleanup();
                return (E_BADPARM);
            }
        }
        else if ((data->type & IF_VARTYPES) == IF_STRING) {
            // given AC table
            inst->SRCacTabName = value->sValue;
            inst->SRCacGiven = true;
        }
        break;
    case SRC_AC_MAG:
        inst->SRCacMag = value->rValue;
        inst->SRCacMGiven = true;
        inst->SRCacGiven = true;
        break;
    case SRC_AC_PHASE:
        inst->SRCacPhase = value->rValue;
        inst->SRCacPGiven = true;
        inst->SRCacGiven = true;
        break;
    case SRC_FUNC:
        if (inst->SRCtree) {
            if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
                delete value->tValue;
                value->tValue = 0;
            }
            return E_BADPARM;
        }
        if ((data->type & IF_VARTYPES) == IF_PARSETREE) {
            inst->SRCtree = value->tValue;
            inst->SRCtree->differentiate();
            value->tValue = 0;
        }
        break;
    case SRC_D_F1:
        inst->SRCdF1given = true;
        inst->SRCdGiven = true;
        switch (value->v.numValue) {
        case 2:
            inst->SRCdF1phase = *(value->v.vec.rVec+1);
            inst->SRCdF1mag = *(value->v.vec.rVec);
            break;
        case 1:
            inst->SRCdF1mag = *(value->v.vec.rVec);
            inst->SRCdF1phase = 0.0;
            break;
        case 0:
            inst->SRCdF1mag = 1.0;
            inst->SRCdF1phase = 0.0;
            data->cleanup();
            break;
        default:
            data->cleanup();
            return(E_BADPARM);
        }
        break;
    case SRC_D_F2:
        inst->SRCdF2given = true;
        inst->SRCdGiven = true;
        switch (value->v.numValue) {
        case 2:
            inst->SRCdF2phase = *(value->v.vec.rVec+1);
            inst->SRCdF2mag = *(value->v.vec.rVec);
            break;
        case 1:
            inst->SRCdF2mag = *(value->v.vec.rVec);
            inst->SRCdF2phase = 0.0;
            break;
        case 0:
            inst->SRCdF2mag = 1.0;
            inst->SRCdF2phase = 0.0;
            data->cleanup();
            break;
        default:
            data->cleanup();
            return(E_BADPARM);
        }
        break;
    case SRC_GAIN:
        if (inst->SRCccCoeffGiven || inst->SRCvcCoeffGiven)
            return (E_BADPARM);
        if ((data->type & IF_VARTYPES) == IF_REAL)
            inst->SRCcoeff.real = value->rValue;
        else if ((data->type & IF_VARTYPES) == IF_COMPLEX)
            inst->SRCcoeff = value->cValue;
        else
            return (E_BADPARM);
        if (inst->SRCdep == SRC_CC)
            inst->SRCccCoeffGiven = true;
        else if (inst->SRCdep == SRC_VC)
            inst->SRCvcCoeffGiven = true;
        else
            return (E_BADPARM);
        break;
    case SRC_CONTROL:
        inst->SRCcontName = value->uValue;
        break;
    case SRC_CONTROL:
        inst->SRCcontName = value->uValue;
        break;
    case SRC_PRM1:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 0);
        break;
    case SRC_PRM2:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 1);
        break;
    case SRC_PRM3:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 2);
        break;
    case SRC_PRM4:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 3);
        break;
    case SRC_PRM5:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 4);
        break;
    case SRC_PRM6:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 5);
        break;
    case SRC_PRM7:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 6);
        break;
    case SRC_PRM8:
        IP.setTranFuncParam(inst->SRCtree, value->rValue, 7);
        break;
    default:
        return (E_BADPARM);
    }
#endif
    return (OK);
}
