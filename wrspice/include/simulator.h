
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
Authors: 1985 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include <unistd.h>
#include "ifdata.h"
#include "variable.h"
#include "miscutil/lstring.h"


//
// Top level simulator interface.
//

// references
class cUdf;
struct sLine;
struct sParamTab;
struct sMacroMapTab;
struct sCKT;
struct sTASK;
struct sJOB;
struct sControl;
struct sSymTab;
struct sOPTIONS;
struct sSWEEPprms;
struct sCHECKprms;
struct sPlot;
struct pnode;
struct pnlist;
struct sDataVec;
struct sDvList;
struct wordlist;
struct variable;
struct sTrie;
struct sHtab;
struct IFspecial;
struct sRunopSave;
struct sRunopTrace;
struct sRunopIplot;
struct sRunopMeas;
struct sRunopStop;
struct sPlotList;
struct sRunDesc;
struct sSaveList;
union va_t;

// Default char used to separate units string from numbers, was '_' in
// releases before 3.2.4.
//
#define DEF_UNITS_CATCHAR '#'

// Default character that separtes numerator from denomintor in units
// strings, was '/' before 4.3.9, which could lead to ambiguous
// expressions.
//
#define DEF_UNITS_SEPCHAR '_'

// Default char used as a field separator when creating flat token
// names when expanding subckts, was ':' in releases before 3.2.5. 
// Changed to '_' in 3.2.5 to avoid clash with ternary a?b:c operator,
// though this shouldn't happen.  Changed to '.' in 3.2.15 since this
// seems to produce nicer-looking output and is the same as used in
// HSPICE.
//
// In port-monitor mode, the default is to ':' for backward compatibility,
// but this may be changed via the protocol with Xic.
//
#define DEF_SUBC_CATCHAR '.'

// Default character used to separate a plot name from a vector name
// when referencing vectors.
//
#define DEF_PLOT_CATCHAR '.'

// Defulat character that indicates a "special" vector name.
//
#define DEF_SPEC_CATCHAR '@'

// startup file names
#define SYS_STARTUP(buf) sprintf(buf, "%sinit", CP.Program())
#define USR_STARTUP(buf) sprintf(buf, ".%sinit", CP.Program())

// Executable block (codeblock).
struct sExBlk
{
    sExBlk()
        {
            xb_name = 0;
            xb_text = 0;
            xb_tree = 0;
        }

    void clear();
    void exec(bool);

    const char *name()              { return (xb_name); }
    void set_name(const char *n)
        {
            char *s = lstring::copy(n);
            delete [] xb_name;
            xb_name = s;
        }

    wordlist *text()                { return (xb_text); }
    void set_text(wordlist *w)      { xb_text = w; }

    sControl *tree()                { return (xb_tree); }
    void set_tree(sControl *t)      { xb_tree = t; }

private:
    char *xb_name;
    wordlist *xb_text;
    sControl *xb_tree;
};

// Container for runop list bases, etc.
//
struct sRunopDb
{
    sRunopDb()
        {
            rd_save = 0;
            rd_trace = 0;
            rd_iplot = 0;
            rd_meas = 0;
            rd_stop = 0;
            rd_runopcnt = 1;
            rd_stepcnt = 0;
            rd_steps = 0;
        }

    ~sRunopDb()
        {
            clear();
        }

    void clear();

    bool isset()
        { return (rd_iplot || rd_trace || rd_save || rd_meas || rd_stop); }

    sRunopSave *saves()             { return (rd_save); }
    void set_saves(sRunopSave *d)   { rd_save = d; }

    sRunopTrace *traces()           { return (rd_trace); }
    void set_traces(sRunopTrace *d) { rd_trace = d; }

    sRunopIplot *iplots()           { return (rd_iplot); }
    void set_iplots(sRunopIplot *d) { rd_iplot = d; }

    sRunopMeas *measures()          { return (rd_meas); }
    void set_measures(sRunopMeas *m){ rd_meas = m; }

    sRunopStop *stops()             { return (rd_stop); }
    void set_stops(sRunopStop *d)   { rd_stop = d; }

    int new_count()             { return (rd_runopcnt++); }
    int decrement_count()       { return (rd_runopcnt--); }

