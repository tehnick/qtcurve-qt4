#ifndef __COMMON_H__
#define __COMMON_H__

/*
  QtCurve (C) Craig Drummond, 2003 - 2008 Craig.Drummond@lycos.co.uk

  ----

  This program is free software; you can redistr ibute it and/or
  modify it under the terms of the GNU General Public
  License version 2 as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

/* This file is quite 'hacky' as it contains lots of static function definitions - but I want to share the code
   between Qt and Gtk, but not polute the namespace with exported functions... */

#include <string.h>
#include <math.h>
#include "config.h"

/*
    The following #define disables the rounding when scrollbar type==none.
#define QTC_SIMPLE_SCROLLBARS
*/

/*
    The following #define disables the custom focus rectangle
#define QTC_PLAIN_FOCUS_ONLY
*/

/*
    The following #define controls whether a scrollbar's slider should overlap
    the scrollbar buttons when at min/max. This removes the thick looking line
    between the slider and the buttons.
*/
#define QTC_INCREASE_SB_SLIDER

/*
    Control whether toolbar, window decoration, and dock window, buttons should have coloured mouse-over
*/
#define QTC_DONT_COLOUR_MOUSEOVER_TBAR_BUTTONS
/*
    Control shading used for glass variants.
    0 => As used in 0.51.1 +
    1 => As used in 0.51
    2 => As used in <0.51
*/
#define QTC_GLASS_SHADING 0

typedef enum
{
    SHADING_SIMPLE=0,
    SHADING_HSL=1,
    SHADING_HSV=2
} EShading;

#if (!defined QTC_KWIN)
static EShading shading=SHADING_HSL;
#endif

#ifdef __cplusplus
#include <qconfig.h>
#ifdef QTC_CONFIG_DIALOG
#include <qapplication.h>
#endif
#include <map>
#include <set>
#include <vector>
#else
#include <glib.h>
#endif

#ifdef __cplusplus
typedef QColor color;
#else
typedef gboolean bool;
typedef GdkColor color;
#define true TRUE
#define false FALSE
#endif

#define QTC_GROUP        "Settings"
/*#define QTC_DESCR_GROUP  "Description"*/

/* qtc_<theme name>.themerc support */
#define KDE_PREFIX(V) ((4==(V)) ? KDE4PREFIX : KDE3PREFIX)
#define QTC_THEME_DIR    "/share/apps/kstyle/themes/"
#define QTC_THEME_PREFIX "qtc_"
#define QTC_THEME_SUFFIX ".themerc"


#define QTC_CHECK_SIZE   13
#define QTC_RADIO_SIZE   13
#define QTC_MIN_BTN_SIZE 8
#define QTC_LV_SIZE      7

#define LARGE_ARR_WIDTH  7
#define LARGE_ARR_HEIGHT 4
#define SMALL_ARR_WIDTH  5
#define SMALL_ARR_HEIGHT 3

#define NUM_STD_SHADES   6
#define NUM_EXTRA_SHADES 3

#define TOTAL_SHADES     NUM_STD_SHADES+NUM_EXTRA_SHADES
#define ORIGINAL_SHADE   TOTAL_SHADES

#define SHADE_ORIG_HIGHLIGHT NUM_STD_SHADES
#define SHADE_4_HIGHLIGHT    NUM_STD_SHADES+1
#define SHADE_2_HIGHLIGHT    NUM_STD_SHADES+2

/* 3d effect - i.e. buttons, etc */
#define QTC_SHADES \
    static const double shades[2][11][NUM_STD_SHADES]=\
    { \
        { /* HSV & HSL */ \
            { 1.05, 1.04, 0.90, 0.800, 0.830, 0.82 }, \
            { 1.06, 1.04, 0.90, 0.790, 0.831, 0.78 }, \
            { 1.07, 1.04, 0.90, 0.785, 0.832, 0.75 }, \
            { 1.08, 1.05, 0.90, 0.782, 0.833, 0.72 }, \
            { 1.09, 1.05, 0.90, 0.782, 0.834, 0.70 }, \
            { 1.10, 1.06, 0.90, 0.782, 0.836, 0.68 }, \
            { 1.12, 1.06, 0.90, 0.782, 0.838, 0.63 }, \
            { 1.16, 1.07, 0.90, 0.782, 0.840, 0.62 }, /* default */ \
            { 1.18, 1.07, 0.90, 0.783, 0.842, 0.60 }, \
            { 1.20, 1.08, 0.90, 0.784, 0.844, 0.58 }, \
            { 1.22, 1.08, 0.90, 0.786, 0.848, 0.55 }  \
        }, \
        { /* SIMPLE */ \
            { 1.07, 1.03, 0.91, 0.780, 0.834, 0.75 }, \
            { 1.08, 1.03, 0.91, 0.781, 0.835, 0.74 }, \
            { 1.09, 1.03, 0.91, 0.782, 0.836, 0.73 }, \
            { 1.10, 1.04, 0.91, 0.783, 0.837, 0.72 }, \
            { 1.11, 1.04, 0.91, 0.784, 0.838, 0.71 }, \
            { 1.12, 1.05, 0.91, 0.785, 0.840, 0.70 }, \
            { 1.13, 1.05, 0.91, 0.786, 0.842, 0.69 }, \
            { 1.14, 1.06, 0.91, 0.787, 0.844, 0.68 }, /* default */ \
            { 1.16, 1.06, 0.91, 0.788, 0.846, 0.66 }, \
            { 1.18, 1.07, 0.91, 0.789, 0.848, 0.64 }, \
            { 1.20, 1.07, 0.91, 0.790, 0.850, 0.62 }  \
        } \
    } ;

#define QTC_SIMPLE_SHADING (!shading)

#define QTC_GLOW_MO        ORIGINAL_SHADE
#define QTC_GLOW_DEFBTN    0

