
/*========================================================================*
 *                                                                        *
 *  XICTOOLS Integrated Circuit Design System                             *
 *  Copyright (c) 1996 Whiteley Research Inc, all rights reserved.        *
 *                                                                        *
 *                                                                        *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      *
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-        *
 *   INFRINGEMENT.  IN NO EVENT SHALL STEPHEN R. WHITELEY BE LIABLE       *
 *   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION      *
 *   OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN           *
 *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN         *
 *   THE SOFTWARE.                                                        *
 *                                                                        *
 *========================================================================*
 *                                                                        *
 * Whiteley Research Circuit Simulation and Analysis Tool                 *
 *                                                                        *
 *========================================================================*
 $Id: linear.cc,v 2.35 2015/06/20 01:58:12 stevew Exp $
 *========================================================================*/

/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

#include "frontend.h"
#include "commands.h"
#include "ftedata.h"
#include "ttyio.h"
#include "errors.h"
#include "hash.h"
#include "graphics.h"
#include "wlist.h"


//
// Interpolate all the vectors in a plot to a linear time scale, which
// we determine by looking at the transient parameters in the CKT struct.
//

void
CommandTab::com_linearize(wordlist *wl)
{
    if (!Sp.CurPlot()) {
        GRpkgIf()->ErrPrintf(ET_ERROR, "no current plot.\n");
        return;
    }
    if (!Sp.CurPlot()->scale()) {
        GRpkgIf()->ErrPrintf(ET_ERROR, "current plot has no scale.\n");
        return;
    }
    if (!Sp.CurPlot()->scale()->isreal()) {
        GRpkgIf()->ErrPrintf(ET_ERROR, "non-real scale for %s\n", 
            Sp.CurPlot()->type_name());
        return;
    }
    if (!lstring::ciprefix("tran", Sp.CurPlot()->type_name())) {
        GRpkgIf()->ErrPrintf(ET_ERROR,
            "plot must be from transient analysis.\n");
        return;
    }
    Sp.CurPlot()->linearize(wl);
}
// End of CommandTab functions.


namespace {
    void lincopy(sDataVec *ov, double *newscale, int newlen,
        sDataVec *oldscale)
    {
        if (!ov->isreal()) {
            GRpkgIf()->ErrPrintf(ET_WARN, "%s is not real.\n", ov->name());
            return;
        }
        if (ov->length() < oldscale->length()) {
            GRpkgIf()->ErrPrintf(ET_WARN, "%s is too short.\n", ov->name());
            return;
        }
        sDataVec *v = new sDataVec(
            lstring::copy(ov->name()), ov->flags(), newlen, ov->units());
        double *nd = v->realvec();
        sPoly po(1);
        if (!po.interp(ov->realvec(), nd, oldscale->realvec(), 
                oldscale->length(), newscale, newlen)) {
            GRpkgIf()->ErrPrintf(ET_WARN, "can't interpolate %s.\n",
                ov->name());
            delete v;
            return;
        }
        v->newperm();
    }
}


// Create a new linearized plot, if success link it in and make it the
// current plot.
//
void
sPlot::linearize(wordlist *wl)
{
    double tstart = pl_start;
    double tstop = pl_stop;
    double tstep = pl_step;
    char buf[BSIZE_SP];

    if (((tstop - tstart) * tstep <= 0.0) || ((tstop - tstart) < tstep)) {
        GRpkgIf()->ErrPrintf(ET_ERROR,
           "bad parameters -- start = %G, stop = %G, step = %G.\n", 
            tstart, tstop, tstep);
        return;
    }

    sDataVec *oldtime = scale();
    sPlot *newp = new sPlot("transient");
    sprintf(buf, "%s (linearized)", name());
    newp->set_name(buf);
    newp->set_title(title());
    newp->set_date(date());
    newp->pl_start = tstart;
    newp->pl_stop = tstop;
    newp->pl_step = tstep;
    newp->new_plot();
    Sp.SetCurPlot(newp->type_name());
    int len = (int)((tstop - tstart) / tstep + 1.5);
    sDataVec *newtime = new sDataVec(
        lstring::copy(oldtime->name()), oldtime->flags(), len,
        oldtime->units());
    newtime->set_plot(newp);
    int i;
    double d;
    for (i = 0, d = tstart; i < len; i++, d += tstep)
        newtime->set_realval(i, d);
    newtime->newperm(); // set to scale

    if (wl && !lstring::cieq(wl->wl_word, "all")) {
        while (wl) {
            sDataVec *v = find_vec(wl->wl_word);
            if (!v) {
                Sp.Error(E_NOVEC, 0, wl->wl_word);
                continue;
            }
            lincopy(v, newtime->realvec(), len, oldtime);
            wl = wl->wl_next;
        }
    }
    else {
        sHgen gen(pl_hashtab);
        sHent *h;
        while ((h = gen.next()) != 0) {
            sDataVec *v = (sDataVec*)h->data();
            if (v == scale())
                continue;
            lincopy(v, newtime->realvec(), len, oldtime);
        }
    }
}
