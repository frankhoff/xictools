
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

#include "cd.h"
#include "cd_types.h"
#include "cd_propnum.h"
#include "cd_terminal.h"
#include "cd_celldb.h"
#include "cd_netname.h"
#include <algorithm>


//XXX  A temporary consistency check.
#define CHECK_CELLTYPE

//----------------------------------------------------------------------
// The NameOK function becomes a huge bottleneck when called multiple
// times in a cell with lots of instances (such as when updating the
// terminal names).  So, a caching functionality has been
// incorporated.  This should be enabled/disabled with
// cCD::EnableNameCache around operations that will call getName many
// times, electrical mode only.  This should be enabled locally only,
// since the cache will go stale when an assigned name is changed,
// added, or deleted.

struct CDnameCache
{
    CDnameCache()
        {
            nc_celltab = new SymTab(false, false);
            nc_hprev = 0;
        }

    ~CDnameCache();

    void add(const CDs*, const char*);
    bool in_cache(const CDs*, const char*);

private:
    SymTab *nc_celltab;
    SymTabEnt *nc_hprev;
    eltab_t<sl_t> nc_eltab;
    stbuf_t nc_stbuf;
};


CDnameCache::~CDnameCache()
{
    SymTabGen gen(nc_celltab, true);
    SymTabEnt *h;
    while ((h = gen.next()) != 0) {
        delete (table_t<sl_t>*)h->stData;
        delete h;
    }
    delete nc_celltab;
}


void
CDnameCache::add(const CDs *sd, const char *name)
{
    if (!sd || !name || !*name)
        return;
    SymTabEnt *h = SymTab::get_ent(nc_celltab, (uintptr_t)sd);
    if (!h) {
        nc_celltab->add((uintptr_t)sd, 0, false);
        h = SymTab::get_ent(nc_celltab, (uintptr_t)sd);
        h->stData = new table_t<sl_t>;
    }
    table_t<sl_t> *tab = (table_t<sl_t>*)h->stData;
    sl_t *st = nc_eltab.new_element();
    st->set_tab_name(nc_stbuf.new_string(name));
    st->set_tab_next(0);
    tab->link(st);
    h->stData = tab->check_rehash();
};


bool
CDnameCache::in_cache(const CDs *sd, const char *name)
{
    // If no name, check if cell is in table.
    if (!name || !*name)
        return (SymTab::get_ent(nc_celltab, (uintptr_t)sd) != 0);

    if (!nc_hprev || (CDs*)nc_hprev->stTag != sd) {
        nc_hprev = SymTab::get_ent(nc_celltab, (uintptr_t)sd);
        // If cell not in table, too bad.
        if (!nc_hprev)
            return (false);
    }
    table_t<sl_t> *tab = (table_t<sl_t>*)nc_hprev->stData;
    return (tab && tab->find(name));
}
// End of CDnameCache functions.


// Set or clear the "use cache" flag.  When clearing, the present
// cache is destroyed.  The previous state is returned.
//
bool
cCD::EnableNameCache(bool b)
{
    bool tmp = cdUseNameCache;
    cdUseNameCache = b;
    if (!b) {
        delete cdNameCache;
        cdNameCache = 0;
    }
    return (tmp);
}


// Return the name cache, allocate if necessary.
//
CDnameCache *
cCD::GetNameCache(const CDs *parent)
{
    if (cdUseNameCache) {
        if (!cdNameCache)
            cdNameCache = new CDnameCache;
        if (parent && !cdNameCache->in_cache(parent, 0)) {
            CDm_gen mgen(parent, GEN_MASTERS);
            for (CDm *m = mgen.m_first(); m; m = mgen.m_next()) {
                if (!m->celldesc())
                    continue;
                CDc_gen cgen(m);
                for (CDc *c = cgen.c_first(); c; c = cgen.c_next()) {
                    CDp_cname *pnx = (CDp_cname*)c->prpty(P_NAME);
                    if (pnx && pnx->assigned_name())
                        cdNameCache->add(parent, pnx->assigned_name());
                }
            }
        }
        return (cdNameCache);
    }
    return (0);
}
// End of cCD functions.


// Destruction, called from RTelem destructor.
//
void
CDc::cleanup()
{
    unlinkFromMaster(false);
    if (cMaster && !cMaster->hasUnlinked() && !cMaster->hasInstances()) {
        // This is the last of the linking objects.
        cMaster->unlink();  // unlink from parent
        if (cMaster->celldesc())
            cMaster->unlinkRef();
        delete cMaster;
        cMaster = 0;
    }
}


// Initialize a CallDesc.  Note that the CallDesc CDs is never a
// symbolic representation cell.
//
void
CDc::call(CallDesc *calldesc) const
{
    calldesc->setName(cellname());
    calldesc->setCelldesc(masterCell(true));
}