    int step_count()            { return (rd_stepcnt); }
    void set_step_count(int i)  { rd_stepcnt = i; }
    int dec_step_count()        { return (--rd_stepcnt); }

    int num_steps()             { return (rd_steps); }
    void set_num_steps(int i)   { rd_steps = i; }

private:
    sRunopSave *rd_save;        // save list head
    sRunopTrace *rd_trace;      // trace list head
    sRunopIplot *rd_iplot;      // iplot list head
    sRunopMeas *rd_meas;        // measures list head
    sRunopStop *rd_stop;        // stops list head

    int rd_runopcnt;            // running sum of runops created, provides id
    int rd_stepcnt;             // number of steps done
    int rd_steps;               // number of steps to do
};

// Element list for Alter command.
struct dfrdlist
{
    dfrdlist(const char *d, const char *p, const char *v)
        {
            next = 0;
            dname = lstring::copy(d);
            param = lstring::copy(p);
            rhs = lstring::copy(v);
        }

    ~dfrdlist()
        {
            delete [] dname;
            delete [] param;
            delete [] rhs;
        }

    static void destroy(dfrdlist *dl)
        {
            while (dl) {
                dfrdlist *dx = dl;
                dl = dl->next;
                delete dx;
            }
        }

    dfrdlist *next;
    char *dname;
    char *param;
    char *rhs;
};

// ci_runtype values
#define MONTE_GIVEN    1
#define CHECKALL_GIVEN 2

// Type of analysis mode for sFtCirc::run and  IFsimulator::Simulate.
//
enum SIMtype { SIMresume, SIMrun, SIMac, SIMdc, SIMdisto, SIMnoise,
    SIMop, SIMpz, SIMsens, SIMtf, SIMtran };

//
// The circuit interface.
//
struct sFtCirc
{
    // circuit.cc
    sFtCirc();
    ~sFtCirc();

    void clear();
    int dotparam(const char*, IFdata*);
    double getPctDone();
    wordlist *getAnalysisFromDeck();
    bool getVerilog(const char*, const char*, IFdata*);
    void getSaves(sSaveList*, const sCKT*);

    // check.cc
    int checkCodeblocks();
    int setAnalysis(wordlist**);

    // device.cc
    dfrdlist *findDeferred(const char*, const char*);
    void addDeferred(const char*, const char*, const char*);
    void clearDeferred();
    void applyDeferred(sCKT*);
    void alter(const char*, wordlist*);
    void printAlter(FILE* = 0, bool = false);
    static bool devParams(int, wordlist**, wordlist**, bool);
    static void showDevParms(wordlist*, bool, bool);
    static wordlist *devExpand(const char*, bool);
    static bool parseDevParams(wordlist*, wordlist**, wordlist**, bool);
    static bool parseDevParams(const char*, wordlist**, wordlist**, bool);

    // simulate.cc
    int run(SIMtype, wordlist*);
    int run(const char*, wordlist*);
    void rebuild(bool);
    void reset();
    int runTrial();
    void resetTrial(bool);
    static bool isAnalysis(SIMtype);
    static const char *analysisString(SIMtype);
    static int analysisType(const char*);

    // source.cc
    void setDefines(cUdf*);
    void setup(sLine*, const char*, wordlist*, wordlist*, sLine*, sParamTab*);
    bool expand(sLine*, bool*);
    int newCKT(sCKT**, sTASK**);

    // subexpand()
    bool expandSubckts();

    // Exported access to private data.

    sFtCirc *next()             { return (ci_next); }
    const char *name()          { return (ci_name); }
    const char *descr()         { return (ci_descr); }
    const char *filename()      { return (ci_filename); }
    void clear_filename()       { delete [] ci_filename; ci_filename = 0; }

    sLine *deck()               { return (ci_deck); }
    sLine *origdeck()           { return (ci_origdeck); }
    sLine *options()            { return (ci_options); }
    sParamTab *params()         { return (ci_params); }
    variable *vars()            { return (ci_vars); }
    cUdf *defines()             { return (ci_defines); }

    sExBlk &execBlk()           { return (ci_execBlk); }
    sExBlk &controlBlk()        { return (ci_controlBlk); }
    sExBlk &postrunBlk()        { return (ci_postrunBlk); }
    void set_postrun(wordlist *w) { ci_postrunBlk.set_text(w); }