#define QT_STD_BORDER      5
#define QT_DISABLED_BORDER QT_STD_BORDER /*3*/
#define QT_BORDER(E) (/*(E) ?*/ QT_STD_BORDER/* : QT_DISABLED_BORDER*/)

#define QT_FRAME_DARK_SHADOW 2
#define QT_FOCUS             3
#define QTC_MENU_STRIPE_SHADE (opts.lighterPopupMenuBgnd ? ORIGINAL_SHADE : 2)
#define QTC_MENU_SEP_SHADE    (opts.lighterPopupMenuBgnd ? 4 : 3)

#define QTC_SHADE(c, s) \
    (c>10 || c<0 || s>=NUM_STD_SHADES || s<0 \
        ? 1.0 \
        : opts.darkerBorders && (QT_STD_BORDER==i || QT_DISABLED_BORDER==i) \
            ? shades[SHADING_SIMPLE==shading ? 1 : 0][c][s] - 0.1 \
            : shades[SHADING_SIMPLE==shading ? 1 : 0][c][s] )

#define TAB_APPEARANCE(A)   (A) /* (APPEARANCE_GLASS==(A) ? APPEARANCE_GRADIENT : (A)) */
#define QTC_COLOR_SEL_TAB_FACTOR 1.2
#define SHADE_COLOR_SEL_TAB_TOP  1.1
#define SHADE_COLOR_SEL_TAB_BOT  0.95

#define INVERT_SHADE(A) (1.0+(1.0-A))

#define QTC_ROUNDED (ROUND_NONE!=opts.round)

#define QTC_ETCHED_DARK            0.95
#define SHADE_BEVEL_GRAD_LIGHT     (QTC_SIMPLE_SHADING ? 1.05 : 1.07)
#define SHADE_BEVEL_GRAD_DARK      (QTC_SIMPLE_SHADING ? 0.93 : 0.91)
#define SHADE_BEVEL_GRAD_SEL_LIGHT (QTC_SIMPLE_SHADING ? 1.05 : 1.07)
#define SHADE_BEVEL_GRAD_SEL_DARK  (QTC_SIMPLE_SHADING ? 0.95 : 0.93)

#define SHADE_BEVEL_MENU_ITEM_LIGHT (QTC_SIMPLE_SHADING ? 1.07 : 1.09)
#define SHADE_BEVEL_MENU_ITEM_DARK  (QTC_SIMPLE_SHADING ? 0.85 : 0.83)

#define SHADE_SLIDER_LIGHT          1.1
#define SHADE_SLIDER_DARK           0.8

#define SHADE_SBAR_LIGHT            1.02
#define SHADE_SBAR_DARK             0.95

#define SHADE_PBAR_TROUGH_LIGHT     1.03
#define SHADE_PBAR_TROUGH_DARK      0.94

#define SHADE_MENU_LIGHT            1.02
#define SHADE_MENU_DARK             0.96

#define SHADE_TAB_SEL_LIGHT         1.1
#define SHADE_TAB_SEL_DARK          1.0

#define SHADE_BOTTOM_TAB_SEL_LIGHT  1.0
#define SHADE_BOTTOM_TAB_SEL_DARK   0.96

#define SPLIT_GRADIENT_FACTOR       0.415

#if !defined QTC_GLASS_SHADING || QTC_GLASS_SHADING==0

    #define SHADE_GLASS_TOP_A(A, W) (APPEARANCE_DULL_GLASS==A \
                                        ? (WIDGET_DEF_BUTTON==W ? 0.99 : 0.98) \
                                        : (WIDGET_DEF_BUTTON==W ? 1.08 : 1.55))
    #define SHADE_GLASS_TOP_B(A, W) (APPEARANCE_DULL_GLASS==A \
                                        ? (WIDGET_DEF_BUTTON==W ? 0.94 : 0.92) \
                                        : 0.92)
    #define SHADE_GLASS_BOT_A(A) (APPEARANCE_DULL_GLASS==A ? 1.02 : 0.99)
    #define SHADE_GLASS_BOT_B(A) (APPEARANCE_DULL_GLASS==A ? 1.10 : 1.16)

#elif QTC_GLASS_SHADING==1

    #define SHADE_GLASS_TOP_A(A, W) (APPEARANCE_DULL_GLASS==A \
                                        ? (WIDGET_DEF_BUTTON==W ? 1.0 : 1.0) \
                                        : (WIDGET_DEF_BUTTON==W ? 1.08 : 1.7))
    #define SHADE_GLASS_TOP_B(A, W) (APPEARANCE_DULL_GLASS==A ? 0.96 : 0.96)
    #define SHADE_GLASS_BOT_A(A) 0.99
    #define SHADE_GLASS_BOT_B(A) (APPEARANCE_DULL_GLASS==A ? 1.08 : 1.16)

#else

    #define SHADE_GLASS_TOP_A(A, W) (APPEARANCE_DULL_GLASS==A \
                                        ? (WIDGET_DEF_BUTTON==W ? 1.05 : 1.05) \
                                        : (WIDGET_DEF_BUTTON==W ? 1.08 : 1.7))
    #define SHADE_GLASS_TOP_B(A, W) 0.96
    #define SHADE_GLASS_BOT_A(A) 0.99
    #define SHADE_GLASS_BOT_B(A) (APPEARANCE_DULL_GLASS==A ? 1.08 : 1.16)

#endif

#define IS_GLASS(A) (APPEARANCE_DULL_GLASS==A || APPEARANCE_SHINY_GLASS==A)
#define IS_CUSTOM(A) (A>=APPEARANCE_CUSTOM1 && A<(APPEARANCE_CUSTOM1+QTC_NUM_CUSTOM_GRAD))
#define IS_FLAT(A)  (APPEARANCE_FLAT==A || APPEARANCE_RAISED==A)
#define SHADE_SELECION_LIGHT 1.15
#define SHADE_SELECION_DARK  0.90

