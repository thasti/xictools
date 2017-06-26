
/*========================================================================*
 *                                                                        *
 *  XICTOOLS Integrated Circuit Design System                             *
 *  Copyright (c) 2013 Whiteley Research Inc, all rights reserved.        *
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
 $Id: ext_ufb.h,v 5.4 2015/10/30 04:37:51 stevew Exp $
 *========================================================================*/

#ifndef EXT_UFB_H
#define EXT_UFB_H

#include "promptline.h"
#include "timer.h"
#include "dsp_tkif.h"


namespace ext_group {
    // Class for updating the prompt for user feedback.  This also polls
    // for interrupts.
    //
    struct Ufb
    {
        Ufb() : u_wheel("|/-\\"), u_i1(0xff), u_i2(0xfff)
            {
                u_cnt = 0;
                u_printed = false;
                u_check_time = 0;
                u_buf[0] = 0;
            }

        void save(const char *fmt, ...)
            {
                if (EX()->isVerbosePromptline()) {
                    va_list args;
                    va_start(args, fmt);
                    vsnprintf(u_buf, 256, fmt, args);
                }
            }

        void print()
            {
                if (EX()->isVerbosePromptline()) {
                    int wcnt = (u_cnt/(u_i2 + 1))%4;
                    if (!u_printed) {
                        if (u_cnt == 0)
                            PL()->ShowPrompt(u_buf);
                        else
                            PL()->ShowPromptV("%s  %c", u_buf, u_wheel[wcnt]);
                        u_printed = true;
                    }
                    else
                        PL()->ShowPromptNoTeeV("%s  %c", u_buf, u_wheel[wcnt]);
                }
            }

        bool checkPrint()
            {
                u_cnt++;
                if (!(u_cnt & u_i1)) {
                    if (EX()->isVerbosePromptline()) {
                        if (!(u_cnt & u_i2)) {
                            int wcnt = (u_cnt/(u_i2 + 1))%4;
                            if (!u_printed) {
                                PL()->ShowPromptV("%s  %c", u_buf,
                                    u_wheel[wcnt]);
                                u_printed = true;
                            }
                            else
                                PL()->ShowPromptNoTeeV("%s  %c", u_buf,
                                    u_wheel[wcnt]);
                        }
                    }
                    if (Timer()->check_interval(u_check_time)) {
                        if (DSP()->MainWdesc() && DSP()->MainWdesc()->Wdraw())
                            dspPkgIf()->CheckForInterrupt();
                        return (XM()->ConfirmAbort());
                    }
                }
                return (false);
            }

        void reset()
            {
                u_cnt = 0;
                u_printed = false;
            }

    private:
        const char *u_wheel;
        const int u_i1, u_i2;
        int u_cnt;
        bool u_printed;
        unsigned long u_check_time;
        char u_buf[256];
    };
}

using namespace ext_group;

#endif
