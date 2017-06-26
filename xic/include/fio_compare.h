
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
 $Id: fio_compare.h,v 5.8 2010/11/13 05:44:33 stevew Exp $
 *========================================================================*/

#ifndef FIO_COMPARE_H
#define FIO_COMPARE_H

#include "cd_compare.h"


//
// Implementation of a diff command for cell hierarchies.
//

// Name of output file.
#define DIFF_LOG_FILE "diff.log"

// Start of first line of output, used to ID file as diff output.
#define DIFF_LOG_HEADER "V3.1 hierarchy comparison output"

// Tokens for log file.
#define DIFF_ARCHIVES   "Archives:"
#define DIFF_CELLS      "Cells:"
#define DIFF_MODE       "Mode:"

class cCompare
{
public:
    cCompare();
    ~cCompare();

    bool parse(const char*);
    bool setup();
    DFtype compare();

private:
    // These are set in parse.
    const char *c_fname1;           // <<< file/chd name
    const char *c_fname2;           // >>> file/chd name (null for in-memory)
    const char *c_layer_list;       // layer list, to consider or skip
    const char *c_obj_types;        // "bpwlc" or subset
    const char *c_cell_list1;       // <<< cells
    const char *c_cell_list2;       // >>> cells (defaults to list1)
    BBox c_AOI;                     // area for flat mode
    unsigned int c_max_diffs;       // stop on this many differences
    unsigned int c_fine_grid;       // fine grid for flat mode
    unsigned int c_coarse_mult;     // coarse grid multiplier for flat mode
    unsigned int c_properties;      // properties comparison flags
    DisplayMode c_dmode;            // compare elec/phys cells
    bool c_skip_layers;             // skip layers in layer list
    bool c_geometric;               // geometric comparison
    bool c_diff_only;               // don't record differing geometry
    bool c_exp_arrays;              // expand instance arrays before comp.
    bool c_flat_geometric;          // flat mode
    bool c_aoi_given;               // true if c_AOI was given
    bool c_recurse;                 // recurse in non-flat modes
    bool c_sloppy;                  // allow box/wire and box/poly comparison
    bool c_ignore_dups;             // ignore duplicate objects

    // These are set in setup.
    bool c_free_chd1;               // true if local chd <<<
    bool c_free_chd2;               // true if local chd >>>
    cCHD *c_chd1;                   // <<< CHD
    cCHD *c_chd2;                   // >>> CHD
    stringlist *c_cell_names1;      // cell list 1 as string list
    stringlist *c_cell_names2;      // cell list 2 as string list
    FILE *c_fp;                     // output file
};


// This reads a diff.log file and creates cells in memory containing the
// difference objects.
//
struct diff_parser
{
    enum DirecType { DirNone, DirL, DirR };

    diff_parser();
    ~diff_parser();

    bool diff2cells(const char*);

private:
    bool open(const char*);
    bool read_cell();
    bool process();
    bool add_object(CDs*, const CDo*);

    FILE *dp_fp;
    char *dp_fname;
    char *dp_cn1;
    char *dp_cn2;
    char *dp_next_cn1;
    char *dp_next_cn2;
    const CDo *dp_o12;
    const CDo *dp_o21;
    int dp_linecnt;
    DisplayMode dp_mode;
};

#endif