// Set the instance transform from the passed matrix.  This should
// only be called on cells that are not in the database.
//
void
CDc::setTransform(const CDtf *tf, const CDap *ap)
{
    tf->txty(&cX, &cY);

    CDattr at(this, ap);
    int a, b, c, d;
    tf->abcd(&a, &b, &c, &d);
    at.refly = ((a && (a == -d)) || (b && (b == c)));
    at.ax = (a > 0 ? 1 : (a < 0 ? -1 : 0));
    at.ay = (c > 0 ? 1 : (c < 0 ? -1 : 0));
    at.magn = tf->mag();
    cAttr = CD()->RecordAttr(&at);
}


// Add the structure properties to the instance.  If keepex is true,
// retain any non-null values of the model/value/param properties.
//
void
CDc::prptyAddStruct(bool keepex)
{
    CDs *sdesc = masterCell();
    if (!sdesc || !sdesc->isElectrical())
        return;

    // Make sure that the master (and instance) have a P_NAME property.
    if (!sdesc->prpty(P_NAME)) {
        int cnt = 0;
        CDp_snode *p = (CDp_snode*)sdesc->prpty(P_NODE);
        if (p) {
            cnt++;
            p = p->next();
            if (p)
                cnt++;
        }
        if (cnt != 1) {
            // If there is exactly one node, the obejct is probably a
            // ground terminal, which does not have a name property.

            CDp_sname *pname = new CDp_sname;
            if (cnt == 0)
                pname->set_name_string(P_NAME_NULL_STR);
            else
                pname->set_name_string(P_NAME_SUBC_STR);
            pname->set_next_prp(sdesc->prptyList());
            sdesc->setPrptyList(pname);
        }
    }

    // Copy inherited properties from master.
    for (CDp *pd = sdesc->prptyList(); pd; pd = pd->next_prp()) {
        CDp *pn = 0;
        switch (pd->value()) {
        case P_BNODE:
            pn = ((CDp_bsnode*)pd)->dupc(sdesc, this);
            break;
        case P_NODE:
            pn = ((CDp_snode*)pd)->dupc(sdesc, this);
            break;
        case P_NAME:
            prptyRemove(pd->value());
            pn = new CDp_cname(*(CDp_sname*)pd);
            break;
        case P_NOPHYS:
            prptyRemove(pd->value());
            pn = pd->dup();
            break;
        case P_BRANCH:
            if (sdesc->isDevice()) {
                prptyRemove(pd->value());
                pn = pd->dup();
            }
            break;
        case P_MODEL:
        case P_VALUE:
        case P_PARAM:
            if (sdesc->isDevice()) {
                if (!keepex) {
                    prptyRemove(pd->value());
                    pn = pd->dup();
                }
                else if (!prpty(pd->value()))
                    pn = pd->dup();
            }
            break;
        }
        if (pn)
            link_prpty_list(pn);
    }

    // Transform the list to instance coordinates.
    cTfmStack stk;
    stk.TPush();
    stk.TApplyTransform(this);
    for (CDp *pd = prpty_list(); pd; pd = pd->next_prp())
        pd->transform(&stk);
    stk.TPop();
}


#ifdef CHECK_CELLTYPE
namespace {
    // Check consistency with master, this should be temporary.
    void check_celltype(CDs *sd, CDelecCellType etype)
    {
        if (sd) {
            CDelecCellType tp = sd->elecCellType();
            if (etype != tp) {
                CDelecCellTypeName ncell(tp);
                CDelecCellTypeName ninst(etype);
                printf(
                    "WARNING: master and instance of %s have different cell\n"
                    "types \"%s\" and \"%s\".\n",  Tstring(sd->cellname()),
                    ncell.name(), ninst.name());
            }
        }
    }
}
#endif

