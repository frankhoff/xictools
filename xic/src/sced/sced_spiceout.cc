
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
#include "sced_spiceipc.h"
#include "sced_spiceout.h"
#include "sced_modlib.h"
#include "sced_nodemap.h"
#include "cd_propnum.h"
#include "cd_netname.h"
#include "cd_lgen.h"
#include "dsp_tkif.h"
#include "dsp_inlines.h"
#include "fio.h"
#include "promptline.h"
#include "errorlog.h"
#include "miscutil/filestat.h"
#include <algorithm>


//
//  Functions to support Spice netlist creation.
//

// Return the analysis string.  If ascii is true, return as ascii
// hypertext, otherwise as plain text.  Hypertext is needed for source
// names in the dc spec, for example.
//
char *
cSced::getAnalysis(bool ascii)
{
    if (!sc_analysis_cmd)
        return (0);
    return (hyList::string(sc_analysis_cmd, ascii ? HYcvAscii : HYcvPlain,
        false));
}


// Set the current analysis text, input is ascii format hypertext.
//
void
cSced::setAnalysis(const char *cmd)
{
    CDs *cursde = CurCell(Electrical, true);
    if (cursde) {
        if (sc_analysis_cmd)
            hyList::destroy(sc_analysis_cmd);
        sc_analysis_cmd = new hyList(cursde, cmd, HYcvAscii);
    }
}


// Return the current analysis command hypertext.
//
hyList *
cSced::getAnalysisList()
{
    return (sc_analysis_cmd);
}


// Replace the analysis hypertext.
//
hyList *
cSced::setAnalysisList(hyList *hp)
{
    hyList *t = sc_analysis_cmd;
    sc_analysis_cmd = hp;
    return (t);
}


// Generate the complete SPICE listing to the named file, or to a default
// name if filename is 0 or empty.  Used by the Dump command.
//
bool
cSced::dumpSpiceFile(const char *fnamein)
{
    if (!DSP()->CurCellName())
        return (false);
    CDs *cursde = CurCell(Electrical, true);
    if (!cursde)
        return (false);
    if (!connectAll(true))
        return (false);

    char *filename = 0;
    if (fnamein)
        filename = lstring::getqtok(&fnamein);
    char tbuf[256];
    if (!filename) {
        sprintf(tbuf, "%s", Tstring(DSP()->CurCellName()));
        if ((filename = strrchr(tbuf, '.')) != 0)
            *filename = '\0';
        strcat(tbuf, ".cir");
        filename = lstring::copy(tbuf);
    }

    if (!filestat::create_bak(filename)) {
        GRpkgIf()->ErrPrintf(ET_ERROR, "%s", filestat::error_msg());
        return (false);
    }
    FILE *fp = fopen(filename, "w");
    delete [] filename;
    if (!fp)
        return (false);

    dumpSpiceDeck(fp);
    fclose(fp);
    return (true);
}


// Dump the complete spice listing to the given file pointer.  This is
// used to append the spice listing to the end of the symbol file.
//
void
cSced::dumpSpiceDeck(FILE *fp)
{
    CDs *cursde = CurCell(Electrical, true);
    if (!cursde)
        return;
    if (!connectAll(true))
        return;
    SpOut sp(cursde);
    SpiceLine *deck = sp.makeSpiceDeck();
    if (!deck)
        return;

    // Add analysis and .plot lines.
    if (cursde->cellname() == DSP()->TopCellName()) {
        char *analysis = getAnalysis(false);
        const char *astr = analysis;
        const char *atype = 0;
        if (analysis)
            atype = lstring::gettok(&astr);
        if (atype) {
            if (!strcmp(atype, "run")) {
                // given analysis starts with "run", if we have a .plot,
                // look through the deck for an analysis string to get
                // the plot type
                char *t = getPlotCmd(false);
                // syntax: .plot analtype veclist

                if (t && *t) {
                    char *aactual = 0;
                    for (SpiceLine *dd = deck; dd; dd = dd->li_next) {
                        char *q = dd->li_line;
                        char *tok = lstring::gettok(&q);
                        if (tok && *tok == '.' &&
                                (lstring::cieq(tok, ".tran") ||
                                lstring::cieq(tok, ".dc") ||
                                lstring::cieq(tok, ".ac") ||
                                lstring::cieq(tok, ".noise") ||
                                lstring::cieq(tok, ".tf") ||
                                lstring::cieq(tok, ".pz") ||
                                lstring::cieq(tok, ".disto") ||
                                lstring::cieq(tok, ".sens") ||
                                lstring::cieq(tok, ".op"))) {
                            aactual = tok;
                            break;
                        }
                        delete [] tok;
                    }
                    if (aactual) {
                        sLstr lstr;
                        lstr.add(".plot ");
                        lstr.add(aactual+1);
                        lstr.add_c(' ');
                        lstr.add(t);
                        SpiceLine *dd = new SpiceLine(lstr.string());
                        dd->li_next = deck->li_next;
                        deck->li_next = dd;
                        delete [] aactual;
                    }
                }
                delete [] t;
            }
            else {
                char *t = getPlotCmd(false);
                // syntax: .plot analtype veclist
                if (t && *t) {
                    sLstr lstr;
                    lstr.add(".plot ");
                    lstr.add(atype);
                    lstr.add_c(' ');
                    lstr.add(t);
                    SpiceLine *dd = new SpiceLine(lstr.string());
                    dd->li_next = deck->li_next;
                    deck->li_next = dd;
                }
                sLstr lstr;
                lstr.add_c('.');
                lstr.add(atype);
                lstr.add_c(' ');
                lstr.add(astr);
                SpiceLine *dd = new SpiceLine(lstr.string());
                dd->li_next = deck->li_next;
                deck->li_next = dd;
                delete [] t;
            }
            delete [] atype;
        }
        delete [] analysis;
    }
    for (SpiceLine *d = deck; d; d = d->li_next) {
#ifdef SP_LINE_FULL
        if (d->li_actual) {
            for (SpiceLine *d1 = d->li_actual; d1; d1 = d1->li_next)
                fprintf(fp, "%s\n", d1->li_line);
        }
        else
#endif
            fprintf(fp, "%s\n", d->li_line);
    }
    SpiceLine::destroy(deck);
}


// Interface to the electrical netlisting in the extraction menu.  In
// this usage, the top-level cell will be in a .subckt block if it has
// terminals.  Also, non-physical devices will be skipped.
//
// Note that shorted NOPHYS devices are ignored in this listing.
//
SpiceLine *
cSced::makeSpiceDeck(CDs *sd, SymTab **sctab)
{
    if (!connectAll(false, sd))
        return (0);
    SpOut sp(sd);
    sp.setSkipNoPhys(true);
    return (sp.makeSpiceDeck(sctab, true));
}


