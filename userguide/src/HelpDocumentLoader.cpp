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

#include "HelpCategoryView.h"
#include "HelpContentsView.h"
#include "BrowserWrapper.h"

#include "HelpDocumentLoader.h"


////////////////////////////////////////////////////////////////////////////////////////////

class HelpDocumentLoader : public HbDocumentLoader
{
private:
    virtual QObject* createObject(const QString& type, const QString& name);
};
    
QObject* HelpDocumentLoader::createObject(const QString& type, const QString& name)
{
    if(type == HelpCategoryView::staticMetaObject.className())
    {
        QObject* object = new HelpCategoryView;
        object->setObjectName(name);
        return object;
    }

    if(type == HelpContentsView::staticMetaObject.className())
    {
        QObject* object = new HelpContentsView;
        object->setObjectName(name);
        return object;
    }
    
    if(type == BrowserWrapper::staticMetaObject.className())    
    {
        QObject* object = new BrowserWrapper;
        object->setObjectName(name);
        return object;
    }
        
    return HbDocumentLoader::createObject(type, name);
}


////////////////////////////////////////////////////////////////////////////////////////////

HbDocumentLoader* gHelpUIBuilderInstance = NULL;

HbDocumentLoader* HelpUIBuilder::instance()
{
    if(!gHelpUIBuilderInstance)
    {
        gHelpUIBuilderInstance = new HelpDocumentLoader();
    }

    return gHelpUIBuilderInstance;
}

void HelpUIBuilder::destroyInstance()
{
    delete gHelpUIBuilderInstance;
    gHelpUIBuilderInstance = NULL;
}

QObjectList HelpUIBuilder::load(const QString& fileName)
{ 
    bool ok = false;
    QObjectList list = instance()->load(fileName, &ok);
    Q_ASSERT_X(ok, "Help", "HelpUIBuilder load file");
    return list;
}

QObjectList HelpUIBuilder::load(const QString& fileName, const QString& section)
{
    bool ok = false;
    QObjectList list = instance()->load(fileName, section, &ok);
    Q_ASSERT_X(ok, "Help", "HelpUIBuilder load section");
    return list;
}

QGraphicsWidget* HelpUIBuilder::findWidget(const QString& name)
{
    return instance()->findWidget(name);
}

QObject* HelpUIBuilder::findObject(const QString& name)
{
    return instance()->findObject(name);
}



// end of file