// Return the type of the cell for electrical mode, based on instance
// properties.
//
CDelecCellType
CDc::elecCellType(const char **nret)
{
    if (nret)
        *nret = 0;

    // Check the master for display view only, not an error if not
    // found.
    CDs *sd = masterCell();
    if (sd && !sd->isElectrical())
        return (CDelecBad);

    CDp_cname *pa = (CDp_cname*)prpty(P_NAME);
    if (!pa) {
        // A cell without a name property and with exactly one node
        // property is a "gnd" device.

        CDp_node *pn = (CDp_node*)prpty(P_NODE);
        if (pn && !pn->next()) {
#ifdef CHECK_CELLTYPE
            check_celltype(sd, CDelecGnd);
#endif
            return (CDelecGnd);
        }
        if (sd) {
            // Master has a name but not the instance, shouldn't
            // happen, create an instance name property.

            CDp_sname *ps = (CDp_sname*)sd->prpty(P_NAME);
            if (ps) {
                pa = new CDp_cname(*ps);
                link_prpty_list(pa);
            }
        }
        if (!pa) {
#ifdef CHECK_CELLTYPE
            check_celltype(sd, CDelecNull);
#endif
            return (CDelecNull);
        }
    }

    CDelecCellType etype = CDelecBad;
    int key = pa->key();
    if (!key) {
        // "can't happen"
        pa->set_name_string(P_NAME_NULL_STR);
    }
    if (nret)
        *nret = Tstring(pa->name_string());

    if (key == P_NAME_NULL) {
        // A "null" device is for decoration only, it is not
        // electrically active, and should not have node properties.

        etype = CDelecNull;
    }
    else if (key == P_NAME_TERM) {
        // A terminal.

        etype = CDelecTerm;
    }
    else if (key == P_NAME_BTERM_DEPREC) {
        // Obsolete prefix, update.

        pa->set_name_string(P_NAME_TERM_STR);
        etype = CDelecTerm;
    }
    else if (isalpha(key)) {
        // A regular device, the key is the conventional SPICE key
        // character.

        // If a device is keyed by 'X', it is either a subcircuit
        // or a macro call to the model library.  The name of the
        // subcircuit macro is stored in a model property, so that
        // the text will be added along with the model text.  The
        // subckt macros are saved in the model database.

        if (key == P_NAME_SUBC) {
            if (pa->is_macro())
                etype = CDelecMacro;
            else
                etype = CDelecSubc;
        }
        else
            etype = CDelecDev;
    }
    else {
        // Name not recognized, null device.

        etype = CDelecNull;
    }
#ifdef CHECK_CELLTYPE
    check_celltype(sd, etype);
#endif
    return (etype);
}


// Return the base (not indexed) instance name which is unique among
// 'setnames'.  The argument is simply to avoid a property lookup if
// the caller already has the name property.
//
const char *
CDc::getElecInstBaseName(const CDp_cname *pn) const
{
    CDs *sd = masterCell(true);
    if (!sd || !sd->isElectrical())
        return (0);

    if (!pn)
        pn = (CDp_cname*)prpty(P_NAME);
    if (pn) {
        if (pn->assigned_name())
            return (pn->assigned_name());

        if (pn->name_string()) {
            char buf[256];
            char *s = lstring::stpcpy(buf, Tstring(pn->name_string()));
            s = mmItoA(s, pn->number());
            if (!nameOK(buf)) {
                // there is a clash
                *s++ = '_';
                for (int i = 0; i < 10000; i++) {
                    mmItoA(s, i);
                    if (nameOK(buf))
                        break;
                }
            }
            return (Tstring(CD()->InstNameTableAdd(buf)));
        }
    }
    return ("_UNKNOWN_");
}


// Similar to above, however if the instance is vectorized, tack on
// the subscript, the ix is the bit index and not the range subscript
// value.  The return is a copy and should be deleted.
//
char *
CDc::getElecInstName(unsigned int ix) const
{
    CDs *sd = masterCell(true);
    if (!sd || !sd->isElectrical())
        return (0);

    CDp_range *pr = (CDp_range*)prpty(P_RANGE);
    char buf[256];
    CDp_cname *pn = (CDp_cname*)prpty(P_NAME);
    if (pn && pn->assigned_name()) {
        if (!pr)
            return (lstring::copy(pn->assigned_name()));
        strcpy(buf, pn->assigned_name());
    }
    else if (pn && pn->name_string()) {
        char *s = lstring::stpcpy(buf, Tstring(pn->name_string()));
        s = mmItoA(s, pn->number());
        if (!nameOK(buf)) {
            // there is a clash
            *s++ = '_';
            for (int i = 0; i < 10000; i++) {
                mmItoA(s, i);
                if (nameOK(buf))
                    break;
            }
        }
        CD()->InstNameTableAdd(buf);
        if (!pr)
            return (lstring::copy(buf));
    }
    else {
        if (!pr)
            return (lstring::copy("_UNKNOWN_"));
        strcpy(buf, "_UNKNOWN_");
    }
    int n;
    if (pr->beg_range() > pr->end_range())
        n = pr->beg_range() - ix;
    else
        n = pr->beg_range() + ix;
    sprintf(buf + strlen(buf), "%c%d%c", cTnameTab::subscr_open(), n,
        cTnameTab::subscr_close());
    return (lstring::copy(buf));
}