#ifdef __cplusplus
#define MENUBAR_DARK_LIMIT 160
#define TOO_DARK(A) ((A).red()<MENUBAR_DARK_LIMIT || (A).green()<MENUBAR_DARK_LIMIT || (A).blue()<MENUBAR_DARK_LIMIT)
#else
#define MENUBAR_DARK_LIMIT (160<<8)
#define TOO_DARK(A) ((A).red<MENUBAR_DARK_LIMIT || (A).green<MENUBAR_DARK_LIMIT || (A).blue<MENUBAR_DARK_LIMIT)
#endif

#define DEFAULT_HIGHLIGHT_FACTOR   1.05
#define MAX_HIGHLIGHT_FACTOR         50
#define MIN_HIGHLIGHT_FACTOR        -50
#define MENUBAR_DARK_FACTOR        0.97
#define POPUPMENU_LIGHT_FACTOR     1.15
#define INACTIVE_HIGHLIGHT_FACTOR  1.20

#define USE_SHADED_MENU_BAR_COLORS (SHADE_CUSTOM==opts.shadeMenubars || SHADE_BLEND_SELECTED==opts.shadeMenubars)
#define MENUBAR_GLASS_SELECTED_DARK_FACTOR 0.9
#define GLASS_BORDER         0.4
#define BEVEL_BORDER(w)      (WIDGET_LISTVIEW_HEADER==w ? 6 : 4)
#define SHADE_BEVEL_TOP      1.07
#define SHADE_BEVEL_MID_TOP  1.03
#define SHADE_BEVEL_MID_BOT  0.975
#define SHADE_BEVEL_BOT(w)   (WIDGET_LISTVIEW_HEADER==(w) ? 0.88 : 0.90)

#define SHADE_INVERTED_TAB_TOP 0.85
#define SHADE_INVERTED_TAB_BOT 1.15

#define NUM_SPLITTER_DASHES 21

#ifdef __cplusplus
#define WIDGET_BUTTON(w) (WIDGET_STD_BUTTON==w || WIDGET_DEF_BUTTON==w || WIDGET_TOGGLE_BUTTON==w || WIDGET_CHECKBOX==w || \
                          WIDGET_COMBO==w || WIDGET_COMBO_BUTTON==w)
#define ETCH_WIDGET(w) (WIDGET_STD_BUTTON==w || WIDGET_DEF_BUTTON==w || WIDGET_TOGGLE_BUTTON==w || WIDGET_SLIDER_TROUGH==w)
#else
#define WIDGET_BUTTON(w) (WIDGET_STD_BUTTON==w || WIDGET_DEF_BUTTON==w || WIDGET_TOGGLE_BUTTON==w || WIDGET_CHECKBOX==w || \
                          WIDGET_COMBO==w || WIDGET_COMBO_BUTTON==w || WIDGET_UNCOLOURED_MO_BUTTON==w)
#define ETCH_WIDGET(w) (WIDGET_STD_BUTTON==w || WIDGET_DEF_BUTTON==w || WIDGET_TOGGLE_BUTTON==w || WIDGET_SLIDER_TROUGH==w || \
                        WIDGET_COMBO==w || WIDGET_UNCOLOURED_MO_BUTTON==w)
#endif
#define COLORED_BORDER_SIZE 3
#define PROGRESS_CHUNK_WIDTH 10
#define QTC_DRAW_LIGHT_BORDER(SUKEN, WIDGET, APP) \
    ((!SUKEN && (IS_GLASS(APP) || customHasLightBorder((Options *)&opts, APP)) && WIDGET_MENU_ITEM!=WIDGET && WIDGET_DEF_BUTTON!=WIDGET) || \
                          (WIDGET_PROGRESSBAR==WIDGET && APPEARANCE_FLAT!=APP && \
                           APPEARANCE_RAISED!=APP && APPEARANCE_INVERTED!=APP && APPEARANCE_BEVELLED!=APP))

#define PROGRESS_ANIMATION 100
#define MIN_SLIDER_SIZE(A) (LINE_DOTS==A ? 24 : 20)

#define QTC_TAB_APP(A)   (APPEARANCE_BEVELLED==A || APPEARANCE_SPLIT_GRADIENT==A \
                            ? APPEARANCE_GRADIENT : A)
#define QTC_NORM_TAB_APP QTC_TAB_APP(opts.tabAppearance)
#define QTC_SEL_TAB_APP  QTC_TAB_APP(opts.activeTabAppearance)

#define QTC_SLIDER_MO_SHADE  (SHADE_SELECTED==opts.shadeSliders ? 1 : (SHADE_BLEND_SELECTED==opts.shadeSliders ? 0 : ORIGINAL_SHADE))
#define QTC_SLIDER_MO_BORDER (SHADE_SELECTED==opts.shadeSliders || SHADE_BLEND_SELECTED==opts.shadeSliders ? 2 : 1)
#define QTC_SLIDER_MO_LEN (SLIDER_TRIANGULAR==opts.sliderStyle ? 2 : (SHADE_SELECTED==opts.shadeSliders || SHADE_BLEND_SELECTED==opts.shadeSliders ? 4 : 3))
#define QTC_SB_SLIDER_MO_LEN(A) ((A)<22 && ROUND_FULL!=opts.round \
                                    ? 2 \
                                    : ((A)<32 || (SHADE_SELECTED!=opts.shadeSliders && SHADE_BLEND_SELECTED!=opts.shadeSliders) \
                                        ? 4 \
                                        : 6))

#define QTC_CR_MO_FILL          1
#define QTC_MO_DEF_BTN          2
#define QTC_MO_PLASTIK_DARK(W)  (WIDGET_DEF_BUTTON==W && IND_COLORED==opts.defBtnIndicator ? 3 : 2) /*? 2 : 1) */
#define QTC_MO_PLASTIK_LIGHT(W) (WIDGET_DEF_BUTTON==W && IND_COLORED==opts.defBtnIndicator ? 4 : 1) /*? 2 : 0) */

