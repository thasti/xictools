
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
 $Id: edit_variables.h,v 5.29 2016/10/10 18:30:08 stevew Exp $
 *========================================================================*/

#ifndef EDIT_VARIABLES_H
#define EDIT_VARIABLES_H


//
// Edit Module Variables
//

// Editing General (see main_variables.h)
// #define VA_Constrain45          "Constrain45"
#define VA_NoMergeObjects       "NoMergeObjects"
#define VA_NoMergePolys         "NoMergePolys"
// #define VA_AskSaveNative        "AskSaveNative"
// #define VA_NoFixRot45           "NoFixRot45"

// Side Menu Commands (see sced.h)
#define VA_MasterMenuLength     "MasterMenuLength"
// #define VA_DevMenuStyle         "DevMenuStyle"
// #define VA_LabelDefHeight       "LabelDefHeight"
// #define VA_LabelMaxLen          "LabelMaxLen"
// #define VA_LabelMaxLines        "LabelMaxLines"
// #define VA_LabelHiddenMode      "LabelHiddenMode"
#define VA_LogoEndStyle         "LogoEndStyle"
#define VA_LogoPathWidth        "LogoPathWidth"
#define VA_LogoAltFont          "LogoAltFont"
#define VA_LogoPixelSize        "LogoPixelSize"
#define VA_LogoPrettyFont       "LogoPrettyFont"
#define VA_LogoToFile           "LogoToFile"
#define VA_NoConstrainRound     "NoConstrainRound"
// #define VA_RoundFlashSides      "RoundFlashSides"
// #define VA_ElecRoundFlashSides  "ElecRoundFlashSides"
#define VA_SpotSize             "SpotSize"

// Edit/Modify Menu Commands
#define VA_UndoListLength       "UndoListLength"
#define VA_MaxGhostDepth        "MaxGhostDepth"
#define VA_MaxGhostObjects      "MaxGhostObjects"
#define VA_NoWireWidthMag       "NoWireWidthMag"
#define VA_CrCellOverwrite      "CrCellOverwrite"
#define VA_LayerChangeMode      "LayerChangeMode"
#define VA_JoinMaxPolyVerts     "JoinMaxPolyVerts"
#define VA_JoinMaxPolyGroup     "JoinMaxPolyGroup"
#define VA_JoinMaxPolyQueue     "JoinMaxPolyQueue"
#define VA_JoinBreakClean       "JoinBreakClean"
#define VA_JoinSplitWires       "JoinSplitWires"
#define VA_PartitionSize        "PartitionSize"
#define VA_Threads              "Threads"

// Parameterized Cells (see also main_variables.h)
#define VA_PCellAbutMode        "PCellAbutMode"
#define VA_PCellHideGrips       "PCellHideGrips"
#define VA_PCellGripInstSize    "PCellGripInstSize"
// #define VA_PCellKeepSubMasters      "PCellKeepSubMasters"
// #define VA_PCellListSubMasters      "PCellListSubMasters"
// #define VA_PCellScriptPath          "PCellScriptPath"
// #define VA_PCellShowAllWarnings     "PCellShowAllWarnings"

#endif
