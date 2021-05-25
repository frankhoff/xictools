
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

/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

//
// All the functions used in the parse tree.
//

#include <math.h>
#include "config.h"
#include "input.h"
#include "inpptree.h"
#include "circuit.h"
#include "misc.h"
#include "miscutil/random.h"

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2          1.57079632679489661923
#endif

// These are defined in cmath1.cc
#ifdef WIN32
extern double cbrt(double);
extern double erf(double);
extern double erfc(double);
#endif

// Value returned when out of range.  This allows a little slop for
// processing, unlike MAXDOUBLE
//
#define HUGENUM  1.0e+300

#define MODULUS(NUM,LIMIT)  ((NUM) - ((int) ((NUM) / (LIMIT))) * (LIMIT))


// The binary operators

IFparseNode::PTop IFparseNode::PTops[] = { 
    PTop( PT_COMMA,     ",",        0 ),
    PTop( PT_PLUS,      "+",        &IFparseNode::PTplus ),
    PTop( PT_MINUS,     "-",        &IFparseNode::PTminus ),
    PTop( PT_TIMES,     "*",        &IFparseNode::PTtimes ),
    PTop( PT_DIVIDE,    "/",        &IFparseNode::PTdivide ),
    PTop( PT_POWER,     "^",        &IFparseNode::PTpower ),

#ifdef NEWOPS
    PTop( PT_EQ,        "==",       &IFparseNode::PTeq ),
    PTop( PT_GT,        ">",        &IFparseNode::PTgt ),
    PTop( PT_LT,        "<",        &IFparseNode::PTlt ),
    PTop( PT_GE,        ">=",       &IFparseNode::PTge ),
    PTop( PT_LE,        "<=",       &IFparseNode::PTle ),
    PTop( PT_NE,        "!=",       &IFparseNode::PTne ),
    PTop( PT_AND,       "&",        &IFparseNode::PTand ),
    PTop( PT_OR,        "|",        &IFparseNode::PTor ),
    PTop( PT_NOT,       "~",        &IFparseNode::PTnot ),
#endif

    PTop( PT_PLACEHOLDER, 0,        0 )
};

double
IFparseNode::PTplus(const double *args)
{   return (args[0] + args[1]); }


double
IFparseNode::PTminus(const double *args)
{   return (args[0] - args[1]); }


double
IFparseNode::PTtimes(const double *args)
{   return (args[0] * args[1]); }


double 
IFparseNode::PTdivide(const double *args)
{
    if (args[1] == 0.0) {
        if (args[0] == 0.0)
            return (1.0);
        if (args[0] < 0.0)
            return (-HUGENUM);
        return (HUGENUM);
    }
    return (args[0] / args[1]);
}


double
IFparseNode::PTpower(const double *args)
{
    if (args[0] == 0.0)
        return (0.0);
    return (pow(args[0], args[1]));
}


#ifdef NEWOPS

double
IFparseNode::PTeq(const double *args)
{ return (args[0] == args[1]); }


double
IFparseNode::PTgt(const double *args)
{ return (args[0] > args[1]); }


double
IFparseNode::PTlt(const double *args)
{ return (args[0] < args[1]); }


double
IFparseNode::PTge(const double *args)
{ return (args[0] >= args[1]); }


double
IFparseNode::PTle(const double *args)
{ return (args[0] <= args[1]); }


double
IFparseNode::PTne(const double *args)
{ return (args[0] != args[1]); }


double
IFparseNode::PTand(const double *args)
{ return (rint(args[0]) && rint(args[1])); }


double
IFparseNode::PTor(const double *args)
{ return (rint(args[0]) || rint(args[1])); }


double
IFparseNode::PTnot(const double *args)
{ return (!rint(args[0])); }

#endif


// The math functions

