/*
  QtCurve KWin window decoration
  Copyright (C) 2007 - 2009 Craig Drummond <craig_p_drummond@yahoo.co.uk>

  This class is taken, and modified from the Bespin KWin window decoration.
  Copyright (c) 2008 Thomas Lübking <baghira-style@gmx.net>

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

#ifndef RESIZECORNER_H
#define RESIZECORNER_H

#include <QWidget>

namespace KWinQtCurve
{

class QtCurveClient;

class ResizeCorner : public QWidget
{
    Q_OBJECT
    
    public:
    
    ResizeCorner(QtCurveClient *parent, const QColor &c);
    void move(int x, int y);
    void setColor(const QColor &c);
    
    public Q_SLOTS:

    void raise();

    protected:
    
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
//     void paintEvent(QPaintEvent *ev);

    private:

    bool eventFilter(QObject *obj, QEvent *e);
    
    private:

    QtCurveClient *client;
//     QColor        col;
};

} 

#endif
