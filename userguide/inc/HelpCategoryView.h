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

#ifndef HELPCATEGORYVIEW_H
#define HELPCATEGORYVIEW_H

#include <hbview.h>

#include "HelpCommon.h"

class HbTreeView;
class HbToolBar;
class HbSearchPanel;

class HelpCategoryView : public HbView
{
	Q_OBJECT
	
public:
	HelpCategoryView();
	~HelpCategoryView();
	void init();

private:
	void initAllList();
	void initSearchList();
	void initSearchPanel();
	void initToolbar();
	void initBackAction();
	
public:
    enum ViewMode
    {
        ViewModeNull = 0, 
        ViewModeAll, 
        ViewModeSearch
    };
    void switchViewMode(ViewMode viewMode);

signals:
    void activateView(HelpViewName viewName);
    
signals: // from tollbar event
    void showAllList();
    void showFindList();
    void showOnlineSupport();

private:
	void RefreshToolbarText(bool isLandscape);
	void RefreshTitlebarVisibility();
	void ResetSearchPanel();
	
private slots: // handle system event
    void onCurrentViewChanged(HbView *view);
    void onOrientationChanged(Qt::Orientation orientation);

private slots: // handle button action
    void onBackAction();
    
private slots: // handle tollbar event
    void onToolbarAll()             { emit showAllList(); };
    void onToolbarFind()            { emit showFindList(); };
    void onToolbarOnlineSupport()   { emit showOnlineSupport(); };

private slots: // handle list event
    void onAllListActivated(const QModelIndex& index);
    void onSearchListActivated(const QModelIndex& index);

private slots: // handle search panel event
	void onSearchPanelExitClicked();
	void onSearchPanelCriteriaChanged(const QString &criteria);

private:
    ViewMode           mViewMode;
	HbTreeView*        mListAll;
	HbTreeView*        mListSearch;
	HbSearchPanel*     mSearchPanel;
	HbToolBar*         mToolBar;
	HbAction* mSoftKeyAction;
};

#endif //HELPCATEGORYVIEW_H