// Return a SPICE listing as a stringlist, used by the Run command.
//
stringlist *
cSced::makeSpiceListing(CDs *sd)
{
    if (!connectAll(true, sd))
        return (0);
    SpOut sp(sd);
    SpiceLine *deck = sp.makeSpiceDeck(0);
    stringlist *s0 = 0, *se = 0;
    while (deck) {
#ifdef SP_LINE_FULL
        if (deck->li_actual) {
            for (SpiceLine *dd = deck->li_actual; dd; dd = dd->li_next) {
                if (!s0)
                    s0 = se = new stringlist(dd->li_line, 0);
                else {
                    se->next = new stringlist(dd->li_line, 0);
                    se = se->next;
                }
                dd->li_line = 0;
            }
        }
        else
#endif
        {
            if (!s0)
                s0 = se = new stringlist(deck->li_line, 0);
            else {
                se->next = new stringlist(deck->li_line, 0);
                se = se->next;
            }
            deck->li_line = 0;
        }
        SpiceLine *dx = deck;
        deck = deck->li_next;
        delete dx;
    }
    return (s0);
}
// End of cSced functions


namespace {
    bool check_terms(const CDs*, const CDc*, stringlist**, int);
    bool indeck(const char*, CDs*);
}

// Limit SPICE line to this many characters
#define MAXLINELEN 78


SpOut::SpOut(CDs *sd)
{
    sp_celldesc = sd;
    sp_alias_list = 0;
    sp_properties = FIO()->GetLibraryProperties(XM()->DeviceLibName());
    sp_skip_nophys = false;
    read_alias();
}


// Return a linked list of SpiceLine structures representing the circuit.
// If an address is passed, only the title line is returned, and the
// other lines can be extracted from the returned table.  Otherwise,
// the full deck is returned.
//
// If add_top_sc, the top level will be placed in a .subckt block, if
// it has terminals.
//
SpiceLine *
SpOut::makeSpiceDeck(SymTab **sctab, bool add_top_sc)
{
    if (!sp_celldesc)
        return (0);
    char buf[256];
    dspPkgIf()->SetWorking(true);
    bool cache_state = CD()->EnableNameCache(true);

    sprintf(buf, "* Generated by %s from cell %s", XM()->Product(),
        Tstring(sp_celldesc->cellname()));
    SpiceLine *d0 = new SpiceLine(buf);
    SpiceLine *d = d0;
    d->li_next = add_dot_global();
    while (d->li_next)
        d = d->li_next;
    d->li_next = add_dot_include();
    while (d->li_next)
        d = d->li_next;
    d->li_next = ckt_deck(sp_celldesc, add_top_sc);
    while (d->li_next)
        d = d->li_next;
    if (sctab)
        *sctab = subcircuits_tab(sp_celldesc);
    else {
        d->li_next = subcircuits(sp_celldesc);
        while (d->li_next)
            d = d->li_next;
    }
    if (SCD()->modlib()) {
        d->li_next = SCD()->modlib()->PrintSubckts();
        while (d->li_next)
            d = d->li_next;
        d->li_next = SCD()->modlib()->PrintModels();
        while (d->li_next)
            d = d->li_next;
    }
    d->li_next = assert_lib_properties(d0->li_next);
#ifdef SP_LINE_FULL
    int count = 0;
    for (d = d0; d; d = d->li_next)
        d->li_linenum = count++;
#endif
    if (sctab) {
        (*sctab)->add(lstring::copy(Tstring(sp_celldesc->cellname())),
            d0->li_next, false);
        d0->li_next = 0;
    }

    CD()->EnableNameCache(cache_state);
    dspPkgIf()->SetWorking(false);
    return (d0);
}


namespace {
    // The str is a single line, break it up into multiple lines each
    // shorter than maxlen and using SPICE continuation.  The str is
    // either returned or freed.  Assume that words are separated by
    // white space or commas.
    //
    char *spice_format(char *str, int maxlen)
    {
        if (!str || (int)strlen(str) <= maxlen)
            return (str);

        sLstr lstr;
        int lfix = 0;
        const char *s = str;
        while (*s) {

            // Find the last word break before maxlen.
            int last_cut_pt = -1;
            for (int i = 0; i < maxlen - lfix; i++) {
                if (!s[i]) {
                    // end of text
                    last_cut_pt = i;
                    break;
                }
                if (isspace(s[i]) || s[i] == ',')
                    last_cut_pt = i;
            }
            if (last_cut_pt < 0) {
                // Word too long!  bail out.
                lstr.add(s);
                break;
            }

            for (int i = 0; i < last_cut_pt; i++)
                lstr.add_c(*s++);
            if (!*s)
                break;
            if (*s == ',')
                lstr.add_c(*s);
            lstr.add("\n+ ");
            lfix = 2;
            s++;
        }
        delete [] str;
        return (lstr.string_clear());
    }
}


namespace {
    inline bool ncmp(const CDp_cnode *p1, const CDp_cnode *p2)
    {
        return (p2->index() > p1->index());
    }


    // Return an array containing the node properties in ascending
    // index order, and the array size, or null if no node properties. 
    // The return value is true on success.
    //
    bool setup_nodes(const CDc *cdesc, const CDp_cnode ***ptr,
        unsigned int *psz)
    {
        *ptr = 0;
        *psz = 0;
        CDp_cnode *pn = (CDp_cnode*)cdesc->prpty(P_NODE);
        if (!pn)
            return (true);
        unsigned int ix = 0, node_cnt = 0;
        for ( ; pn; pn = pn->next()) {
            if (pn->index() > ix)
                ix = pn->index();
            node_cnt++;
        }
        if (node_cnt != ix + 1) {
            // Node numbering failure.
            Errs()->add_error("index sequence error");
            return (false);
        }
        const CDp_cnode **nodes = new const CDp_cnode*[node_cnt];
        memset(nodes, 0, node_cnt*sizeof(CDp_cnode*));

        pn = (CDp_cnode*)cdesc->prpty(P_NODE);
        for ( ; pn; pn = pn->next()) {
            if (nodes[pn->index()]) {
                // error, duplicate index
                delete [] nodes;
                Errs()->add_error("duplicate index");
                return (false);
            }
            nodes[pn->index()] = pn;
        }
        std::sort(nodes, nodes + node_cnt, ncmp);
        *ptr = nodes;
        *psz = node_cnt;
        return (true);
    }
}


