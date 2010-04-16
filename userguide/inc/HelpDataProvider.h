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

#ifndef HELPDATAPROVIDER_H
#define HELPDATAPROVIDER_H

class HelpModel;
class HelpItems;
class HelpProxyModel;
class QStandardItemModel;
class HelpStandardItem;

class HelpDataProvider
{	
private:
    HelpDataProvider();
    ~HelpDataProvider();
    
public:
    static HelpDataProvider* instance();
    static void destroyInstance();

public:
	QAbstractItemModel* getCategoryData();
	QAbstractItemModel* getSearchData(const QString& key=QString());
	void setHelpContentUrl(const QString& uid, const QString& href);
	void getHelpContentData(QString& content, QString& url);

private:
	void createHelpCategory();
	void createBuiltInCategory(const QString& path);
	void constructAppHelp(const QString& path);
	void constructKeywordModel(const QString& path);
	HelpStandardItem* constructCategory2(const QString& title, const QString& uid);
	HelpStandardItem* findItemWithHref(HelpStandardItem* itemParent, const QString& href);

private:
	QStandardItemModel*    mHelpModel;         //category tree model
	QStandardItemModel*    mKeywordModel;      //keyword list model
	HelpProxyModel*        mSearhResultModel;  //search result proxy model of keyword model
	
	QString                mHelpContentRoot;

};

#endif //HELPDATAPROVIDER_H
