#ifndef CASSOCIATIONSPANELMATRIX_H
#define CASSOCIATIONSPANELMATRIX_H

#include <QTableWidget>
#include "applicationdata.h"
#include <QWidget>

class CMainController;
class CColorAndFontMap;

class CEffectsPanelMatrix : public QTableWidget
{
    Q_OBJECT
public:
    typedef enum {Normal, RightAggregates, BottomAggregates} Mode;

    CEffectsPanelMatrix(CEffectsPanelMatrix::Mode mode = Normal);
    ~CEffectsPanelMatrix();

    static QSize cellSize();
    static void setCellSize(const QSize &cellSize);
    QSize resizeToContents();

//    CApplicationData applicationData() const;
//    void setApplicationData(const CApplicationData &applicationData);

//    CAssociationsModel associationsModel() const;
//    void setAssociationsModel(const CAssociationsModel &associationsModel);

protected:
    CMainController& _mainController;
    CColorAndFontMap& mapColorsAndFonts_;
    CEffectsPanelMatrix::Mode _mode;
    static QSize _cellSize;

//    CApplicationData _applicationData;
//    CAssociationsModel _associationsModel;

    void applyStyleSheet();
    void updateTable();

protected slots:

signals:
};

#endif // CASSOCIATIONSPANELMATRIX_H