// Add the lines from the circuit, queue the subcircuits and models.
// Not recursive.
//
SpiceLine *
SpOut::ckt_deck(CDs *sdesc, bool add_sc)
{
    if (!sdesc || !sdesc->isElectrical())
        return (0);
    SpiceLine *d = 0, *d0 = 0;

    int tsize;
    stringlist **tnames = SCD()->getElecContactNames(sdesc, &tsize);
    if (CDvdb()->getVariable(VA_CheckSolitary) && tnames) {
        // list nodes with only one connection
        for (int i = 1; i < tsize; i++) {
            if (tnames[i] && !tnames[i]->next) {
                Log()->WarningLogV(mh::NetlistCreation,
                    "In cell %s terminal %s is solitary for node %d.",
                    Tstring(sdesc->cellname()), tnames[i]->string, i);
            }
        }
    }
    sp_nmlist_t *globs = def_node_term_list(sdesc);

    // if true, list disconnected devices/subckts
    bool list_all = (CDvdb()->getVariable(VA_SpiceListAll) != 0);

    // first add device and subcircuit calls
    CDg gdesc;
    gdesc.init_gen(sdesc, CellLayer());
    CDc *cdesc;
    while ((cdesc = (CDc*)gdesc.next()) != 0) {
        // skip terminals
        if (!isDevOrSubc(cdesc->elecCellType()))
            continue;
        if (sp_skip_nophys) {
            // Extraction system output, skip non-physical devices.
            if (cdesc->prpty(P_NOPHYS))
                continue;
        }

        // Skip unconnected devices.  If the device is vectorized,
        // skip or not based on the <0>-element connections.
        if (!list_all && !check_terms(sdesc, cdesc, tnames, tsize))
            continue;

        CDp_cname *pname = (CDp_cname*)cdesc->prpty(P_NAME);
        if (!pname)
            continue;
        bool macrocall = pname->is_subckt();
        char *device = lstring::copy(cdesc->getElecInstBaseName());
        char *subname = 0;
        if (macrocall && !pname->is_macro())
            subname = lstring::copy(Tstring(cdesc->cellname()));
        if (!device) {
            // should never happen
            delete [] subname;
            continue;
        }

        // macrocall will be true if this is an 'X' device, in which
        // case the model property gives the name of an archival
        // subcircuit.

        unsigned int node_cnt;
        const CDp_cnode **nodes;
        if (!setup_nodes(cdesc, &nodes, &node_cnt)) {
            Log()->ErrorLogV(mh::NetlistCreation,
                "Internal error:  in %s, bad node property index numbers\n"
                "in %s (%s), %s.", Tstring(sdesc->cellname()),
                device ? device : "?", Tstring(cdesc->cellname()),
                Errs()->get_error());
            delete [] device;
            delete [] subname;
            continue;
        }
        // The nodes array may be empty!

        CDp_user *pu = (CDp_user*)cdesc->prpty(P_MODEL);
        char *model = pu ? hyList::string(pu->data(), HYcvPlain, true) : 0;
        pu = (CDp_user*)cdesc->prpty(P_VALUE);
        char *value = pu ? hyList::string(pu->data(), HYcvPlain, true) : 0;
        pu = (CDp_user*)cdesc->prpty(P_PARAM);
        char *param = pu ? hyList::string(pu->data(), HYcvPlain, true) : 0;
        pu = (CDp_user*)cdesc->prpty(P_DEVREF);
        char *devref = pu ? hyList::string(pu->data(), HYcvPlain, true) : 0;

        // If a device is a macro, we add an 'X' ahead of the name so
        // that SPICE treats the line as a subcircuit call.  The
        // "model" string should be the name of a .subckt, which is
        // usually followed by parameters.

        bool pcell_macro = false;
        if (!macrocall) {
            if (pname->is_macro()) {
                pcell_macro = true;
                if (!model && !subname)
                    subname = lstring::copy(Tstring(cdesc->cellname()));
            }
        }
        if (pcell_macro) {
            // We need to add an 'X' ahead of the device name.
            sLstr tstr;
            tstr.add_c('X');
            tstr.add(device);
            delete [] device;
            device = tstr.string_trim();
        }

        if (model) {
            if (macrocall)
                SCD()->modlib()->QueueSubckt(model);
            else if (!pcell_macro)
                SCD()->modlib()->QueueModel(model, device, param);
        }

        CDp_range *pr = (CDp_range*)cdesc->prpty(P_RANGE);
        if (pr && pr->num_nodes() != node_cnt) {
            Log()->ErrorLogV(mh::NetlistCreation,
                "Internal error:  in %s, node property count differs "
                "from range\ninstance property in %s (%s).",
                Tstring(sdesc->cellname()), device ? device : "?",
                Tstring(cdesc->cellname()));
            delete [] device;
            delete [] subname;
            delete [] model;
            delete [] value;
            delete [] param;
            delete [] devref;
            delete [] nodes;
            continue;
        }

        // Loop over vector instance range.  The generator will allow
        // one pass if pr is null.
        //
        CDgenRange rgen(pr);
        unsigned int vindex, vcount = 0;
        while (rgen.next(&vindex)) {

            sLstr lstr;
            lstr.add(device);
            if (pr) {
                // Hope that this doesn't cause a name clash!
                lstr.add_c(cTnameTab::subscr_open());
                lstr.add_i(vindex);
                lstr.add_c(cTnameTab::subscr_close());
            }

            char buf[128];
            for (unsigned int i = 0; i < node_cnt; i++) {
                int node;
                if (vcount == 0)
                    node = nodes[i]->enode();
                else {
                    CDp_cnode *pc = pr->node(0, vcount, nodes[i]->index());
                    node = pc ? pc->enode() : -1;
                }
                if (node < 0)
                    // This shouldn't happen
                    strcpy(buf, " BAD_NODE");
                else if (node == 0)
                    // ground node, never mapped
                    strcpy(buf, " 0");
                else {
                    const char *gname = sp_nmlist_t::globname(globs, node);
                    if (gname)
                        sprintf(buf, " %s", gname);
                    else
                        sprintf(buf, " %s", SCD()->nodeName(sdesc, node));
                }
                lstr.add(buf);
            }
            vcount++;

            if (!macrocall)
                add_global_node(&lstr, node_cnt - 1, cdesc, globs);

            if (devref) {
                lstr.add_c(' ');
                lstr.add(devref);
            }

            if (subname) {
                lstr.add_c(' ');
                lstr.add(subname);
            }
            else if (model) {
                lstr.add_c(' ');
                lstr.add(model);
            }
            if (value) {
                if (!subname && !model) {
                    lstr.add_c(' ');
                    lstr.add(value);
                }
                else {
                    char k = *device;
                    if (isupper(k))
                        k = tolower(k);
                    if (k == 'r' || k  == 'c' || k == 'l') {
                        lstr.add_c(' ');
                        lstr.add_c(k);
                        lstr.add_c('=');
                        lstr.add(value);
                    }
                    // Else model supersedes value, value ignored.
                }
            }
            if (param) {
                lstr.add_c(' ');
                lstr.add(param);
            }

            // Convert newlines to spaces, filter out any control chars.
            char *string = lstr.string_clear();
            char *s1 = string;
            char *s2 = string;
            for ( ; *s1; s1++) {
                if (*s1 == '\n')
                    *s2++ = ' ';
                else if (*s1 >= 0x20)
                    *s2++ = *s1;
            }
            *s2 = 0;

            string = subst_alias(string);
            string = spice_format(string, MAXLINELEN);

            if (string) {
                if (!macrocall && !model && !value && !param &&
                        *string != 'v' && *string != 'V') {
                    // Warn if device has no additional text, unless it is
                    // a voltage source (which can be a "current meter").
                    Log()->WarningLogV(mh::NetlistCreation,
                    "Warning: in %s, device with no model or value:\n\t%s\n",
                        Tstring(sdesc->cellname()), string);
                }
                if (d0 == 0)
                    d = d0 = new SpiceLine(string);
                else {
                    d->li_next = new SpiceLine(string);
                    d = d->li_next;
                }
                delete [] string;
            }
        }
        delete [] device;
        delete [] subname;
        delete [] model;
        delete [] value;
        delete [] param;
        delete [] devref;
        delete [] nodes;
    }

    SpiceLine *d1 = add_mutual(sdesc, list_all, tnames, tsize);
    if (d0 == 0)
        d = d0 = d1;
    else {
        d->li_next = d1;
        for (; d->li_next; d = d->li_next) ;
    }

    sp_nmlist_t::destroy(globs);
    for (int i = 0; i < tsize; i++)
        stringlist::destroy(tnames[i]);
    delete [] tnames;

    // Sort the element cards, and check for duplicate tokens.
    spice_deck_sort(d0);
    check_dups(d0, sdesc);

    // Add text from labels.
    d1 = get_sptext_labels(sdesc);
    if (d1) {
        SpiceLine *tmpd = d1;
        while (d1->li_next)
            d1 = d1->li_next;
        d1->li_next = d0;
        d0 = tmpd;
    }

    // Maybe add a .subckt header.  This should only be done here at the
    // top level, otherwise this produces duplicate lines.  This should
    // not be done if the SPICE output is to be directly simulated.

    if (add_sc) {
        SpiceLine *scl = subckt_line(sdesc);
        if (scl) {
            SpiceLine *tmpd = scl;
            while (tmpd->li_next)
                tmpd = tmpd->li_next;
            tmpd->li_next = d0;
            d0 = scl;
            while (tmpd->li_next)
                tmpd = tmpd->li_next;
            sLstr lstr;
            lstr.add(".ends ");
            lstr.add(Tstring(sdesc->cellname()));
            tmpd->li_next = new SpiceLine(lstr.string());
        }
    }
    return (d0);
}


