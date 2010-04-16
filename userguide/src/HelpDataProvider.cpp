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
#include <QXmlQuery>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QDir>

#include "HelpUtils.h"
#include "HelpCommon.h"
#include "HelpProxyModel.h"
#include "HelpStandardItem.h"

#include "HelpDataProvider.h"


HelpDataProvider::HelpDataProvider()
{
	mHelpModel = new QStandardItemModel();
	mKeywordModel = new QStandardItemModel();
	mSearhResultModel = NULL;
}

HelpDataProvider::~HelpDataProvider()
{
	delete mHelpModel;
	delete mSearhResultModel;
	delete mKeywordModel;
}


///////////////////////////////////////////////////////////////////////////////////////

HelpDataProvider* gHelpDataProviderInstance = NULL;

HelpDataProvider* HelpDataProvider::instance()
{
    if(!gHelpDataProviderInstance)
    {
        gHelpDataProviderInstance = new HelpDataProvider();
    }

    return gHelpDataProviderInstance;
}

void HelpDataProvider::destroyInstance()
{
    delete gHelpDataProviderInstance;
    gHelpDataProviderInstance = NULL;
}


///////////////////////////////////////////////////////////////////////////////////////

QAbstractItemModel* HelpDataProvider::getCategoryData()
{
    if(!mHelpModel->rowCount())
    {
        createHelpCategory();
    }

    return mHelpModel;
}

QAbstractItemModel* HelpDataProvider::getSearchData(const QString& key)
{
    delete mSearhResultModel;
    mSearhResultModel = NULL;
    
    mSearhResultModel = new HelpProxyModel();;
    mSearhResultModel->setSourceModel(mKeywordModel);
    mSearhResultModel->setFilterRole(KeywordRole);
    mSearhResultModel->setFilterRegExp(key);

    return mSearhResultModel;
}

void HelpDataProvider::setHelpContentUrl(const QString& uid, const QString& href)
{
    QUrl url = QUrl::fromLocalFile(uid+BACKSLASH+href);
    mHelpContentRoot = url.toString();
}

void HelpDataProvider::getHelpContentData(QString& content, QString& url)
{
    // url
    if(url.isEmpty())
    {
        url = mHelpContentRoot;
    }
    url.remove(URL_HEADER_LOCAL);

    // uid
    QString uid = url.section(BACKSLASH, 0, -3);
    uid.append(BACKSLASH + url.section(BACKSLASH, -2,-2));

    // href
    QString href = url.section(BACKSLASH, -1,-1);
    int anchorPos = href.indexOf(HASHMARK);
    if(anchorPos != -1)
    {
        href = href.section(HASHMARK, 0, 0);
    }

    QString path(uid);
    path.append(BACKSLASH);
    path.append(CONTENTSZIP);
    HelpUtils::loadHtmlFromZipFile(path , href, content);
}


///////////////////////////////////////////////////////////////////////////////////////

void HelpDataProvider::createHelpCategory()
{
	QFileInfoList driveList = QDir::drives();
	QDir dir;
	QString lang = HelpUtils::UILocaleFromQtToSymbian();

	QString path(HelpUtils::rootPath());
	path.append(XHTMLPATH);
	path.append(lang);

	//construct help in rom
	createBuiltInCategory(path);

	//scan other root path and construct 3rd party help
	foreach(QFileInfo fi, driveList)
	{
		path.clear();
		path.append(fi.absolutePath());
		path.append(XHTMLPATH);
		path.append(lang);
		dir.setPath(path);
		if(dir.exists())
		{
			if(QString::compare(fi.absolutePath(), HelpUtils::rootPath(), Qt::CaseInsensitive) == 0)
			{
				continue;
			}
			constructAppHelp(path);
		}
	}
}

void HelpDataProvider::createBuiltInCategory(const QString& path)
{
	QString pathIndex(path);
	pathIndex.append(BACKSLASH);
	pathIndex.append(INDEXXML);

	QFile file(pathIndex);
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	//parse index xml to a stringlist, each string include id and navtitle and seperate by "specilchar"
	QXmlQuery query;
	query.bindVariable("inputdoc", &file);
	QXmlItem xmlItem(SPECIALCHAR);
	query.bindVariable("specilchar", xmlItem);
	query.setQuery("doc($inputdoc)/collections/collection/ \
					string-join((xs:string(@id), xs:string(@navtitle)), $specilchar)");

	if(!query.isValid())
	{
		return;
	}
	QStringList strLst;
	if(!query.evaluateTo(&strLst))
	{
		return;
	}

	foreach(QString str, strLst)
	{
		QStringList temp;
		temp = str.split(SPECIALCHAR);
		QString uid(path);
		uid.append(BACKSLASH);
		uid.append(temp[0]);
		HelpStandardItem* item = constructCategory2(temp[1], uid);
		if(item)
		{
			mHelpModel->appendRow(item);
			constructKeywordModel(uid);
		}
	}
	file.close();
	mHelpModel->sort(0, HelpUtils::sortOrder());
}

