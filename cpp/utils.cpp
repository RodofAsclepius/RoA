
#include "utils.h"
#include "colorandfontmap.h"

#include <QString>

namespace utils
{

QString getLocalTime()
{
    static time_t currentTime;
    static struct tm * pLocalTime;
    QString strLocalTime;

    time( &currentTime );
    pLocalTime = localtime( &currentTime );

    int iDay    = pLocalTime->tm_mday;
    int iMonth  = pLocalTime->tm_mon + 1;
    int iYear   = pLocalTime->tm_year + 1900;
    int iHour   = pLocalTime->tm_hour;
    int iMin    = pLocalTime->tm_min;
    int iSec    = pLocalTime->tm_sec;

    strLocalTime = QString("%1").arg(iDay, 2, 10, QChar('0')) + "/" +
                    QString("%1").arg(iMonth, 2, 10, QChar('0')) + "/" +
                    QString("%1").arg(iYear, 2, 10, QChar('0')) + " @ " +
                    QString("%1").arg(iHour, 2, 10, QChar('0')) + ":" +
                    QString("%1").arg(iMin, 2, 10, QChar('0')) + ":" +
                    QString("%1").arg(iSec, 2, 10, QChar('0'));

    return strLocalTime;
}



CSpacer g_spacer;


CSpacer::CSpacer()
{

}


CSpacer::~CSpacer()
{

}


void CSpacer::indent(int nIndents)
{
    _nIndents += nIndents;

    updateString();
}


void CSpacer::unindent(int nIndents)
{
    _nIndents -= nIndents;
    if(_nIndents < 0)
    {
        _nIndents = 0;
    }

    updateString();
}


QString CSpacer::getSeparator() const
{
    return QString("");
}


QString CSpacer::getSpacerString() const
{
    return strSpacer;
}


void CSpacer::updateString()
{
    strSpacer.clear();
    for(int i=0; i<_nIndents; ++i)
    {
        strSpacer.append("   ");
    }
}


QDebug operator<<(QDebug dbg, const CSpacer& spacer)
{
    dbg.noquote();
    dbg.nospace() << spacer.getSpacerString();
    dbg.quote();
    return dbg.maybeSpace();
}

} // namespace

