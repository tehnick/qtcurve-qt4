/*
  QtCurve KWin window decoration
  Copyright (C) 2007 - 2009 Craig Drummond <craig_p_drummond@yahoo.co.uk>

  based on the window decoration "Plastik":
  Copyright (C) 2003-2005 Sandro Giessl <sandro@giessl.com>

  based on the window decoration "Web":
  Copyright (C) 2001 Rik Hemsley (rikkus) <rik@kde.org>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
 */

#include <klocale.h>
#include <QBitmap>
#include <QDateTime>
#include <QFontMetrics>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QPixmap>
#include <QStyleOptionTitleBar>
#include <QStyle>
#include <qdesktopwidget.h>
#include "qtcurvehandler.h"
#include "qtcurveclient.h"
#include "qtcurvebutton.h"
#define QTC_KWIN
#include "common.h"

namespace KWinQtCurve
{

QtCurveClient::QtCurveClient(KDecorationBridge *bridge, KDecorationFactory *factory)
#if KDE_IS_VERSION(4,1,80)
             : KCommonDecorationUnstable(bridge, factory),
#else
             : KCommonDecoration(bridge, factory),
#endif
               itsTitleFont(QFont())
{
}

QString QtCurveClient::visibleName() const
{
    return i18n("QtCurve");
}

bool QtCurveClient::decorationBehaviour(DecorationBehaviour behaviour) const
{
    switch (behaviour)
    {
        case DB_MenuClose:
            return Handler()->menuClose();
        case DB_WindowMask:
            return false;
        default:
            return KCommonDecoration::decorationBehaviour(behaviour);
    }
}

int QtCurveClient::layoutMetric(LayoutMetric lm, bool respectWindowState,
                                const KCommonDecorationButton *btn) const
{
    bool maximized(maximizeMode()==MaximizeFull && !options()->moveResizeMaximizedWindows());

    switch (lm)
    {
        case LM_BorderLeft:
        case LM_BorderRight:
        case LM_BorderBottom:
            return respectWindowState && maximized ? 0 : Handler()->borderSize();
        case LM_TitleEdgeTop:
            return respectWindowState && maximized ? 0 : 3;
        case LM_TitleEdgeBottom:
            return /*respectWindowState && maximized ? 1 : */ 3;
        case LM_TitleEdgeLeft:
        case LM_TitleEdgeRight:
            return respectWindowState && maximized ? 0 : 3;
        case LM_TitleBorderLeft:
        case LM_TitleBorderRight:
            return 5;
        case LM_ButtonWidth:
        case LM_ButtonHeight:
        case LM_TitleHeight:
            return respectWindowState && isToolWindow() ? Handler()->titleHeightTool() : Handler()->titleHeight();
        case LM_ButtonSpacing:
            return 0;
        case LM_ButtonMarginTop:
            return 0;
        default:
            return KCommonDecoration::layoutMetric(lm, respectWindowState, btn);
    }
}

KCommonDecorationButton *QtCurveClient::createButton(ButtonType type)
{
    switch (type)
    {
        case MenuButton:
        case OnAllDesktopsButton:
        case HelpButton:
        case MinButton:
        case MaxButton:
        case CloseButton:
        case AboveButton:
        case BelowButton:
        case ShadeButton:
            return new QtCurveButton(type, this);
        default:
            return 0;
    }
}

void QtCurveClient::init()
{
    itsTitleFont = isToolWindow() ? Handler()->titleFontTool() : Handler()->titleFont();

    KCommonDecoration::init();
    widget()->setAutoFillBackground(false);
    widget()->setAttribute(Qt::WA_OpaquePaintEvent);
    widget()->setAttribute(Qt::WA_PaintOnScreen, false);
}

void QtCurveClient::drawBtnBgnd(QPainter *p, const QRect &r, bool active)
{
    int    state(active ? 1 : 0);
    QColor col(KDecoration::options()->color(KDecoration::ColorTitleBar, active));
    bool   diffSize(itsButtonBackground[state].pix.width()!=r.width() ||
                    itsButtonBackground[state].pix.height()!=r.height());
    int    app(Handler()->wStyle()->pixelMetric((QStyle::PixelMetric)QtC_TitleBarButtonAppearance, NULL, NULL));

    if(diffSize || itsButtonBackground[state].col!=col || itsButtonBackground[state].app!=app)
    {
        if(diffSize)
            itsButtonBackground[state].pix=QPixmap(r.width(), r.height());

        QRect                br(r);
        QStyleOptionTitleBar opt;
        QPainter             pixPainter(&(itsButtonBackground[state].pix));

        br.adjust(-3, -3, 3, 3);
        opt.rect=br;

        opt.state=QStyle::State_Horizontal|QStyle::State_Enabled|QStyle::State_Raised|
                 (active ? QStyle::State_Active : QStyle::State_None);
        opt.titleBarState=(active ? QStyle::State_Active : QStyle::State_None);
        opt.palette.setColor(QPalette::Button, col);
        Handler()->wStyle()->drawComplexControl(QStyle::CC_TitleBar, &opt, &pixPainter, widget());
        itsButtonBackground[state].col=col;
        itsButtonBackground[state].app=app;
    }

    p->drawPixmap(r, itsButtonBackground[state].pix);
}

void QtCurveClient::paintEvent(QPaintEvent *e)
{
    QPainter             painter(widget());
    QRect                r(widget()->rect());
    QStyleOptionTitleBar opt;
    bool                 active(isActive()),
                         colorTitleOnly(Handler()->wStyle()->pixelMetric((QStyle::PixelMetric)QtC_TitleBarColorTopOnly,
                                        NULL, NULL)),
                         mximised(maximizeMode()==MaximizeFull && !options()->moveResizeMaximizedWindows());
    const int            maximiseOffset(mximised ? 3 : 0),
                         titleHeight(layoutMetric(LM_TitleHeight)),
                         titleEdgeTop(layoutMetric(LM_TitleEdgeTop)),
                         titleEdgeBottom(layoutMetric(LM_TitleEdgeBottom)),
                         titleEdgeLeft(layoutMetric(LM_TitleEdgeLeft)),
                         titleEdgeRight(layoutMetric(LM_TitleEdgeRight)),
                         titleBarHeight(titleHeight+titleEdgeTop+titleEdgeBottom+maximiseOffset),
                         borderSize(Handler()->borderSize()),
                         round=Handler()->wStyle()->pixelMetric((QStyle::PixelMetric)QtC_Round, NULL, NULL);
    int                  rectX, rectY, rectX2, rectY2;

    r.getCoords(&rectX, &rectY, &rectX2, &rectY2);

    QColor    col(KDecoration::options()->color(KDecoration::ColorTitleBar, active)),
              windowCol(widget()->palette().color(QPalette::Window));

#if KDE_IS_VERSION(4,1,80)
    if(!(Handler()->coloredShadow() && shadowsActive() && active))
#endif
    {
        painter.setClipRegion(e->region());
        painter.fillRect(r, windowCol); // Makes hings look nicer for kcmshell preview...
    }
    painter.setClipRegion(e->region().intersected(getMask(round, r.width(), r.height())));
    painter.fillRect(r, colorTitleOnly ? windowCol : col);

    if(ROUND_FULL==round)
    {
        QColor cornerCol(colorTitleOnly ? windowCol : col);
        painter.setPen(windowCol);
        painter.drawRect(r.x()+borderSize-1, r.y()+borderSize-1,
                         r.x()+r.width()-((borderSize*2)-1), r.y()+r.height()-((borderSize*2)-1));
        painter.setPen(cornerCol);
        painter.drawPoint(r.x()+borderSize-1, r.y()+r.height()-(borderSize));
        painter.drawPoint(r.x()+r.width()-borderSize, r.y()+r.height()-borderSize);
        cornerCol.setAlphaF(0.5);
        painter.setPen(cornerCol);
        painter.drawPoint(r.x()+borderSize, r.y()+r.height()-(borderSize));
        painter.drawPoint(r.x()+borderSize-1, r.y()+r.height()-(borderSize+1));
        painter.drawPoint(r.x()+r.width()-borderSize-1, r.y()+r.height()-borderSize);
        painter.drawPoint(r.x()+r.width()-borderSize, r.y()+r.height()-(borderSize+1));
    }

    opt.init(widget());

    if(mximised)
        r.adjust(-3, -3, 3, 0);
    opt.palette.setColor(QPalette::Button, col);
    opt.palette.setColor(QPalette::Window, windowCol);
    opt.rect=QRect(r.x(), r.y()+6, r.width(), r.height()-6);
    opt.state=QStyle::State_Horizontal|QStyle::State_Enabled|QStyle::State_Raised|
             (active ? QStyle::State_Active : QStyle::State_None)|QtC_StateKWin;

#if KDE_IS_VERSION(4,1,80)
    if(Handler()->coloredShadow() && shadowsActive())
    {
        opt.state|=QtC_StateKWinShadows;
        if(active)
            opt.state|=QtC_StateKWinHighlight;
    }
#endif
    Handler()->wStyle()->drawPrimitive(QStyle::PE_FrameWindow, &opt, &painter, widget());

    opt.palette.setColor(QPalette::Button, col);
    opt.rect=QRect(r.x(), r.y(), r.width(), titleBarHeight);
    opt.titleBarState=(active ? QStyle::State_Active : QStyle::State_None)|QtC_StateKWin;
    if(KDecoration::options()->color(KDecoration::ColorTitleBar, true)!=windowCol ||
       KDecoration::options()->color(KDecoration::ColorTitleBar, false)!=windowCol)
       opt.titleBarState|=QtCStateKWinDrawLine;
    Handler()->wStyle()->drawComplexControl(QStyle::CC_TitleBar, &opt, &painter, widget());

    itsCaptionRect = captionRect(); // also update itsCaptionRect!

    if(!caption().isEmpty())
    {
        painter.setFont(itsTitleFont);

        QString       str(painter.fontMetrics().elidedText(caption(), Qt::ElideRight,
                                                           itsCaptionRect.width(), QPalette::WindowText));
        Qt::Alignment hAlign((Qt::Alignment)Handler()->wStyle()->pixelMetric((QStyle::PixelMetric)QtC_TitleAlignment,
                                                                                 0L, 0L)),
                      alignment(Qt::AlignVCenter|hAlign);
        const int     titleEdgeBottomBottom(rectY+titleEdgeTop+titleHeight+titleEdgeBottom);
        QRect         textRect(Qt::AlignHCenter==hAlign
                                    ? QRect(rectX+titleEdgeLeft, rectY+titleEdgeTop,
                                            rectX2-titleEdgeRight-(rectX+titleEdgeLeft),
                                            titleEdgeBottomBottom-(rectY+titleEdgeTop))
                                    : itsCaptionRect);

        painter.setClipRect(itsCaptionRect);
        painter.setPen(shadowColor(KDecoration::options()->color(KDecoration::ColorFont, active)));
        painter.drawText(textRect.adjusted(1, 1, 1, 1), alignment, str);
        painter.setPen(KDecoration::options()->color(KDecoration::ColorFont, active));
        painter.drawText(textRect, alignment, str);
    }
    painter.end();
}

void QtCurveClient::updateWindowShape()
{
    setMask(getMask(Handler()->wStyle()->pixelMetric((QStyle::PixelMetric)QtC_Round, NULL, NULL),
                    widget()->width(), widget()->height(),
                    maximizeMode()==MaximizeFull && !options()->moveResizeMaximizedWindows()));
}

QRegion QtCurveClient::getMask(int round, int w, int h, bool maximised) const
{  
    switch(maximised ? ROUND_NONE : round)
    {
        case ROUND_NONE:
            return  QRegion(0, 0, w, h);
        case ROUND_SLIGHT:
        {
            QRegion mask(1, 0, w-2, h);
            mask += QRegion(0, 1, 1, h-2);
            mask += QRegion(w-1, 1, 1, h-2);

            return mask;
        }
        default: // ROUND_FULL
        {
            QRegion mask(5, 0, w-10, h);

            mask += QRegion(0, 5, 1, h-10);
            mask += QRegion(1, 3, 1, h-6);
            mask += QRegion(2, 2, 1, h-4);
            mask += QRegion(3, 1, 2, h-2);

            mask += QRegion(w-1, 5, 1, h-10);
            mask += QRegion(w-2, 3, 1, h-6);
            mask += QRegion(w-3, 2, 1, h-4);
            mask += QRegion(w-5, 1, 2, h-2);

            return mask;
        }
    }

    return QRegion();
}

QRect QtCurveClient::captionRect() const
{
    QRect     r(widget()->rect());
    const int titleHeight(layoutMetric(LM_TitleHeight)),
              titleEdgeTop(layoutMetric(LM_TitleEdgeTop)),
              titleEdgeLeft(layoutMetric(LM_TitleEdgeLeft)),
              marginLeft(layoutMetric(LM_TitleBorderLeft)),
              marginRight(layoutMetric(LM_TitleBorderRight)),
              titleLeft(r.left() + titleEdgeLeft + buttonsLeftWidth() + marginLeft),
              titleWidth(r.width() -
                           titleEdgeLeft - layoutMetric(LM_TitleEdgeRight) -
                           buttonsLeftWidth() - buttonsRightWidth() -
                           marginLeft - marginRight);

    return QRect(titleLeft, r.top()+titleEdgeTop, titleWidth, titleHeight);
}

void QtCurveClient::updateCaption()
{
    QRect oldCaptionRect(itsCaptionRect);

    itsCaptionRect = QtCurveClient::captionRect();

    if (oldCaptionRect.isValid() && itsCaptionRect.isValid())
        widget()->update(oldCaptionRect|itsCaptionRect);
    else
        widget()->update();
}

bool QtCurveClient::eventFilter(QObject *o, QEvent *e)
{
    if(QEvent::StyleChange==e->type())
        Handler()->setStyle();

    return KCommonDecoration::eventFilter(o, e);
}

#if KDE_IS_VERSION(4,1,80)
// Taken form Oxygen! rev873805
QList<QRect> QtCurveClient::shadowQuads(ShadowType type) const
{
    Q_UNUSED(type)

    QSize size = widget()->size();
    int outside=20, underlap=5, cornersize=25;
    // These are underlap under the decoration so the corners look nicer 10px on the outside
    QList<QRect> quads;
    quads.append(QRect(-outside, size.height()-underlap, cornersize, cornersize));
    quads.append(QRect(underlap, size.height()-underlap, size.width()-2*underlap, cornersize));
    quads.append(QRect(size.width()-underlap, size.height()-underlap, cornersize, cornersize));
    quads.append(QRect(-outside, underlap, cornersize, size.height()-2*underlap));
    quads.append(QRect(size.width()-underlap, underlap, cornersize, size.height()-2*underlap));
    quads.append(QRect(-outside, -outside, cornersize, cornersize));
    quads.append(QRect(underlap, -outside, size.width()-2*underlap, cornersize));
    quads.append(QRect(size.width()-underlap, -outside, cornersize, cornersize));
    return quads;
}

double QtCurveClient::shadowOpacity(ShadowType type) const
{
    switch( type ) {
        case ShadowBorderedActive:
            return isActive() ? 1.0 : 0.0;
        case ShadowBorderedInactive:
            return isActive() ? 0.0 : 1.0;
        default:
            return 0;
    }
    return 0;
}
#endif
    
void QtCurveClient::reset(unsigned long changed)
{
    if (changed & SettingColors)
    {
        // repaint the whole thing
        widget()->update();
        updateButtons();
    }
    else if (changed & SettingFont)
    {
        // font has changed -- update title height and font
        itsTitleFont = isToolWindow() ? Handler()->titleFontTool() : Handler()->titleFont();

        updateLayout();
        widget()->update();
    }

    KCommonDecoration::reset(changed);
}

}