    sRunopDb &runops()          { return (ci_runops); }
    sRunopSave *saves()             { return (ci_runops.saves()); }
    void set_saves(sRunopSave *d)   { ci_runops.set_saves(d); }
    sRunopTrace *traces()           { return (ci_runops.traces()); }
    void set_traces(sRunopTrace *d) { ci_runops.set_traces(d); }
    sRunopIplot *iplots()           { return (ci_runops.iplots()); }
    void set_iplots(sRunopIplot *d) { ci_runops.set_iplots(d); }
    sRunopMeas *measures()          { return (ci_runops.measures()); }
    void set_measures(sRunopMeas *m){ ci_runops.set_measures(m); }
    sRunopStop *stops()             { return (ci_runops.stops()); }
    void set_stops(sRunopStop *d)   { ci_runops.set_stops(d); }

    wordlist *commands()        { return (ci_commands); }

    sTrie **nodes()             { return (&ci_nodes); }
    sTrie **devices()           { return (&ci_devices); }
    sTrie **models()            { return (&ci_models); }

    sOPTIONS *defOpt()          { return (ci_defOpt); }

    sSymTab *symtab()           { return (ci_symtab); }
    sCKT *runckt()              { return (ci_runckt); }
    void set_runckt(sCKT *c)    { ci_runckt = c; }
    sSWEEPprms *sweep()         { return (ci_sweep); }
    void set_sweep(sSWEEPprms *p) { ci_sweep = p; }
    sCHECKprms *check()         { return (ci_check); }
    void set_check(sCHECKprms *j) { ci_check = j; }
    sPlot *runplot()            { return (ci_runplot); }
    void set_runplot(sPlot *p)  { ci_runplot = p; }

    int runtype()               { return (ci_runtype); }
    void set_runtype(int r)     { ci_runtype = r; }
    bool inprogress()           { return (ci_inprogress); }
    void set_inprogress(bool b) { ci_inprogress = b; }
    bool runonce()              { return (ci_runonce); }
    void set_runonce(bool b)    { ci_runonce = b; }

    void set_keep_deferred(bool b)          { ci_keep_deferred = b; }
    void set_use_trial_deferred(bool b)     { ci_use_trial_deferred = b; }

private:
    sFtCirc *ci_next;           // The next in the list
    const char *ci_name;        // Unique name for this circuit
    const char *ci_descr;       // Descriptive string
    const char *ci_filename;    // Where this circuit came from

    sLine *ci_deck;             // The input deck
    sLine *ci_origdeck;         // The input deck, before subckt expansion
    sLine *ci_verilog;          // Verilog text
    sLine *ci_options;          // The .option cards from the deck...
    sParamTab *ci_params;       // .param parameter list
    variable *ci_vars;          // ... and the parsed versions
    cUdf *ci_defines;           // Functions defined by .param lines

    sExBlk ci_execBlk;          // Pre-parse executable block
    sExBlk ci_controlBlk;       // Post-parse executable block
    sExBlk ci_postrunBlk;       // Post-run executable block
    sRunopDb ci_runops;         // Circuit-specific runops, if any

    wordlist *ci_commands;      // Things to do when this circuit is done

    sTrie *ci_nodes;            // ccom structs for the nodes...
    sTrie *ci_devices;          // and devices in the circuit
    sTrie *ci_models;           // ccom struct for models

    sOPTIONS *ci_defOpt;        // .options set for this circuit
    dfrdlist *ci_deferred;      // Deferred special asignments
    dfrdlist *ci_trial_deferred; // Deferred special asignments, loop/check

    sSymTab *ci_symtab;         // String (UID) table for circuit.
    sCKT *ci_runckt;            // The running or most-recently run ckt
    sSWEEPprms *ci_sweep;       // Structure pointer used in analysis loops
    sCHECKprms *ci_check;       // Structure pointer used in check loops
    sPlot *ci_runplot;          // Set to plot struct during analysis

