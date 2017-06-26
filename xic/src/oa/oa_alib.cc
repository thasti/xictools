
/*========================================================================*
 *                                                                        *
 *  XICTOOLS Integrated Circuit Design System                             *
 *  Copyright (c) 2016 Whiteley Research Inc, all rights reserved.        *
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
 $Id: oa_alib.cc,v 1.1 2016/03/09 05:43:35 stevew Exp $
 *========================================================================*/

#include "cd.h"
#include "oa.h"
#include "oa_alib.h"
#include "pcell_params.h"


cAlibFixup::alib_elt cAlibFixup::al_list[] = {
    alib_elt("vdc", alib_vdc),
    alib_elt("iprobe", alib_iprobe),
    alib_elt("vexp", alib_vexp),
    alib_elt("vpulse", alib_vpulse),
    alib_elt("vpwl", alib_vpwl),
    alib_elt("vsffm", alib_vsffm),
    alib_elt("vsin", alib_vsin),
    alib_elt("idc", alib_idc),
    alib_elt("iexp", alib_iexp),
    alib_elt("ipulse", alib_ipulse),
    alib_elt("ipwl", alib_ipwl),
    alib_elt("isffm", alib_isffm),
    alib_elt("isin", alib_isin),
    alib_elt("cccs", alib_cccs),
    alib_elt("ccvs", alib_ccvs),
    alib_elt("vccs", alib_vccs),
    alib_elt("vcvs", alib_vcvs),
    alib_elt("cap", alib_cap),
    alib_elt("ind", alib_ind),
    alib_elt("mind", alib_mind),
    alib_elt("res", alib_res),
    alib_elt("jj", alib_jj),
    alib_elt("tline", alib_tline),
    alib_elt("diode", alib_diode),
    alib_elt("npn", alib_npn),
    alib_elt("pnp", alib_pnp),
    alib_elt("nbsim", alib_nbsim),
    alib_elt("nbsim4", alib_nbsim4),
    alib_elt("nmos", alib_nmos),
    alib_elt("nmos4", alib_nmos4),
    alib_elt("pbsim", alib_pbsim),
    alib_elt("pbsim4", alib_pbsim4),
    alib_elt("pmos", alib_pmos),
    alib_elt("pmos4", alib_pmos4),
    alib_elt("njfet", alib_njfet),
    alib_elt("pjfet", alib_pjfet),
    alib_elt("nmes", alib_nmes),
    alib_elt("pmes", alib_pmes),
    alib_elt(0, 0)
};


bool
cAlibFixup::prpty_fix(const char *cname, sLstr &lstr)
{
    if (!al_linked) {
        for (alib_elt *e = al_list; e->name; e++) {
            unsigned int n = string_hash(e->name, AL_HMASK);
            e->next = al_array[n];
            al_array[n] = e;
        }
        al_linked = true;
    }
    unsigned int n = string_hash(cname, AL_HMASK);
    for (alib_elt *e = al_array[n]; e; e = e->next) {
        if (!strcmp(cname, e->name))
            return ((*e->func)(lstr));
    }
    return (false);
}


//
// The remaining functions are private and static.
//

// Add the dc/ac keywords for sources.
//
void
cAlibFixup::dcac(const PCellParam *prms, sLstr &lstr)
{
    const PCellParam *p = prms->find_c("dc");
    if (p) {
        lstr.add(" dc ");
        lstr.add(p->stringVal());
    }
    const PCellParam *pm = prms->find_c("acm");
    const PCellParam *pp = prms->find_c("acp");
    if (pm || pp) {
        lstr.add(" ac ");
        lstr.add(pm ? pm->stringVal() : "1.0");
        if (pp) {
            lstr.add_c(',');
            lstr.add(pp->stringVal());
        }
    }
}

// Common function for sources.
//
bool
cAlibFixup::src_params(sLstr &lstr, const char *dname, const char **params)
{
    if (!params || !*params || !dname)
        return (false);
    PCellParam *prms;
    if (!PCellParam::parseParams(lstr.string(), &prms))
        return (false);

    PCellParam *p = prms->find(*params++);
    if (!p) {
        prms->free();
        return (false);
    }
    lstr.free();
    lstr.add(dname);
    lstr.add_c(' ');
    lstr.add(p->stringVal());

    while ((p = prms->find(*params++)) != 0) {
        lstr.add_c(' ');
        lstr.add(p->stringVal());
    }
    dcac(prms, lstr);
    prms->free();
    return (true);
}


