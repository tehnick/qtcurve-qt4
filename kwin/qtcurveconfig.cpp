/*
  QtCurve (C) Craig Drummond, 2010 craig.p.drummond@googlemail.com

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

#include "qtcurveconfig.h"
#include <KDE/KConfig>
#include <KDE/KConfigGroup>
#include <kdecoration.h>

namespace KWinQtCurve
{

#define GROUP "General"
#define NEW_GROUP "KWinQtCurve"

void QtCurveConfig::defaults()
{
    itsBorderSize=BORDER_NORMAL;
    itsRoundBottom=true;
    itsOuterBorder=false;
    itsBorderlessMax=false;
    itsCustomShadows=false;
    itsGrouping=true;
    itsTitleBarPad=0;
    itsActiveOpacity=itsInactiveOpacity=100;
    itsOpaqueBorder=true;
}

#define READ_ENTRY(ENTRY) \
    its##ENTRY=group.readEntry(#ENTRY, def.its##ENTRY);

void QtCurveConfig::load(const KConfig *cfg)
{
    KConfigGroup  group(cfg, GROUP);
    QtCurveConfig def;

    if(group.hasKey("BorderSize"))
        itsBorderSize=(Size)group.readEntry("BorderSize", (int)def.borderSize());
    else
    {
        KConfig      kwin("kwinrc");
        KConfigGroup style(&kwin, "Style");
        int          size=style.readEntry("BorderSize", 1);

        if(KDecorationDefines::BorderTiny==size)
        {
            if(group.readEntry("DrawBottom", false))
                itsBorderSize=BORDER_NO_SIDES;
            else
                itsBorderSize=BORDER_NONE;
        }
        else
            itsBorderSize=(Size)(size+2);
    }

    if(itsBorderSize<BORDER_NONE || itsBorderSize>BORDER_OVERSIZED)
        itsBorderSize=BORDER_NORMAL;
    READ_ENTRY(BorderlessMax)
    READ_ENTRY(CustomShadows)
    READ_ENTRY(Grouping)
    READ_ENTRY(TitleBarPad)
    READ_ENTRY(ActiveOpacity)
    READ_ENTRY(InactiveOpacity)
    READ_ENTRY(OpaqueBorder)

    if(itsTitleBarPad<0 || itsTitleBarPad>10)
        itsTitleBarPad=0;
    if(BORDER_NONE==itsBorderSize)
        itsRoundBottom=false;
    else
        READ_ENTRY(RoundBottom)

    if(itsBorderSize<BORDER_TINY)
        itsOuterBorder=false;
    else
        READ_ENTRY(OuterBorder)
    if(itsActiveOpacity<0 || itsActiveOpacity>100)
        itsActiveOpacity=100;
    if(itsInactiveOpacity<0 || itsInactiveOpacity>100)
        itsInactiveOpacity=100;
}

#define WRITE_ENTRY(ENTRY) \
    if (def.its##ENTRY==its##ENTRY) \
        group.deleteEntry(#ENTRY); \
    else \
        group.writeEntry(#ENTRY, its##ENTRY);

void QtCurveConfig::save(KConfig *cfg)
{
    KConfigGroup  group(cfg, GROUP);
    QtCurveConfig def;

    //WRITE_ENTRY(BorderSize)
    // Hace to write BorderSize, because if not found we read the kwin setting - to be
    // compatible with QtCurve<1.4
    group.writeEntry("BorderSize", itsBorderSize);
    WRITE_ENTRY(RoundBottom)
    WRITE_ENTRY(OuterBorder)
    WRITE_ENTRY(BorderlessMax)
    WRITE_ENTRY(CustomShadows)
    WRITE_ENTRY(Grouping)
    WRITE_ENTRY(TitleBarPad)
    WRITE_ENTRY(ActiveOpacity)
    WRITE_ENTRY(InactiveOpacity)
    WRITE_ENTRY(OpaqueBorder)
}

}
