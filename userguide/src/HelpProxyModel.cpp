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

#include <QStringList>
#include <QDebug>

#include <hbstringutil.h>

#include "HelpCommon.h"
#include "HelpProxyModel.h"

HelpProxyModel::HelpProxyModel(QObject * parent):QSortFilterProxyModel(parent)
{
}

HelpProxyModel::~HelpProxyModel()
{
}

bool HelpProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	switch(filterRole())
	{
	case KeywordRole:
		{			
			QStringList keywordLst = sourceModel()->data(index, KeywordRole).toStringList();

			foreach(QString str, keywordLst)
			{
				if(HbStringUtil::findC(str, filterRegExp().pattern()) != -1)
				{
					return true;
				}
			}
			return false;
		}
	default:
		return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
	}
}
