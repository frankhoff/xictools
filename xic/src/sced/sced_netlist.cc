
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

#include "main.h"
#include "sced.h"
#include "cd_terminal.h"
#include "cd_netname.h"
#include "cd_hypertext.h"
#include "cd_lgen.h"


bool
cSced::isCheckingSolitary()
{
    return (CDvdb()->getVariable(VA_CheckSolitary) != 0);
}


// Return a list of the P_NODE properties that are comnected to node.
//
CDpl *
cSced::getElecNodeProps(CDs *sdesc, int node)
{
    if (!sdesc || node < 1)
        return (0);
    CDpl *p0 = 0;
    CDg gdesc;
    gdesc.init_gen(sdesc, CellLayer());
    CDc *cdesc;
    while ((cdesc = (CDc*)gdesc.next()) != 0) {
        if (!cdesc->is_normal())
            continue;
        if (!includeNoPhys() && cdesc->prpty(P_NOPHYS))
            continue;
        CDp_cnode *pc = (CDp_cnode*)cdesc->prpty(P_NODE);
        for ( ; pc; pc = pc->next()) {
            if (pc->enode() == node)
                p0 = new CDpl(pc, p0);
        }
    }

    CDp_snode *ps = (CDp_snode*)sdesc->prpty(P_NODE);
    for ( ; ps; ps = ps->next()) {
        if (ps->enode() == node)
            p0 = new CDpl(ps, p0);
    }
    return (p0);
}


// Return a list of connection names for node.  This returns names for
// contacts that don't have terminals (voltage sources, etc), unlike
// the similar function in the extraction package.  The includeNoPhys()
// setting controls this.
//
stringlist *
cSced::getElecNodeContactNames(CDs *sdesc, int node)
{
    if (!sdesc || node < 1)
        return (0);

    stringlist *s0 = 0;
    CDm_gen mgen(sdesc, GEN_MASTERS);
    for (CDm *m = mgen.m_first(); m; m = mgen.m_next()) {
        if (!m->celldesc())
            continue;
        CDs *msd = m->celldesc();
        bool is_term = (msd->elecCellType() == CDelecTerm);
        CDc_gen cgen(m);
        for (CDc *cdesc = cgen.c_first(); cdesc; cdesc = cgen.c_next()) {
            if (!cdesc->is_normal())
                continue;
            if (!includeNoPhys() && cdesc->prpty(P_NOPHYS))
                continue;
            CDp_cnode *pc = (CDp_cnode*)cdesc->prpty(P_NODE);
            if (!pc)
                continue;

            if (is_term) {
                // This is a scalar terminal device.  The term_name is
                // the same as the label text, set in the connection
                // function setup.

                if (pc->enode() == node) {
                    const char *lname = Tstring(pc->get_term_name());
                    if (!lname)
                        lname = "UNNAMED";
                    char *lbl = new char[strlen(lname) + 3];
                    char *t = lbl;
                    *t++ = 'T';
                    *t++ = ' ';
                    strcpy(t, lname);
                    s0 = new stringlist(lbl, s0);
                }
                continue;
            }

            CDp_range *pr = (CDp_range*)cdesc->prpty(P_RANGE);
            for ( ; pc; pc = pc->next()) {
                CDgenRange rgen(pr);
                int cnt = 0;
                while (rgen.next(0)) {
                    CDp_cnode *px;
                    if (!cnt)
                        px = pc;
                    else
                        px = pr->node(0, cnt, pc->index());
                    cnt++;
                    if (px && px->enode() == node)
                        s0 = new stringlist(
                            lstring::copy(Tstring(px->term_name())), s0);
                }
            }
        }
    }

    // Add an indicator for wire labels in the net.
    CDsLgen gen(sdesc);
    CDl *ld;
    while ((ld = gen.next()) != 0) {
        if (!ld->isWireActive())
            continue;
        CDg gdesc;
        gdesc.init_gen(sdesc, ld);
        CDo *odesc;
        while ((odesc = gdesc.next()) != 0) {
            if (!odesc->is_normal())
                continue;
            if (odesc->type() != CDWIRE)
                continue;
            CDp_node *pn = (CDp_node*)odesc->prpty(P_NODE);
            if (!pn || (pn->enode() != node) || !pn->get_term_name())
                continue;

            const char *lname = Tstring(pn->get_term_name());
            char *lbl = new char[strlen(lname) + 3];
            char *t = lbl;
            *t++ = 'W';
            *t++ = ' ';
            strcpy(t, lname);
            s0 = new stringlist(lbl, s0);
        }
    }

    CDp_snode *ps = (CDp_snode*)sdesc->prpty(P_NODE);
    for ( ; ps; ps = ps->next()) {
        if (ps->enode() == node)
            s0 = new stringlist(lstring::copy(Tstring(ps->term_name())), s0);
    }
    stringlist::sort(s0);
    return (s0);
}


// Return an array of stringlists, each entry is a list of connection
// names for that node.
//
stringlist **
cSced::getElecContactNames(CDs *sdesc, int *size)
{
    if (!sdesc)
        return (0);

    SymTab *list_tab = new SymTab(false, false);

    int max_node = 0;
    CDg gdesc;
    gdesc.init_gen(sdesc, CellLayer());
    CDc *cdesc;
    while ((cdesc = (CDc*)gdesc.next()) != 0) {
        if (!cdesc->is_normal())
            continue;
        CDp_range *pr = (CDp_range*)cdesc->prpty(P_RANGE);
        CDgenRange rgen(pr);
        int cnt = 0;
        while (rgen.next(0)) {
            CDp_cnode *pn = (CDp_cnode*)cdesc->prpty(P_NODE);
            for ( ; pn; pn = pn->next()) {
                CDp_cnode *pn1;
                if (cnt == 0)
                    pn1 = pn;
                else
                    pn1 = pr->node(0, cnt, pn->index());

                // Skip ground and unassigned nodes.
                if (pn1->enode() < 1)
                    continue;
                if (pn1->enode() > max_node)
                    max_node = pn1->enode();

                SymTabEnt *h = SymTab::get_ent(list_tab, pn1->enode());
                if (!h) {
                    list_tab->add(pn1->enode(), 0, false);
                    h = SymTab::get_ent(list_tab, pn1->enode());
                }
                h->stData = new stringlist(
                    lstring::copy(Tstring(pn1->term_name())),
                    (stringlist*)h->stData);
            }
            cnt++;
        }
    }
    CDp_snode *pn = (CDp_snode*)sdesc->prpty(P_NODE);
    for ( ; pn; pn = pn->next()) {
        // Skip ground and unassigned nodes.
        if (pn->enode() < 1)
            continue;
        if (pn->enode() > max_node)
            max_node = pn->enode();

        SymTabEnt *h = SymTab::get_ent(list_tab, pn->enode());
        if (!h) {
            list_tab->add(pn->enode(), 0, false);
            h = SymTab::get_ent(list_tab, pn->enode());
        }
        h->stData = new stringlist(lstring::copy(Tstring(pn->term_name())),
            (stringlist*)h->stData);
    }
    stringlist **ary = new stringlist*[max_node+1];
    *size = max_node+1;
    for (int i = 0; i <= max_node; i++)
        ary[i] = 0;

    SymTabGen gen(list_tab, true);
    SymTabEnt *h;
    while ((h = gen.next()) != 0) {
        int n = (intptr_t)h->stTag;
        stringlist *sl = (stringlist*)h->stData;
        stringlist::sort(sl);
        if (n >= 0 && n <= max_node)
            // should always be true
            ary[n] = sl;
        delete h;
    }
    delete list_tab;

    return (ary);
}

