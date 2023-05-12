#ifndef GROUPTYPES_H
#define GROUPTYPES_H


#include <QString>

namespace GroupModelTypes
{

typedef enum
{
    GROUP_MODEL_TYPE_GENERIC,
} GroupModelType;

inline QString getGroupModelTypeLabel(GroupModelType GroupModelType)
{
    QString strGroupModelType = QString("Unkonwn (error)");

    switch(GroupModelType)
    {
        case GROUP_MODEL_TYPE_GENERIC:
            strGroupModelType = QString("Generic");
        break;
    }

    return strGroupModelType;
}

}


#endif // GROUPTYPES_H