#define QTC_MO_STD_DARK(W)     (MO_GLOW==opts.coloredMouseOver \
                                    ? QTC_GLOW_MO \
                                    : QTC_MO_PLASTIK_DARK(W))
#define QTC_MO_STD_LIGHT(W, S) (MO_GLOW==opts.coloredMouseOver \
                                    ? QTC_GLOW_MO \
                                    : QTC_MO_PLASTIK_LIGHT(W))

#define QTC_DO_EFFECT          (ROUND_FULL==opts.round && EFFECT_NONE!=opts.buttonEffect)

#if !defined __cplusplus || (defined QT_VERSION && (QT_VERSION >= 0x040000))
#define QTC_BORDER_BLEND_ALPHA       0.7
#define QTC_ETCH_TOP_ALPHA           0.055
#define QTC_ETCH_BOTTOM_ALPHA        0.80
// #if defined QT_VERSION && (QT_VERSION >= 0x040000)
// #define QTC_ETCH_RADIO_TOP_ALPHA     0.055
// #define QTC_ETCH_RADIO_BOTTOM_ALPHA  0.80
// #else
#define QTC_ETCH_RADIO_TOP_ALPHA     0.09
#define QTC_ETCH_RADIO_BOTTOM_ALPHA  1.0
// #endif
#endif

#if defined __cplusplus && defined QT_VERSION && (QT_VERSION >= 0x040000)

#include <qstyle.h>
typedef enum
{
    QtC_Round = QStyle::PM_CustomBase,
    QtC_TitleBarAppearance
} QtCMetrics;

#define QtC_StateKWin QStyle::State_Top

#endif

typedef enum
{
    EFFECT_NONE,
    EFFECT_ETCH,
    EFFECT_SHADOW
} EEffect;

typedef enum
{
    PIX_RADIO_BORDER,
    PIX_RADIO_LIGHT,
    PIX_RADIO_ON,
    PIX_CHECK,
    PIX_SLIDER,
    PIX_SLIDER_LIGHT,
    PIX_SLIDER_V,
    PIX_SLIDER_LIGHT_V
#if !defined __cplusplus && defined QTC_GTK2_MENU_STRIPE_HACK_MENU
    , PIX_BLANK
#endif
} EPixmap;

typedef enum
{
    WIDGET_TAB_TOP,
    WIDGET_TAB_BOT,
    WIDGET_STD_BUTTON,
    WIDGET_DEF_BUTTON,
    WIDGET_LISTVIEW_HEADER,
    WIDGET_SLIDER,
    WIDGET_SLIDER_TROUGH,
    WIDGET_SB_SLIDER,
    WIDGET_SB_BUTTON,
    WIDGET_TROUGH,
    WIDGET_CHECKBOX,
    WIDGET_TOGGLE_BUTTON,
    WIDGET_COMBO,
    WIDGET_COMBO_BUTTON,
    WIDGET_MENU_ITEM,
    WIDGET_PROGRESSBAR,
    WIDGET_PBAR_TROUGH,
#ifndef __cplusplus
    WIDGET_SPIN_UP,
    WIDGET_SPIN_DOWN,
    WIDGET_UNCOLOURED_MO_BUTTON,
#endif
    WIDGET_SPIN,
#ifdef __cplusplus
    WIDGET_CHECKBUTTON,        // Qt4 only
    WIDGET_MDI_WINDOW,         // Qt4 only
    WIDGET_MDI_WINDOW_TITLE,   // Qt4 only
    WIDGET_MDI_WINDOW_BUTTON,  // Qt4 only
    WIDGET_MENU_BUTTON,        // Qt4 only
    WIDGET_ENTRY,
    WIDGET_FRAME,
    WIDGET_NO_ETCH_BTN,
#endif
#if defined QTC_CONFIG_DIALOG || (defined QT_VERSION && (QT_VERSION >= 0x040000)) || !defined __cplusplus
    WIDGET_SELECTION,
#endif
    WIDGET_FOCUS,
    WIDGET_OTHER
} EWidget;

typedef enum
{
    APPEARANCE_CUSTOM1,
    APPEARANCE_CUSTOM2,
    APPEARANCE_CUSTOM3,
    APPEARANCE_CUSTOM4,
    APPEARANCE_CUSTOM5,
    APPEARANCE_CUSTOM6,
    APPEARANCE_CUSTOM7,
    APPEARANCE_CUSTOM8,
    APPEARANCE_CUSTOM9,
    APPEARANCE_CUSTOM10,

        QTC_NUM_CUSTOM_GRAD,

    APPEARANCE_SUNKEN = QTC_NUM_CUSTOM_GRAD,

    APPEARANCE_FLAT = QTC_NUM_CUSTOM_GRAD,
    APPEARANCE_RAISED,
    APPEARANCE_DULL_GLASS,
    APPEARANCE_SHINY_GLASS,
    APPEARANCE_GRADIENT,
    APPEARANCE_INVERTED,
    APPEARANCE_SPLIT_GRADIENT,
    APPEARANCE_BEVELLED
} EAppearance;

#define IS_SLIDER(W)        (WIDGET_SLIDER==W || WIDGET_SB_SLIDER==W)
#define IS_TOGGLE_BUTTON(W) (WIDGET_TOGGLE_BUTTON==W || WIDGET_CHECKBOX==W)

typedef enum
{
    CORNER_TL = 0x1,
    CORNER_TR = 0x2,
    CORNER_BR = 0x4,
    CORNER_BL = 0x8
} ECornerBits;