namespace {
    // Return true if sdesc or any subcell connects to a global net.
    //
    bool has_global(const CDs *sdesc)
    {
        if (sdesc->owner())
            return (has_global(sdesc->owner()));
        if (sdesc->nodes() && sdesc->nodes()->hasGlobal())
            return (true);
        CDm_gen mgen(sdesc, GEN_MASTERS);
        for (CDm *mdesc = mgen.m_first(); mdesc; mdesc = mgen.m_next()) {
            CDs *msdesc = mdesc->celldesc();
            if (!msdesc)
                continue;
            if (has_global(msdesc))
                return (true);
        }
        return (false);
    }


    // Return true if:
    //  - the device/subckt has a range property (is vectorized)
    //  - the device/subckt has a global node
    //  - the device/subckt has two or more (non-ground) connections
    //  - the device/subckt has a connection and a ground
    //  - the device/subckt has a connection and no opens
    //  - the subckt has a connection
    //
    bool check_terms(const CDs *sdesc, const CDc *cdesc, stringlist **tlist,
        int size)
    {
        if (cdesc->prpty(P_RANGE))
            return (true);
        CDp_node *pn = (CDp_node*)cdesc->prpty(P_NODE);
        CDp_cname *pna = (CDp_cname*)cdesc->prpty(P_NAME);
        int num_gnd = 0;
        int num_open = 0;
        int num_ok = 0;
        for ( ; pn; pn = pn->next()) {
            int n = pn->enode();
            if (n == 0)
                num_gnd++;
            else if (n >= size) {
                const char *instname = cdesc->getElecInstBaseName(pna);
                Log()->ErrorLogV(mh::NetlistCreation,
                    "Internal error:  in %s, for %s (%s), node number %d "
                    "too large",
                    Tstring(sdesc->cellname()), instname,
                    Tstring(cdesc->cellname()), n);
                return (false);
            }
            else if (n < 0) {
                const char *instname = cdesc->getElecInstBaseName(pna);
                Log()->ErrorLogV(mh::NetlistCreation,
                    "Internal error:  in %s, for %s (%s), node number %d "
                    "negative", Tstring(sdesc->cellname()), instname,
                    Tstring(cdesc->cellname()), n);
                return (false);
            }
            else if (!tlist[n]) {
                const char *instname = cdesc->getElecInstBaseName(pna);
                Log()->ErrorLogV(mh::NetlistCreation,
                    "Internal error:  in %s, for %s (%s), no terminal for "
                    "node %d", Tstring(sdesc->cellname()), instname,
                    Tstring(cdesc->cellname()), n);
            }
            else if (!tlist[n]->next)
                num_open++;
            else
                num_ok++;
        }
        if (num_ok > 1)
            return (true);
        if (num_ok == 1 &&
                (num_gnd > 0 || num_open == 0 || !cdesc->isDevice()))
            return (true);

        // If the device/subckt has a global net connection, keep it. 
        // Note that this includes the case where there are no node
        // properties at all, and connection is by global nets, for
        // example the cell might contain decoupling caps.

        CDs *msdesc = cdesc->masterCell();
        return (msdesc && has_global(msdesc));
    }
}


namespace {
    inline bool
    slmatch(stringlist *sl, const char *n)
    {
        while (sl) {
            if (!strcmp(n, sl->string))
                return (true);
            sl = sl->next;
        }
        return (false);
    }
}


// Return a list of nodes connected to terminals that have labels which
// match a default node name from the DefaultNode properties.
//
SpOut::sp_nmlist_t *
SpOut::def_node_term_list(CDs *sdesc)
{
    stringlist *defnames = list_def_node_names();
    if (!defnames)
        return (0);
    sp_nmlist_t *nm0 = 0;
    CDm_gen mgen(sdesc, GEN_MASTERS);
    for (CDm *m = mgen.m_first(); m; m = mgen.m_next()) {
        CDs *msdesc = m->celldesc();
        if (!msdesc)
            continue;
        CDelecCellType tp = msdesc->elecCellType();
        if (tp == CDelecTerm) {
            // terminal
            CDc_gen cgen(m);
            for (CDc *c = cgen.c_first(); c; c = cgen.c_next()) {
                CDp_cname *pna = (CDp_cname*)c->prpty(P_NAME);
                if (!pna)
                    continue;
                CDla *olabel = pna->bound();
                if (!olabel)
                    continue;
                char *label = hyList::string(olabel->label(), HYcvPlain,
                    false);
                if (slmatch(defnames, label)) {
                    CDp_cnode *pn = (CDp_cnode*)c->prpty(P_NODE);
                    if (pn)
                        nm0 = new sp_nmlist_t(pn->enode(), label, nm0);
                    else
                        delete [] label;
                }
                else
                    delete [] label;
            }
        }
    }
    stringlist::destroy(defnames);
    return (nm0);
}


// Return a list of the global node names found in DefaultNode
// properties.
//
stringlist *
SpOut::list_def_node_names()
{
    stringlist *s0 = 0;
    for (const stringlist *wl = sp_properties; wl; wl = wl->next) {
        char *s = wl->string;
        char *ntok = lstring::gettok(&s);
        if (!ntok)
            continue;
        if (!strcasecmp(ntok, LpDefaultNodeStr) ||
                LpDefaultNodeVal == atoi(ntok)) {
            lstring::advtok(&s);  // skip name
            lstring::advtok(&s);  // skip num
            char *tok = lstring::gettok(&s);
            if (tok)
                s0 = new stringlist(tok, s0);
        }
        delete [] ntok;
    }
    return (s0);
}