//
// The remaining functions correspond to the devices in the
// analogLibrary that we support.  This is based on the simInfo.il
// file from the WRspice ADE interface in Virtuoso.
//

//================
// V Sources
//================

// vdc
// instParameters      (dc acm acp)
//
bool
cAlibFixup::alib_vdc(sLstr &lstr)
{
    PCellParam *prms;
    if (!PCellParam::parseParams(lstr.string(), &prms))
        return (false);

    lstr.free();
    if (!prms) {
        lstr.add("0");
        return (true);
    }
    dcac(prms, lstr);
    prms->free();
    return (true);
}

// iprobe
// instParameters      (vdummy)
//
bool
cAlibFixup::alib_iprobe(sLstr &lstr)
{
    lstr.free();
    lstr.add("0");
    return (true);
}

// vexp
// instParameters      (dc acm acp v1 v2 td1 td2 tau1 tau2)
//
bool
cAlibFixup::alib_vexp(sLstr &lstr)
{
    const char *params[] = { "v1", "v2", "td1", "tau1", "td2", "tau2", 0 };
    return (src_params(lstr, "exp", params));
}

// vpulse
// instParameters      (dc acm acp v1 v2 td tr tf pw per)
//
bool
cAlibFixup::alib_vpulse(sLstr &lstr)
{
    const char *params[] = { "v1", "v2", "td", "tr", "tf", "pw", "per", 0 };
    return (src_params(lstr, "pulse", params));
}

// vpwl
// instParameters      (dc acm acp tvpairs t1 v1 t2 v2 t3 v3 ... t50 v50)
//
bool
cAlibFixup::alib_vpwl(sLstr &lstr)
{
    // t1 v1 t2 v2 ...
    PCellParam *prms;
    if (!PCellParam::parseParams(lstr.string(), &prms))
        return (false);

    GCfree<PCellParam*> gc_prms(prms);
    PCellParam *p = prms->find("tvpairs");
    if (!p)
        return (false);
    int tvp = atoi(p->stringVal());
    if (tvp < 2 || tvp > 50)
        return (false);
    lstr.free();
    lstr.add("pwl");
    char txx[4], vxx[4];
    txx[0] = 't';
    vxx[0] = 'v';
    for (int i = 1; i <= tvp; i++) {
        sprintf(txx+1, "%d", i);
        sprintf(vxx+1, "%d", i);
        PCellParam *p = prms->find(txx);
        lstr.add_c(' ');
        lstr.add(p ? p->stringVal() : "0");
        lstr.add_c(' ');
        p = prms->find(vxx);
        lstr.add(p ? p->stringVal() : "0");
    }
    dcac(prms, lstr);
    return (true);
}

// vsffm
// instParameters      (dc acm acp vo va freq mdi fs)
//
bool
cAlibFixup::alib_vsffm(sLstr &lstr)
{
    const char *params[] = { "vo", "va", "freq", "mdi", "fs", 0 };
    return (src_params(lstr, "sffm", params));
}

// vsin
// instParameters      (dc acm acp vo va freq td theta phi)
//
bool
cAlibFixup::alib_vsin(sLstr &lstr)
{
    const char *params[] = { "vo", "va", "freq", "td", "theta", 0 };
    return (src_params(lstr, "sin", params));
}


//================
// I Sources
//================

// idc
// instParameters      (dc acm acp)
//
bool
cAlibFixup::alib_idc(sLstr &lstr)
{
    PCellParam *prms;
    if (!PCellParam::parseParams(lstr.string(), &prms))
        return (false);

    lstr.free();
    if (!prms) {
        lstr.add("0");
        return (true);
    }
    dcac(prms, lstr);
    prms->free();
    return (true);
}