    int ci_runtype;             // MONTE_GIVEN, CHECKALL_GIVEN, or zero
    bool ci_inprogress;         // We are in a break now
    bool ci_runonce;            // So com_run can do a reset if necessary...
    bool ci_keep_deferred;      // Don't clear deferred alter list in run
    bool ci_use_trial_deferred; // Use trial deferred list
};


// Used in GetVar below.  The type returned is always the type requested.
// Strings are returned as copies, and are destroyed in the destructor.
//
struct VTvalue
{
    VTvalue()
        {
            u.v_real = 0.0;
            string_ptr = 0;
        }

    ~VTvalue()
        {
            delete [] string_ptr;
        }

    void reset()
        {
            delete [] string_ptr;
            string_ptr = 0;
            memset(&u, 0, sizeof(u));
        }

    void set_real(double d)     { u.v_real = d; }
    double get_real()           { return (u.v_real); }
    void set_int(int i)         { u.v_num = i; }
    int get_int()               { return (u.v_num); }
    void set_bool(bool b)       { u.v_bool = b; }
    bool get_bool()             { return (u.v_bool); }
    void set_string(char *s)    { u.v_string = s; string_ptr = s; }
    char *get_string()          { return (u.v_string); }
    void set_list(variable *v)  { u.v_list = v; }
    variable *get_list()        { return (u.v_list); }

private:
    va_t u;
    char *string_ptr;
};


// A file list used for handling multiple input files, and for dealing with
// the ".newjob" card.
//
struct file_elt
{
    file_elt(const char *fn, int lv, int ofs)
        {
            fe_next = 0;
            fe_filename = lstring::copy(fn);
            fe_lineval = lv;
            fe_offset = ofs;
        }

    ~file_elt()
        {
            delete [] fe_filename;
        }

    static void destroy(file_elt *f)
        {
            while (f) {
                file_elt *fx = f;
                f = f->fe_next;
                delete fx;
            }
        }

    // Find the end of the circuit started by this.  The end will either
    // be null, or a .newjob line.
    //
    file_elt *get_end()
        {
            file_elt *fend = fe_next;
            while (fend && fend->fe_lineval < 0)
                fend = fend->fe_next;
            return (fend);
        }

    file_elt *next()        { return (fe_next); }
    const char *filename()  { return (fe_filename); }

    static file_elt *wl_to_fl(const wordlist*, wordlist**);

private:
    file_elt *fe_next;
    char *fe_filename;  // File name.
    int fe_lineval;     // Line count of .newjob line.
    int fe_offset;      // Byte offset to .newjob line start.
};

// Global constants.
//
struct sConstant
{
    sConstant(const char *n, double v, const char *u)
        {
            name = n;
            value = v;
            units = u;
        }

    const char *name;
    double value;
    const char *units;
};

// Flags used in IFsimulator.
//
enum FT_FLAG
{
    FT_BATCHMODE,       // Batch mode simulator.
    FT_SERVERMODE,      // Variant of batch mode.
    FT_JS3EMU,          // Jspice3 emulation mode.
    FT_SILENT,          // Only print really serious error messages.
    FT_DCOSILENT,       // Don't print convergence error message in DCOP.
    FT_NOERRWIN,        // Don't use a separate error message window.
    FT_INTERRUPT,       // Interrupt received.
    FT_SIMFLAG,         // Set while running simulation.
    FT_MONTE,           // Set while running Monte Carlo analysis.
    FT_NOPAGE,          // Tree evaluation debugging.
    FT_RAWFGIVEN,       // A rawfile was given on the command line.

    FT_EVDB,            // Tree evaluation debugging.
    FT_VECDB,           // Vector debugging.
    FT_GRDB,            // Graphics debugging.
    FT_GIDB,            // Graphics interface debugging.
    FT_CTRLDB,          // Control stack debugging.
    FT_ASYNCDB,         // Job control debugging.
    FT_SIMDB,           // Simulator debugging.

    FT_LISTPRNT,        // Batch mode, list file.
    FT_OPTSPRNT,        // Batch mode, list options.
    FT_NODESPRNT,       // Batch mode, list nodes.
    FT_ACCTPRNT,        // Batch mode, list accounting info.
    FT_MODSPRNT,        // Batch mode, list models.
    FT_DEVSPRNT,        // Batch mode, list devices.

