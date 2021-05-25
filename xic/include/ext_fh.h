
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
 * Xic Integrated Circuit Layout and Schematic Editor                     *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

#ifndef EXT_FH_H
#define EXT_FH_H

#include "geo_zlist.h"
#include "geo_3d.h"
#include "tech_ldb3d.h"
#include "ext_fxunits.h"
#include "ext_fxjob.h"


//
// Definitions for the "new" FastHenry interface.
//

// Variables
#define VA_FhArgs               "FhArgs"
#define VA_FhDefaults           "FhDefaults"
#define VA_FhDefNhinc           "FhDefNhinc"
#define VA_FhDefRh              "FhDefRh"
#define VA_FhForeg              "FhForeg"
#define VA_FhFreq               "FhFreq"
#define VA_FhLayerName          "FhLayerName"
#define VA_FhManhGridCnt        "FhManhGridCnt"
#define VA_FhMonitor            "FhMonitor"
#define VA_FhOverride           "FhOverride"
#define VA_FhPath               "FhPath"
#define VA_FhUnits              "FhUnits"
#define VA_FhUseFilament        "FhUseFilament"
#define VA_FhVolElEnable        "FhVolElEnable"
#define VA_FhVolElMin           "FhVolElMin"
#define VA_FhVolElTarget        "FhVolElTarget"

// Debugging (undocumented)
#define VA_FhZoids              "FhZoids"

#define FH_LAYER_NAME           "FHRY"

// Default partitioning params, microns
#define FH_MIN_MANH_GRID_CNT    1e2
#define FH_MAX_MANH_GRID_CNT    1e5
#define FH_DEF_MANH_GRID_CNT    1e3

#define FH_MIN_DEF_NHINC        1
#define FH_MAX_DEF_NHINC        20

#define FH_MIN_DEF_RH           0.5
#define FH_MAX_DEF_RH           4.0

#define FH_MIN_VOLEL_MIN        0.0
#define FH_MAX_VOLEL_MIN        1.0
#define FH_DEF_VOLEL_MIN        0.1

#define FH_MIN_VOLEL_TARG       1e2
#define FH_MAX_VOLEL_TARG       1e5
#define FH_DEF_VOLEL_TARG       1e3

// File extensions
#define FH_INP_SFX              "inp"

struct fhLayout;
struct fhLayer;
struct fhConductor;
struct SymTab;

struct fhNode
{
    fhNode(int nm, xyz3d *p, fhNode *nx)
        {
            n_num_and_ref = (nm << 3);
            n_loc = *p;
            n_next = nx;
        }

    static void destroy(fhNode *n)
        {
            while (n) {
                fhNode *x = n;
                n = n->n_next;
                delete x;
            }
        }

    unsigned int number()           const { return (n_num_and_ref >> 3); }
    unsigned int refcnt()           const { return (n_num_and_ref & 0x7); }

    void inc_ref()
        {
            if (refcnt() < 7)
                n_num_and_ref++;
        }

    const xyz3d *loc()              const { return (&n_loc); }

    fhNode *next()                  const { return (n_next); }
    void set_next(fhNode *n)        { n_next = n; }

private:
    unsigned int n_num_and_ref;
    xyz3d n_loc;
    fhNode *n_next;
};

struct fhNodeList
{
    fhNodeList(fhNode *n, fhNodeList *x)
        {
            nd = n;
            next = x;
        }

    static void destroy(fhNodeList *n)
        {
            while (n) {
                fhNodeList *x = n;
                n = n->next;
                delete x;
            }
        }

    fhNode *nd;
    fhNodeList *next;
};

// Initial node hashing bit count, dynamically increases with table size.
#define FH_NDHASHW 6

struct fhNodeGen
{
    fhNodeGen()
        {
            ng_allocated = 0;
            ng_mask = ~((unsigned int)-1 << FH_NDHASHW);
            ng_tab = new fhNode*[ng_mask+1];
            memset(ng_tab, 0, (ng_mask+1)*sizeof(fhNode*));
        }

    ~fhNodeGen()
        {
            for (unsigned int i = 0; i <= ng_mask; i++)
                fhNode::destroy(ng_tab[i]);
            delete [] ng_tab;
        }

    unsigned int allocated()    { return (ng_allocated); }

    int newnode(xyz3d*);
    void nodeBB(BBox*) const;
    fhNodeList *find_nodes(int, const Point*) const;
    fhNode *find_node(int, const xyz3d*) const;
    int fh_nodes_print(FILE*, e_unit) const;
    void clear();

private:
    void rehash();

    fhNode **ng_tab;
    unsigned int ng_allocated;
    unsigned int ng_mask;
};