HelpStandardItem* HelpDataProvider::constructCategory2(const QString& title, const QString& uid)
{
	QString pathIndex(uid);
	pathIndex.append(BACKSLASH);
	pathIndex.append(INDEXXML);

	QFile file(pathIndex);
	if (!file.open(QIODevice::ReadOnly)) {
		return NULL;
	}

	//parse index xml to a stringlist, each string include href and navtitle and seperate by "specilchar"
	QXmlQuery query;
	QXmlItem xmlItem(SPECIALCHAR);
	query.bindVariable("inputdoc", &file);
	query.bindVariable("specilchar", xmlItem);
	query.setQuery("doc($inputdoc)/topics/topicref/ \
					string-join((xs:string(@href), xs:string(@navtitle)), $specilchar)");
	if(!query.isValid())
	{
		return NULL;
	}

	QStringList strLst;
	if(!query.evaluateTo(&strLst))
	{
		return NULL;
	}	
	if(strLst.count() <= 0)
	{
		return NULL;
	}

	HelpStandardItem* itemParent = NULL;
	itemParent = new HelpStandardItem(title);
	itemParent->setData(uid, UidRole);
	foreach(QString str, strLst)
	{
		QStringList temp;
		temp = str.split(SPECIALCHAR);
		HelpStandardItem* item = new HelpStandardItem(temp[1]);
		item->setData(temp[0], HrefRole);
		itemParent->appendRow(item);
	}

	file.close();
	itemParent->sortChildren(0, HelpUtils::sortOrder());
	return itemParent;
}

void HelpDataProvider::constructAppHelp(const QString& path)
{
	QDir dir(path);	
	if(!dir.exists())
	{
		return;
	}

	QStringList uidList = dir.entryList();
	HelpStandardItem* itemApp = NULL;
	QString pathTemp;
	foreach(QString uid, uidList)
	{
		pathTemp.clear();
		pathTemp.append(path);
		pathTemp.append(BACKSLASH);
		pathTemp.append(uid);
		pathTemp.append(BACKSLASH);
		pathTemp.append(METAXML);
		QFile file(pathTemp);
		if (!file.open(QIODevice::ReadOnly)) {
			continue;
		}

		//parse meta xml, get the title string
		QXmlQuery query;
		query.bindVariable("inputdoc", &file);
		query.setQuery("doc($inputdoc)/meta/string(title)");
		if(!query.isValid())
		{
			continue;
		}
		QString titleStr;
		if(!query.evaluateTo(&titleStr))
		{
			continue;
		}

		pathTemp.clear();
		pathTemp.append(path);
		pathTemp.append(BACKSLASH);
		pathTemp.append(uid);
		HelpStandardItem* item = constructCategory2(titleStr, pathTemp);
		if(item)
		{
			if(!itemApp)
			{
				itemApp = new HelpStandardItem("Applications");
			}
			itemApp->appendRow(item);
			constructKeywordModel(pathTemp);
		}
		file.close();
	}

	if(itemApp)
	{
		itemApp->sortChildren(0, HelpUtils::sortOrder());
		mHelpModel->appendRow(itemApp);
	}
}

void HelpDataProvider::constructKeywordModel(const QString& path)
{
	QString pathKeyword(path);
	pathKeyword.append(BACKSLASH);
	pathKeyword.append(KEYWORDXML);

	QFile file(pathKeyword);
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	//construct keyword model, title and keyword is one to more
	QXmlStreamReader reader(&file);
	QString keyword;
	QString title;

	while (!reader.atEnd()) {
		if (!reader.readNextStartElement()) 
		{	
			continue;
		}
		if (reader.name() == "text")
		{
			keyword = reader.readElementText();
		}
		else if (reader.name() == "target")
		{
			QString href = reader.attributes().value("href").toString();
			HelpStandardItem* item = findItemWithHref((HelpStandardItem *)(mKeywordModel->invisibleRootItem()), href);
			QStringList keywordLst;
			if(item)
			{
				keywordLst = item->data(KeywordRole).toStringList();
				keywordLst.append(keyword);
				item->setData(keywordLst,KeywordRole);
			}
			else
			{
				item = new HelpStandardItem(reader.readElementText());
				item->setData(path, UidRole);
				item->setData(href, HrefRole);

				keywordLst.append(keyword);
				item->setData(keywordLst,KeywordRole);
				mKeywordModel->appendRow(item);
			}				
		}			
	}
	file.close();
	mKeywordModel->sort(0, HelpUtils::sortOrder());
}

HelpStandardItem* HelpDataProvider::findItemWithHref(HelpStandardItem* itemParent, const QString& href)
{
	for(int i = 0; i < itemParent->rowCount(); i++)
	{
		if(QString::compare(itemParent->child(i)->data(HrefRole).toString(), href, Qt::CaseInsensitive) == 0)
		{
			return (HelpStandardItem *)(itemParent->child(i));
		}
	}
	return NULL;
}