IFparseNode::PTfunc IFparseNode::PTfuncs[] = {
    PTfunc( "abs",    PTF_ABS,    &IFparseNode::PTabs ),
    PTfunc( "acos",   PTF_ACOS,   &IFparseNode::PTacos ),
    PTfunc( "asin",   PTF_ASIN,   &IFparseNode::PTasin ),
    PTfunc( "atan",   PTF_ATAN,   &IFparseNode::PTatan ),
#ifdef HAVE_ACOSH
    PTfunc( "acosh",  PTF_ACOSH,  &IFparseNode::PTacosh ),
    PTfunc( "asinh",  PTF_ASINH,  &IFparseNode::PTasinh ),
    PTfunc( "atanh",  PTF_ATANH,  &IFparseNode::PTatanh ),
#endif
    PTfunc( "cbrt",   PTF_CBRT,   &IFparseNode::PTcbrt ),
    PTfunc( "cos",    PTF_COS,    &IFparseNode::PTcos ),
    PTfunc( "cosh",   PTF_COSH,   &IFparseNode::PTcosh ),
    PTfunc( "deriv",  PTF_DERIV,  0 ) ,
    PTfunc( "erf",    PTF_ERF,    &IFparseNode::PTerf ),
    PTfunc( "erfc",   PTF_ERFC,   &IFparseNode::PTerfc ),
    PTfunc( "exp",    PTF_EXP,    &IFparseNode::PTexp ),
    PTfunc( "j0",     PTF_J0,     &IFparseNode::PTj0 ),
    PTfunc( "j1",     PTF_J1,     &IFparseNode::PTj1 ),
    PTfunc( "jn",     PTF_JN,     &IFparseNode::PTjn ),
    PTfunc( "ln",     PTF_LN,     &IFparseNode::PTln ),
    // Note: log changed to ln in 3.2.15 for Hspice/Pspice compatibility,
    // was log10 earlier and in Spice3.
    PTfunc( "log",    PTF_LN,     &IFparseNode::PTln ),
    PTfunc( "log10",  PTF_LOG10,  &IFparseNode::PTlog10 ),
    PTfunc( "pow",    PTF_POW,    &IFparseNode::PTpow ),
    PTfunc( "pwr",    PTF_POW,    &IFparseNode::PTpow ),
    PTfunc( "sgn",    PTF_SGN,    &IFparseNode::PTsgn ),
    PTfunc( "sin",    PTF_SIN,    &IFparseNode::PTsin ),
    PTfunc( "sinh",   PTF_SINH,   &IFparseNode::PTsinh ),
    PTfunc( "sqrt",   PTF_SQRT,   &IFparseNode::PTsqrt ),
    PTfunc( "tan",    PTF_TAN,    &IFparseNode::PTtan ),
    PTfunc( "tanh",   PTF_TANH,   &IFparseNode::PTtanh ),
    PTfunc( "y0",     PTF_Y0,     &IFparseNode::PTy0 ),
    PTfunc( "y1",     PTF_Y1,     &IFparseNode::PTy1 ),
    PTfunc( "yn",     PTF_YN,     &IFparseNode::PTyn ),
    PTfunc( "-",      PTF_UMINUS, &IFparseNode::PTuminus ),
    PTfunc( 0,        PTF_NIL,    0 )
};

// These functions from the table above require two arguments, all
// others take one.
//
bool
IFparseNode::p_two_args(int indx)
{
    return (indx == PTF_POW || indx == PTF_JN || indx == PTF_YN);
}


double
IFparseNode::PTabs(const double *arg)
{   return (fabs(*arg)); }


double
IFparseNode::PTsgn(const double *arg)
{   return (*arg > 0.0 ? 1.0 : *arg < 0.0 ? -1.0 : 0.0); }


double
IFparseNode::PTacos(const double *arg)
{
    if (*arg > 1.0)
        return (0.0);
    if (*arg < -1.0)
        return (M_PI);
    return (acos(*arg));
}


double
IFparseNode::PTasin(const double *arg)
{  
    if (*arg > 1.0)
        return (M_PI_2);
    if (*arg < -1.0)
        return (-M_PI_2);
    return (asin(*arg));
}


double
IFparseNode::PTatan(const double *arg)
{   return (atan(*arg)); }


#ifdef HAVE_ACOSH

double
IFparseNode::PTacosh(const double *arg)
{
    if (*arg < 1.0)
        return (0);
    return (acosh(*arg));
}


double
IFparseNode::PTasinh(const double *arg)
{   return (asinh(*arg)); }


double
IFparseNode::PTatanh(const double *arg)
{
    if (*arg >= 1.0)
        return (HUGENUM);
    if (*arg <= -1.0)
        return (-HUGENUM);
    return (atanh(*arg));
}

#endif /* HAVE_ACOSH */


double
IFparseNode::PTcbrt(const double *arg)
{   return (cbrt(*arg)); }


double
IFparseNode::PTcos(const double *arg)
{   return (cos(MODULUS(*arg, 2 * M_PI))); }


double
IFparseNode::PTcosh(const double *arg)
{   return (cosh(*arg)); }


