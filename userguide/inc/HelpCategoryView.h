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
#include "HelpDocumentLoader.h"

class HbTreeView;
class HbListView;
class HbSearchPanel;
class QStandardItem;
class HbStaticVkbHost;
class HbGroupBox;

class HelpCategoryView : public HbView
{
	Q_OBJECT
	
public:
	HelpCategoryView();
	~HelpCategoryView();
	void init();

private:
	void initDocMl();
	void initAllList();
	void initSearchList();
	void initSearchPanel();
	void initBackAction();
	void initEmptyLabel();
	void initVirtualKeyboard();
	
private:
	HbGroupBox* groupBox();

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
    
private:
	void updateVisibleItems(bool visible);
	void ResetSearchPanel();	

private:
	void expandCollapseAllList(QStandardItem* item, bool expand);

private slots: // handle system event
    void onCurrentViewChanged(HbView *view);

private slots: // handle button action
    void onBackAction();
    
private slots: // handle list event
    void onAllListActivated(const QModelIndex& index);
    void onSearchListActivated(const QModelIndex& index);

private slots: // handle search panel event
	void onSearchPanelExitClicked();
	void onSearchPanelCriteriaChanged(const QString &criteria);

private slots:
	void onExpandAll();
	void onCollapseAll();

private slots: // handle virtual keyboard event
    void onHandleKeypadOpen();
    void onHandleKeypadClose();

private:
    ViewMode			mViewMode;
	HbTreeView*			mListAll;
	HbListView*			mListSearch;
	HbSearchPanel*		mSearchPanel;
	HbAction*			mSoftKeyAction;
	HbStaticVkbHost*	mVirtualKeyboard;
	HelpUIBuilder		mBuilder;
};

#endif //HELPCATEGORYVIEW_H
