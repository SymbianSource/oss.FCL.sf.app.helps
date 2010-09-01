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
* Description:  CCSXHContextTopic class definition
*
*/

#include "CSXHContextTopic.h"
#include "CSXHViewIDs.h"
#include "csxhconstants.h"

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHContextTopic* CCSXHContextTopic::NewL(CCSXHHelpContentBase* aToc2)
    {
    CCSXHContextTopic* self = new(ELeave) CCSXHContextTopic(aToc2);
    return self;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHContextTopic::CCSXHContextTopic(CCSXHHelpContentBase* aToc2):
CCSXHHelpContentBase(KEmptyString),iToc2(aToc2)
    {//No Implementation required
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
CCSXHContextTopic::~CCSXHContextTopic()
    {
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
TUid CCSXHContextTopic::GetViewID() const
    {
    return iToc2->GetViewID() == KCSXHHtmlTopicViewID ? 
                KCSXHContextHtmlTopicViewID : KCSXHContextLegacyTopicViewID;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
EXPORT_C CCSXHHelpContentBase* CCSXHContextTopic::GetTopic()
    {
    return iToc2;
    }

