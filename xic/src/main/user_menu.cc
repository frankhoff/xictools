
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
#include "dsp_tkif.h"
#include "si_parsenode.h"
#include "si_parser.h"
#include "menu.h"
#include "user_menu.h"
#include "main_scriptif.h"


namespace {
    namespace user_menu {
        MenuFunc  M_Debug;
        MenuFunc  M_Rehash;
    }
}

using namespace user_menu;

namespace {
    MenuBox UserMenuBox;

    // Recursively fill in the entry data.  The path saved in the
    // description field is in the form User/top/next/...
    // The top component is resolved through the ScriptPath, so *must* be
    // the basename of a .scr or .scm file.  Components that follow
    // are label text.  The entry field is label text.
    //
    MenuEnt *
    fill_menu(umenu *u0, MenuEnt *ent, const char *path, int depth)
    {
        for (umenu *u = u0; u; u = u->next()) {
            char buf[128];
            sprintf(buf, "%s/%s", path,
                depth == 0 ? u->basename() : u->label());
            *ent = MenuEnt(&M_NoOp, lstring::copy(u->label()),
                ME_VANILLA, CMD_NOTSAFE, lstring::copy(buf));
            ent->set_dynamic(true);
            ent->cmd.wdesc = DSP()->MainWdesc();
            if (u->menu()) {
                ent->set_menu(true);
                ent = fill_menu(u->menu(), ent + 1, buf, depth+1);
            }
            else
                ent++;
        }
        return (ent);
    }


    void
    user_menu_rebuild(MenuBox *mbox)
    {
        umenu *u0 = XM()->GetFunctionList();
        int cnt = umenu::numitems(u0);

        // menu size, including empty terminator
        int umenusize = cnt + userMenu_END + 1;

        // number of static (non changing, always present) entries
        int nstatic = userMenu_END;

        {
            int i = 0;
            for (MenuEnt *ent = mbox->menu; ent->entry; ent++) {
                if (i >= nstatic) {
                    delete [] ent->entry;
                    delete [] ent->description;
                }
                i++;
            }
        }
        delete [] mbox->menu;

        mbox->menu = new MenuEnt[umenusize];

        mbox->menu[userMenu] =
            MenuEnt(&M_NoOp,     "",        ME_MENU,     CMD_SAFE,
            0);
        mbox->menu[userMenuDebug] =
            MenuEnt(&M_Debug,    MenuDEBUG, ME_TOGGLE,   CMD_NOTSAFE,
            MenuDEBUG": Pop up the script debugger.");
        mbox->menu[userMenuHash] =
            MenuEnt(&M_Rehash,   MenuHASH,  ME_SEP,      CMD_SAFE,
            MenuHASH": Remake this menu from script files in the script path.");

        for (int i = 0; i < nstatic; i++) {
            mbox->menu[i].cmd.wdesc = DSP()->MainWdesc();
            mbox->menu[i].cmd.caller = 0;
        }
        fill_menu(u0, &mbox->menu[nstatic], "/User", 0);
        umenu::destroy(u0);
    }
}


MenuBox *
cMain::createUserMenu()
{
    MenuEnt *UserMenu = new MenuEnt[userMenu_END + 1];

    UserMenu[userMenu] =
        MenuEnt(&M_NoOp,     "",        ME_MENU,     CMD_SAFE,
        0);
    UserMenu[userMenuDebug] =
        MenuEnt(&M_Debug,    MenuDEBUG, ME_TOGGLE,   CMD_NOTSAFE,
        MenuDEBUG": Pop up the script debugger.");
    UserMenu[userMenuHash] =
        MenuEnt(&M_Rehash,   MenuHASH,  ME_SEP,      CMD_SAFE,
        MenuHASH": Remake this menu from script files in the script path.");
    UserMenu[userMenu_END] =
        MenuEnt();

    UserMenuBox.name = "User";
    UserMenuBox.menu = UserMenu;
    UserMenuBox.registerRebuildMenuProc(&user_menu_rebuild);
    return (&UserMenuBox);
}


//-----------------------------------------------------------------------------
// The DEBUG command.
//
// Bring up a panel for script debugging.
//
void
user_menu::M_Debug(CmdDesc *cmd)
{
    if (cmd && Menu()->GetStatus(cmd->caller))
        XM()->PopUpDebug(cmd->caller, MODE_ON);
    else
        XM()->PopUpDebug(0, MODE_OFF);
}


//-----------------------------------------------------------------------------
// The HASH command.
//

namespace {
    // We can't alter the debug menu while were in it, so use an idle
    // function, for the Rehash button.
    //
    int
    user_menu_update_idle_proc(void*)
    {
        XM()->Rehash();
        return (false);
    }
}


// Command to rebuild the user menu.
//
void
user_menu::M_Rehash(CmdDesc*)
{
    dspPkgIf()->RegisterIdleProc(user_menu_update_idle_proc, 0);
}