// Return the SPICE lines for the mutual inductors.
//
SpiceLine *
SpOut::add_mutual(CDs *sdesc, bool list_all, stringlist **tnames, int tsize)
{
    SpiceLine *d = 0, *d0 = 0;
    int count = 0;
    CDp *pdesc = sdesc->prptyList();
    for (; pdesc; pdesc = pdesc->next_prp()) {
        CDc *odesc1, *odesc2;
        int l1x, l2x, l1y, l2y;
        int refcnt;
        if (pdesc->value() == P_MUT) {
            ((CDp_mut*)pdesc)->get_coords(&l1x, &l1y, &l2x, &l2y);
            odesc1 = CDp_mut::find(l1x, l1y, sdesc);
            odesc2 = CDp_mut::find(l2x, l2y, sdesc);
            if (odesc1 == 0 || odesc2 == 0)
                continue;
            refcnt = count;
        }
        else if (pdesc->value() == P_NEWMUT) {
            if (!((CDp_nmut*)pdesc)->get_descs(&odesc1, &odesc2))
                continue;
            refcnt = ((CDp_nmut*)pdesc)->index();
        }
        else
            continue;
        if (list_all || (check_terms(sdesc, odesc1, tnames, tsize) &&
                check_terms(sdesc, odesc2, tnames, tsize))) {
            // Only write mutuals if both inductors are written.

            char tbuf[256];
            const char *name1 = odesc1->getElecInstBaseName();
            const char *name2 = odesc2->getElecInstBaseName();
            count++;
            if (pdesc->value() == P_NEWMUT) {
                if (((CDp_nmut*)pdesc)->assigned_name())
                    sprintf(tbuf, "%s %s %s %s",
                        ((CDp_nmut*)pdesc)->assigned_name(),
                        name1, name2, ((CDp_nmut*)pdesc)->coeff_str());
                else
                    sprintf(tbuf, "%s%d %s %s %s", MUT_CODE, refcnt, name1,
                        name2, ((CDp_nmut*)pdesc)->coeff_str());
            }
            else
                sprintf(tbuf, "%s%d %s %s %g", MUT_CODE, refcnt, name1,
                    name2, ((CDp_mut*)pdesc)->coeff());
            if (d0 == 0)
                d = d0 = new SpiceLine(tbuf);
            else {
                d->li_next = new SpiceLine(tbuf);
                d = d->li_next;
            }
        }
    }
    return (d0);
}


// Return a list of .save lines to add to the deck.
//
SpiceLine *
SpOut::add_dot_save(const char *key, char **params)
{
    sp_dsave_t ds;
    ds.process(sp_celldesc, key);
    SpiceLine *l0 = ds.lines();
    char buf[256];
    SpiceLine *d0 = 0, *d = 0;
    for (SpiceLine *l = l0; l; l = l->li_next) {
        strcpy(buf, ".save @");
        char *t = buf + 7;
        char *s = l->li_line;
        while (*s && !isspace(*s))
            *t++ = *s++;
        *t++ = '[';
        s = t;
        for (char **p = params; *p; p++) {
            t = s;
            strcpy(t, *p);
            while (*t)
                t++;
            *t++ = ']';
            *t = '\0';
            if (!d0)
                d = d0 = new SpiceLine(buf);
            else {
                d->li_next = new SpiceLine(buf);
                d = d->li_next;
            }
        }
    }
    return (d0);
}


// Return a ".global xxx" line for global nodes found in the
// hierarchy.  There are nodes that have been assigned a name ending
// with a '!' character, or otherwise set as global, such as names
// that match a DefaultNode name.
//
SpiceLine *
SpOut::add_dot_global()
{
    sLstr lstr;
    lstr.add(".global");
    bool found = false;
    SymTab *gtab = SCD()->tabGlobals(sp_celldesc);
    for (const stringlist *wl = sp_properties; wl; wl = wl->next) {
        char *s = wl->string;
        char *ntok = lstring::gettok(&s);
        if (!ntok)
            continue;
        if (!strcasecmp(ntok, LpDefaultNodeStr) ||
                LpDefaultNodeVal == atoi(ntok)) {
            char *name = lstring::gettok(&s);
            lstring::advtok(&s);
            const char *val = lstring::gettok(&s);
            if (val) {
                // Don't list here if already in the table.
                if (!gtab) {
                    if (indeck(name, sp_celldesc)) {
                        lstr.add_c(' ');
                        lstr.add(val);
                        found = true;
                    }
                }
                else {
                    CDnetName nm = CDnetex::name_tab_add(val);
                    if (SymTab::get(gtab, (uintptr_t)nm) == ST_NIL) {
                        if (indeck(name, sp_celldesc)) {
                            lstr.add_c(' ');
                            lstr.add(val);
                            found = true;
                        }
                    }
                }
                delete [] val;
            }
            delete [] name;
        }
        delete [] ntok;
    }

    if (gtab && gtab->allocated()) {
        SymTabGen gen(gtab);
        SymTabEnt *ent;
        while ((ent = gen.next()) != 0) {
            lstr.add_c(' ');
            lstr.add(ent->stTag);
            found = true;
        }
    }
    delete gtab;
    if (found)
        return (new SpiceLine(lstr.string()));
    return (0);
}


SpiceLine *
SpOut::add_dot_include()
{
    // Look for global include addition.  If the file is found, add
    // the contents.  If not found, add a .include line for
    // resolution at run time.
    //
    const char *incl = CDvdb()->getVariable(VA_SpiceInclude);
    if (incl && *incl) {
        SpiceLine *d0 = 0, *de = 0;
        FILE *ip = fopen(incl, "r");
        sLstr lstr;
        if (ip) {
            char c;
            while ((c = fgetc(ip)) != EOF) {
                if (c == '\n') {
                    if (lstr.string()) {
                        SpiceLine *dd = new SpiceLine(lstr.string());
                        if (!d0)
                            d0 = de = dd;
                        else {
                            de->li_next = dd;
                            de = de->li_next;
                        }
                        lstr.free();
                    }
                }
                else
                    lstr.add_c(c);
            }
            if (lstr.string()) {
                SpiceLine *dd = new SpiceLine(lstr.string());
                if (!d0)
                    d0 = de = dd;
                else {
                    de->li_next = dd;
                    de = de->li_next;
                }
            }
            fclose(ip);
        }
        else {
            lstr.add(".include ");
            lstr.add(incl);
            d0 = new SpiceLine(lstr.string());
        }
        return  (d0);
    }
    return (0);
}


namespace {
    // Return true if name is a visible subcircuit of sdesc.
    //
    bool
    indeck(const char *name, CDs *sdesc)
    {
        if (sdesc->isDevice() || sdesc->prpty(P_VIRTUAL))
            return (false);
        if (sdesc->findMaster(CD()->CellNameTableFind(name)))
            return (true);
        CDm_gen mgen(sdesc, GEN_MASTERS);
        for (CDm *m = mgen.m_first(); m; m = mgen.m_next()) {
            if (m->celldesc() && indeck(name, m->celldesc()))
                return (true);
        }
        return (false);
    }
}