#define ROUNDED_NONE            0x0
#define ROUNDED_TOP             (CORNER_TL|CORNER_TR)
#define ROUNDED_BOTTOM          (CORNER_BL|CORNER_BR)
#define ROUNDED_LEFT            (CORNER_TL|CORNER_BL)
#define ROUNDED_RIGHT           (CORNER_TR|CORNER_BR)
#define ROUNDED_TOPRIGHT        CORNER_TR
#define ROUNDED_BOTTOMRIGHT     CORNER_BR
#define ROUNDED_TOPLEFT         CORNER_TL
#define ROUNDED_BOTTOMLEFT      CORNER_BL
#define ROUNDED_ALL             (CORNER_TL|CORNER_TR|CORNER_BR|CORNER_BL)

typedef enum
{
    IND_CORNER,
    IND_FONT_COLOR,
    IND_COLORED,
    IND_TINT,
    IND_GLOW,
    IND_NONE
} EDefBtnIndicator;

typedef enum
{
    LINE_SUNKEN,
    LINE_FLAT,
    LINE_DOTS,
    LINE_DASHES
} ELine;

#define LINE_NONE LINE_DASHES

typedef enum
{
    TB_NONE,
    TB_LIGHT,
    TB_DARK,
    TB_LIGHT_ALL,
    TB_DARK_ALL,
} ETBarBorder;

typedef enum
{
    BORDER_FLAT,
    BORDER_RAISED,
    BORDER_SUNKEN
} EBorder;

/*
    This whole EShade enum is a complete mess!
    For menubars, we dont blend - so blend is selected, and selected is darken
    For check/radios - we dont blend, so blend is selected, and we dont allow darken
*/
typedef enum
{
    SHADE_NONE,
    SHADE_CUSTOM,
    SHADE_BLEND_SELECTED,  /* In the case of check/radios this is SHADE_SELECTED */
      SHADE_SELECTED,
      SHADE_DARKEN = SHADE_SELECTED ,  /* For menubar only! */
} EShade;

typedef enum
{
    ECOLOR_BASE,
    ECOLOR_BACKGROUND,
    ECOLOR_DARK,
} EColor;

typedef enum
{
    ROUND_NONE,
    ROUND_SLIGHT,
    ROUND_FULL
} ERound;

typedef enum
{
    SCROLLBAR_KDE,
    SCROLLBAR_WINDOWS,
    SCROLLBAR_PLATINUM,
    SCROLLBAR_NEXT,
    SCROLLBAR_NONE
} EScrollbar;

typedef enum
{
    MO_NONE,
    MO_COLORED,
    MO_PLASTIK,
    MO_GLOW
} EMouseOver;

typedef enum
{
    STRIPE_NONE,
    STRIPE_PLAIN,
    STRIPE_DIAGONAL
} EStripe;

typedef enum
{
    SLIDER_PLAIN,
    SLIDER_ROUND,
    SLIDER_TRIANGULAR,
} ESliderStyle;

#define DEF_IND_STR                "fontcolor"
#define DEF_LINE_STR               "dots"
#define DEF_TB_BORDER              "none"
#define DEF_APPEARANCE_STR         "bevelled"
#define DEF_MENU_APPEARANCE_STR    "gradient"
#define DEF_TOOLBAR_APPEARANCE_STR "gradient"
#define DEF_SLIDER_SHADE_STR       "selected"
#define DEF_TBS_STR                "dots"
#define DEF_COLOR_STR              "background"
#define DEF_TOOLBAR_SHADE_STR      "none"

#ifdef __cplusplus
inline
#else
static
#endif
bool equal(double d1, double d2)
{
    return (fabs(d1 - d2) < 0.0001);
}

#ifdef QTC_COMMON_FUNCTIONS
#if (!defined QTC_CONFIG_DIALOG)
static double getWidgetShade(EWidget w, bool light, bool sunken, EAppearance app)
{
    switch(w)
    {
        case WIDGET_TROUGH:
            return light ? SHADE_SBAR_LIGHT : SHADE_SBAR_DARK;
        case WIDGET_SLIDER_TROUGH:
            return light ? SHADE_SLIDER_LIGHT : SHADE_SLIDER_DARK;
#if defined QTC_CONFIG_DIALOG || (defined QT_VERSION && (QT_VERSION >= 0x040000)) || !defined __cplusplus
        case WIDGET_SELECTION:
            return light ? SHADE_SELECION_LIGHT : SHADE_SELECION_DARK;
#endif
        case WIDGET_MENU_ITEM:
            if(APPEARANCE_DULL_GLASS!=app && APPEARANCE_SHINY_GLASS!=app)
                return light ? SHADE_BEVEL_MENU_ITEM_LIGHT : SHADE_BEVEL_MENU_ITEM_DARK;
        case WIDGET_PBAR_TROUGH:
            if(APPEARANCE_GRADIENT==app || APPEARANCE_INVERTED==app)
                return light ? SHADE_PBAR_TROUGH_LIGHT : SHADE_PBAR_TROUGH_DARK;
        default:
            return light
                    ? sunken
                        ? SHADE_BEVEL_GRAD_SEL_LIGHT
                        : SHADE_BEVEL_GRAD_LIGHT
                    : sunken
                        ? SHADE_BEVEL_GRAD_SEL_DARK
                        : SHADE_BEVEL_GRAD_DARK;
    }
}
#endif

#define QTC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define QTC_MAX(a, b) ((b) < (a) ? (a) : (b))


/* Taken from rgb->hsl routines taken from KColor
    Copyright 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
*/
static inline double normalize(double a)
{
    return (a < 0.0 ? 0.0 : a > 1.0 ? 1.0 : a);
}

static inline double mix(double a, double b, double k)
{
    return a + ( ( b - a ) * k );
}

static inline double wrap(double a, double d)
{
    register double r = fmod( a, d );
    return ( r < 0.0 ? d + r : ( r > 0.0 ? r : 0.0 ) );
}

