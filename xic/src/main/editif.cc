
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
 $Id: editif.cc,v 5.7 2013/11/10 06:13:13 stevew Exp $
 *========================================================================*/

#include "main.h"
#include "editif.h"

//-----------------------------------------------------------------------------
// Interface for physical cell editing.
//

cEditIf *cEditIf::instancePtr = 0;

cEditIf::cEditIf()
{
    if (instancePtr) {
        fprintf(stderr, "Singleton class cEditIf already instantiated.\n");
        exit(1);
    }
    instancePtr = this;
}


// Private static error exit.
//
void
cEditIf::on_null_ptr()
{
    fprintf(stderr, "Singleton class cEditIf used before instantiated.\n");
    exit(1);
}
