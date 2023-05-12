#ifndef GRAPHVIEWOPTIONSDIALOG_H
#define GRAPHVIEWOPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class GraphViewOptionsDialog;
}

class GraphViewOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphViewOptionsDialog(QWidget *parent = nullptr);
    ~GraphViewOptionsDialog();

    int markovBlanketDistance() const;
    void setMarkovBlanketDistance(int newIMarkovBlanketDistance);

    bool showMarkovBlanketPlus() const;
    void setShowMarkovBlanketPlus(bool newBShowMarkovBlanketPlus);

    bool showUnconnected() const;
    void setShowUnconnected(bool newBShowUnconnected);

    void updateSettings();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GraphViewOptionsDialog *ui;
    int iMarkovBlanketDistance_ = 5;
    bool bShowMarkovBlanketPlus_ = false;
    bool bShowUnconnected_ = false;
};

#endif // GRAPHVIEWOPTIONSDIALOG_H
