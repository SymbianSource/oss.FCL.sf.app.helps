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

#include <hbnotificationdialog.h>
#include <hbaction.h>

#include "HelpCategoryView.h"
#include "HelpContentsView.h"

#include "HelpMainWindow.h"
#include "HelpDocumentLoader.h"
#include "HelpDataProvider.h"

HelpMainWindow::HelpMainWindow() : 
mCategoryView(NULL),
mContentsView(NULL)
{
    activateCategoryView();
}

HelpMainWindow::~HelpMainWindow()
{
    HelpUIBuilder::destroyInstance();
    HelpDataProvider::destroyInstance();
}

void HelpMainWindow::onActivateView(HelpViewName viewName)
{
	switch(viewName)
	{
	    case HelpViewCategory:
            activateCategoryView();
			break;

	    case HelpViewContents:
            activateContentsView();
			break;

	    default:
	        break;
	}
}

void HelpMainWindow::activateCategoryView()
{
    if(!mCategoryView)
    {
        HelpUIBuilder::load(QRC_DOCML_CATEGORY);
        mCategoryView = HelpUIBuilder::findWidget<HelpCategoryView*>(DOCML_VIEW_CATEGORY);
        addView(mCategoryView);
        mCategoryView->init();
        emit currentViewChanged(mCategoryView);
		connectViewSignal(mCategoryView);
    }

    setCurrentView(mCategoryView);
}

void HelpMainWindow::activateContentsView()
{
    if(!mContentsView)
    {
        HelpUIBuilder::load(QRC_DOCML_CONTENTS);
        mContentsView = HelpUIBuilder::findWidget<HelpContentsView*>(DOCML_VIEW_CONTENTS);
        addView(mContentsView);
        mContentsView->init();

        connectViewSignal(mContentsView);
    }

    setCurrentView(mContentsView);
}

void HelpMainWindow::connectViewSignal(const QObject *object)
{
    connect(object, SIGNAL(activateView(HelpViewName)), this, SLOT(onActivateView(HelpViewName)));
    
    connect(object, SIGNAL(showAllList()), this, SLOT(onShowAllList()));
    connect(object, SIGNAL(showFindList()), this, SLOT(onShowFindList()));
    connect(object, SIGNAL(showOnlineSupport()), this, SLOT(onShowOnlineSupport()));
}


////////////////////////////////////////////////////////////////////////////////////
// handle view event

void HelpMainWindow::onShowAllList()
{
    activateCategoryView();
    mCategoryView->switchViewMode(HelpCategoryView::ViewModeAll);
}

void HelpMainWindow::onShowFindList()
{
    activateCategoryView();
    mCategoryView->switchViewMode(HelpCategoryView::ViewModeSearch);
}

void HelpMainWindow::onShowOnlineSupport()
{
    HbNotificationDialog *notificationDialog = new HbNotificationDialog();
    notificationDialog->setParent(this);
    notificationDialog->setTitle(URL_LINK_SUPPORT);
    notificationDialog->show();
}

// end of file