// iexp
// instParameters      (dc acm acp i1 i2 td1 td2 tau1 tau2)
//
bool
cAlibFixup::alib_iexp(sLstr &lstr)
{
    const char *params[] = { "i1", "i2", "td1", "tau1", "td2", "tau2", 0 };
    return (src_params(lstr, "exp", params));
}

// ipulse
// instParameters      (dc acm acp i1 i2 td tr tf pw per)
//
bool
cAlibFixup::alib_ipulse(sLstr &lstr)
{
    const char *params[] = { "i1", "i2", "td", "tr", "tf", "pw", "per", 0 };
    return (src_params(lstr, "pulse", params));
}

// ipwl
// instParameters      (dc acm acp tvpairs t1 i1 t2 i2 t3 i3 ... t50 i50)
//
bool
cAlibFixup::alib_ipwl(sLstr &lstr)
{
    // t1 i1 t2 i2 ...
    PCellParam *prms;
    if (!PCellParam::parseParams(lstr.string(), &prms))
        return (false);

    GCfree<PCellParam*> gc_prms(prms);
    PCellParam *p = prms->find("tvpairs");
    if (!p)
        return (false);
    int tvp = atoi(p->stringVal());
    if (tvp < 2 || tvp > 50)
        return (false);
    lstr.free();
    lstr.add("pwl");
    char txx[4], ixx[4];
    txx[0] = 't';
    ixx[0] = 'i';
    for (int i = 1; i <= tvp; i++) {
        sprintf(txx+1, "%d", i);
        sprintf(ixx+1, "%d", i);
        PCellParam *p = prms->find(txx);
        lstr.add_c(' ');
        lstr.add(p ? p->stringVal() : "0");
        lstr.add_c(' ');
        p = prms->find(ixx);
        lstr.add(p ? p->stringVal() : "0");
    }
    dcac(prms, lstr);
    return (true);
}

// isffm
// instParameters      (dc acm acp io ia freq mdi fs)
//
bool
cAlibFixup::alib_isffm(sLstr &lstr)
{
    const char *params[] = { "io", "ia", "freq", "mdi", "fs", 0 };
    return (src_params(lstr, "sffm", params));
}

// isin
// instParameters      (dc acm acp io ia freq td theta phi)
//
bool
cAlibFixup::alib_isin(sLstr &lstr)
{
    const char *params[] = { "io", "ia", "freq", "td", "theta", 0 };
    return (src_params(lstr, "sin", params));
}


//================
// Dep. Sources
//================

