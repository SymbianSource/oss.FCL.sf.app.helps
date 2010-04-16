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

#ifndef HELPDOCUMENTLOADER_H
#define HELPDOCUMENTLOADER_H

#include <hbdocumentloader.h>

class HelpUIBuilder
{
public:
    static void destroyInstance();
    static QObjectList load(const QString& fileName);
    static QObjectList load(const QString& fileName, const QString& section);
    static QGraphicsWidget* findWidget(const QString& name);
    static QObject* findObject(const QString& name);

public:
    template<class T> static T findWidget(const QString& name) { return qobject_cast<T>(findWidget(name)); }
    template<class T> static T findObject(const QString& name) { return qobject_cast<T>(findObject(name)); }
    
private:
    static HbDocumentLoader* instance();
};


#endif //HELPDOCUMENTLOADER_H