// Return a name for the physical instance, will update the instance
// index numbers if necessary.
//
char *
CDc::getPhysInstName() const
{
    CDm *md = master();
    if (!md || !md->parent() || !md->celldesc())
        return (0);
    CDs *sd = md->celldesc();
    if (sd->isElectrical())
        return (0);

    // Update the instance numbers if necessary.
    if (!md->parent()->isInstNumValid())
        md->parent()->numberInstances();

    const char *nm = Tstring(sd->cellname());

    char tbf[32];
    sprintf(tbf, "%d", index());
    char *str = new char[strlen(nm) + strlen(tbf) + 2];
    sprintf(str, "%s%c%s", nm, CD_INST_NAME_SEP, tbf);
    return (str);
}


// Return false if name clashes with assigned name of another
// instance.
//
bool
CDc::nameOK(const char *name) const
{
    if (!name || !*name)
        return (false);
    if (!cMaster)
        return (true);
    CDs *prnt = cMaster->parent();
    if (!prnt)
        return (true);
    CDp_cname *pn = (CDp_cname*)prpty(P_NAME);
    if (!pn || !pn->name_string())
        return (true);

    CDnameCache *name_cache = CD()->GetNameCache(prnt);
    if (name_cache) {
        if (name_cache->in_cache(prnt, name)) {
            // Name is in use, return false if not used by this.
            if (!pn->assigned_name() || strcmp(pn->assigned_name(), name))
                return (false);
        }
    }
    else {
        CDm_gen mgen(prnt, GEN_MASTERS);
        for (CDm *m = mgen.m_first(); m; m = mgen.m_next()) {
            if (!m->celldesc())
                continue;
            CDp_sname *pna = (CDp_sname*)m->celldesc()->prpty(P_NAME);
            if (!pna || !pna->name_string() ||
                    pn->name_string() != pna->name_string())
                continue;
            CDc_gen cgen(m);
            for (CDc *c = cgen.c_first(); c; c = cgen.c_next()) {
                if (c == this)
                    continue;
                CDp_cname *pnx = (CDp_cname*)c->prpty(P_NAME);
                if (pnx && pnx->assigned_name() &&
                        !strcmp(pnx->assigned_name(), name))
                    return (false);
            }
        }
    }
    return (true);
}


// Return the electrical counterpart of the physical instance, or
// array component if this is an array.  The px,py field of the
// electrical name property falls in the physcial instance's bounding
// box, in the physical instance parent's coordinates.  The pvix
// argument returns the corresponding vector index of the electrical
// instance, which may be nonzero if the electrical instance is
// vectorized.  If this is an array, chances are we'll need to loop
// over the array elements.
//
// IMPORTANT:
// This needs to work when the association in the extraction system is
// not up to date, for example if a physical instance was just moved,
// the new instance will be unknown in the extraction system.  We can
// maintain duality when moving physical cells by moving the
// coordinates in the name properties of the electrical duals.
//
CDc *
CDc::findElecDualOfPhys(int *pvix, unsigned int ix, unsigned int iy) const
{
    if (pvix)
        *pvix = 0;
    CDs *psdesc = masterCell();
    if (!psdesc)
        return (0);
    if (psdesc->isElectrical())
        return (0);
    CDap ap(this);
    if (ix >= ap.nx || iy >= ap.ny)
        return (0);

    CDs *ppar = cMaster->parent();
    if (!ppar)
        return (0);
    CDs *epar = CDcdb()->findCell(ppar->cellname(), Electrical);
    if (!epar)
        return (0);
    CDs *esdesc = CDcdb()->findCell(psdesc->cellname(), Electrical);
    if (!esdesc)
        return (0);

    cTfmStack stk;
    stk.TPush();
    stk.TApplyTransform(this);
    int tx, ty;
    stk.TGetTrans(&tx, &ty);

    const BBox *sBB = psdesc->BB();
    CDm_gen mgen(epar, GEN_MASTERS);
    for (CDm *md = mgen.m_first(); md; md = mgen.m_next()) {
        if (md->celldesc() == esdesc) {
            CDc_gen cgen(md);
            for (CDc *c = cgen.c_first(); c; c = cgen.c_next()) {
                CDp_range *pr = (CDp_range*)c->prpty(P_RANGE);
                CDgenRange rgen(pr);
                int vec_ix = 0;
                while (rgen.next(0)) {
                    CDp_cname *pn;
                    if (vec_ix == 0)
                        pn = (CDp_cname*)c->prpty(P_NAME);
                    else
                        pn = pr->name_prp(0, vec_ix);
                    if (!pn)
                        continue;
                    Point_c px(pn->pos_x(), pn->pos_y());
                    if (px.x == -1 && px.y == -1)
                        continue;
                    xyg_t xyg(0, ap.nx - 1, 0, ap.ny - 1);
                    do {
                        stk.TTransMult(xyg.x*ap.dx, xyg.y*ap.dy);
                        BBox BB(*sBB);
                        stk.TBB(&BB, 0);
                        if (BB.intersect(&px, false)) {
                            if (pvix)
                                *pvix = vec_ix;
                            stk.TPop();
                            return (c);
                        }
                        stk.TSetTrans(tx, ty);
                    } while (xyg.advance());
                    vec_ix++;
                }
            }
            break;
        }
    }
    stk.TPop();
    return (0);
}