struct fhSegment
{
    fhSegment(int n1, int n2, const xyz3d *p1, const xyz3d *p2, int w, int h,
        fhSegment *n)
        {
            s_node1 = n1;
            s_node2 = n2;
            s_pn1 = *p1;
            s_pn2 = *p2;
            s_wid = w;
            s_hei = h;
            s_next = n;
        }

    static void destroy(fhSegment *s)
        {
            while (s) {
                fhSegment *x = s;
                s = s->s_next;
                delete x;
            }
        }

    int node1()                 const { return (s_node1); }
    int node2()                 const { return (s_node2); }
    const xyz3d *pn1()          const { return (&s_pn1); }
    const xyz3d *pn2()          const { return (&s_pn2); }
    int wid()                   const { return (s_wid); }
    int hei()                   const { return (s_hei); }
    fhSegment *next()           const { return (s_next); }

private:
    int s_node1, s_node2;       // nodes connected
    xyz3d s_pn1, s_pn2;         // coordinates of nodes
    int s_wid, s_hei;           // filament width and height
    fhSegment *s_next;
};

struct fhTermList
{
    fhTermList(const BBox *bb, const char *pnm, const char *sfx,
            int ln, int cn, int cc, fhTermList *nx)
        {
            tl_next = nx;
            tl_portname = lstring::copy(pnm);
            tl_sfx = lstring::copy(sfx);
            tl_nodes = 0;
            tl_points = new Point[2];
            tl_points[0].set(bb->left, bb->bottom);
            tl_points[1].set(bb->right, bb->top);
            tl_npts = 2;
            tl_lnum = ln;
            tl_cnum = cn;
            tl_ccnt = cc;
        }

    fhTermList(const Poly *po, const char *pnm, const char *sfx,
            int ln, int cn, int cc, fhTermList *nx)
        {
            tl_next = nx;
            tl_portname = lstring::copy(pnm);
            tl_sfx = lstring::copy(sfx);
            tl_nodes = 0;
            tl_points = new Point[po->numpts];
            memcpy(tl_points, po->points, po->numpts*sizeof(Point));
            tl_npts = po->numpts;
            tl_lnum = ln;
            tl_cnum = cn;
            tl_ccnt = cc;
        }

    ~fhTermList()
        {
            delete [] tl_portname;
            delete [] tl_sfx;
            delete [] tl_points;
            fhNodeList::destroy(tl_nodes);
        }

    static void destroy(fhTermList *l)
        {
            while (l) {
                fhTermList *x = l;
                l = l->next();
                delete x;
            }
        }

    fhTermList *next()              const { return(tl_next); }
    void set_next(fhTermList *n)          { tl_next = n; }

    const char *portname()          const { return (tl_portname); }
    const char *suffix()            const { return (tl_sfx); }
    fhNodeList *nodes()             const { return (tl_nodes); }
    const Point *points()           const { return (tl_points); }
    int num_points()                const { return (tl_npts); }
    int lnum()                      const { return (tl_lnum); }
    int cnum()                      const { return (tl_cnum); }
    int ccnt()                      const { return (tl_ccnt); }

    void set_nodes(fhNodeList *l)
        {
            fhNodeList::destroy(tl_nodes);
            tl_nodes = l;
            for (fhNodeList *n = tl_nodes; n; n = n->next)
                n->nd->inc_ref();
        }

private:
    fhTermList *tl_next;
    char *tl_portname;     // terminal port name
    char *tl_sfx;          // terminal suffix
    fhNodeList *tl_nodes;  // enclosed nodes
    Point *tl_points;      // terminal vertices
    int tl_npts;           // number terminal vertices, closed poly or box==2
    int tl_lnum;           // associated layer index
    int tl_cnum;           // associated conductor index
    int tl_ccnt;           // copy count, 0 for original
};

struct fhConductor
{
    fhConductor(int lix = -1, int g = -1)
        {
            hc_next = 0;
            hc_zlist3d_ref = 0;
            hc_zlist3d = 0;
            hc_group = g;
            hc_layer_ix = lix;
            hc_sigma = 0.0;
            hc_tau = 0.0;
            hc_lambda = 0.0;
            hc_segments = 0;
        }

    ~fhConductor()
        {
            glZlistRef3d::destroy(hc_zlist3d_ref);
            glZlist3d::destroy(hc_zlist3d);
            fhSegment::destroy(hc_segments);
        }

    static void destroy(fhConductor *c)
        {
            while (c) {
                fhConductor *x = c;
                c = c->hc_next;
                delete x;
            }
        }

    void manhattanize(int minsize)
        {
            hc_zlist3d = glZlist3d::manhattanize(hc_zlist3d, minsize);
        }