    FT_STRICTNUM,       // Fail if trailing alphas in numbers if set.
    FT_DEFERFN,         // Create dummy node for unresolved functions.

    FT_NUMFLAGS
};

// Listing types and flags.
enum LStype
{
    LS_NONE,
    LS_LOGICAL,
    LS_PHYSICAL,
    LS_DECK
};
#define LS_EXPAND   0x4
#define LS_NOCONT   0x8

inline LStype LS_TYPE(unsigned int t)   { return ((LStype)(t & 3)); }

// Subcircuit expansion algorithms.  See inpsubc.cc for descriptions.
enum {
    SUBC_CATMODE_WR,
    SUBC_CATMODE_SPICE3
};

// Floating point exception handling.
enum FPEmode
{
    FPEcheck,       // No signals, exceptions checked in code.
    FPEnoCheck,     // No signals, no exception checking.
    FPEdebug        // Emit/catch signals, print error and revert to
                    // FPEnoCheck, for GDB.
};
#define FPEdefault  FPEcheck

// Structure: IFsimulator
//
// This structure dscribes the top level simulator interface.
//
struct IFsimulator
{
    // bin/wrspice.cc
    IFsimulator();
    static bool StartupFileName(char**);
#ifdef WIN32
    static void SigHdlr(int);
#else
    static void SigHdlr(int, void*, void*);
#endif

    // fte/check.cc
    void MargAnalysis(wordlist*);

    // fte/circuit.cc
    void SetCircuit(const char*);
    void FreeCircuit(const char*);
    void Bind(const char*, bool);
    void Unbind(const char*);
    void ListBound();
    void SetOption(bool, const char*, IFdata*);
    void OptUpdate();
    void AddNodeNames(const char*);

    // fte/define.cc
    cUdf *PushUserFuncs(cUdf*);
    cUdf *PopUserFuncs();
    bool DefineUserFunc(wordlist*);
    bool DefineUserFunc(const char*, const char*);
    bool IsUserFunc(const char*, int);
    bool IsUserFunc(const char*, const pnode*);
    bool UserFunc(const char*, int, char**, const char**);
    pnode *GetUserFuncTree(const char*, const pnode*);
    bool TestAndPromote(pnode*, const sParamTab*, sMacroMapTab*);

    // fte/device.cc
    void Show(wordlist*, char**, bool, int);
    void CheckDevlib();
    void LoadModules(const char*);

    // fte/dotcards.cc
    sLine *GetDotOpts(sLine*);
    void GetDotSaves();
    void SaveDotArgs();
    void ExecCmds(wordlist*);
    void RunBatch();
    wordlist *ExtractPlotCmd(int, const char*);
    wordlist *ExtractPrintCmd(int);

    // fte/error.cc
    void FPexception(int);
    void Error(int, const char* = 0, const char* = 0);
    const char *ErrorShort(int);
    bool HspiceFriendly();

    // fte/evaluate.cc
    sDataVec *Evaluate(pnode*);
    sDvList *DvList(pnlist*);

    // fte/initialize.cc
    void PreInit();
    void PostInit();
    void Periodic();
    bool ImplicitCommand(wordlist*);
    bool IsTrue(wordlist*);
    void SetFPEmode(FPEmode);
    FPEmode SetCircuitFPEmode();

    // fte/inpcom.cc
    bool Edit(const char*, bool(*)(const char*, void*, int), bool, bool*);
    void Listing(FILE*, sLine*, sLine*, int);
    FILE *PathOpen(const char*, const char*, bool* = 0);
    char *FullPath(const char*);

    // fte/parse.cc
    pnlist *GetPtree(wordlist*, bool);
    pnlist *GetPtree(const char*, bool);
    pnode *GetPnode(const char**, bool, bool = false, bool = false);
    bool CheckFuncName(const char*);

    // fte/resource.cc
    void ShowResource(wordlist*, char**);
    void CheckSpace();
    void InitResource();
    void ShowOption(wordlist*, char**);

    // fte/simulate.cc
    void Simulate(SIMtype, wordlist*);
    void Run(const char*);
    int InProgress();

