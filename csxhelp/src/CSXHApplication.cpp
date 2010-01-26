/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CCSXHApplication class definition
*
*/


#include "CSXHDocument.h"
#include "CSXHApplication.h"

// UID for the application, this should correspond to the uid defined in the mmp file
static const TUid KUidCSXHApp = {0x10005234};

CApaDocument* CCSXHApplication::CreateDocumentL()
    {  
    // Create an CSXH document, and return a pointer to it
    CApaDocument* document = CCSXHDocument::NewL(*this);
    return document;
    }

TUid CCSXHApplication::AppDllUid() const
    {
    // Return the UID for the CSXH application
    return KUidCSXHApp;
    }

