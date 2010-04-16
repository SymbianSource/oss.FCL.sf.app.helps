/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

#include <QStandardItemModel>
#include <QDebug>

#include <hbmainwindow.h>
#include <hbapplication.h>
#include <hbaction.h>

#include <hbtreeview.h>
#include <hbtoolbar.h>
#include <hbsearchpanel.h>
#include <hbscrollbar.h>
#include <hblineedit.h>
#include <hbmenu.h>

#include "HelpDocumentLoader.h"
#include "HelpDataProvider.h"
#include "HelpProxyModel.h"

#include "HelpCategoryView.h"


//////////////////////////////////////////////////////////////////////////////////////////////

HelpCategoryView::HelpCategoryView() : 
mViewMode(ViewModeNull), 
mListAll(NULL), 
mListSearch(NULL), 
mSearchPanel(NULL), 
mToolBar(NULL)
{
}

HelpCategoryView::~HelpCategoryView()
{
}


////////////////////////////////////////////////////////////////////////////////////////////

void HelpCategoryView::init()
{
    initBackAction();
    initAllList();
    initSearchList();
    initSearchPanel();
    initToolbar();

    switchViewMode(ViewModeAll);
    
    connect(mainWindow(), SIGNAL(currentViewChanged(HbView*)), this, SLOT(onCurrentViewChanged(HbView*)));
    connect(mainWindow(), SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onOrientationChanged(Qt::Orientation)));
}

void HelpCategoryView::initBackAction()
{
    mSoftKeyAction = new HbAction(Hb::BackAction);
    connect(mSoftKeyAction, SIGNAL(triggered()), this, SLOT(onBackAction()));
}

void HelpCategoryView::initAllList()
{
    mListAll = HelpUIBuilder::findWidget<HbTreeView*>(DOCML_LIST_CATEGORY_ALL);
    mListAll->setHorizontalScrollBarPolicy(HbScrollArea::ScrollBarAlwaysOff);
    mListAll->setModel(HelpDataProvider::instance()->getCategoryData());
    connect(mListAll, SIGNAL(activated(const QModelIndex&)), this, SLOT(onAllListActivated(const QModelIndex&)));
}

void HelpCategoryView::initSearchList()
{
    mListSearch = HelpUIBuilder::findWidget<HbTreeView*>(DOCML_LIST_CATEGORY_SEARCH);
    mListSearch->setHorizontalScrollBarPolicy(HbScrollArea::ScrollBarAlwaysOff);
    mListSearch->setModel(HelpDataProvider::instance()->getSearchData());
    connect(mListSearch, SIGNAL(activated(const QModelIndex&)), this, SLOT(onSearchListActivated(const QModelIndex&)));
}
void HelpCategoryView::initSearchPanel()
{
    mSearchPanel = HelpUIBuilder::findWidget<HbSearchPanel*>(DOCML_SEARCH_PANEL_CATEGORY);
    connect(mSearchPanel, SIGNAL(exitClicked()), this, SLOT(onSearchPanelExitClicked()));
    connect(mSearchPanel, SIGNAL(criteriaChanged(const QString&)), this, SLOT(onSearchPanelCriteriaChanged(const QString&)));
}

void HelpCategoryView::initToolbar()
{
    mToolBar = HelpUIBuilder::findWidget<HbToolBar*>(DOCML_TOOLBAR);

    HbAction* allAction = HelpUIBuilder::findObject<HbAction*>(DOCML_ACTION_ALL);
    HbAction* findAction = HelpUIBuilder::findObject<HbAction*>(DOCML_ACTION_SEARCH);
    QActionGroup* toolBarActionGroup = new QActionGroup(mToolBar);
    allAction->setActionGroup(toolBarActionGroup);
    findAction->setActionGroup(toolBarActionGroup);
}


////////////////////////////////////////////////////////////////////////////////////////////

void HelpCategoryView::switchViewMode(ViewMode viewMode)
{
    if(ViewModeNull == viewMode) return;
    
    mViewMode = viewMode;
    if(ViewModeAll == viewMode)
    {
        HelpUIBuilder::load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_ALL);
    }
    else if(ViewModeSearch == viewMode)
    {
        HelpUIBuilder::load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_SEARCH);
    }

    RefreshTitlebarVisibility();
}


////////////////////////////////////////////////////////////////////////////////////////////

void HelpCategoryView::RefreshToolbarText(bool isLandscape)
{
    HbAction* tollbarAction = HelpUIBuilder::findObject<HbAction*>(DOCML_ACTION_ALL);
    tollbarAction->setText(isLandscape ? qtTrId(TXT_BUTTON_ALL) : QString());

    tollbarAction = HelpUIBuilder::findObject<HbAction*>(DOCML_ACTION_SEARCH);
    tollbarAction->setText(isLandscape ? qtTrId(TXT_BUTTON_FIND) : QString());

    tollbarAction = HelpUIBuilder::findObject<HbAction*>(DOCML_ACTION_LINK_NOKIA);
    tollbarAction->setText(isLandscape ? qtTrId(TXT_BUTTON_LINK_SUPPORT) : QString());
}

void HelpCategoryView::RefreshTitlebarVisibility()
{
	setTitleBarVisible(!(mSearchPanel->isVisible()));
}

void HelpCategoryView::ResetSearchPanel()
{
    foreach(QGraphicsItem *obj, mSearchPanel->childItems())
    {
        QGraphicsWidget *const widget = static_cast<QGraphicsWidget*>(obj);
        if(widget != NULL)
        {
            HbLineEdit *const lineEdit = qobject_cast<HbLineEdit*>(widget);
            if(lineEdit != NULL)
            {
                lineEdit->setText("");
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// handle system event

void HelpCategoryView::onCurrentViewChanged(HbView *view)
{
    if(this == view)
    {
        setNavigationAction(mSoftKeyAction);        
    }
}

void HelpCategoryView::onOrientationChanged(Qt::Orientation orientation)
{
    bool isLandscape = (Qt::Horizontal==orientation);
    RefreshToolbarText(isLandscape);
}


///////////////////////////////////////////////////////////////////////////////////////
// handle button back action

void HelpCategoryView::onBackAction()
{
	if(this == mainWindow()->currentView())
	{
		HbApplication::exit();
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
// handle list event

void HelpCategoryView::onAllListActivated(const QModelIndex& index)
{
    if(!index.isValid() ||          // invalid
       index.child(0,0).isValid())  // this is a node
    {
        return;
    }

    QString uid = mListAll->model()->data(index.parent(), UidRole).toString();
    QString href = mListAll->model()->data(index, HrefRole).toString();
    HelpDataProvider::instance()->setHelpContentUrl(uid, href);
    emit activateView(HelpViewContents);
}

void HelpCategoryView::onSearchListActivated(const QModelIndex& index)
{
    if(!index.isValid() ||          // invalid
       index.child(0,0).isValid())  // this is a node
    {
        return;
    }
    
    QString uid = mListSearch->model()->data(index, UidRole).toString();
    QString href = mListSearch->model()->data(index, HrefRole).toString();
    HelpDataProvider::instance()->setHelpContentUrl(uid, href);
    emit activateView(HelpViewContents);
}


////////////////////////////////////////////////////////////////////////////////////////////
// handle search panel event

void HelpCategoryView::onSearchPanelExitClicked()
{
    setTitleBarVisible(true);
    HelpUIBuilder::load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_SEARCH_NO_SRHPAL);
}

void HelpCategoryView::onSearchPanelCriteriaChanged(const QString &criteria)
{
    mListSearch->setModel(HelpDataProvider::instance()->getSearchData(criteria));
    mListSearch->update();
}


// end of file