static inline double h2c(double h, double m1, double m2)
{
    h = wrap( h, 6.0 );

    if ( h < 1.0 )
        return mix( m1, m2, h );
    if ( h < 3.0 )
        return m2;
    if ( h < 4.0 )
        return mix( m1, m2, 4.0 - h );
    return m1;
}

static inline void rgbToHsl(double r, double g, double b, double *h, double *s, double *l)
{
    double min=QTC_MIN(QTC_MIN(r, g), b),
           max=QTC_MAX(QTC_MAX(r, g), b);

    *l = 0.5 * (max + min);
    *s = 0.0;
    *h = 0.0;

    if (max != min)
    {
        double delta = max - min;

        if ( *l <= 0.5 )
            *s = delta / ( max + min );
        else
            *s = delta / ( 2.0 - max - min );

        if ( r == max )
            *h = ( g - b ) / delta;
        else if ( g == max )
            *h = 2.0 + ( b - r ) / delta;
        else if ( b == max )
            *h = 4.0 + ( r - g ) / delta;

        *h /= 6.0;
        if ( *h < 0.0 )
            (*h) += 1.0;
    }
}

static inline void hslToRgb(double h, double s, double l, double *r, double *g, double *b)
{
    double m1, m2;

    // TODO h2rgb( h, r, g, b );
    h *= 6.0;

    if ( l <= 0.5 )
        m2 = l * ( 1.0 + s );
    else
        m2 = l + s * ( 1.0 - l );
    m1 = 2.0 * l - m2;

    *r = h2c( h + 2.0, m1, m2 );
    *g = h2c( h,       m1, m2 );
    *b = h2c( h - 2.0, m1, m2 );
}

static void rgbToHsv(double r, double g, double b, double *h, double *s, double *v)
{
    double min=QTC_MIN(QTC_MIN(r, g), b),
           max=QTC_MAX(QTC_MAX(r, g), b),
           delta=max - min;

    *v=max;
    if(max != 0)
        *s=delta / max;
    else
    {
        /* r=g=b=0                  s=0, v is undefined */
        *s=0;
        *h=-1;
        return;
    }
    if(r == max)
        *h=(g - b) / delta;         /* between yellow & magenta */
    else if(g == max)
        *h=2 + (b - r) / delta;     /* between cyan & yellow */
    else
        *h=4 + (r - g) / delta;     /* between magenta & cyan */
    *h *= 60;                       /* degrees */
    if(*h < 0)
        *h += 360;
}

static void hsvToRgb(double *r, double *g, double *b, double h, double s, double v)
{
    if(0==s)
        *r=*g=*b=v;
    else
    {
        int    i;
        double f,
               p;

        h /= 60;                      /* sector 0 to 5 */
        i=(int)floor(h);
        f=h - i;                      /* factorial part of h */
        p=v * (1 - s);
        switch(i)
        {
            case 0:
                *r=v;
                *g=v * (1 - s * (1 - f));
                *b=p;
                break;
            case 1:
                *r=v * (1 - s * f);
                *g=v;
                *b=p;
                break;
            case 2:
                *r=p;
                *g=v;
                *b=v * (1 - s * (1 - f));
                break;
            case 3:
                *r=p;
                *g=v * (1 - s * f);
                *b=v;
                break;
            case 4:
                *r=v * (1 - s * (1 - f));
                *g=p;
                *b=v;
                break;
            /* case 5: */
            default:
                *r=v;
                *g=p;
                *b=v * (1 - s * f);
                break;
        }
    }
}

#ifdef __cplusplus
inline int limit(double c)
{
    return c < 0.0 ? 0 : (c > 255.0  ? 255 : (int)c);
}
#else
inline int limit(double c)
{
    return c < 0.0
               ? 0
               : c > 65535.0
                     ? 65535
                     : (int)c;
}
#endif

#ifdef __cplusplus
static void shade(const color &ca, color *cb, double k)
#else
static void shade(const color *ca, color *cb, double k)
#endif
{
    if(equal(k, 1.0))
    {
#ifdef __cplusplus
        *cb=ca;
#else
        cb->red = ca->red;
        cb->green = ca->green;
        cb->blue = ca->blue;
#endif
    }
    else
        switch(shading)
        {
            case SHADING_SIMPLE:
            {
    #ifdef __cplusplus
                int v=(int)(255.0*(k-1.0));

                cb->setRgb(limit(ca.red()+v), limit(ca.green()+v), limit(ca.blue()+v));
    #else
                double v=65535.0*(k-1.0);

                cb->red = limit(ca->red+v);
                cb->green = limit(ca->green+v);
                cb->blue = limit(ca->blue+v);
    #endif
                break;
            }
            case SHADING_HSL:
            {
    #ifdef __cplusplus
                double r(ca.red()/255.0),
                       g(ca.green()/255.0),
                       b(ca.blue()/255.0);
    #else
                double r=ca->red/65535.0,
                       g=ca->green/65535.0,
                       b=ca->blue/65535.0;
    #endif
                double h, s, l;

                rgbToHsl(r, g, b, &h, &s, &l);
                l=normalize(l*k);
                s=normalize(s*k);
                hslToRgb(h, s, l, &r, &g, &b);
    #ifdef __cplusplus
                cb->setRgb(limit(r*255.0), limit(g*255.0), limit(b*255.0));
    #else
                cb->red=limit(r*65535.0);
                cb->green=limit(g*65535.0);
                cb->blue=limit(b*65535.0);
    #endif
                break;
            }
            case SHADING_HSV:
            {
    #ifdef __cplusplus
                double r(ca.red()/255.0),
                       g(ca.green()/255.0),
                       b(ca.blue()/255.0);
    #else
                double r=ca->red/65535.0,
                       g=ca->green/65535.0,
                       b=ca->blue/65535.0;
    #endif
                double h, s, v;

                rgbToHsv(r, g, b, &h, &s, &v);

                v*=k;
                if (v > 1.0)
                {
                    s -= v - 1.0;
                    if (s < 0)
                        s = 0;
                    v = 1.0;
                }
                hsvToRgb(&r, &g, &b, h, s, v);
    #ifdef __cplusplus
                cb->setRgb(limit(r*255.0), limit(g*255.0), limit(b*255.0));
    #else
                cb->red=limit(r*65535.0);
                cb->green=limit(g*65535.0);
                cb->blue=limit(b*65535.0);
    #endif
            }
        }
}

