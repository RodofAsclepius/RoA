#ifndef GROUPSSELECTOR_H
#define GROUPSSELECTOR_H

#include <QWidget>

namespace Ui {
class CGroupsSelector;
}

class CGroupsSelector : public QWidget
{
    Q_OBJECT

public:
    explicit CGroupsSelector(QWidget *parent = nullptr);
    ~CGroupsSelector();

private:
    Ui::CGroupsSelector *ui;
};

#endif // GROUPSSELECTOR_H
