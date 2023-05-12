CONFIG   += c++17
QT       += core widgets network

TARGET = roa
TEMPLATE = app
SOURCES += main.cpp \
    controller/maincontroller.cpp \
    graphviewoptionsdialog.cpp \
    minegraphdialog.cpp \
    model/adjustmentmodel/adjustmentmodel.cpp \
    model/adjustmentmodel/adjustmentmodellist.cpp \
    model/adjustmentmodel/adjustmentset.cpp \
    model/adjustmentmodel/adjustmentsetlist.cpp \
    model/balancemodel/balancemodel.cpp \
    model/balancemodel/balancemodelslist.cpp \
    model/causalmodel/causalmodel.cpp \
    model/causalmodel/causalmodelcache.cpp \
    model/causalmodel/causalmodelcachelist.cpp \
    model/causalmodel/causalmodellist.cpp \
    model/database/databaseconnection.cpp \
    model/distribution/distributionlist.cpp \
    model/effect/effect.cpp \
    model/effect/effectlist.cpp \
    model/effectsmodel/effectsmodel.cpp \
    model/effectsmodel/effectsmodelslist.cpp \
    model/graph/edge.cpp \
    model/graph/edgeslist.cpp \
    model/graph/graph.cpp \
    model/graph/graphlist.cpp \
    model/graph/multiedge.cpp \
    model/graph/multigraph.cpp \
    model/graph/node.cpp \
    model/graph/nodeslist.cpp \
    model/markovmodel/markovmodel.cpp \
    model/markovmodel/markovmodellist.cpp \
    model/treatment/treatment.cpp \
    model/treemodel/treeitem.cpp \
    model/treemodel/treemodel.cpp \
    model/treemodel/treemodelproxy.cpp \
    model/variable/variablebalance.cpp \
    model/variable/variablebalanceinfo.cpp \
    model/variable/variablebalancelist.cpp \
    model/variable/variableeffect.cpp \
    model/variable/variableeffectinfo.cpp \
    model/variable/variableeffectlist.cpp \
    sortlayoutdialog.cpp \
    view/adjustmentspanel/adjustmentspanel.cpp \
    view/adjustmentspanel/adjustmentstable.cpp \
    view/adjustmentspanel/adjustmentstablemodel.cpp \
    view/balancepanel/balancepaneldistributionsview.cpp \
    view/balancepanel/balancepaneltree.cpp \
    view/balancepanel/balancepaneltreedelegate.cpp \
    view/balancepanel/balancepaneltreeheader.cpp \
    view/balancepanel/balancepaneltreemodel.cpp \
    view/balancepanel/balancepaneltreeproxymodel.cpp \
    view/balancepanel/balancepaneltreeweights.cpp \
    view/balancepanel/balancepaneltreeweightsdelegate.cpp \
    view/causalitypanel/causalitypanel.cpp \
    view/causalitypanel/causalitypanelcombodelegate.cpp \
    view/causalitypanel/causalitypanelgraphview.cpp \
    view/causalitypanel/causalitypanelgraphviewlegend.cpp \
    view/causalitypanel/causalitypanelgraphviewnode.cpp \
    view/causalitypanel/causalitypanelgraphviewscene.cpp \
    view/causalitypanel/causalitypanelgrapviewarrowitem.cpp \
    view/causalitypanel/causalitypanelgrapviewhedge.cpp \
    view/causalitypanel/causalpaneladjacencypanelmultiedge.cpp \
    view/charts/balanceview.cpp \
    view/charts/balanceviewheader.cpp \
    view/charts/distributionview.cpp \
    view/charts/renderarea.cpp \
    view/editnetworkdialog.cpp \
    view/effectspanel/effectspanel.cpp \
    view/effectspanel/effectstable.cpp \
    view/effectspanel/effectstablegraph.cpp \
    view/exgraphicsview/extendedgraphicsscene.cpp \
    view/exgraphicsview/extendedgraphicsview.cpp \
    view/exportgraphdialog.cpp \
    view/graphicsview.cpp \
    view/groupspanel/groupslistdelegatecolor.cpp \
    view/groupspanel/groupslistdelegatesize.cpp \
    view/groupspanel/groupspanel.cpp \
    view/groupspanel/groupspaneleditgroupdialog.cpp \
    view/importgraphdialog.cpp \
    view/mainwindow.cpp \
    view/colorandfontmap.cpp \
    model/project/project.cpp \
    controller/rcontroller.cpp \
    propertiesinterface.cpp \
    utils.cpp \
    model/variable/variable.cpp \
    view/panelwidget.cpp \
    view/propertiespanel.cpp \
    view/refreshbutton.cpp \
    view/splitmergebutton.cpp \
    view/toolbar.cpp \
    view/transfergraphdialog.cpp \
    view/treatmentpanel/treatmentpanel.cpp \
    model/variable/variableslist.cpp \
    model/group/groupslist.cpp \
    model/group/group.cpp \
    view/loadprojectdialog.cpp \
    view/settingsdialog/mainsettingsdialog.cpp \
    view/settingsdialog/environmentsettingswidget.cpp \
    view/settingsdialog/appearancesettingswidget.cpp \
    view/settingsdialog/computationsandcheckssettingswidget.cpp \
    view/aboutdialog.cpp \
    view/processingoverlaywidget.cpp \
    newprojectdialog.cpp \
    view/panellabel.cpp \
    view/balancepanel/balancepanel.cpp \
    model/distribution/distribution.cpp \
    model/applicationdata.cpp