#if (!defined QTC_CONFIG_DIALOG)
static unsigned char checkBounds(int num)
{
    return num < 0   ? 0   :
           num > 255 ? 255 :
                       num;
}

static void adjustPix(unsigned char *data, int numChannels, int w, int h, int stride,
                      int ro, int go, int bo, double shade)
{
    int width=w*numChannels,
        offset=0,
        row,
        r=(int)((ro*shade)+0.5),
        g=(int)((go*shade)+0.5),
        b=(int)((bo*shade)+0.5);

    for(row=0; row<h; ++row)
    {
        int column;

        for(column=0; column<width; column+=numChannels)
        {
            unsigned char source=data[offset+column+1];

#if defined  __cplusplus
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
            /* ARGB */
            data[offset+column+1] = checkBounds(r-source);
            data[offset+column+2] = checkBounds(g-source);
            data[offset+column+3] = checkBounds(b-source);
#else
            /* BGRA */
            data[offset+column] = checkBounds(b-source);
            data[offset+column+1] = checkBounds(g-source);
            data[offset+column+2] = checkBounds(r-source);
#endif
#else
            /* GdkPixbuf is RGBA */
            data[offset+column] = checkBounds(r-source);
            data[offset+column+1] = checkBounds(g-source);
            data[offset+column+2] = checkBounds(b-source);
#endif

        }
        offset+=stride;
    }
}
#endif
#endif /* QTC_COMMON_NO_FUNCTIONS */

#ifdef __cplusplus
struct Gradient
#else
typedef struct
#endif
{
#ifdef __cplusplus
    Gradient(double p=0.0, double v=0.0) : pos(p), val(v) { }

    bool operator==(const Gradient &o) const
    {
        return equal(pos, o.pos) && equal(val, o.val);
    }

    bool operator<(const Gradient &o) const
    {
        return pos<o.pos || (equal(pos, o.pos) && val<o.val);
    }
#endif

    double pos,
           val;

}
#ifndef __cplusplus
Gradient
#endif
;

#ifdef __cplusplus
typedef std::set<Gradient> GradientCont;
struct CustomGradient
#else
typedef struct
#endif
{
#ifdef __cplusplus
    CustomGradient() : lightBorder(false) { }

    CustomGradient & operator=(const CustomGradient &o)
    {
        if(&o!=this)
        {
            grad=o.grad;
            lightBorder=o.lightBorder;
        }

        return *this;
    }
#ifdef QTC_CONFIG_DIALOG
    bool operator==(const CustomGradient &o) const
    {
        return lightBorder==o.lightBorder && grad==o.grad;
    }
#endif
#endif
    bool         lightBorder;
#ifdef __cplusplus
    GradientCont grad;
#else
    int          numGrad;
    Gradient     *grad;
#endif
}
#ifndef __cplusplus
CustomGradient
#endif
;

#ifdef __cplusplus
typedef std::map<EAppearance, CustomGradient> CustomGradientCont;
typedef std::vector<double> ShadesCont;
struct Options
#else
typedef double * ShadesCont;
typedef struct
#endif
{
#ifdef __cplusplus
    Options & operator=(const Options &o)
    {
        if(&o!=this)
            customGradient=o.customGradient;

        return *this;
    }

#endif
    int              contrast,
                     passwordChar;
    double           highlightFactor;
    ERound           round;
    bool             embolden,
                     lighterPopupMenuBgnd,
                     highlightTab,
                     colorSelTab,
                     animatedProgress,
                     fixParentlessDialogs,
                     customMenuTextColor,
                     menubarMouseOver,
                     shadeMenubarOnlyWhenActive,
                     thinnerMenuItems,
#ifndef QTC_PLAIN_FOCUS_ONLY
                     stdFocus,
#endif
                     lvLines,
                     drawStatusBarFrames,
                     fillSlider,
                     roundMbTopOnly,
                     gtkScrollViews,
#ifdef __cplusplus
                     stdSidebarButtons,
                     gtkComboMenus,
/*
#else
                     setDialogButtonOrder,
*/
#endif
#if !defined __cplusplus || defined QTC_CONFIG_DIALOG
                     mapKdeIcons,
#endif
#if defined QTC_CONFIG_DIALOG || (defined QT_VERSION && (QT_VERSION >= 0x040000)) || !defined __cplusplus
                     gtkButtonOrder,
#endif
                     borderMenuitems,
                     colorMenubarMouseOver,
                     darkerBorders,
                     vArrows,
                     xCheck,
                     framelessGroupBoxes,
#if defined __cplusplus || defined QTC_GTK2_MENU_STRIPE
                     menuStripe,
#endif
                     inactiveHighlight,
                     crHighlight,
                     fillProgress,
                     comboSplitter,
                     squareScrollViews,
                     highlightScrollViews,
                     sunkenScrollViews;
    EStripe          stripedProgress;
    ESliderStyle     sliderStyle;
    EMouseOver       coloredMouseOver;
    ETBarBorder      toolbarBorders;
    EDefBtnIndicator defBtnIndicator;
    ELine            sliderThumbs,
                     handles,
                     toolbarSeparators,
                     splitters;
    EAppearance      appearance,
                     menubarAppearance,
                     menuitemAppearance,
                     toolbarAppearance,
                     lvAppearance,
                     tabAppearance,
                     activeTabAppearance,
                     sliderAppearance,
#ifdef __cplusplus
                     titlebarAppearance,
#endif
#if defined QTC_CONFIG_DIALOG || (defined QT_VERSION && (QT_VERSION >= 0x040000)) || !defined __cplusplus
                     selectionAppearance,
#endif
#if defined __cplusplus || defined QTC_GTK2_MENU_STRIPE
                     menuStripeAppearance,
#endif
                     progressAppearance,
                     progressGrooveAppearance;
    EShade           shadeSliders,
                     shadeMenubars,
                     shadeCheckRadio;
    EColor           progressGrooveColor;
    EEffect          buttonEffect;
    EScrollbar       scrollbarType;
    color            customMenubarsColor,
                     customSlidersColor,
                     customMenuNormTextColor,
                     customMenuSelTextColor,
                     customCheckRadioColor;
#ifndef __cplusplus
    bool             newFirefox,
                     newThunderbird;
#endif
    #ifdef QTC_CONFIG_DIALOG
    EShading         shading;
    #endif
#ifdef __cplusplus
    CustomGradientCont customGradient;
#else
    CustomGradient     *customGradient[QTC_NUM_CUSTOM_GRAD+1];
#endif
    ShadesCont       customShades;
#ifndef __cplusplus
} Options;
#else
};
#endif