// Return the physical counterpart of the scalar electrical instance
// (indicated by vecif (if the electrical instance is vectorized). 
// The px,py field of the electrical name property falls in the
// physcial instance's (or array component's) bounding box, in the
// physical instance parent's coordinates.
//
CDc *
CDc::findPhysDualOfElec(int vecix, unsigned int *pix, unsigned int *piy) const
{
    if (pix)
        *pix = 0;
    if (piy)
        *piy = 0;
    CDs *msdesc = masterCell();
    if (!msdesc)
        return (0);
    if (!msdesc->isElectrical())
        return (0);

    if (msdesc->isDevice())
        return (0);

    CDp_range *pr = (CDp_range*)prpty(P_RANGE);
    CDp_cname *pn;
    if (vecix > 0 && pr)
        pn = pr->name_prp(0, vecix);
    else
        pn = (CDp_cname*)prpty(P_NAME);
    if (!pn)
        return (0);

    Point_c px(pn->pos_x(), pn->pos_y());
    if (px.x == -1 && px.y == -1)
        // not assigned
        return (0);
    CDs *epar = cMaster->parent();
    if (!epar)
        return (0);
    CDs *ppar = CDcdb()->findCell(epar->cellname(), Physical);
    if (!ppar)
        return (0);
    msdesc = CDcdb()->findCell(msdesc->cellname(), Physical);
    if (!msdesc)
        return (0);
    CDm_gen mgen(ppar, GEN_MASTERS);
    const BBox *sBB = msdesc->BB();
    for (CDm *md = mgen.m_first(); md; md = mgen.m_next()) {
        if (md->celldesc() == msdesc) {
            cTfmStack stk;
            CDc_gen cgen(md);
            for (CDc *c = cgen.c_first(); c; c = cgen.c_next()) {
                CDap ap(c);
                stk.TPush();
                stk.TApplyTransform(c);
                int tx, ty;
                stk.TGetTrans(&tx, &ty);

                for (unsigned int iy = 0; iy < ap.ny; iy++) {
                    for (unsigned int ix = 0; ix < ap.nx; ix++) {
                        stk.TTransMult(ix*ap.dx, iy*ap.dy);
                        BBox BB(*sBB);
                        stk.TBB(&BB, 0);
                        if (BB.intersect(&px, false)) {
                            if (pix)
                                *pix = ix;
                            if (piy)
                                *piy = iy;
                            stk.TPop();
                            return (c);
                        }
                        stk.TSetTrans(tx, ty);
                    }
                }
                stk.TPop();
            }
            break;
        }
    }
    return (0);
}


// Set the device name index.  The refcnt is the unique identifying
// number for the type of device, or for the subcircuit.
// This function also takes care of the label references.
//
void
CDc::updateDeviceName(unsigned int refcnt)
{
    CDp_cname *pna = (CDp_cname*)prpty(P_NAME);
    if (!pna || !pna->name_string())
        return;
    if (!refcnt || pna->number() != refcnt) {
        // Subtlty here: If this is a new copy, it will have a new name
        // property with index 0, but the associated labels will reference
        // the original index.  If refcnt is 0, the update wouldn't happen.

        pna->set_number(refcnt);
        // set the id of associated labels
        for (CDp *pd = prpty_list(); pd; pd = pd->next_prp()) {
            CDo *olabel = pd->bound();
            if (olabel) {
                CDp_lref *pl = (CDp_lref*)olabel->prpty(P_LABRF);
                if (!pl)
                    continue;
                pl->set_number(refcnt);
                if (!pl->name())
                    // should already be set
                    pl->set_name(pna->name_string());
            }
        }
    }
}