HEADERS += controller/maincontroller.h \
    graphviewoptionsdialog.h \
    minegraphdialog.h \
    model/adjustmentmodel/adjustmentmodel.h \
    model/adjustmentmodel/adjustmentmodellist.h \
    model/adjustmentmodel/adjustmentset.h \
    model/adjustmentmodel/adjustmentsetlist.h \
    model/balancemodel/balancemodel.h \
    model/balancemodel/balancemodelslist.h \
    model/causalmodel/causalmodel.h \
    model/causalmodel/causalmodelcache.h \
    model/causalmodel/causalmodelcachelist.h \
    model/causalmodel/causalmodellist.h \
    model/distribution/distributionlist.h \
    model/effect/effect.h \
    model/effect/effectlist.h \
    model/effectsmodel/effectsmodel.h \
    model/effectsmodel/effectsmodelslist.h \
    model/graph/edge.h \
    model/graph/edgeslist.h \
    model/graph/graph.h \
    model/graph/graphlist.h \
    model/graph/graphtypes.h \
    model/graph/multiedge.h \
    model/graph/multigraph.h \
    model/graph/node.h \
    model/graph/nodeslist.h \
    model/markovmodel/markovmodel.h \
    model/markovmodel/markovmodellist.h \
    model/project/project.h \
    model/treatment/treatment.h \
    model/treemodel/treeitem.h \
    model/treemodel/treemodel.h \
    model/treemodel/treemodelproxy.h \
    model/variable/variablebalance.h \
    model/variable/variablebalanceinfo.h \
    model/variable/variablebalancelist.h \
    model/variable/variableeffect.h \
    model/variable/variableeffectinfo.h \
    model/variable/variableeffectlist.h \
    sortlayoutdialog.h \
    view/adjustmentspanel/adjustmentspanel.h \
    view/adjustmentspanel/adjustmentstable.h \
    view/adjustmentspanel/adjustmentstablemodel.h \
    view/balancepanel/balancepanel.h \
    view/balancepanel/balancepaneldistributionsview.h \
    view/balancepanel/balancepaneltree.h \
    view/balancepanel/balancepaneltreedelegate.h \
    view/balancepanel/balancepaneltreeheader.h \
    view/balancepanel/balancepaneltreemodel.h \
    view/balancepanel/balancepaneltreeproxymodel.h \
    view/balancepanel/balancepaneltreeweights.h \
    view/balancepanel/balancepaneltreeweightsdelegate.h \
    view/causalitypanel/causalitypanel.h \
    view/causalitypanel/causalitypanelcombodelegate.h \
    view/causalitypanel/causalitypanelgraphview.h \
    view/causalitypanel/causalitypanelgraphviewlegend.h \
    view/causalitypanel/causalitypanelgraphviewnode.h \
    view/causalitypanel/causalitypanelgraphviewscene.h \
    view/causalitypanel/causalitypanelgrapviewarrowitem.h \
    view/causalitypanel/causalitypanelgrapviewhedge.h \
    view/causalitypanel/causalpaneladjacencypanelmultiedge.h \
    view/charts/balanceview.h \
    view/charts/balanceviewheader.h \
    view/charts/distributionview.h \
    view/charts/renderarea.h \
    view/editnetworkdialog.h \
    view/effectspanel/effectspanel.h \
    view/effectspanel/effectstable.h \
    view/effectspanel/effectstablegraph.h \
    view/exgraphicsview/extendedgraphicsscene.h \
    view/exgraphicsview/extendedgraphicsview.h \
    view/exportgraphdialog.h \
    view/groupspanel/groupslistdelegatecolor.h \
    view/groupspanel/groupslistdelegatesize.h \
    view/groupspanel/groupspanel.h \
    view/groupspanel/groupspaneleditgroupdialog.h \
    view/importgraphdialog.h \
    view/mainwindow.h \
    view/colorandfontmap.h \
    controller/rcontroller.h \
    propertiesinterface.h \
    model/variable/variable.h \
    view/panelwidget.h \
    view/propertiespanel.h \
    view/refreshbutton.h \
    view/splitmergebutton.h \
    view/toolbar.h \
    view/transfergraphdialog.h \
    view/treatmentpanel/treatmentpanel.h \
    model/variable/variableslist.h \
    model/group/group.h \
    model/group/groupslist.h \
    view/loadprojectdialog.h \
    view/settingsdialog/mainsettingsdialog.h \
    view/settingsdialog/computationsandcheckssettingswidget.h \
    view/settingsdialog/environmentsettingswidget.h \
    view/settingsdialog/appearancesettingswidget.h \
    view/aboutdialog.h \
    view/processingoverlaywidget.h \
    newprojectdialog.h \
    model/database/databaseconnection.h \
    view/panellabel.h \
    model/distribution/distribution.h \
    model/applicationdata.h

