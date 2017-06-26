
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
 $Id: gtkprpty.h,v 5.13 2015/07/31 22:37:03 stevew Exp $
 *========================================================================*/

#ifndef GTKPRPTY_H
#define GTKPRPTY_H

struct sPbase : public gtk_bag
{
    sPbase()
        {
            pi_line_selected = -1;
            pi_list = 0;
            pi_odesc = 0;
            pi_btn_callback = 0;
            pi_start = 0;
            pi_end = 0;
            pi_drag_x = 0;
            pi_drag_y = 0;
            pi_dragging = false;
        }
    virtual ~sPbase() { pi_list->free(); }

    Ptxt *resolve(int, CDo**);

    static sPbase *prptyInfoPtr();

protected:
    Ptxt *get_selection();
    void update_display();
    void select_range(int, int);
    int button_dn(GtkWidget*, GdkEvent*, void*);
    int button_up(GtkWidget*, GdkEvent*, void*);
    int motion(GtkWidget*, GdkEvent*, void*);
    void drag_data_get(GtkSelectionData*);
    void data_received(GtkWidget*, GdkDragContext*, GtkSelectionData*, guint);

    static void pi_cancel_proc(GtkWidget*, void*);
    static void pi_font_changed();
    static int pi_bad_cb(void*);
    static int pi_text_btn_hdlr(GtkWidget*, GdkEvent*, void*);
    static int pi_text_btn_release_hdlr(GtkWidget*, GdkEvent*, void*);
    static int pi_motion_hdlr(GtkWidget*, GdkEvent*, void*);
    static void pi_drag_data_get(GtkWidget*, GdkDragContext*,
        GtkSelectionData*, guint, guint, void*);
    static void pi_drag_data_received(GtkWidget*, GdkDragContext*,
        gint x, gint y, GtkSelectionData*, guint, guint);

    int pi_line_selected;
    Ptxt *pi_list;
    CDo *pi_odesc;
    int(*pi_btn_callback)(Ptxt*);
    int pi_start;
    int pi_end;
    int pi_drag_x;
    int pi_drag_y;
    bool pi_dragging;
};

#endif