// Return a list of lines generated in accordance with the "library
// properties" for inclusion in the deck.
//
SpiceLine *
SpOut::assert_lib_properties(SpiceLine *deck)
{
    if (!deck)
        return (0);
#define MAX_TOKS 50
    char *params[MAX_TOKS];
    SpiceLine *l0 = 0;
    for (const stringlist *wl = sp_properties; wl; wl = wl->next) {
        char *s = wl->string;
        char *ntok = lstring::gettok(&s);
        if (!ntok)
            continue;
        if (!strcasecmp(ntok, LpSpiceDotSaveStr) ||
                LpSpiceDotSaveVal == atoi(ntok)) {
            char *key = lstring::gettok(&s);
            if (key) {
                SpiceLine *l = 0;
                int i = 0;
                char *t;
                while ((t = lstring::gettok(&s)) != 0) {
                    params[i++] = t;
                    if (i == MAX_TOKS)
                        break;
                }
                if (i) {
                    params[i] = 0;
                    SpiceLine *lx = add_dot_save(key, params);
                    while (i--)
                        delete [] params[i];
                    if (!l0)
                        l = l0 = lx;
                    else
                        l->li_next = lx;
                    while (l && l->li_next)
                        l = l->li_next;
                }
                delete [] key;
            }
        }
        delete [] ntok;
    }
    return (l0);
}


void
SpOut::add_global_node(sLstr *lstr, int numn, CDc *cdesc,
    const sp_nmlist_t *globs)
{
    const char *cname = Tstring(cdesc->cellname());
    for (const stringlist *wl = sp_properties; wl; wl = wl->next) {
        char *s = wl->string;
        char *ntok = lstring::gettok(&s);
        if (!ntok)
            continue;
        if (!strcasecmp(ntok, LpDefaultNodeStr) ||
                LpDefaultNodeVal == atoi(ntok)) {
            char *name = lstring::gettok(&s);
            if (name && !strcmp(cname, name)) {
                int n;
                char tbuf[128];
                if (sscanf(s, "%d %s", &n, tbuf) == 2) {
                    for (const sp_nmlist_t *nm = globs; nm; nm = nm->next) {
                        if (!strcmp(nm->name, tbuf) && nm->node == 0) {
                            strcpy(tbuf, "0");
                            break;
                        }
                    }
                    while (numn < n-1) {
                        lstr->add_c(' ');
                        lstr->add(tbuf);
                        numn++;
                    }
                }
            }
            delete [] name;
        }
        delete [] ntok;
    }
}


namespace {
    // grab and return name=val, advance pointer.
    //
    bool get_pair(const char **pstr, char **pname, char **pval)
    {
        *pname = 0;
        *pval = 0;
        char *nam = 0;
        char *val = 0;

        const char *s = *pstr;
        while (isspace(*s))
            s++;
        if (!*s)
            return (false);

        const char *eq = s;
        while (*eq && *eq != '=' && !isspace(*eq))
            eq++;
        nam = new char[eq - s + 1];
        strncpy(nam, s, eq - s);
        nam[eq - s] = 0;
        if (!isalpha(*nam)) {
            delete [] nam;
            return (false);
        }

        s = eq + 1;
        while (*s == '=' || isspace(*s))
            s++;
        const char *sep = s;
        if (*sep) {
            if (*sep == '"' || *sep == '\'') {
                char q = *sep++;
                while (*sep && (*sep != q || *(sep-1) == '\\'))
                    sep++;
            }
            while (*sep && !isspace(*sep))
                sep++;
            val = new char[sep - s + 1];
            strncpy(val, s, sep - s);
            val[sep - s] = 0;
        }
        if (!val) {
            val = new char[3];
            val[0] = '"';
            val[1] = '"';
            val[2] = 0;
        }
        *pname = nam;
        *pval = val;
        *pstr = sep;
        return (true);
    }
}


// Return the ".subckt..." line for sub.
//
SpiceLine *
SpOut::subckt_line(CDs *sub)
{
    CDp_snode **nodes;
    unsigned int nsize = sub->checkTerminals(&nodes);

    sLstr lstr;
    lstr.add(".subckt ");
    lstr.add(Tstring(sub->cellname()));
    int linelen = strlen(lstr.string());
    char tbuf[128];

    int ncnt = 0;
    for (unsigned int i = 0; i < nsize; i++) {
        if (!nodes[i])
            continue;
        tbuf[0] = ' ';
        strcpy(tbuf+1, SCD()->nodeName(sub, nodes[i]->enode()));
        int tlen = strlen(tbuf);
        if (linelen + tlen > MAXLINELEN) {
            lstr.add_c('\n');
            lstr.add_c('+');
            lstr.add(tbuf);
            linelen = tlen + 1;
        }
        else {
            lstr.add(tbuf);
            linelen += tlen;
        }
        ncnt++;
    }
    delete [] nodes;

    CDp *prp = sub->prpty(P_PARAM);
    if (prp) {
        char *str = hyList::string(((CDp_user*)prp)->data(), HYcvPlain, false);
        const char *s = str;
        char *name, *value;
        while (get_pair(&s, &name, &value) != 0) {
            int prmlen = strlen(name) + strlen(value) + 1;
            if (linelen + prmlen > MAXLINELEN) {
                lstr.add_c('\n');
                lstr.add_c('+');
                lstr.add(name);
                lstr.add_c('=');
                lstr.add(value);
                linelen = prmlen + 1;
            }
            else {
                lstr.add_c(' ');
                lstr.add(name);
                lstr.add_c('=');
                lstr.add(value);
                linelen += prmlen + 1;
            }
        }
        delete [] str;
    }
    SpiceLine *line = new SpiceLine(lstr.string());
    return (line);
}


// Add the subcircuit text for the subcircuits under sdesc.  Each
// subcircuit is listed once, there is no hierarchy.
//
SpiceLine *
SpOut::subcircuits(CDs *sdesc)
{
    SpiceLine *d0 = 0, *d = 0;

    SymTab *subctab = new SymTab(false, false);
    subc_list(sdesc, subctab);

    SymTabGen gen(subctab);
    SymTabEnt *h;
    while ((h = gen.next()) != 0) {

        CDs *sub = (CDs*)h->stData;
        SpiceLine *scl = subckt_line(sub);
        if (!scl)
            continue;

        if (d0 == 0)
            d = d0 = scl;
        else {
            d->li_next = scl;
            d = d->li_next;
        }
        while (d->li_next)
            d = d->li_next;

        d->li_next = ckt_deck(sub);
        while (d->li_next)
            d = d->li_next;
        sLstr lstr;
        lstr.add(".ends ");
        lstr.add(h->stTag);
        d->li_next = new SpiceLine(lstr.string());
        d = d->li_next;
    }
    delete subctab;
    return (d0);
}


