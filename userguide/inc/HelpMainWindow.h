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

class HelpCategoryView;
class HelpContentsView;

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
	void initBackAction();
	
private:
	void connectViewSignal(const QObject *object);

signals:
	void backActionSignal();
	
private slots: // handle view event
    void onShowAllList();
    void onShowFindList();
    void onShowOnlineSupport();

private:
	HelpCategoryView* mCategoryView;
	HelpContentsView* mContentsView;

};

#endif //HELPMAINWINDOW_H