FORMS    += view/mainwindow.ui \
    graphviewoptionsdialog.ui \
    minegraphdialog.ui \
    sortlayoutdialog.ui \
    view/balancepanel/balancepaneldistributionsview.ui \
    view/balancepanel/balancepaneltreeweightsform.ui \
    view/editnetworkdialog.ui \
    view/exportgraphdialog.ui \
    view/groupspanel/groupspaneleditgroupdialog.ui \
    view/importgraphdialog.ui \
    view/loadprojectdialog.ui \
    view/settingsdialog/mainsettingsdialog.ui \
    view/settingsdialog/computationsandcheckssettingswidget.ui \
    view/settingsdialog/environmentsettingswidget.ui \
    view/settingsdialog/appearancesettingswidget.ui \
    view/aboutdialog.ui \
    newprojectdialog.ui \
    view/transfergraphdialog.ui

RESOURCES += \
    resources.qrc
DESTDIR = ../build

CONFIG(debug, debug|release) {
    DESTDIR = $$DESTDIR/debug
    DEFINES += ENABLE_DEBUG_OBJ_INFO
}
CONFIG(release, debug|release) {
    DESTDIR = $$DESTDIR/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u

INCLUDEPATH += MOC_DIR
INCLUDEPATH += model view controller model/variable model/group model/balancemodel model/effectsmodel \
            += view/settingsdialog model/treemodel model/graph \
            += model/project model/database view/balancepanel view/groupspanel model/causalmodel \
            += model/distribution model/causalmodel model/treatment model/effect view/studypanel \
            += view/groupspanel view/treatmentpanel view/causalitypanel view/charts view/exgraphicsview \
            += view/effectspanel model/adjustmentmodel view/adjustmentspanel model/markovmodel

INCLUDEPATH += ../lib/ogdf/include
LIBS += -L../lib/ogdf -lCOIN -lOGDF

DISTFILES +=

HEADERS +=