// Return a symbol table containing the lines for all of the subcircuits
// in the hierarchy.
//
SymTab *
SpOut::subcircuits_tab(CDs *sdesc)
{
    SymTab *subctab = new SymTab(false, false);
    subc_list(sdesc, subctab);

    SymTab *otab = new SymTab(true, false);

    SymTabGen gen(subctab);
    SymTabEnt *h;
    while ((h = gen.next()) != 0) {
        SpiceLine *d0 = 0, *d = 0;

        CDs *sub = (CDs*)h->stData;
        SpiceLine *scl = subckt_line(sub);
        if (!scl)
            continue;
        d = d0 = scl;
        while (d->li_next)
            d = d->li_next;
        d->li_next = ckt_deck(sub);
        while (d->li_next)
            d = d->li_next;
        sLstr lstr;
        lstr.add(".ends ");
        lstr.add(h->stTag);
        d->li_next = new SpiceLine(lstr.string());
        otab->add(lstring::copy(Tstring(sub->cellname())), d0, false);
    }
    delete subctab;
    return (otab);
}


// Recursively descend through the hierarchy, adding subcircuits to
// the symbol table.
//
void
SpOut::subc_list(CDs *sdesc, SymTab *tab)
{
    CDm_gen mgen(sdesc, GEN_MASTERS);
    for (CDm *mdesc = mgen.m_first(); mdesc; mdesc = mgen.m_next()) {
        if (!mdesc->hasInstances())
            continue;
        CDs *msdesc = mdesc->celldesc();
        if (!msdesc)
            continue;
        if (msdesc->isDevice() || msdesc->prpty(P_VIRTUAL))
            continue;
        CDelecCellType tp = msdesc->elecCellType();
        if (tp == CDelecSubc) {
            tab->add(Tstring(mdesc->cellname()), msdesc, true);
            subc_list(msdesc, tab);
        }
    }
}


namespace {
    // Sort function for spicetext lines.
    //
    inline bool
    complines(const SpOut::sp_stl_t &ss, const SpOut::sp_stl_t &tt)
    {
        return (ss.n < tt.n);
    }
}


// Return a sorted list of the "active" labels.  This includes labels
// on the SPTX layer, and labels on any layer that start with
// "spicetext" with an optional integer after "spicetext" for
// sorting.
//
SpiceLine *
SpOut::get_sptext_labels(CDs *sdesc)
{
    SpiceLine *dl = 0, *dl0 = 0;

    // First, list the labels from the SPTX layer, if any.  These are sorted
    // in database order (by position, top to bottom, left to right).

    CDl *ldesc = CDldb()->findLayer("SPTX", Electrical);
    if (ldesc) {
        CDg gdesc;
        gdesc.init_gen(sdesc, ldesc);
        CDo *odesc;
        while ((odesc = gdesc.next()) != 0) {
            if (odesc->type() != CDLABEL)
                continue;
            if (!odesc->is_normal())
                continue;
            CDp_lref *prf = (CDp_lref*)odesc->prpty(P_LABRF);
            if (prf && prf->devref())
                // Don't use property labels (shouldn't happen anyway).
                continue;
            hyList *hlabel = OLABEL(odesc)->label();
            char *string = hyList::string(hlabel, HYcvPlain, true);
            if (!string)
                continue;
            char *s;
            for (s = string; isspace(*s); s++) ;
            if (!*s) {
                // Skip empty labels.
                delete [] string;
                continue;
            }
            if (!strncmp(s, "spicetext", 9)) {
                // Skip "spicetext" labels for now.
                delete [] string;
                continue;
            }
            if (dl0 == 0)
                dl = dl0 = new SpiceLine;
            else {
                dl->li_next = new SpiceLine;
                dl = dl->li_next;
            }
            dl->li_line = string;
        }
    }

    // Next, list the "spicetext" labels.  These can appear on any
    // layer, and are sorted according to the integer that appears
    // after "spicetext".

    int stlsz = 200;
    sp_stl_t *lines = new sp_stl_t[stlsz];
    int nlines = 0;

    CDsLgen lgen(sdesc);
    while ((ldesc = lgen.next()) != 0) {
        CDg gdesc;
        gdesc.init_gen(sdesc, ldesc);
        CDo *odesc;
        while ((odesc = gdesc.next()) != 0) {
            if (odesc->type() != CDLABEL)
                continue;
            if (!odesc->is_normal())
                continue;
            CDp_lref *prf = (CDp_lref*)odesc->prpty(P_LABRF);
            if (prf && prf->devref())
                // don't use property labels
                continue;
            hyList *hlabel = OLABEL(odesc)->label();
            char *string = hyList::string(hlabel, HYcvPlain, true);
            if (!string)
                continue;
            char *s, *t;
            for (s = string; isspace(*s); s++) ;
            for (t = s; *s && isalpha(*s); s++) ;
            if (!*s || t == s || strncmp(t, "spicetext", 9)) {
                delete [] string;
                continue;
            }
            for (t = s; *s && !isspace(*s); s++) ;
            int n;
            for (n = 0; t < s; t++) {
                if (isdigit(*t)) {
                    n = atoi(t);
                    break;
                }
            }
            while (isspace(*s))
                s++;
            if (!*s) {
                delete [] string;
                continue;
            }
            if (nlines >= stlsz) {
                stlsz *= 2;
                sp_stl_t *tmp = new sp_stl_t[stlsz];
                for (int i = 0; i < nlines; i++) {
                    tmp[i] = lines[i];
                    delete [] lines;
                    lines = tmp;
                }
            }
            lines[nlines].n = n;
            lines[nlines].str = lstring::copy(s);
            delete [] string;
            nlines++;
        }
    }
    if (nlines > 1)
        std::sort(lines, lines + nlines, complines);
    for (int n = 0; n < nlines; n++) {
        if (dl0 == 0)
            dl = dl0 = new SpiceLine;
        else {
            dl->li_next = new SpiceLine;
            dl = dl->li_next;
        }
        dl->li_line = lines[n].str;
    }
    delete [] lines;
    return (dl0);
}


namespace {
    // Line sort function.
    //
    inline bool
    lcomp(const char *s, const char *t)
    {
        while (*s && *t) {
            if (isdigit(*s) && isdigit(*t)) {
                unsigned int is = atoi(s);
                unsigned int it = atoi(t);
                if (is != it)
                    return (is < it);
                s++;
                t++;
                while (isdigit(*s))
                    s++;
                while (isdigit(*t))
                    t++;
                continue;
            }
            if (*s != *t)
                return (*s < *t);
            s++;
            t++;
        }
        return (strcmp(s, t) < 0);
    }
}


// Sort the line struct lexicographically.
//
void
SpOut::spice_deck_sort(SpiceLine *lines)
{
    SpiceLine *l;
    int i;
    for (i = 0,l = lines; l; i++,l = l->li_next) ;
    if (i < 2) return;
    char **stuff = new char*[i];
    for (i = 0, l = lines; l; i++,l = l->li_next)
        stuff[i] = l->li_line;
    std::sort(stuff, stuff + i, lcomp);
    for (i = 0, l = lines; l; i++,l = l->li_next)
        l->li_line = stuff[i];
    delete [] stuff;
    return;
}