    // fte/source.cc
    bool Block(const char*);
    void Source(file_elt*, bool, bool, bool);
    void EditSource(const char*, bool, bool);
    bool SpSource(FILE*, bool, bool, const char*);
    void DeckSource(sLine*, bool, bool, const char*, bool);
    sFtCirc *SpDeck(sLine*, const char*, wordlist*, wordlist*, sLine*,
        bool, bool, bool *err);
    char *ReadLine(FILE*, bool*);
    sLine *ReadDeck(FILE*, char*, bool*, sParamTab**, const char*);
    void ContinueLines(sLine*);
    void CaseFix(char*);

    // fte/spvariable.cc
    void SetVar(const char*);
    void SetVar(const char*, int);
    void SetVar(const char*, double);
    void SetVar(const char*, const char*);
    void SetVar(const char*, variable*);
    void SetVar(wordlist*);
    variable *GetRawVar(const char*, sFtCirc* = 0);
    bool GetVar(const char*, VTYPenum, VTvalue*, sFtCirc* = 0);
    void VarPrint(sLstr*);
    variable *EnqPlotVar(const char*);
    variable *EnqCircuitVar(const char*);
    variable *EnqVectorVar(const char*, bool = false);
    void RemVar(const char*);
    void ClearVariables();

    // fte/sweep.cc
    void SweepAnalysis(wordlist*);

    // fte/types.cc
    const char *TypeAbbrev(int);
    const char *TypeNames(int);
    int TypeNum(const char*);
    const char *PlotAbbrev(const char*);

    // Access to private members.

    const char *Simulator()         { return (ft_simulator); }
    const char *Description()       { return (ft_description); }
    const char *Version()           { return (ft_version); }

    void ControlsPush()             { ft_control_depth++;}
    void ControlsPop()              { if (ft_control_depth)
                                          ft_control_depth--; }
    int ControlsDepth()             { return (ft_control_depth); }

    void ExecsPush()                { ft_exec_depth++;}
    void ExecsPop()                 { if (ft_exec_depth)
                                          ft_exec_depth--; }
    int ExecsDepth()                { return (ft_exec_depth); }

    FPEmode GetFPEmode()            { return (ft_fpe_mode); }

    sHtab *Options()                { return (ft_options); }

    sFtCirc *CurCircuit()           { return (ft_curckt); }
    void SetCurCircuit(sFtCirc *c)  { ft_curckt = c; }
    sFtCirc *RunCircuit()           { return (ft_runckt); }
    void SetRunCircuit(sFtCirc *c)  { ft_runckt = c; }
    sFtCirc *CircuitList()          { return (ft_circuits); }
    void SetCircuitList(sFtCirc *c) { ft_circuits = c; }

    IFanalysis *CurAnalysis()       { return (ft_cur_analysis); }
    void SetCurAnalysis(IFanalysis *a) { ft_cur_analysis = a; }

    void PushFPEinhibit()           { ft_fpe_inhibit++; }
    void PopFPEinhibit()            { if (ft_fpe_inhibit) ft_fpe_inhibit--; }
    int GetFPEinhibit()             { return (ft_fpe_inhibit); }

    int GetTranTrace()              { return (ft_trantrace); }
    void SetTranTrace(int i)        { ft_trantrace = i; }

    bool GetFlag(FT_FLAG which)     { return (ft_flags[which]); }
    void SetFlag(FT_FLAG which, bool val) { ft_flags[which] = val; }

    char SubcCatchar()              { return (ft_subc_catchar); }
    void SetSubcCatchar(char c)     { ft_subc_catchar = c; }
    char UnitsCatchar()             { return (ft_units_catchar); }
    void SetUnitsCatchar(char c)    { ft_units_catchar = c; }
    char UnitsSepchar()             { return (ft_units_sepchar); }
    void SetUnitsSepchar(char c)    { ft_units_sepchar = c; }
    char PlotCatchar()              { return (ft_plot_catchar); }
    void SetPlotCatchar(char c)     { ft_plot_catchar = c; }
    char SpecCatchar()              { return (ft_spec_catchar); }
    void SetSpecCatchar(char c)     { ft_spec_catchar = c; }

    int SubcCatmode()               { return (ft_subc_catmode); }
    void SetSubcCatmode(int m)      { ft_subc_catmode = m; }