// Update the node, bnode, and branch properties.  The argument is an
// integer array that maps existing new (compact) index numbers for
// nodes in sdesc into old numbers in this.  If 0, no remapping is
// done.
//
void
CDc::updateTerminals(int *map_n)
{
    CDs *sdesc = masterCell();
    if (!sdesc || !sdesc->isElectrical())
        return;
    cTfmStack stk;
    stk.TPush();
    stk.TApplyTransform(this);

    // Just replace the branch properties.
    prptyRemove(P_BRANCH);
    CDp_branch *pb = (CDp_branch*)sdesc->prpty(P_BRANCH);
    for ( ; pb; pb = pb->next()) {
        CDp_branch *pb1 = (CDp_branch*)prptyAddCopy(pb);
        if (pb1)
            pb1->transform(&stk);
    }

    // For nodes, update the coordinates only, since we don't want
    // to disturb the physical terminal data.  If given a map, use
    // it to establish correspondence.
    //
    CDp_cnode *ptmp = 0;
    CDp_snode *pn = (CDp_snode*)sdesc->prpty(P_NODE);
    for ( ; pn; pn = pn->next()) {
        unsigned int ix = map_n ? map_n[pn->index()] : pn->index();
        // Find the corresponding node in the instance.
        CDp_cnode *pn1 = (CDp_cnode*)prpty(P_NODE);
        while (pn1 && pn1->index() != ix)
            pn1 = pn1->next();
        if (pn1) {
            CDs *sdp = sdesc->owner() ? sdesc->owner() : sdesc;
            if (sdp->symbolicRep(this)) {
                pn1->alloc_pos(pn->size_pos());
                for (unsigned int ii = 0; ; ii++) {
                    int x, y;
                    if (!pn->get_pos(ii, &x, &y))
                        break;
                    pn1->set_pos(ii, x, y);
                }
            }
            else {
                pn1->alloc_pos(0);
                int x, y;
                pn->get_schem_pos(&x, &y);
                pn1->set_pos(0, x, y);
            }
            pn1->set_index(pn->index());
            pn1->transform(&stk);
            prptyUnlink(pn1);
            pn1->set_next_prp(ptmp);
            ptmp = pn1;

            // Sync the flags, terminal type.
            if (pn->has_flag(TE_BYNAME))
                pn1->set_flag(TE_BYNAME);
            else
                pn1->unset_flag(TE_BYNAME);
            if (pn->has_flag(TE_SCINVIS))
                pn1->set_flag(TE_SCINVIS);
            else
                pn1->unset_flag(TE_SCINVIS);
            if (pn->has_flag(TE_SYINVIS))
                pn1->set_flag(TE_SYINVIS);
            else
                pn1->unset_flag(TE_SYINVIS);
            pn1->set_termtype(pn->termtype());
        }
        else {
            // Create new property, then pull it into the
            // deferred list.
            //
            pn1 = pn->dupc(sdesc, this);
            if (pn1) {
                pn1->set_next_prp(ptmp);
                ptmp = pn1;
                pn1->transform(&stk);
            }
        }
    }

    // Any node properties left in this have no correspondence
    // in the sdesc, so remove them.
    //
    prptyRemove(P_NODE);

    // Put back the changes, updating the flags and terminal type as
    // we go.
    if (ptmp) {
        CDp_cnode *pn1 = ptmp;
        while (pn1->next_prp()) {
            CDp_snode *ps = (CDp_snode*)sdesc->prpty(P_NODE);
            for ( ; ps; ps = ps->next()) {
                if (ps->index() == pn1->index())
                    break;
            }
            if (ps) {
                if (ps->has_flag(TE_BYNAME))
                    pn1->set_flag(TE_BYNAME);
                else
                    pn1->unset_flag(TE_BYNAME);
                if (ps->has_flag(TE_SCINVIS))
                    pn1->set_flag(TE_SCINVIS);
                else
                    pn1->unset_flag(TE_SCINVIS);
                if (ps->has_flag(TE_SYINVIS))
                    pn1->set_flag(TE_SYINVIS);
                else
                    pn1->unset_flag(TE_SYINVIS);
                pn1->set_termtype(ps->termtype());
            }
            pn1 = (CDp_cnode*)pn1->next_prp();
        }
        pn1->set_next_prp(prpty_list());
        set_prpty_list(ptmp);
    }

    if (!prpty(P_NAME)) {
        CDp_sname *ps = (CDp_sname*)sdesc->prpty(P_NAME);
        if (ps) {
            // Master has a name but not the instance, shouldn't
            // happen, create an instance name property.

            link_prpty_list(new CDp_cname(*ps));
        }
    }

    // Update the bus node properties.  The index is known to be
    // unique among the cell bterms, but otherwise the index number is
    // arbitrary.
    //
    CDp_bsnode *ps = (CDp_bsnode*)sdesc->prpty(P_BNODE);
    if (ps) {
        CDp_bcnode *pbtmp = 0;
        for ( ; ps; ps = ps->next()) {
            unsigned int ix = map_n ? map_n[ps->index()] : ps->index();
            // Find the corresponding bnode in the instance.
            CDp_bcnode *pc = (CDp_bcnode*)prpty(P_BNODE);
            while (pc && pc->index() != ix)
                pc = pc->next();
            if (pc) {
                pc->update_range(ps);
                CDs *sdp = sdesc->owner() ? sdesc->owner() : sdesc;
                if (sdp->symbolicRep(this)) {
                    pc->alloc_pos(ps->size_pos());
                    for (unsigned int ii = 0; ; ii++) {
                        int x, y;
                        if (!ps->get_pos(ii, &x, &y))
                            break;
                        pc->set_pos(ii, x, y);
                    }
                }
                else {
                    pc->alloc_pos(0);
                    int x, y;
                    ps->get_schem_pos(&x, &y);
                    pc->set_pos(0, x, y);
                }
                pc->transform(&stk);
                prptyUnlink(pc);
                pc->set_next_prp(pbtmp);
                pbtmp = pc;
            }
            else {
                // Create new property, then pull it into the
                // deferred list.
                //
                pc = ps->dupc(sdesc, this);
                if (pc) {
                    pc->set_next_prp(pbtmp);
                    pbtmp = pc;
                    pc->transform(&stk);
                }
            }
        }
        // Any bnode properties left in this have no correspondence in
        // the sdesc, so remove them.
        //
        prptyRemove(P_BNODE);

        // Put back the changes.
        if (pbtmp) {
            CDp_bcnode *pc = pbtmp;
            while (pc->next_prp())
                pc = (CDp_bcnode*)pc->next_prp();
            pc->set_next_prp(prpty_list());
            set_prpty_list(pbtmp);
        }
    }

    // Finally, if the instance is vectorized, check the setup.  This
    // will copy the flags and terminal type set previously to the
    // instance bits properties.

    CDp_range *pr = (CDp_range*)prpty(P_RANGE);
    if (pr)
        pr->setup(this);

    // Assume we've made changes and invalidate connectivity.  In
    // particular, calling setup on the range property blows away
    // everything.
    if (parent())
        parent()->unsetConnected();

    stk.TPop();
}


