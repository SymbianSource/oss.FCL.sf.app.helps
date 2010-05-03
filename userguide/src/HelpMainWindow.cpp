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

#include <hbtoolbar.h>
#include <hbnotificationdialog.h>
#include <hbaction.h>

#include "HelpCategoryView.h"
#include "HelpContentsView.h"

#include "HelpMainWindow.h"
#include "HelpDataProvider.h"

HelpMainWindow::HelpMainWindow() : 
mCategoryView(NULL),
mContentsView(NULL)
{
	connect(this, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onOrientationChanged(Qt::Orientation)));
	initToolbar();
    activateCategoryView();
}

HelpMainWindow::~HelpMainWindow()
{
    HelpDataProvider::destroyInstance();
}

void HelpMainWindow::initToolbar()
{
	mBuilder.load(QRC_DOCML_TOOLBAR);
	mToolBar = mBuilder.findWidget<HbToolBar*>(DOCML_TOOLBAR);

    HbAction* allAction = mBuilder.findObject<HbAction*>(DOCML_ACTION_ALL);
    HbAction* findAction = mBuilder.findObject<HbAction*>(DOCML_ACTION_SEARCH);
	HbAction* onLineSupportAction = mBuilder.findObject<HbAction*>(DOCML_ACTION_LINK_NOKIA);

	connect(allAction, SIGNAL(triggered()), this, SLOT(onToolbarAll()));
	connect(findAction, SIGNAL(triggered()), this, SLOT(onToolbarFind()));
	connect(onLineSupportAction, SIGNAL(triggered()), this, SLOT(onToolbarOnlineSupport()));
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
        mCategoryView = new HelpCategoryView();
        addView(mCategoryView);
        mCategoryView->init();
		mCategoryView->setToolBar(mToolBar);
        emit currentViewChanged(mCategoryView);
		connectViewSignal(mCategoryView);
    }

    setCurrentView(mCategoryView);
}

void HelpMainWindow::activateContentsView()
{
    if(!mContentsView)
    {
		mContentsView = new HelpContentsView();
        addView(mContentsView);
        mContentsView->init();
		mContentsView->setToolBar(mToolBar);

        connectViewSignal(mContentsView);
    }

    setCurrentView(mContentsView);
}

void HelpMainWindow::connectViewSignal(const QObject *object)
{
    connect(object, SIGNAL(activateView(HelpViewName)), this, SLOT(onActivateView(HelpViewName)));
}


////////////////////////////////////////////////////////////////////////////////////
// handle view event

void HelpMainWindow::onToolbarAll()
{
    activateCategoryView();
    mCategoryView->switchViewMode(HelpCategoryView::ViewModeAll);
}

void HelpMainWindow::onToolbarFind()
{
    activateCategoryView();
    mCategoryView->switchViewMode(HelpCategoryView::ViewModeSearch);
}

void HelpMainWindow::onToolbarOnlineSupport()
{
    HbNotificationDialog *notificationDialog = new HbNotificationDialog();
    notificationDialog->setParent(this);
    notificationDialog->setTitle(URL_LINK_SUPPORT);
    notificationDialog->show();
}

void HelpMainWindow::onOrientationChanged(Qt::Orientation orientation)
{
    RefreshToolbarText(orientation);
}

void HelpMainWindow::RefreshToolbarText(Qt::Orientation orientation)
{
	bool isLandscape = (Qt::Horizontal==orientation);
    HbAction* tollbarAction = mBuilder.findObject<HbAction*>(DOCML_ACTION_ALL);
    tollbarAction->setText(isLandscape ? qtTrId(TXT_BUTTON_ALL) : QString());

    tollbarAction = mBuilder.findObject<HbAction*>(DOCML_ACTION_SEARCH);
    tollbarAction->setText(isLandscape ? qtTrId(TXT_BUTTON_FIND) : QString());

    tollbarAction = mBuilder.findObject<HbAction*>(DOCML_ACTION_LINK_NOKIA);
    tollbarAction->setText(isLandscape ? qtTrId(TXT_BUTTON_LINK_SUPPORT) : QString());
}

// end of file