double
IFparseNode::PTerf(const double *arg)
{   return (erf(*arg)); }


double
IFparseNode::PTerfc(const double *arg)
{   return (erfc(*arg)); }


double
IFparseNode::PTexp(const double *arg)
{   return (exp(*arg)); }


double
IFparseNode::PTj0(const double *arg)
{   return (j0(*arg)); }


double
IFparseNode::PTj1(const double *arg)
{   return (j1(*arg)); }


double
IFparseNode::PTjn(const double *args)
{   return (jn((int)args[0], args[1])); }


double
IFparseNode::PTln(const double *arg)
{
    if (*arg <= 0.0)
        return (-log(HUGENUM));
    return (log(*arg));
}


double
IFparseNode::PTlog10(const double *arg)
{
    if (*arg <= 0.0)
        return (-log10(HUGENUM));
    return (log10(*arg));
}


// pow(x, y), pwr(x, y), x^y are equivalent.
double
IFparseNode::PTpow(const double *args)
{
    if (args[0] == 0.0)
        return (0.0);
    return (pow(args[0], args[1]));
}


double
IFparseNode::PTsin(const double *arg)
{   return (sin(MODULUS(*arg, 2 * M_PI))); }


double
IFparseNode::PTsinh(const double *arg)
{   return (sinh(*arg)); }


double
IFparseNode::PTsqrt(const double *arg)
{
    if (*arg < 0.0)
        return (0.0);
    return (sqrt(*arg));
}


double
IFparseNode::PTtan(const double *arg)
{   return (tan(MODULUS(*arg, M_PI))); }


double
IFparseNode::PTtanh(const double *arg)
{   return (tanh(*arg)); }


double
IFparseNode::PTuminus(const double *arg)
{   return (- *arg); }


double
IFparseNode::PTy0(const double *arg)
{
    if (*arg <= 0)
        return (-HUGENUM);
    return (y0(*arg));
}


double
IFparseNode::PTy1(const double *arg)
{
    if (*arg <= 0)
        return (-HUGENUM);
    return (y1(*arg));
}


double
IFparseNode::PTyn(const double *args)
{
    if (args[1] <= 0)
        return (-HUGENUM);
    return (yn((int)args[0], args[1]));
}


// The "tran" functions.  The "t" functions avoid name clash with
// cmath library functions.

IFparseNode::PTtfunc IFparseNode::PTtFuncs[] = {
    PTtfunc( "pulse",  PTF_tPULSE,  &IFparseNode::tran_func ),
    PTtfunc( "gpulse", PTF_tGPULSE, &IFparseNode::tran_func ),
    PTtfunc( "pwl",    PTF_tPWL,    &IFparseNode::tran_func ),
    PTtfunc( "sin",    PTF_tSIN,    &IFparseNode::tran_func ),
    PTtfunc( "tsin",   PTF_tSIN,    &IFparseNode::tran_func ),
    PTtfunc( "spulse", PTF_tSPULSE, &IFparseNode::tran_func ),
    PTtfunc( "exp",    PTF_tEXP,    &IFparseNode::tran_func ),
    PTtfunc( "texp",   PTF_tEXP,    &IFparseNode::tran_func ),
    PTtfunc( "sffm",   PTF_tSFFM,   &IFparseNode::tran_func ),
    PTtfunc( "am",     PTF_tAM,     &IFparseNode::tran_func ),
    PTtfunc( "gauss",  PTF_tGAUSS,  &IFparseNode::tran_func ),
    PTtfunc( "tgauss", PTF_tGAUSS,  &IFparseNode::tran_func ),
    PTtfunc( "interp", PTF_tINTERP, &IFparseNode::tran_func ),
    PTtfunc( "table",  PTF_tTABLE,  &IFparseNode::PTtTable ),
    PTtfunc( 0,        PTF_tNIL,    0 )
};


double
IFparseNode::tran_func(const double *arg)
{
    return (v.td->eval_func(*arg));
}


double
IFparseNode::tran_deriv(const double *arg)
{
    return (v.td->eval_deriv(*arg));
}


double
IFparseNode::PTtTable(const double *arg)
{
    if (v.table) {
        double t = *arg;
        return ((v.table->tablEval(t)).real);
    } 
    return (0.0);
}
    
 
double
IFparseNode::PTtTableD(const double *arg)
{
    if (v.table) {
        double t = *arg;
        return (v.table->tablEvalDeriv(t));
    }
    return (0.0);
} 

