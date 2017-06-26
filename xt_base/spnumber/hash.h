
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
 $Id: hash.h,v 1.1 2012/03/03 04:51:24 stevew Exp $
 *========================================================================*/

#ifndef HASH_H
#define HASH_H

#include "lstring.h"


struct wordlist;

#define HTAB_MAX_DENS   5
#define HTAB_START_MASK 31

// Flags to indicate case-sensitivity status for various WRspice classes.
//
enum CSEtype
{
    CSE_FUNC,       // functions
    CSE_UDF,        // user-defined functions
    CSE_VEC,        // vectors
    CSE_PARAM,      // parameters
    CSE_CBLK,       // codeblocks
    CSE_NODE,       // node and device names
    CSE_NUMTYPES
};

struct sHent
{
    friend struct sHtab;

    sHent(const char *nm, void *dt)
        {
            h_name = lstring::copy(nm);
            h_data = dt;
            h_next = 0;
        };

    ~sHent()
        {
            // Data not freed here!
            delete [] h_name;
        }

    const char *name()      const { return (h_name); }
    void *data()            const { return (h_data); }
    void set_data(void *v)  { h_data = v; }
    sHent *next()           const { return (h_next); }

private:
    const char *h_name;
    void *h_data;
    sHent *h_next;
};

struct sHtab
{
    friend struct sHgen;

    sHtab(bool = false);
    virtual ~sHtab();

    void chg_ciflag(bool);
    void add(const char*, void*);
    void *remove(const char*);
    void *get(const char*) const;
    sHent *get_ent(const char*) const;
    void print(const char*) const;
    wordlist *wl() const;
    bool empty() const;
    void clear_data(void(*)(void*, void*), void*);

    unsigned int allocated() const { return (ht_allocated); }

    // Case sensitivity setting, true -> case insensitive.
    static bool get_ciflag(CSEtype type) { return (ht_ciflags[type]); }
    static void parse_ciflags(const char*);

protected:
    static void set_ciflag(CSEtype type, bool b) { ht_ciflags[type] = b; }

    inline unsigned int ht_hash(const char*) const;
    inline int ht_comp(const char*, const char*) const;
    void ht_rehash();

    sHent **ht_base;
    unsigned int ht_allocated;
    unsigned int ht_hashmask;
    bool ht_ci;

    static bool ht_ciflags[CSE_NUMTYPES];
};

// Generator, recurse through entries
//
struct sHgen
{
    sHgen(const sHtab*);
    sHgen(sHtab*, bool);
    sHent *next();
    
private:
    const sHtab *tab;
    sHent *ent;
    unsigned int ix;
    bool remove;
};

#endif