#if defined QTC_COMMON_FUNCTIONS && !defined QTC_CONFIG_DIALOG
static bool customHasLightBorder(Options *opts, EAppearance app)
{
    if(IS_CUSTOM(app))
#ifdef __cplusplus
        return opts->customGradient[app].lightBorder;
#else
        if(opts->customGradient[app])
            return opts->customGradient[app]->lightBorder;
#endif

    return false;
}

static EAppearance widgetApp(EWidget w, const Options *opts)
{
    switch(w)
    {
        case WIDGET_LISTVIEW_HEADER:
            return opts->lvAppearance;
        case WIDGET_SB_BUTTON:
        case WIDGET_SLIDER:
        case WIDGET_SB_SLIDER:
            return opts->sliderAppearance;
        case WIDGET_TAB_TOP:
        case WIDGET_TAB_BOT:
            return opts->tabAppearance;
        case WIDGET_MENU_ITEM:
            return opts->menuitemAppearance;
        case WIDGET_PROGRESSBAR:
            return opts->progressAppearance;
        case WIDGET_PBAR_TROUGH:
            return opts->progressGrooveAppearance;
#if defined QTC_CONFIG_DIALOG || (defined QT_VERSION && (QT_VERSION >= 0x040000)) || !defined __cplusplus
        case WIDGET_SELECTION:
            return opts->selectionAppearance;
#endif
#ifdef __cplusplus
        case WIDGET_MDI_WINDOW:
        case WIDGET_MDI_WINDOW_TITLE:
        case WIDGET_MDI_WINDOW_BUTTON:
            return opts->titlebarAppearance;
#endif
        case WIDGET_SLIDER_TROUGH:
            return APPEARANCE_FLAT==opts->appearance || APPEARANCE_RAISED==opts->appearance
                    ? APPEARANCE_FLAT : APPEARANCE_GRADIENT;
        default:
            break;
    }

    return opts->appearance;
};

#if !defined __cplusplus || (defined QT_VERSION && (QT_VERSION >= 0x040000))

#if defined __cplusplus
#define QTC_FULL_INNER_RADIUS   1.5
#define QTC_FULL_OUTER_RADIUS   2.5
#define QTC_FULL_ETCH_RADIUS    3.5
#define QTC_SLIGHT_INNER_RADIUS 0.5
#define QTC_SLIGHT_OUTER_RADIUS 1.5
#define QTC_SLIGHT_ETCH_RADIUS  2.5
#else
#define QTC_FULL_INNER_RADIUS   2
#define QTC_FULL_OUTER_RADIUS   3
#define QTC_FULL_ETCH_RADIUS    4
#define QTC_SLIGHT_INNER_RADIUS 0.5
#define QTC_SLIGHT_OUTER_RADIUS 1.5
#define QTC_SLIGHT_ETCH_RADIUS  2.5
#endif

typedef enum
{
    RADIUS_INTERNAL,
    RADIUS_EXTERNAL,
    RADIUS_ETCH
} ERadius;

static double getRadius(ERound r, int w, int h, EWidget widget, ERadius rad)
{
    if((WIDGET_CHECKBOX==widget || WIDGET_FOCUS==widget) && ROUND_NONE!=r)
        r=ROUND_SLIGHT;

    switch(rad)
    {
        case RADIUS_INTERNAL:
            switch(r)
            {
                case ROUND_FULL:
                    if(w>QTC_MIN_BTN_SIZE && h>QTC_MIN_BTN_SIZE)
                        return QTC_FULL_INNER_RADIUS;
                case ROUND_SLIGHT:
                    return QTC_SLIGHT_INNER_RADIUS;
                case ROUND_NONE:
                    return 0;
            }
        case RADIUS_EXTERNAL:
            switch(r)
            {
                case ROUND_FULL:
                    if(w>QTC_MIN_BTN_SIZE && h>QTC_MIN_BTN_SIZE)
                        return QTC_FULL_OUTER_RADIUS;
                case ROUND_SLIGHT:
                    return QTC_SLIGHT_OUTER_RADIUS;
                case ROUND_NONE:
                    return 0;
            }
        case RADIUS_ETCH:
            switch(r)
            {
                case ROUND_FULL:
                    if(w>QTC_MIN_BTN_SIZE && h>QTC_MIN_BTN_SIZE)
                        return QTC_FULL_ETCH_RADIUS;
                case ROUND_SLIGHT:
                    return QTC_SLIGHT_ETCH_RADIUS;
                case ROUND_NONE:
                    return 0;
            }
    }

    return 0;
}
#endif

#endif

#endif
