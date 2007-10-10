#ifndef __QTCURVE_H__
#define __QTCURVE_H__

/*
  QtCurve (C) Craig Drummond, 2007 Craig.Drummond@lycos.co.uk

  ----

  This program is free software; you can redistribute it and/or
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

#define QTC_INT_CACHE_KEY  // Use 64bit ints as cache index - should be faster than strings!

#include <QProgressBar>
#include <QTime>
#include <QPalette>
#include <QMap>
#include <QCache>
#include <QStyleOption>
#ifdef QTC_INT_CACHE_KEY
#include <Q_UINT64>
typedef qulonglong QtcKey;
#else
typedef QString QtcKey;
#endif
#include "common.h"

// #ifdef QTC_KSTYLE
// #include <kstyle.h>
// #define QTC_BASE_STYLE KStyle
// #else
#include <QWindowsStyle>
#define QTC_BASE_STYLE QWindowsStyle
// #endif

class QStyleOptionSlider;

class QtCurveStyle : public QWindowsStyle
{
    Q_OBJECT

    enum EBlend
    {
        BLEND_NONE,
        BLEND_BGND,
        BLEND_BASE
    };

    public:

    QtCurveStyle(const QString &name=QString());
    ~QtCurveStyle();

    void polish(QApplication *app);
    void polish(QPalette &palette);
    void polish(QWidget *widget);
    void unpolish(QWidget *widget);
    bool eventFilter(QObject *object, QEvent *event);
    void timerEvent(QTimerEvent *event);
    int pixelMetric(PixelMetric metric, const QStyleOption *option=0, const QWidget *widget=0) const;
    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData=0) const;
    QPalette standardPalette() const;
    QPixmap standardPixmap(StandardPixmap pix, const QStyleOption *opttion, const QWidget *widget) const;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawControl(ControlElement control, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const;
    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const;
    QRect subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const;
    SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                     const QPoint &pos, const QWidget *widget) const;

    private:

    void drawBevelGradient(const QColor &base, bool increase, QPainter *p, QRect const &r,
                           bool horiz, double shadeTop, double shadeBot, bool sel, EAppearance bevApp,
                           EWidget w=WIDGET_OTHER) const;
    void drawGradient(const QColor &top, const QColor &bot, bool increase, QPainter *p,
                      const QRect &r, bool horiz=true) const;
    void drawLightBevel(QPainter *p, const QRect &r, const QStyleOption *option, int round, const QColor &fill,
                        const QColor *custom=0, bool doBorder=true, EWidget w=WIDGET_OTHER) const;
    void drawEtch(QPainter *p, const QRect &r, /*const QStyleOption *option, */bool top, bool bot, bool raised=false) const;
    void drawBorder(QPainter *p, const QRect &r, const QStyleOption *option,
                    int round, const QColor *custom=0, EWidget w=WIDGET_OTHER, EBorder borderProfile=BORDER_FLAT,
                    EBlend blend=BLEND_BGND, int borderVal=QT_STD_BORDER) const;
    void drawMdiButton(QPainter *painter, const QRect &r, bool hover, bool sunken, const QColor *cols) const;
    void drawEntryField(QPainter *p, const QRect &rx, const QStyleOption *option, int round, EWidget w=WIDGET_OTHER) const;
    void drawMenuItem(QPainter *p, const QRect &r, const QStyleOption *option, bool mbi, int round, const QColor *cols) const;
    void drawProgress(QPainter *p, const QRect &r, const QStyleOption *option, int round, bool vertical=false, bool reverse=false) const;
    void drawArrow(QPainter *p, const QRect &r, PrimitiveElement pe, const QColor &col, bool small=false) const;
    void drawArrow(QPainter *p, const QRect &r, const QStyleOption *option, PrimitiveElement pe, bool small=false, bool checkActive=false) const;
    void drawSbSliderHandle(QPainter *p, const QRect &r, const QStyleOption *option, bool slider=false) const;
    void drawSliderHandle(QPainter *p, const QRect &r, const QStyleOptionSlider *option) const;
    void drawSliderGroove(QPainter *p, const QRect &groove, const QRect &handle, const QStyleOptionSlider *slider) const;
    void drawMenuOrToolBarBackground(QPainter *p, const QRect &r, const QStyleOption *option, bool menu=true, bool horiz=true) const;
    void drawHandleMarkers(QPainter *p, const QRect &r, const QStyleOption *option, bool tb, ELine handles) const;
    void fillTab(QPainter *p, const QRect &r, const QStyleOption *option, const QColor &fill, bool horiz, bool increase, EWidget tab) const;
    void shadeColors(const QColor &base, QColor *vals) const;
    const QColor * buttonColors(const QStyleOption *option) const;
    const QColor * sliderColors(const QStyleOption *option) const;
    const QColor * backgroundColors(const QColor &col) const;
    const QColor * backgroundColors(const QStyleOption *option) const
        { return backgroundColors(option->palette.background().color()); }
    const QColor * borderColors(const QStyleOption *option, const QColor *use) const;
    const QColor * getSidebarButtons() const;
    void setMenuColors(const QColor &bgnd);
    const QColor * getMdiColors(const QStyleOptionTitleBar *option) const;
    const QColor & getFill(const QStyleOption *option, const QColor *use) const;
    const QColor & getTabFill(bool current, bool highlight, const QColor *use) const;
    QPixmap *      getPixmap(const QColor col, EPixmap p, double shade=1.0) const;

    private Q_SLOTS:

    void           widgetDestroyed(QObject *o);

    private:

    Options                            opts;
    QColor                             itsMenuitemCols[TOTAL_SHADES+1],
                                       itsBackgroundCols[TOTAL_SHADES+1],
                                       itsMenubarCols[TOTAL_SHADES+1],
                                       *itsSliderCols,
                                       *itsDefBtnCols,
                                       *itsMouseOverCols,
                                       itsButtonCols[TOTAL_SHADES+1],
                                       itsLighterPopupMenuBgndCol,
                                       itsCheckRadioCol;
    mutable QColor                     *itsSidebarButtonsCols;
    mutable QColor                     *itsActiveMdiColors;
    mutable QColor                     *itsMdiColors;
    mutable QColor                     itsActiveMdiTextColor;
    mutable QColor                     itsMdiTextColor;
    mutable QColor                     itsColoredButtonCols[TOTAL_SHADES+1];
    mutable QColor                     itsColoredBackgroundCols[TOTAL_SHADES+1];
    mutable QCache<QtcKey, QPixmap>    itsPixmapCache;
    mutable bool                       itsActive;
    mutable const QWidget              *itsSbWidget;
    QList<QProgressBar *>              itsProgressBars;
    int                                itsProgressBarAnimateTimer,
                                       itsAnimateStep;
    QTime                              itsTimer;
    mutable QMap<QWidget *, QWidget *> itsReparentedDialogs;
    // Required for Q3Header hover...
    QPoint                             itsPos;
    QWidget                            *itsHoverWidget;
};

#endif
