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

#ifndef HELPMAINWINDOW_H
#define HELPMAINWINDOW_H

#include <hbmainwindow.h>

#include "HelpCommon.h"
#include "HelpDocumentLoader.h"

class HelpCategoryView;
class HelpContentsView;
class HbToolBar;

class HelpMainWindow : public HbMainWindow
{
	Q_OBJECT

public:
	HelpMainWindow();
	~HelpMainWindow();

private slots:
    void onActivateView(HelpViewName viewName);

private: // activate views
	void activateCategoryView();
	void activateContentsView();
	void initToolbar();
	void RefreshToolbarText(Qt::Orientation orientation);
	
private:
	void connectViewSignal(const QObject *object);

signals:
	void backActionSignal();
	
private slots: // handle tollbar event
    void onToolbarAll();
    void onToolbarFind();
    void onToolbarOnlineSupport();

private slots:
	void onOrientationChanged(Qt::Orientation orientation);

private:
	HelpCategoryView* mCategoryView;
	HelpContentsView* mContentsView;
	HbToolBar*         mToolBar;
	HelpUIBuilder mBuilder;

};

#endif //HELPMAINWINDOW_H