// Set the names for the node terminals, and update the name label.
//
void
CDc::updateTermNames()
{
    CDp_cname *pna = (CDp_cname*)prpty(P_NAME);
    if (!pna)
        return;
    CDs *msdesc = masterCell();
    if (!msdesc || !msdesc->isElectrical())
        return;

    // Don't do anything with terminal devices here.  The term_name
    // is set externally.
    if (msdesc->elecCellType() == CDelecTerm)
        return;

    // If the instance is vectorized, we can update the names of the
    // properties here, but we can't do setup, which would destroy the
    // connectivity.  The vector setup should have been done before.
    CDp_range *pr = (CDp_range*)prpty(P_RANGE);

    // Update the name label, this will include a range if vectorized.
    CD()->ifUpdateNameLabel(this, pna);

    const char *basename = getElecInstBaseName(pna);
    if (!basename)
        basename = "?";
    CDp_cnode *pn = (CDp_cnode*)prpty(P_NODE);
    for ( ; pn; pn = pn->next()) {
        CDp_snode *ps = (CDp_snode*)msdesc->prpty(P_NODE);
        for ( ; ps; ps = ps->next()) {
            if (ps->index() == pn->index())
                break;
        }
        // The generator will pass through once if pr is null.
        CDgenRange rgen(pr);
        unsigned int cnt = 0, n;
        while (rgen.next(&n)) {
            if (!pr) {
                sLstr lstr;
                lstr.add(basename);
                lstr.add(ps ? Tstring(ps->term_name()) : "?");
                pn->set_term_name(lstr.string());
                break;
            }
            if (!cnt) {
                sLstr lstr;
                lstr.add(basename);
                lstr.add_c(cTnameTab::subscr_open());
                lstr.add_i(n);
                lstr.add_c(cTnameTab::subscr_close());
                lstr.add(ps ? Tstring(ps->term_name()) : "?");
                pn->set_term_name(lstr.string());
            }
            else {
                CDp_cnode *px = pr->node(this, cnt, pn->index());
                if (!px)
                    continue;
                sLstr lstr;
                lstr.add(basename);
                lstr.add_c(cTnameTab::subscr_open());
                lstr.add_i(n);
                lstr.add_c(cTnameTab::subscr_close());
                lstr.add(ps ? Tstring(ps->term_name()) : "?");
                px->set_term_name(lstr.string());
            }
            cnt++;
        }
    }

    CDp_bcnode *pb = (CDp_bcnode*)prpty(P_BNODE);
    for ( ; pb; pb = pb->next()) {
        CDp_bsnode *ps = (CDp_bsnode*)msdesc->prpty(P_BNODE);
        for ( ; ps; ps = ps->next()) {
            if (ps->index() == pb->index())
                break;
        }
        if (!pr) {
            sLstr lstr;
            lstr.add(basename);
            lstr.add(ps ? Tstring(ps->term_name()) : "?");
            pb->set_term_name(lstr.string());
            break;
        }
        else {
            sLstr lstr;
            lstr.add(basename);
            lstr.add_c(cTnameTab::subscr_open());
            lstr.add_i(pr->beg_range());
            lstr.add_c(':');
            lstr.add_i(pr->end_range());
            lstr.add_c(cTnameTab::subscr_close());
            lstr.add(ps ? Tstring(ps->term_name()) : "?");
            pb->set_term_name(lstr.string());
        }
    }
}


