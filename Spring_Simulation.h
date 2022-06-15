/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: exitFunc */
#define  PANEL_CANVAS                     2       /* control type: canvas, callback function: (none) */
#define  PANEL_MASS                       3       /* control type: numeric, callback function: (none) */
#define  PANEL_K_2                        4       /* control type: numeric, callback function: (none) */
#define  PANEL_K                          5       /* control type: numeric, callback function: (none) */
#define  PANEL_PICTURE_SCREEN             6       /* control type: picture, callback function: (none) */
#define  PANEL_STRIPCHART                 7       /* control type: strip, callback function: (none) */
#define  PANEL_RESET_BUTTON               8       /* control type: pictButton, callback function: resetFunc */
#define  PANEL_PAUSE_BUTTON               9       /* control type: pictButton, callback function: pauseFunc */
#define  PANEL_START_BUTTON               10      /* control type: pictButton, callback function: startFunc */
#define  PANEL_TIMER                      11      /* control type: timer, callback function: TimerFunc */
#define  PANEL_GRAPH                      12      /* control type: graph, callback function: (none) */
#define  PANEL_OFFSET_2                   13      /* control type: scale, callback function: offsetFunc */
#define  PANEL_OFFSET                     14      /* control type: scale, callback function: offsetFunc */
#define  PANEL_TEXTMSG_3                  15      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  16      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG                    17      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK exitFunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK offsetFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK pauseFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resetFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif