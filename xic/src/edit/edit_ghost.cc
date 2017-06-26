
/*========================================================================*
 *                                                                        *
 *  XICTOOLS Integrated Circuit Design System                             *
 *  Copyright (c) 2007 Whiteley Research Inc, all rights reserved.        *
 *                                                                        *
 *  WHITELEY RESEARCH INCORPORATED PROPRIETARY SOFTWARE                   *
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
 *                                                                        *
 * XIC Integrated Circuit Layout and Schematic Editor                     *
 *                                                                        *
 *========================================================================*
 $Id: edit_ghost.cc,v 1.14 2016/08/13 19:47:51 stevew Exp $
 *========================================================================*/

#include "main.h"
#include "edit.h"
#include "events.h"
#include "ghost.h"


//-----------------------------------------------------------------------------
// Ghost drawing setup

cEditGhost *cEditGhost::instancePtr = 0;

cEditGhost::cEditGhost()
{
   if (instancePtr) {
        fprintf(stderr, "Singleton class cEditGhost already instantiated.\n");
        exit(1);
    }
    instancePtr = this;

    Gst()->RegisterFunc(GFpathseg, ghost_path_line);
    Gst()->RegisterFunc(GFwireseg, ghost_wire_line);
    Gst()->RegisterFunc(GFdisk, ghost_disk);
    Gst()->RegisterFunc(GFdonut, ghost_donut);
    Gst()->RegisterFunc(GFarc, ghost_arc);
    Gst()->RegisterFunc(GFstretch, ghost_stretch, ghost_stretch_setup);
    Gst()->RegisterFunc(GFrotate, ghost_rotate, ghost_rotate_setup);
    Gst()->RegisterFunc(GFmove, ghost_move, ghost_move_setup);
    Gst()->RegisterFunc(GFplace, ghost_place);
    Gst()->RegisterFunc(GFlabel, ghost_label);
    Gst()->RegisterFunc(GFput, ghost_put, ghost_put_setup);
    Gst()->RegisterFunc(GFgrip, ghost_grip);

    eg_max_ghost_objects = DEF_MAX_GHOST_OBJECTS;
    eg_max_ghost_depth = -1;  // Same as expansion depth.
}


// Private static error exit.
//
void
cEditGhost::on_null_ptr()
{
    fprintf(stderr, "Singleton class cEditGhost used before instantiated.\n");
    exit(1);
}


void
cEditGhost::ghost_path_line(int x, int y, int refx, int refy, bool)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y, true);
    EGst()->showGhostPathSeg(x, y, refx, refy);
    DSP()->TPop();
}


void
cEditGhost::ghost_wire_line(int x, int y, int refx, int refy, bool)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y, true);
    EGst()->showGhostWireSeg(x, y, refx, refy);
    DSP()->TPop();
}


void
cEditGhost::ghost_disk(int x, int y, int refx, int refy, bool erase)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y, true);
    EGst()->showGhostDisk(x, y, refx, refy, erase);
    DSP()->TPop();
}


void
cEditGhost::ghost_donut(int x, int y, int refx, int refy, bool erase)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y, true);
    EGst()->showGhostDonut(x, y, refx, refy, erase);
    DSP()->TPop();
}


void
cEditGhost::ghost_arc(int x, int y, int refx, int refy, bool erase)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y, true);
    EGst()->showGhostArc(x, y, refx, refy, erase);
    DSP()->TPop();
}


void
cEditGhost::ghost_stretch(int x, int y, int refx, int refy, bool)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y);
    EGst()->showGhostStretch(x, y, refx, refy);
    DSP()->TPop();
}


void
cEditGhost::ghost_rotate(int x, int y, int refx, int refy, bool erase)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y);
    EGst()->showGhostRotate(x, y, refx, refy, erase);
    DSP()->TPop();
}


void
cEditGhost::ghost_move(int x, int y, int refx, int refy, bool)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y);
    EGst()->showGhostMove(x, y, refx, refy);
    DSP()->TPop();
}


void
cEditGhost::ghost_place(int x, int y, int refx, int refy, bool)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y);
    EGst()->showGhostInstance(x, y, refx, refy);
    DSP()->TPop();
}


void
cEditGhost::ghost_label(int x, int y, int refx, int refy, bool)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y);
    EGst()->showGhostLabel(x, y, refx, refy);
    DSP()->TPop();
}


void
cEditGhost::ghost_put(int x, int y, int refx, int refy, bool)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y);
    EGst()->showGhostYankBuf(x, y, refx, refy);
    DSP()->TPop();
}


void
cEditGhost::ghost_grip(int x, int y, int refx, int refy, bool erase)
{
    if (!EV()->CurrentWin())
        return;
    DSP()->TPush();

    EV()->CurrentWin()->PToL(x, y, x, y);
    EV()->CurrentWin()->Snap(&x, &y);
    EGst()->showGhostGrip(x, y, refx, refy, erase);
    DSP()->TPop();
}
