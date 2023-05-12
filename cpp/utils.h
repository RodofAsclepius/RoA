#ifndef UTILS_H
#define UTILS_H


#include "colorandfontmap.h"

#include <typeinfo>
#include <QThread>
#include <QModelIndex>
#include <QDebug>
#include <QSet>
#include <QSetIterator>

const int MAX_LABEL_HEIGHT = 32;

namespace utils
{

    QString getLocalTime();


    class CSpacer
    {
    public:
        CSpacer();
        ~CSpacer();

        void indent(int nIndents = 1);
        void unindent(int nIndents = 1);

        QString getSeparator() const;
        QString getSpacerString() const;
    private:

        int _nIndents;
        QString strSpacer;
        void updateString();
    };


    extern CSpacer g_spacer;
    QDebug operator<<(QDebug dbg, const CSpacer& spacer);

}

template <typename T>
QSet<QSet<T> > genPowerset(QSet<T> set)
{
    QSet<QSet<T> > powerset;
    QSet<T> emptyset;
    powerset.insert(emptyset);

    QSetIterator<T> sit(set);
    while (sit.hasNext())
    {
        T element = sit.next();

        QSet<QSet<T> > tempSet;
        QSetIterator<QSet<T> > sit2(powerset);
        while (sit2.hasNext())
        {
            QSet<T> powersetset = sit2.next();

            powersetset.insert(element);
            tempSet.insert(powersetset);
        }

        powerset.unite(tempSet);
    }

    return powerset;
}

template <typename T> int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}


//#undef ENABLE_DEBUG_OBJ_INFO

#if defined(QT_DEBUG) && defined(ENABLE_DEBUG_OBJ_INFO)
    #define DEBUG_OBJ_INFO qDebug() << utils::getLocalTime() << "\t| " \
                                    << __PRETTY_FUNCTION__ << "\t| " \
                                    << (qobject_cast<const QObject*>(this) != nullptr ? (this->thread() != nullptr ? this->thread()->objectName() : "no thread") : "no QObject");
#else
    #define DEBUG_OBJ_INFO
#endif


#define ENABLE_SHOW_TODO_BOOKMARKS

#if defined(QT_DEBUG) && defined(ENABLE_SHOW_TODO_BOOKMARKS)
    #define BOOKMARK_TODO qDebug()  << "todo bookmark: " << "\t| " \
                                    << __PRETTY_FUNCTION__ << "\t| " \
                                    << (qobject_cast<const QObject*>(this) != nullptr ? (this->thread() != nullptr ? this->thread()->objectName() : "no thread") : "no QObject");
#else
    #define DEBUG_OBJ_INFO
#endif

#endif // UTILS_H