// Check for duplicate device identifiers.  Must be called after sort.
//
void
SpOut::check_dups(SpiceLine *lines, CDs *sdesc)
{
    char buf[64];
    *buf = 0;
    for (SpiceLine *l = lines; l; l = l->li_next) {
        if (l->li_next) {
            SpiceLine *ll = l->li_next;
            char *s = l->li_line;
            while (isspace(*s))
                s++;
            char *t = ll->li_line;
            while (isspace(*t))
                t++;
            if (isalpha(*s) && isalpha(*t)) {
                for (;;) {
                    if (!*s || isspace(*s) || !*t || isspace(*t))
                        break;
                    if (*s == *t) {
                        s++;
                        t++;
                        continue;
                    }
                    break;
                }
                if ((!*s || isspace(*s)) && (!*t || isspace(*t))) {
                    // duplicate token
                    s = l->li_line;
                    t = lstring::gettok(&s);
                    if (strcmp(buf, t)) {
                        Log()->WarningLogV(mh::NetlistCreation,
                            "Cell %s, multiple definitions of %s.",
                            sdesc->cellname(), t);
                        strcpy(buf, t);
                    }
                    delete [] t;
                }
            }
        }
    }
}


// The following functions implement an aliasing feature for Spice
// output.  This enables device keys to be altered, or devices to
// become "null" (no line in deck) if the alias is 0.  Aliases are
// set from a string containing name=alias pairs.  Only the first
// token on a line can be aliased.


// Read the aliases found in the environment into the global
// alias list.  The previous list is freed.
//
void
SpOut::read_alias()
{
    const char *s = CDvdb()->getVariable(VA_SpiceAlias);
    if (!s)
        return;
    sp_alias_t *a;
    while ((a = get_alias(&s)) != 0) {
        a->next = sp_alias_list;
        sp_alias_list = a;
    }
}


// Return an alias structure, and advance the pointer.
//
SpOut::sp_alias_t *
SpOut::get_alias(const char **s)
{
    char buf[128];
    sp_alias_t *a = 0;
    if (!s || !*s)
        return (0);
    const char *line = *s;
    while (isspace(*line))
        line++;
    char *t = buf;
    while (*line && !isspace(*line) && *line != '=')
        *t++ = *line++;
    *t = '\0';
    if (*buf) {
        a = new sp_alias_t;
        a->name = lstring::copy(buf);
        if (*line && *line == '=') {
            line++;
            if (*line && !isspace(*line)) {
                t = buf;
                while (*line && !isspace(*line))
                    *t++ = *line++;
                *t = '\0';
                a->alias = lstring::copy(buf);
            }
        }
    }
    *s = line;
    return (a);
}


// Return the string with alias substitution.  If substitution, free
// old string.
//
char *
SpOut::subst_alias(char *s)
{
    for (sp_alias_t *a = sp_alias_list; a; a = a->next) {
        if (lstring::prefix(a->name, s)) {
            if (!a->alias) {
                delete [] s;
                return (0);
            }
            char *t = new char[strlen(a->alias) + strlen(s) -
                strlen(a->name) + 1];
            strcpy(t, a->alias);
            strcat(t, s + strlen(a->name));
            delete [] s;
            return (t);
        }
    }
    return (s);
}


// Free and clear the current alias list.
//
void
SpOut::free_alias()
{
    for (sp_alias_t *a = sp_alias_list; a; a = sp_alias_list) {
        sp_alias_list = a->next;
        delete [] a->name;
        delete [] a->alias;
        delete a;
    }
}
// End SpOut functions


// Add the devices with names keyed by pref which exist under sdesc
// to a list of .save lines.
//
void
SpOut::sp_dsave_t::process(CDs *sdesc, const char *pref)
{
    CDm_gen mgen(sdesc, GEN_MASTERS);
    for (CDm *md = mgen.m_first(); md; md = mgen.m_next()) {
        if (!md->hasInstances())
            continue;
        CDs *msdesc = md->celldesc();
        if (msdesc && msdesc->isDevice()) {
            CDc_gen cgen(md);
            for (CDc *c = cgen.c_first(); c; c = cgen.c_next()) {
                const char *instname = c->getElecInstBaseName();
                if (lstring::prefix(pref, instname)) {
                    CDp_range *pr = (CDp_range*)c->prpty(P_RANGE);
                    CDgenRange rgen(pr);
                    unsigned int vindex;
                    while (rgen.next(&vindex)) {
                        sLstr lstr;
                        lstr.add(instname);
                        if (pr) {
                            lstr.add_c(cTnameTab::subscr_open());
                            lstr.add_i(vindex);
                            lstr.add_c(cTnameTab::subscr_close());
                        }
                        save(lstr.string());
                    }
                }
            }
        }
    }
    mgen = CDm_gen(sdesc, GEN_MASTERS);
    for (CDm *md = mgen.m_first(); md; md = mgen.m_next()) {
        if (!md->hasInstances())
            continue;
        CDs *msdesc = md->celldesc();
        if (msdesc && !msdesc->isDevice()) {
            CDc_gen cgen(md);
            for (CDc *c = cgen.c_first(); c; c = cgen.c_next()) {
                const char *instname = c->getElecInstBaseName();
                CDp_range *pr = (CDp_range*)c->prpty(P_RANGE);
                CDgenRange rgen(pr);
                unsigned int vindex;
                while (rgen.next(&vindex)) {
                    sLstr lstr;
                    lstr.add(instname);
                    if (pr) {
                        lstr.add_c(cTnameTab::subscr_open());
                        lstr.add_i(vindex);
                        lstr.add_c(cTnameTab::subscr_close());
                    }
                    if (push(lstr.string())) {
                        process(msdesc, pref);
                        pop();
                    }
                }
            }
        }
    }
}


// Push device context (add name to list).
//
bool
SpOut::sp_dsave_t::push(const char *name)
{
    if (name && *name) {
        ds_names[ds_sp] = lstring::copy(name);
        ds_sp++;
        return (true);
    }
    return (false);
}


// Pop device context (remove name from list).
//
void
SpOut::sp_dsave_t::pop()
{
    if (ds_sp > 0) {
        ds_sp--;
        delete [] ds_names[ds_sp];
        ds_names[ds_sp] = 0;
    }
}


// Save the named device in the .save list.  Note that the list does not
// include ".save" or the following "[param]".
//
void
SpOut::sp_dsave_t::save(const char *name)
{
    if (!name || !*name)
        return;
    char buf[256];
    if (CD()->GetSubcCatmode() == cCD::SUBC_CATMODE_WR) {
        char *b = lstring::stpcpy(buf, name);
        for (int i = ds_sp - 1; i >= 0; i--) {
            *b++ = CD()->GetSubcCatchar();
            b = lstring::stpcpy(b, ds_names[i]);
        }
    }
    else if (CD()->GetSubcCatmode() == cCD::SUBC_CATMODE_SPICE3) {
        char *b = buf;
        *b++ = *name;
        if (ds_sp > 0) {
            *b++ = CD()->GetSubcCatchar();
            for (int i = 0; i < ds_sp; i++) {
                strcpy(b, ds_names[i] + 1);
                while (*b)
                    b++;
                *b++ = CD()->GetSubcCatchar();
            }
        }
        strcpy(b, name+1);
    }
    else
        return;
    SpiceLine *l = new SpiceLine(buf);
    l->li_next = ds_saves;
    ds_saves = l;
}
// end of sp_dsave_t methods

