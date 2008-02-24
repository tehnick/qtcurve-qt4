/*
  QtCurve KWin window decoration
  Copyright (C) 2007 Craig Drummond <Craig.Drummond@lycos.co.uk>

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

#include "qtcurvebutton.h"
#include <QAbstractButton>
#include <QStyle>
#include <QStyleOption>
#include <QBitmap>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include "qtcurveclient.h"
#define QTC_KWIN
#include "common.h"

namespace KWinQtCurve
{

QtCurveButton::QtCurveButton(ButtonType type, QtCurveClient *parent)
             : KCommonDecorationButton(type, parent),
               itsClient(parent),
               itsIconType(NumButtonIcons),
               itsHover(false)
{
    setAttribute(Qt::WA_NoSystemBackground);
}

void QtCurveButton::reset(unsigned long changed)
{
    if (changed&DecorationReset || changed&ManualReset || changed&SizeChange || changed&StateChange)
    {
        switch (type())
        {
            case CloseButton:
                itsIconType = CloseIcon;
                break;
            case HelpButton:
                itsIconType = HelpIcon;
                break;
            case MinButton:
                itsIconType = MinIcon;
                break;
            case MaxButton:
                itsIconType = isChecked() ? MaxRestoreIcon : MaxIcon;
                break;
            case OnAllDesktopsButton:
                itsIconType= isChecked() ? NotOnAllDesktopsIcon : OnAllDesktopsIcon;
                break;
            case ShadeButton:
                itsIconType = isChecked() ? UnShadeIcon : ShadeIcon;
                break;
            case AboveButton:
                itsIconType = isChecked() ? NoKeepAboveIcon : KeepAboveIcon;
                break;
            case BelowButton:
                itsIconType = isChecked() ? NoKeepBelowIcon : KeepBelowIcon;
                break;
            default:
                itsIconType = NumButtonIcons; // empty...
                break;
        }

        this->update();
    }
}

void QtCurveButton::enterEvent(QEvent *e)
{
    QAbstractButton::enterEvent(e);

    itsHover = true;
    repaint();
}

void QtCurveButton::leaveEvent(QEvent *e)
{
    QAbstractButton::leaveEvent(e);

    itsHover = false;
    repaint();
}

void QtCurveButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    drawButton(&p);
}

inline int limit(double c)
{
    return c < 0.0 ? 0 : (c > 255.0  ? 255 : (int)c);
}

inline QColor midColor(const QColor &a, const QColor &b, double factor=1.0)
{
    return QColor((a.red()+limit(b.red()*factor))>>1, 
                  (a.green()+limit(b.green()*factor))>>1, 
                  (a.blue()+limit(b.blue()*factor))>>1);
}

void QtCurveButton::drawButton(QPainter *painter)
{
    QRect    r(0, 0, width(), height());
    bool     active(itsClient->isActive());
    QPixmap  tempPixmap;
    QColor   buttonColor(KDecoration::options()->color(KDecoration::ColorTitleBar, active));
    QPixmap  buffer(width(), height());
    QPainter bP(&buffer);

    if(CloseButton==type())
        buttonColor=midColor(QColor(180,64,32), buttonColor);

    itsClient->drawBtnBgnd(&bP, r, active);

    if (itsHover)
    {
        QStyleOption opt;

        opt.init(this);
        opt.rect=QRect(0, 0, width(), height());
        opt.state|=(isDown() ? QStyle::State_Sunken : QStyle::State_Raised)|QStyle::State_MouseOver|QStyle::State_Horizontal|QtC_StateKWin;
        opt.palette.setColor(QPalette::Button, buttonColor);
        Handler()->wStyle()->drawPrimitive(QStyle::PE_PanelButtonCommand, &opt, &bP, 0L);
    }

    if (MenuButton==type())
    {
        QPixmap menuIcon(itsClient->icon().pixmap(style()->pixelMetric(QStyle::PM_SmallIconSize)));
        if (width() < menuIcon.width() || height() < menuIcon.height())
            menuIcon = menuIcon.scaled(width(), height());
        bP.drawPixmap((width()-menuIcon.width())/2, (height()-menuIcon.height())/2, menuIcon);
    }
    else
    {
        const QBitmap &icon(Handler()->buttonBitmap(itsIconType, size(), decoration()->isToolWindow()));
        int           dX(r.x()+(r.width()-icon.width())/2),
                      dY(r.y()+(r.height()-icon.height())/2);

        if(isDown())
        {
            dY++;
            dX++;
        }
        else
        {
            bP.setPen(QtCurveClient::shadowColor(KDecoration::options()->color(KDecoration::ColorFont, active)));
            bP.drawPixmap(dX+1, dY+1, icon);
        }

        bP.setPen(KDecoration::options()->color(KDecoration::ColorFont, active));
        bP.drawPixmap(dX, dY, icon);
    }

    bP.end();
    painter->drawPixmap(0, 0, buffer);
}

QBitmap IconEngine::icon(ButtonIcon icon, int size, QStyle *style)
{
    if (size%2 == 0)
        --size;

    QBitmap bitmap(size,size);
    bitmap.fill(Qt::color0);
    QPainter p(&bitmap);

    p.setPen(Qt::color1);

    QRect r = bitmap.rect();

    // line widths
    int lwTitleBar = 1;
    if (r.width() > 16)
        lwTitleBar = 4;
    else if (r.width() > 4)
        lwTitleBar = 2;

    int lwArrow = 1;
    if (r.width() > 16)
        lwArrow = 4;
    else if (r.width() > 7)
        lwArrow = 2;

    switch(icon)
    {
        case CloseIcon:
        {
            int lineWidth = 1;
            if (r.width() > 16)
                lineWidth = 3;
            else if (r.width() > 4)
                lineWidth = 2;

            drawObject(p, DiagonalLine, r.x(), r.y(), r.width(), lineWidth);
            drawObject(p, CrossDiagonalLine, r.x(), r.bottom(), r.width(), lineWidth);
            break;
        }
        case MaxIcon:
        {
            int lineWidth2 = 1; // frame
            if (r.width() > 16)
                lineWidth2 = 2;
            else if (r.width() > 4)
                lineWidth2 = 1;

            drawObject(p, HorizontalLine, r.x(), r.top(), r.width(), lwTitleBar);
            drawObject(p, HorizontalLine, r.x(), r.bottom()-(lineWidth2-1), r.width(), lineWidth2);
            drawObject(p, VerticalLine, r.x(), r.top(), r.height(), lineWidth2);
            drawObject(p, VerticalLine, r.right()-(lineWidth2-1), r.top(), r.height(), lineWidth2);
            break;
        }
        case MaxRestoreIcon:
        {
            int lineWidth2 = 1; // frame
            if (r.width() > 16)
                lineWidth2 = 2;
            else if (r.width() > 4)
                lineWidth2 = 1;

            int margin1, margin2;
            margin1 = margin2 = lineWidth2*2;
            if (r.width() < 8)
                margin1 = 1;

            // background window
            drawObject(p, HorizontalLine, r.x()+margin1, r.top(), r.width()-margin1, lineWidth2);
            drawObject(p, HorizontalLine, r.right()-margin2, r.bottom()-(lineWidth2-1)-margin1, margin2, lineWidth2);
            drawObject(p, VerticalLine, r.x()+margin1, r.top(), margin2, lineWidth2);
            drawObject(p, VerticalLine, r.right()-(lineWidth2-1), r.top(), r.height()-margin1, lineWidth2);

            // foreground window
            drawObject(p, HorizontalLine, r.x(), r.top()+margin2, r.width()-margin2, lwTitleBar);
            drawObject(p, HorizontalLine, r.x(), r.bottom()-(lineWidth2-1), r.width()-margin2, lineWidth2);
            drawObject(p, VerticalLine, r.x(), r.top()+margin2, r.height(), lineWidth2);
            drawObject(p, VerticalLine, r.right()-(lineWidth2-1)-margin2, r.top()+margin2, r.height(), lineWidth2);

            break;
        }

        case MinIcon:
            drawObject(p, HorizontalLine, r.x(), r.bottom()-(lwTitleBar-1), r.width(), lwTitleBar);
            break;
        case HelpIcon:
        {
            int center = r.x()+r.width()/2 -1;
            int side = r.width()/4;

            // paint a question mark... code is quite messy, to be cleaned up later...! :o

            if (r.width() > 16)
            {
                int lineWidth = 3;

                // top bar
                drawObject(p, HorizontalLine, center-side+3, r.y(), 2*side-3-1, lineWidth);
                // top bar rounding
                drawObject(p, CrossDiagonalLine, center-side-1, r.y()+5, 6, lineWidth);
                drawObject(p, DiagonalLine, center+side-3, r.y(), 5, lineWidth);
                // right bar
                drawObject(p, VerticalLine, center+side+2-lineWidth, r.y()+3, r.height()-(2*lineWidth+side+2+1), lineWidth);
                // bottom bar
                drawObject(p, CrossDiagonalLine, center, r.bottom()-2*lineWidth, side+2, lineWidth);
                drawObject(p, HorizontalLine, center, r.bottom()-3*lineWidth+2, lineWidth, lineWidth);
                // the dot
                drawObject(p, HorizontalLine, center, r.bottom()-(lineWidth-1), lineWidth, lineWidth);
            }
            else if (r.width() > 8)
            {
                int lineWidth = 2;

                // top bar
                drawObject(p, HorizontalLine, center-(side-1), r.y(), 2*side-1, lineWidth);
                // top bar rounding
                if (r.width() > 9)
                    drawObject(p, CrossDiagonalLine, center-side-1, r.y()+3, 3, lineWidth);
                else
                    drawObject(p, CrossDiagonalLine, center-side-1, r.y()+2, 3, lineWidth);
                drawObject(p, DiagonalLine, center+side-1, r.y(), 3, lineWidth);
                // right bar
                drawObject(p, VerticalLine, center+side+2-lineWidth, r.y()+2, r.height()-(2*lineWidth+side+1), lineWidth);
                // bottom bar
                drawObject(p, CrossDiagonalLine, center, r.bottom()-2*lineWidth+1, side+2, lineWidth);
                // the dot
                drawObject(p, HorizontalLine, center, r.bottom()-(lineWidth-1), lineWidth, lineWidth);
            }
            else
            {
                int lineWidth = 1;

                // top bar
                drawObject(p, HorizontalLine, center-(side-1), r.y(), 2*side, lineWidth);
                // top bar rounding
                drawObject(p, CrossDiagonalLine, center-side-1, r.y()+1, 2, lineWidth);
                // right bar
                drawObject(p, VerticalLine, center+side+1, r.y(), r.height()-(side+2+1), lineWidth);
                // bottom bar
                drawObject(p, CrossDiagonalLine, center, r.bottom()-2, side+2, lineWidth);
                // the dot
                drawObject(p, HorizontalLine, center, r.bottom(), 1, 1);
            }

            break;
        }
        case NotOnAllDesktopsIcon:
        {
            int lwMark = r.width()-lwTitleBar*2-2;
            if (lwMark < 1)
                lwMark = 3;

            drawObject(p, HorizontalLine, r.x()+(r.width()-lwMark)/2, r.y()+(r.height()-lwMark)/2, lwMark, lwMark);

            // Fall through to OnAllDesktopsIcon intended!
        }
        case OnAllDesktopsIcon:
        {
            // horizontal bars
            drawObject(p, HorizontalLine, r.x()+lwTitleBar, r.y(), r.width()-2*lwTitleBar, lwTitleBar);
            drawObject(p, HorizontalLine, r.x()+lwTitleBar, r.bottom()-(lwTitleBar-1), r.width()-2*lwTitleBar, lwTitleBar);
            // vertical bars
            drawObject(p, VerticalLine, r.x(), r.y()+lwTitleBar, r.height()-2*lwTitleBar, lwTitleBar);
            drawObject(p, VerticalLine, r.right()-(lwTitleBar-1), r.y()+lwTitleBar, r.height()-2*lwTitleBar, lwTitleBar);
            break;
        }
        case NoKeepAboveIcon:
        {
            int center = r.x()+r.width()/2;

            // arrow
            drawObject(p, CrossDiagonalLine, r.x(), center+2*lwArrow, center-r.x(), lwArrow);
            drawObject(p, DiagonalLine, r.x()+center, r.y()+1+2*lwArrow, center-r.x(), lwArrow);
            if (lwArrow>1)
                drawObject(p, HorizontalLine, center-(lwArrow-2), r.y()+2*lwArrow, (lwArrow-2)*2, lwArrow);

            // Fall through to KeepAboveIcon intended!
        }
        case KeepAboveIcon:
        {
            int center = r.x()+r.width()/2;

            // arrow
            drawObject(p, CrossDiagonalLine, r.x(), center, center-r.x(), lwArrow);
            drawObject(p, DiagonalLine, r.x()+center, r.y()+1, center-r.x(), lwArrow);
            if (lwArrow>1)
                drawObject(p, HorizontalLine, center-(lwArrow-2), r.y(), (lwArrow-2)*2, lwArrow);
            break;
        }
        case NoKeepBelowIcon:
        {
            int center = r.x()+r.width()/2;

            // arrow
            drawObject(p, DiagonalLine, r.x(), center-2*lwArrow, center-r.x(), lwArrow);
            drawObject(p, CrossDiagonalLine, r.x()+center, r.bottom()-1-2*lwArrow, center-r.x(), lwArrow);
            if (lwArrow>1)
                drawObject(p, HorizontalLine, center-(lwArrow-2), r.bottom()-(lwArrow-1)-2*lwArrow, (lwArrow-2)*2, lwArrow);

            // Fall through to KeepBelowIcon intended!
        }
        case KeepBelowIcon:
        {
            int center = r.x()+r.width()/2;

            // arrow
            drawObject(p, DiagonalLine, r.x(), center, center-r.x(), lwArrow);
            drawObject(p, CrossDiagonalLine, r.x()+center, r.bottom()-1, center-r.x(), lwArrow);
            if (lwArrow>1)
                drawObject(p, HorizontalLine, center-(lwArrow-2), r.bottom()-(lwArrow-1), (lwArrow-2)*2, lwArrow);
            break;
        }
        case UnShadeIcon:
        case ShadeIcon:
        {
            QStyleOption opt;

            //opt.init(btn);
            opt.rect=r;
            opt.state=QStyle::State_Enabled;

            opt.palette.setColor(QPalette::Button, Qt::red);
            style->drawPrimitive(ShadeIcon==icon ? QStyle::PE_IndicatorArrowUp
                                                 : QStyle::PE_IndicatorArrowDown,
                                 &opt, &p, 0L);
            break;
        }
        default:
            break;
    }

    p.end();
    bitmap.setMask(bitmap);

    return bitmap;
}

void IconEngine::drawObject(QPainter &p, Object object, int x, int y, int length, int lineWidth)
{
    switch(object)
    {
        case DiagonalLine:
            if (lineWidth <= 1)
                for (int i = 0; i < length; ++i)
                    p.drawPoint(x+i, y+i);
            else if (lineWidth <= 2)
            {
                for (int i = 0; i < length; ++i)
                    p.drawPoint(x+i, y+i);
                for (int i = 0; i < (length-1); ++i)
                {
                    p.drawPoint(x+1+i, y+i);
                    p.drawPoint(x+i, y+1+i);
                }
            }
            else
            {
                for (int i = 1; i < (length-1); ++i)
                    p.drawPoint(x+i, y+i);
                for (int i = 0; i < (length-1); ++i)
                {
                    p.drawPoint(x+1+i, y+i);
                    p.drawPoint(x+i, y+1+i);
                }
                for (int i = 0; i < (length-2); ++i)
                {
                    p.drawPoint(x+2+i, y+i);
                    p.drawPoint(x+i, y+2+i);
                }
            }
            break;
        case CrossDiagonalLine:
            if (lineWidth <= 1)
                for (int i = 0; i < length; ++i)
                    p.drawPoint(x+i, y-i);
            else if (lineWidth <= 2)
            {
                for (int i = 0; i < length; ++i)
                    p.drawPoint(x+i, y-i);
                for (int i = 0; i < (length-1); ++i)
                {
                    p.drawPoint(x+1+i, y-i);
                    p.drawPoint(x+i, y-1-i);
                }
            }
            else
            {
                for (int i = 1; i < (length-1); ++i)
                    p.drawPoint(x+i, y-i);
                for (int i = 0; i < (length-1); ++i)
                {
                    p.drawPoint(x+1+i, y-i);
                    p.drawPoint(x+i, y-1-i);
                }
                for (int i = 0; i < (length-2); ++i)
                {
                    p.drawPoint(x+2+i, y-i);
                    p.drawPoint(x+i, y-2-i);
                }
            }
            break;
        case HorizontalLine:
            for (int i = 0; i < lineWidth; ++i)
                p.drawLine(x, y+i, x+length-1, y+i);
            break;
        case VerticalLine:
            for (int i = 0; i < lineWidth; ++i)
                p.drawLine(x+i, y, x+i, y+length-1);
        default:
            break;
    }
}

}

