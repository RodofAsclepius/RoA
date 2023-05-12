#include "menuwidget.h"
#include "ui_menuwidget.h"
#include "utils.h"

#include <QMenu>
#include <QToolButton>


CMenuWidget::CMenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMenuWidget),
    _pMenu(new QMenu(this))
{
    ui->setupUi(this);

    setStyleSheet(QString("border-bottom: 1px solid rgb(100, 100, 100);"));
}


CMenuWidget::~CMenuWidget()
{
    delete ui;
}


void CMenuWidget::setMenuActions(const QList<QAction *> &actions)
{
    _pMenu->addActions(actions);
    ui->pushButtonMenu->setMenu(_pMenu);
    ui->pushButtonMenu->setStyleSheet("QPushButton {border: none;}"
                                      "QPushButton::menu-indicator {height: 0px;}");
}


void CMenuWidget::setToolBarActions(const QList<QAction *> &actions)
{
    QGridLayout* pGridLayout = new QGridLayout();
    pGridLayout->setContentsMargins(0, 0, 0, 0);

    for(int i=0; i<actions.count(); ++i)
    {
        QAction* pAction = actions.at(i);
        if(!pAction->isSeparator())
        {
            QToolButton* pButton = new QToolButton();
            pButton->setDefaultAction(pAction);
            pButton->setContentsMargins(0, 0, 0, 0);
            pButton->setStyleSheet("QToolButton {border: none;}");
            pButton->setIconSize(QSize(14, 14));
            pGridLayout->addWidget(pButton, 0, i, 1, 1);
        }
    }

    ui->widgetToolBarButtons->setLayout(pGridLayout);
}


void CMenuWidget::onPushButtonClicked()
{
    QPushButton* pPushButtonClicked = qobject_cast<QPushButton*>(sender());

    if(pPushButtonClicked != nullptr)
    {
        /* Update styles */
        QMapIterator<QString, QPushButton*> it(_mapTabs);
        while(it.hasNext())
        {
            it.next();
            QString strTabName = it.key();
            QPushButton* pPushButton = it.value();

            if(strTabName == pPushButtonClicked->text())
            {
                setTabActiveStyle(pPushButton);
            }
            else
            {
                setTabInactiveStyle(pPushButton);
            }

        }

        emit tabClicked(pPushButtonClicked->text());
    }
}


void CMenuWidget::addTab(const QString& strTabName)
{
    /* Add tab */
    if(! _mapTabs.keys().contains(strTabName))
    {
        QPushButton* pPushButton = new QPushButton(strTabName);
        pPushButton->setFocusPolicy(Qt::NoFocus);
        pPushButton->setContentsMargins(0, 0, 0, 0);
        pPushButton->setFixedHeight(34);

        setTabInactiveStyle(pPushButton);
        connect(pPushButton, &QPushButton::clicked, this, &CMenuWidget::onPushButtonClicked);
        ui->widgetTabs->layout()->addWidget(pPushButton);
        _mapTabs.insert(strTabName, pPushButton);

        /* Update active tab */
        if(_mapTabs.count() == 1)
        {
            strActiveTabName = strTabName;
            setTabActiveStyle(pPushButton);
        }
    }
}


void CMenuWidget::setTabInactiveStyle(QPushButton* pPushButton)
{
    if(pPushButton != nullptr)
    {
        pPushButton->setStyleSheet(QString("border: none;"
                                           "border-bottom: 1px solid rgb(100, 100, 100);"
                                           "font-weight: normal;"
                                           ));
    }
}


void CMenuWidget::setTabActiveStyle(QPushButton* pPushButton)
{
    if(pPushButton != nullptr)
    {
        pPushButton->setStyleSheet(QString("border: 1px solid rgb(100, 100, 100);"
                                           "border-top-left-radius: 6px;"
                                           "border-top-right-radius: 6px;"
                                           "border-bottom-left-radius: -6px;"
                                           "border-bottom-right-radius: -6px;"
                                           "border-bottom: none;"
                                           "font-weight: bold;"
                                           ));
    }
}


