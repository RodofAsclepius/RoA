#ifndef TREATMENT_H
#define TREATMENT_H

#include "propertiesinterface.h"
#include "variable.h"

class CTreatment : public CPropertiesInterface
{
    Q_OBJECT
public:
    CTreatment();
    CTreatment(const CTreatment& treatment);
    CTreatment(const CVariable &variable, QString strMethod = QString("ATE"));
    virtual ~CTreatment();

    CTreatment& operator=(const CTreatment& treatment);
    virtual bool operator==(const CTreatment& treatment) const;
    virtual bool operator<(const CTreatment& treatment) const;
    friend QDataStream& operator>>(QDataStream& in, CTreatment& treatment);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonObject(const QJsonObject& jsonObject);

    CVariable variable() const;
    void setVariable(const CVariable &variable);

    QString estimationMethod() const;
    void setEstimationMethod(const QString &estimationMethod);

    static QStringList estimationMethods();

    QString levelTreatment() const;
    void setLevelTreatment(const QString &value);

    QString levelControl() const;
    void setLevelControl(const QString &value);

    bool isValid() const;
    void setIsValid(bool isValid);

protected:
    CVariable variable_;
    QString strEstimationMethod_ = "ATT";
    QString strLevelTreatment;
    QString strLevelControl;

    bool bIsValid_;

    friend uint qHash(const CVariable &key, uint seed);
};

Q_DECLARE_METATYPE(CTreatment)

QDataStream& operator<<(QDataStream& out, const CTreatment& treatment);
QDataStream& operator>>(QDataStream& in, CTreatment& treatment);
QDebug operator<<(QDebug dbg, const CTreatment& treatment);

inline uint qHash(const CTreatment &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());// * key.getName().length();
}

#endif // TREATMENT_H
