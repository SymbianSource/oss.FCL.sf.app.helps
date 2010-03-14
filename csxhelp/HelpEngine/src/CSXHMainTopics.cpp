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
* Description:  CCSXHMainTopics class definition
*
*/

#include "CSXHMainTopics.h"
#include "CSXHHTMLContentParser.h"
#include "CSXHHelpDataBase.h"
#include "CSXHViewIDs.h"
#include "csxhconstants.h"

#include <eikenv.h> 
#include <e32base.h>

CCSXHMainTopics* CCSXHMainTopics::NewL(const TUid& aUid, const TApaAppCaption& aAppCaption, const TInt32 aPriority)
    {
    CCSXHMainTopics* self = CCSXHMainTopics::NewLC(aUid,aAppCaption, aPriority);
    CleanupStack::Pop(self);
    return self;
    }

CCSXHMainTopics* CCSXHMainTopics::NewLC(const TUid& aUid,const TApaAppCaption& aAppCaption, const TInt32 aPriority)
    {
    CCSXHMainTopics* self = new(ELeave) CCSXHMainTopics(aUid, aAppCaption, aPriority);
    CleanupStack::PushL(self);
    return self;
    }

CCSXHMainTopics::CCSXHMainTopics(const TUid& aUid,const TDesC& aName, const TInt32 aPriority) :
CCSXHGenericTOC1(aName, aPriority), iUid(aUid)
    {//No implementation required
    }

CCSXHMainTopics::~CCSXHMainTopics()
    {//No implementation required
    }   

TUid CCSXHMainTopics::GetViewID() const
    {
    return iUid;
    }

void CCSXHMainTopics::ResetChildList()
    {
    if(iChildList == NULL)
        return;
    TInt count = iChildList->Count();
    for(TInt i=0;i<count;++i )
        {
        STATIC_CAST(CCSXHGenericTOC1*,(*iChildList)[i])->ResetChildList();
        }
    }