// cccs
// instParameters      (hfgain vref)
//
bool
cAlibFixup::alib_cccs(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// ccvs
// instParameters      (hhgain vref)
//
bool
cAlibFixup::alib_ccvs(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// vccs
// instParameters      (hggain)
//
bool
cAlibFixup::alib_vccs(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// vcvs
// instParameters      (hegain)
//
bool
cAlibFixup::alib_vcvs(sLstr &lstr)
{
    (void)lstr;
    return (false);
}


//================
// Capacitor
//================

// cap
// instParameters      (c ic w l temp tc1 tc2)
// otherParameters     (model)
//
bool
cAlibFixup::alib_cap(sLstr &lstr)
{
    PCellParam *prms;
    if (!PCellParam::parseParams(lstr.string(), &prms))
        return (false);
    if (!prms)
        return (false);
    PCellParam *p = prms->find("c");
    if (!p) {
        prms->free();
        return (false);
    }
    lstr.free();
    lstr.add(p->stringVal());
    p = prms->find("ic");
    if (p) {
        lstr.add(" ic ");
        lstr.add(p->stringVal());
    }
    prms->free();
    return (true);
}


//================
// Inductor
//================

// ind
// instParameters      (l ic)
//
bool
cAlibFixup::alib_ind(sLstr &lstr)
{
    PCellParam *prms;
    if (!PCellParam::parseParams(lstr.string(), &prms))
        return (false);
    if (!prms)
        return (false);
    PCellParam *p = prms->find("l");
    if (!p) {
        prms->free();
        return (false);
    }
    lstr.free();
    lstr.add(p->stringVal());
    p = prms->find("ic");
    if (p) {
        lstr.add(" ic ");
        lstr.add(p->stringVal());
    }
    prms->free();
    return (true);
}

// mind
// instParameters      (k)
// otherParameters     (ind1 ind2)
//
bool
cAlibFixup::alib_mind(sLstr &lstr)
{
    (void)lstr;
    return (false);
}


//================
// Resistor
//================

// res
// instParameters      (r temp w l tc1 tc2 noise)
// otherParameters     (model)
//
bool
cAlibFixup::alib_res(sLstr &lstr)
{
    PCellParam *prms;
    if (!PCellParam::parseParams(lstr.string(), &prms))
        return (false);
    if (!prms)
        return (false);
    PCellParam *p = prms->find("r");
    if (!p) {
        prms->free();
        return (false);
    }
    lstr.free();
    lstr.add(p->stringVal());
    prms->free();
    return (true);
}


//================
// Josephson Jnc
//================

// jj
// instParameters      (area off phi vj)
// otherParameters     (model)
//
bool
cAlibFixup::alib_jj(sLstr &lstr)
{
    (void)lstr;
    return (false);
}


//================
// TLine
//================

// tline
// instParameters      (len l c r g zo td freq nl v1 i1 v2 i2)
// otherParameters     (model)
//
bool
cAlibFixup::alib_tline(sLstr &lstr)
{
    (void)lstr;
    return (false);
}


//================
// Diode
//================

// diode
// instParameters      (off temp dtemp ic area pj m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_diode(sLstr &lstr)
{
    (void)lstr;
    return (false);
}


//================
// BJT
//================

// npn
// instParameters      (off temp dtemp area Vbe Vce)
// otherParameters     (model)
//
bool
cAlibFixup::alib_npn(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// pnp
// instParameters      (off temp dtemp area Vbe Vce)
// otherParameters     (model)
//
bool
cAlibFixup::alib_pnp(sLstr &lstr)
{
    (void)lstr;
    return (false);
}


//================
// MOS
//================

// nbsim
// instParameters      (l w ad as pd ps nrd nrs rdc rsc off Vds Vgs Vbs
//                      dtemp geo m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_nbsim(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// nbsim4
// instParameters      (l w ad as pd ps nrd nrs rdc rsc off Vds Vgs Vbs
//                      dtemp geo m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_nbsim4(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// nmos
// instParameters      (l w ad as pd ps nrd nrs rdc rsc off Vds Vgs Vbs
//                      dtemp geo m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_nmos(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// nmos4
// instParameters      (l w ad as pd ps nrd nrs rdc rsc off Vds Vgs Vbs
//                      dtemp geo m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_nmos4(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// pbsim
// instParameters      (l w ad as pd ps nrd nrs rdc rsc off Vds Vgs Vbs
//                      dtemp geo m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_pbsim(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// pbsim4
// instParameters      (l w ad as pd ps nrd nrs rdc rsc off Vds Vgs Vbs
//                      dtemp geo m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_pbsim4(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// pmos
// instParameters      (l w ad as pd ps nrd nrs rdc rsc off Vds Vgs Vbs
//                      dtemp geo m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_pmos(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// pmos4
// instParameters      (l w ad as pd ps nrd nrs rdc rsc off Vds Vgs Vbs
//                      dtemp geo m)
// otherParameters     (model)
//
bool
cAlibFixup::alib_pmos4(sLstr &lstr)
{
    (void)lstr;
    return (false);
}


//================
// JFET
//================

// njfet
// instParameters      (Vds Vgs area off temp)
// otherParameters     (model)
//
bool
cAlibFixup::alib_njfet(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// pjfet
// instParameters      (Vds Vgs area off temp)
// otherParameters     (model)
//
bool
cAlibFixup::alib_pjfet(sLstr &lstr)
{
    (void)lstr;
    return (false);
}


//================
// MESFET
//================

// nmes
// instParameters      (Vds Vgs area off temp)
// otherParameters     (model)
//
bool
cAlibFixup::alib_nmes(sLstr &lstr)
{
    (void)lstr;
    return (false);
}

// pmes
// instParameters      (Vds Vgs area off temp)
// otherParameters     (model)
//
bool
cAlibFixup::alib_pmes(sLstr &lstr)
{
    (void)lstr;
    return (false);
}
