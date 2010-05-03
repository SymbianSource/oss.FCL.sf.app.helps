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
#include <hbtoolbar.h>
#include <hblabel.h>

#include <hbtreeview.h>
#include <hblistview.h>
#include <hbsearchpanel.h>
#include <hbscrollbar.h>
#include <hblineedit.h>
#include <hbmenu.h>
#include <hbstaticvkbhost.h>
#include <hbgroupbox.h>

#include "HelpDataProvider.h"
#include "HelpProxyModel.h"

#include "HelpCategoryView.h"


//////////////////////////////////////////////////////////////////////////////////////////////

HelpCategoryView::HelpCategoryView() : 
mViewMode(ViewModeNull), 
mListAll(NULL), 
mListSearch(NULL), 
mSearchPanel(NULL)
{
}

HelpCategoryView::~HelpCategoryView()
{
}


////////////////////////////////////////////////////////////////////////////////////////////

void HelpCategoryView::init()
{
	initDocMl();
    initBackAction();
    initAllList();
    initSearchList();
    initSearchPanel();
	initVirtualKeyboard();
	initEmptyLabel();

    switchViewMode(ViewModeAll);
    
    connect(mainWindow(), SIGNAL(currentViewChanged(HbView*)), this, SLOT(onCurrentViewChanged(HbView*)));
}

void HelpCategoryView::initDocMl()
{
 // Create widget hierarchy
    setObjectName( DOCML_VIEW_CATEGORY );

    // List existing root elements - this allows us to refer to objects in the XML 
    // which are created outside the document.
    QObjectList roots;
    roots.append( this );

	mBuilder.setObjectTree(roots);

    mBuilder.load(QRC_DOCML_CATEGORY);
}

void HelpCategoryView::initBackAction()
{
    mSoftKeyAction = new HbAction(Hb::BackAction);
    connect(mSoftKeyAction, SIGNAL(triggered()), this, SLOT(onBackAction()));
}

void HelpCategoryView::initAllList()
{
    mListAll = mBuilder.findWidget<HbTreeView*>(DOCML_LIST_CATEGORY_ALL);
    mListAll->setHorizontalScrollBarPolicy(HbScrollArea::ScrollBarAlwaysOff);
    mListAll->setModel(HelpDataProvider::instance()->getCategoryData());
    connect(mListAll, SIGNAL(activated(const QModelIndex&)), this, SLOT(onAllListActivated(const QModelIndex&)));
}

void HelpCategoryView::initSearchList()
{
    mListSearch = mBuilder.findWidget<HbListView*>(DOCML_LIST_CATEGORY_SEARCH);
    mListSearch->setHorizontalScrollBarPolicy(HbScrollArea::ScrollBarAlwaysOff);
    mListSearch->setModel(HelpDataProvider::instance()->getSearchData());
    connect(mListSearch, SIGNAL(activated(const QModelIndex&)), this, SLOT(onSearchListActivated(const QModelIndex&)));
}
void HelpCategoryView::initSearchPanel()
{
    mSearchPanel = mBuilder.findWidget<HbSearchPanel*>(DOCML_SEARCH_PANEL_CATEGORY);
    connect(mSearchPanel, SIGNAL(exitClicked()), this, SLOT(onSearchPanelExitClicked()));
    connect(mSearchPanel, SIGNAL(criteriaChanged(const QString&)), this, SLOT(onSearchPanelCriteriaChanged(const QString&)));
}

void HelpCategoryView::initVirtualKeyboard()
{
	mVirtualKeyboard = new HbStaticVkbHost(this);
    connect(mVirtualKeyboard, SIGNAL(keypadOpened()), this, SLOT(onHandleKeypadOpen()));
    connect(mVirtualKeyboard, SIGNAL(keypadClosed()), this, SLOT(onHandleKeypadClose()));
}

