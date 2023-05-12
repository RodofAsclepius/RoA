#ifndef EFFECTSTABLE_H
#define EFFECTSTABLE_H

#include <QTableWidget>
#include "effectsmodelslist.h"

class CColorAndFontMap;

class CEffectsTable : public QTableWidget
{
    Q_OBJECT
public:
    CEffectsTable(QWidget *parent = nullptr);
    int columnVariable() const;
    void setColumnVariable(int columnVariable);

    CEffectsModelsList effectModels() const;
    void setEffectModels(const CEffectsModelsList &effectModels);
    void updateTable();

protected:
    int iColumnVariable_;
    CEffectsModelsList lstEffectModels_;
    CColorAndFontMap &mapColorsFonts_;

    QPair<qreal, qreal> commputeGlobalDomain() const;
public slots:
};

#endif // EFFECTSTABLE_H