    sConstant *Constants()          { return (ft_constants); }

private:
    const char *ft_simulator;   // The simulator's name
    const char *ft_description; // Description of this simulator
    const char *ft_version;     // Version or revision level of simulator

    FPEmode ft_fpe_mode;        // Floating point error checking mode

    sHtab *ft_options;          // Database for 'set' options

    sFtCirc *ft_curckt;         // The default circuit
    sFtCirc *ft_runckt;         // The circuit currently being analyzed
    sFtCirc *ft_circuits;       // List head for circuits

    IFanalysis *ft_cur_analysis; // The most recent analysis started.

    int ft_control_depth;       // .control execution frame.
    int ft_exec_depth;          // .exec execution frame.

    int ft_fpe_inhibit;         // Longjmp on SIGFPE inhibited when nonzero

    int ft_trantrace;           // Transient analysis tracing level.

    bool ft_flags[FT_NUMFLAGS]; // Misc. flag vector.

    // Character used to separate generated name fields in subcircuit
    // token expansion.
    //
    char ft_subc_catchar;

    // This can follow a number, separating a units string from the number.
    //
    char ft_units_catchar;

    // Character used in units strings to separate numerator from
    // denominator.  Avoid use of '/' which can be ambiguous.
    char ft_units_sepchar;

    // Character that separates plot name from vector name when referencing
    // vectors.
    //
    char ft_plot_catchar;

    // Character that indicates a "special" vector.
    //
    char ft_spec_catchar;

    // This sets the name mapping algorithm used for subcircuit expansion.
    // It should be one of the SUBC_CATMODE values.
    //
    char ft_subc_catmode;

    static sConstant ft_constants[];
};

extern IFsimulator Sp;


//
// Macros
//

#define DCOPY(s,d,n) {int ii = (n); \
    double *ps = (double*)(s); double *pd = (double*)(d); \
    while (ii--) *pd++ = *ps++; }

#define CCOPY(s,d,n) {int ii = (n); \
    complex *ps = (complex*)(s); complex *pd = (complex*)(d); \
    while (ii--) *pd++ = *ps++; }


//
// Definitions for external symbols in FTE.
//

namespace ResPrint
{
    // resource.cc
    void print_res(const char*, sLstr*);
    void print_var(const variable*, sLstr*);
    void print_stat(const char*, sLstr*);
    void print_opt(const char*, sLstr*);
    void print_cktvar(const variable*, sLstr*);
    void get_elapsed(double*, double*, double*);
    void get_faults(unsigned int*, unsigned int*);
    void get_space(unsigned int*, unsigned int*, unsigned int*);
};


#define DEF_polydegree          1
#define DEF_polydegree_MIN      1
#define DEF_polydegree_MAX      9

#define DEF_polysteps           10
#define DEF_polysteps_MIN       1
#define DEF_polysteps_MAX       100

#define DEF_checkiterate        0
#define DEF_checkiterate_MIN    0
#define DEF_checkiterate_MAX    10

#define DEF_diff_abstol         1e-12
#define DEF_diff_abstol_MIN     1e-15
#define DEF_diff_abstol_MAX     1e-9

#define DEF_diff_reltol         1e-3
#define DEF_diff_reltol_MIN     1e-5
#define DEF_diff_reltol_MAX     1e-2

#define DEF_diff_vntol          1e-6
#define DEF_diff_vntol_MIN      1e-9
#define DEF_diff_vntol_MAX      1e-3

#define DEF_dpolydegree         2
#define DEF_dpolydegree_MIN     0
#define DEF_dpolydegree_MAX     7

#define DEF_fourgridsize        200
#define DEF_fourgridsize_MIN    1
#define DEF_fourgridsize_MAX    10000

#define DEF_nfreqs              10
#define DEF_nfreqs_MIN          1
#define DEF_nfreqs_MAX          100

#define DEF_numdgt              4
#define DEF_numdgt_MIN          0
#define DEF_numdgt_MAX          15

#define DEF_rawfileprec         15
#define DEF_rawfileprec_MIN     0
#define DEF_rawfileprec_MAX     15

#define DEF_specwindoworder     2
#define DEF_specwindoworder_MIN 2
#define DEF_specwindoworder_MAX 8

#endif // SIMULATOR_H