void HelpCategoryView::initEmptyLabel()
{
	HbLabel* label = mBuilder.findWidget<HbLabel*>(DOCML_NO_MATCH_LABEL);
	label->setFontSpec(HbFontSpec(HbFontSpec::Primary));
}

HbGroupBox* HelpCategoryView::groupBox()
{
	return mBuilder.findWidget<HbGroupBox*>(DOCML_GROUPBOX);
}

////////////////////////////////////////////////////////////////////////////////////////////

void HelpCategoryView::switchViewMode(ViewMode viewMode)
{
    if(ViewModeNull == viewMode) return;
    
    mViewMode = viewMode;
    if(ViewModeAll == viewMode)
    {
        mBuilder.load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_ALL);
		toolBar()->show();
    }
    else if(ViewModeSearch == viewMode)
    {
//        mBuilder.load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_SEARCH);
		ResetSearchPanel();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

void HelpCategoryView::expandCollapseAllList(QStandardItem* item, bool expand)
{
	if(item->rowCount() <= 0)
	{
		return;
	}
	mListAll->setExpanded(item->index(),expand);
	for(int i = 0; i < item->rowCount(); i++)
	{		
		expandCollapseAllList(item->child(i),expand);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////

void HelpCategoryView::updateVisibleItems(bool visible)
{
	static Hb::SceneItems items = Hb::TitleBarItem | Hb::StatusBarItem;
	if(visible)
	{
		showItems(items);
	}
	else
	{
		hideItems(items);
	}
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

///////////////////////////////////////////////////////////////////////////////////////
// handle button back action

void HelpCategoryView::onBackAction()
{
	if(ViewModeSearch == mViewMode)
	{
		switchViewMode(ViewModeAll);
	}
	else
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
	if(mListSearch->model()->rowCount() == 0)
	{
		mBuilder.load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_SEARCH_NO_SRHPAL_NO_MATCH);
	}
	else
	{
		mBuilder.load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_SEARCH_NO_SRHPAL);
	}
	toolBar()->show();
}

void HelpCategoryView::onSearchPanelCriteriaChanged(const QString &criteria)
{
    HelpDataProvider::instance()->getSearchData(criteria);
	if(mListSearch->model()->rowCount() == 0)
	{
		mBuilder.load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_SEARCH_NO_MATCH);
	}
	else
	{
		mBuilder.load(QRC_DOCML_CATEGORY, DOCML_LAYOUT_CATEGORY_SEARCH);
	}

	toolBar()->hide();

	if(criteria.isEmpty())
	{
		groupBox()->setHeading(hbTrId(TXT_SETLABEL_SEARCH));
	}
	else
	{
		QString heading = qtTrId(TXT_SETLABEL_SEARCH_RESULTS);
		heading.append(COLON);
		heading.append(criteria);
		groupBox()->setHeading(heading);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// handle virtual keyboard event

void HelpCategoryView::onHandleKeypadOpen()
{
	updateVisibleItems(false);
    qreal heightToSet = mainWindow()->layoutRect().height() - mVirtualKeyboard->keyboardArea().height();
    this->setMaximumHeight(heightToSet);
}

void HelpCategoryView::onHandleKeypadClose()
{	
	updateVisibleItems(true);
	qreal mainHeight  = mainWindow()->layoutRect().height();
	qreal toolbarHeight = toolBar()->size().height();
	qreal height = mainHeight - (toolBar()->isVisible() ? toolbarHeight : 0);
	this->setMaximumHeight(height);
}

////////////////////////////////////////////////////////////////////////////////////////////
// handle menu event
void HelpCategoryView::onExpandAll()
{
	QStandardItemModel* model = (QStandardItemModel*)(mListAll->model());
	expandCollapseAllList(model->invisibleRootItem(),true);
}

void HelpCategoryView::onCollapseAll()
{
	QStandardItemModel* model = (QStandardItemModel*)(mListAll->model());
	expandCollapseAllList(model->invisibleRootItem(),false);
}


// end of file
