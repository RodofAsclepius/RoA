#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include "propertiesinterface.h"
#include "group.h"
#include "variable.h"

class CMainController;

class CDistribution : public CPropertiesInterface
{
    Q_OBJECT
public:
    CDistribution();
    CDistribution(const CGroup& group, const CVariable& variable);
    CDistribution(const CDistribution& distribution);
    virtual ~CDistribution();

    CDistribution& operator=(const CDistribution& distribution);
    virtual bool operator==(const CDistribution& distribution) const;
    virtual bool operator<(const CDistribution& distribution) const;
    friend QDataStream& operator>>(QDataStream& in, CDistribution& distribution);

    void setVariable(const CVariable& variable);
    CVariable variable() const;
    CGroup group() const;
    void setGroup(const CGroup &group);

    QJsonObject toJsonObject(bool bIncludeProperties = false) const;
    void fromJsonObject(const QJsonObject& jsonDocumentdistribution);

    QMap<QString, int> frequencies() const;
    void setFrequencies(const QMap<QString, int> &frequencies);

    QVector<QPair<qreal, qreal>> densities() const;
    void setDensities(const QVector<QPair<qreal, qreal>> &vecDensities);

    QPair<int, int> frequenciesRange(bool bRounded = false) const;
    QPair<int, int> frequenciesDomain(bool bRounded = false) const;
    QPair<qreal, qreal> densitiesRange(bool bRounded = false) const;
    QPair<qreal, qreal> densitiesDomain(bool bRounded = false) const;
    QPair<qreal, qreal> violinRange(bool bRounded = false) const;
    QPair<qreal, qreal> violinDomain(bool bRounded = false) const;

    qreal densitiesBW() const;
    void setDensitiesBW(const qreal &value);

    int densitiesN() const;
    void setDensitiesN(int value);

    QVector<QPair<qreal, qreal> > violinValues() const;
    void setViolinValues(const QVector<QPair<qreal, qreal> > &violinValues);

    QMap<QString, qreal> boxPlotViolin() const;
    void setBoxPlotViolin(const QMap<QString, qreal> &boxPlotViolin);

    int violinN() const;
    void setViolinN(int violinN);

    QString levelLabelViolin() const;
    void setLevelLabelViolin(const QString &levelLabelViolin);

protected:
    CGroup group_;
    CVariable variable_;
    QMap<QString, int> mapFrequencies_;
    QVector<QPair<qreal, qreal>> vecDensities_;
    qreal qDensitiesBW;
    int iDensitiesN;

    QVector<QPair<qreal, qreal>> vecViolinValues_;
    QMap<QString, qreal> mapBoxPlotViolin_;
    int iViolinN_;
    QString strLevelLabelViolin_;

signals:
};

Q_DECLARE_METATYPE(CDistribution)

QDataStream& operator<<(QDataStream& out, const CDistribution& distribution);
QDataStream& operator>>(QDataStream& in, CDistribution& distribution);
QDebug operator<<(QDebug dbg, const CDistribution& distribution);

#endif // DISTRIBUTION_H