    static fhConductor *addz3d(fhConductor*, const glZoid3d*);
    fhNodeList *get_nodes(const fhNodeGen&, int, const Point*);
    void segmentize(fhNodeGen&);
    int segments_print(FILE*, e_unit, const fhLayout*);
    bool refine(const BBox*);
    PolyList *polylist();
    void accum_points(SymTab*, SymTab*);
    void accum_points_z(SymTab*);
    bool split(int*, int, int*, int);
    bool split_z(int*, int);
    void save_dbg_zlist();

    void set_zlist3d_ref(glZlistRef3d *z)   { hc_zlist3d_ref = z; }
    void set_zlist3d(glZlist3d *z)          { hc_zlist3d = z; }

    glZlistRef3d *zlist3d_ref() const { return (hc_zlist3d_ref); }
    glZlist3d *zlist3d()        const { return (hc_zlist3d); }

    void set_sigma(double d)    { hc_sigma = d; }
    void set_tau(double d)      { hc_tau = d; }
    void set_lambda(double d)   { hc_lambda = d; }

    int group()                 const { return (hc_group); }
    int layer_index()           const { return (hc_layer_ix); }
    double sigma()              const { return (hc_sigma); }
    double tau()                const { return (hc_tau); }
    double lambda()             const { return (hc_lambda); }

    void set_next(fhConductor *n)     { hc_next = n; }
    fhConductor *next()         const { return (hc_next); }

private:
    fhConductor *hc_next;
    glZlistRef3d *hc_zlist3d_ref; // initial conductor representation
    glZlist3d *hc_zlist3d;      // sliced conductor representation
    int hc_group;               // conductor group
    int hc_layer_ix;            // layer index
    double hc_sigma;            // conductivity
    double hc_tau;              // Drude relaxation time
    double hc_lambda;           // penetration depth
    fhSegment *hc_segments;     // segment list
};

struct fhLayer
{
    fhLayer()
        {
            fl_list = 0;
            fl_l3d = 0;
        }

    ~fhLayer()
        {
            fhConductor::destroy(fl_list);
        }

    void set_layer(Layer3d *l)
        {
            fl_l3d = l;
        }

    Layer3d *layer()
        {
            return (fl_l3d);
        }

    void clear_list()
        {
            fhConductor::destroy(fl_list);
            fl_list = 0;
        }

    void addz3d(const glYlist3d *yl)
        {
            for (const glYlist3d *y = yl; y; y = y->next) {
                for (const glZlist3d *z = y->y_zlist; z; z = z->next)
                    fl_list = fhConductor::addz3d(fl_list, &z->Z);
            }
        }

    fhConductor *cndlist()              const { return (fl_list); }
    void set_cndlist(fhConductor *l)    { fl_list = l; }

private:
    fhConductor *fl_list;   // conductors on this layer
    Layer3d *fl_l3d;        // back pointer to layer
};

struct fhLayout : public Ldb3d
{
    fhLayout();

    ~fhLayout()
        {
            delete [] fhl_layers;
            if (fhl_terms) {
                for (int i = 0; i < num_groups(); i++)
                    fhTermList::destroy(fhl_terms[i]);
                delete [] fhl_terms;
            }
        }

    bool setup();
    bool fh_dump(FILE*);

    static void clear_dbg_zlist();

    fhLayer *fhlayers()             const { return (fhl_layers); }
    fhTermList **terms()            const { return (fhl_terms); }
    const fhNodeGen *nodes()        const { return (&fhl_ngen); }

private:
    void slice_groups(int, int);
    void slice_groups_z(int);
    fhConductor *find_conductor(int, int);
    void add_terminal(const Poly*, const char*, const char*, int, int);
    char *check_sort_terms();
    void dump_ports(FILE*);

    fhLayer *fhl_layers;            // layers
    fhTermList **fhl_terms;         // terminal lists, indexed by group
    fhNodeGen fhl_ngen;             // node generator
    bool fhl_zoids;                 // create zoids for debug
};

inline class cFH *FH();

// The exported interface to the Extract RL functionality.  This uses
// FastHenry to compute inductance and resistance of structures.
//
class cFH : public fxGif
{
    static cFH *ptr()
        {
            if (!instancePtr)
                on_null_ptr();
            return (instancePtr);
        }

    static void on_null_ptr();

public:
    friend inline cFH *FH() { return (cFH::ptr()); }

    // ext_fh.cc
    cFH();
    void doCmd(const char*, const char*);
    bool fhDump(const char*);
    void fhRun(const char*, const char*, const char*, bool = false);
    void setPopUpVisible(bool);
    char *getFileName(const char*, int i = -1);
    const char *getUnitsString(const char*);
    int getUnitsIndex(const char*);
    char *statusString();
    const char *command(int);
    char *jobList();
    void killProcess(int);

    // graphics
    void PopUpExtIf(GRobject, ShowMode);
    void updateString();

private:
    bool fh_popup_visible;      // True when controlling pop-up is displayed.

    static cFH *instancePtr;
};

#endif