// Update the instance BB from the master, take care of database
// update.
//
bool
CDc::updateBB()
{
    cTfmStack stk;
    stk.TPush();
    stk.TApplyTransform(this);
    BBox nBB;
    CDs *sd = masterCell();
    if (sd) {
        const BBox *BB = sd->BB();
        if (BB->left != CDinfinity)
            nBB = *BB;
    }
    CDap ap(this);
    if (ap.nx > 1) {
        if (ap.dx > 0)
            nBB.right += (ap.nx - 1)*ap.dx;
        else
            nBB.left += (ap.nx - 1)*ap.dx;
    }
    if (ap.ny > 1) {
        if (ap.dy > 0)
            nBB.top += (ap.ny - 1)*ap.dy;
        else
            nBB.bottom += (ap.ny - 1)*ap.dy;
    }
    stk.TBB(&nBB, 0);
    stk.TPop();
    if (parent() && (!in_db() || nBB != e_BB))
        return (parent()->reinsert(this, &nBB));
    return (true);
}


// Add the schematic and symbolic bounding boxes to BB, used to
// compute update area after symbolic mode change.
//
void
CDc::addSymbChangeBB(BBox *BB)
{
    if (!BB)
        return;
    CDs *sd = masterCell(true);
    if (!sd)
        return;

    cTfmStack stk;
    stk.TPush();
    stk.TApplyTransform(this);

    const BBox *rBB = sd->BB();
    if (rBB->left == CDinfinity) {
        stk.TPop();
        return;
    }
    BBox tBB(*rBB);
    CDs *tsd = sd->symbolicRep(0);
    if (tsd) {
        rBB = tsd->BB();
        if (rBB->left != CDinfinity)
            tBB.add(rBB);
    }

    CDap ap(this);
    if (ap.nx > 1) {
        if (ap.dx > 0)
            tBB.right += (ap.nx - 1)*ap.dx;
        else
            tBB.left += (ap.nx - 1)*ap.dx;
    }
    if (ap.ny > 1) {
        if (ap.dy > 0)
            tBB.top += (ap.ny - 1)*ap.dy;
        else
            tBB.bottom += (ap.ny - 1)*ap.dy;
    }
    stk.TBB(&tBB, 0);
    stk.TPop();
    BB->add(&tBB);
}
// End of CDc functions.


namespace {
    // Alphabetic in cell name, descending in top, ascending in left.
    //
    bool instcmp(const CDc *c1, const CDc *c2)
    {
        const char *s1 = Tstring(c1->cellname());
        const char *s2 = Tstring(c2->cellname());
        if (s1 == s2) {
            const BBox &b1 = c1->oBB();
            const BBox &b2 = c2->oBB();
            if (b1.top > b2.top)
                return (true);
            if (b1.top < b2.top)
                return (false);
            return (b1.left < b2.left);
        }
        return (strcmp(s1, s2) < 0);
    }
}


// Static function.
// Sort the instance list.
//
void
CDcl::sort_instances(CDcl *thiscl)
{
    int cnt = 0;
    for (CDcl *cl = thiscl; cl; cl = cl->next)
        cnt++;
    if (cnt < 2)
        return;
    const CDc **ary = new const CDc*[cnt];
    cnt = 0;
    for (CDcl *cl = thiscl; cl; cl = cl->next)
        ary[cnt++] = cl->cdesc;
    std::sort(ary, ary + cnt, instcmp);
    cnt = 0;
    for (CDcl *cl = thiscl; cl; cl = cl->next)
        cl->cdesc = ary[cnt++];
    delete [] ary;
}
// End CDcl functions


// Struct to encapsulate a text token for cell types.
//
CDelecCellTypeName::CDelecCellTypeName(CDelecCellType tp)
{
    ectn_tp = tp;
};


const char*
CDelecCellTypeName::name()
{
    switch (ectn_tp) {
    case CDelecBad:     return ("Bad");
    case CDelecNull:    return ("Null");
    case CDelecGnd:     return ("Gnd");
    case CDelecTerm:    return ("Term");
    case CDelecDev:     return ("Dev");
    case CDelecMacro:   return ("Macro");
    case CDelecSubc:    return ("Subc");
    }
    // not reached
    return (0);
}

