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
* Description:  CCSXHLegacyTOC2 class definition
*
*/

#include "CSXHLegacyTOC2.h"
#include "CSXHViewIDs.h"
#include "CSXHGenericTOC1.h"
#include "CSXHLegacyContentParser.h"
#include "CSXHHelpDataBase.h"

#include <e32base.h>

CCSXHLegacyTOC2* CCSXHLegacyTOC2::NewL(CCSXHGenericTOC1* aParent,const
TDesC& aName,TBool aKey)
    {
    CCSXHLegacyTOC2* self = CCSXHLegacyTOC2::NewLC(aParent,aName,aKey);
    CleanupStack::Pop(self);
    return self;
    }

CCSXHLegacyTOC2* CCSXHLegacyTOC2::NewLC(CCSXHGenericTOC1* aParent,const
TDesC& aName,TBool aKey)
    {
    CCSXHLegacyTOC2* self = new(ELeave) CCSXHLegacyTOC2(aParent,aName,aKey);
    CleanupStack::PushL(self);
    return self;
    }


CCSXHLegacyTOC2::CCSXHLegacyTOC2(CCSXHGenericTOC1* aParent,const TDesC& aName,TBool aKey) :
CCSXHGenericTOC2(aParent,aName),iKeyWordTopic (aKey)
    {//No implementation required
    }

TUid CCSXHLegacyTOC2::GetViewID() const
    {
    return KCSXHLegacyTopicViewID;
    }
    
EXPORT_C TAny* CCSXHLegacyTOC2::GetTopicContentL()
    {
    
    return CCSXHHelpDataBase::GetInstance()->GetLegacyParser()->GetTopicContentL(this);
    //return NULL;
    }

EXPORT_C  CCSXHLegacyTOC1* CCSXHLegacyTOC2::GetLegacyParent()
    {
    if(!iKeyWordTopic)
        return NULL;
    return CCSXHHelpDataBase::GetInstance()->GetLegacyParser()->GetTOC1ForTOC2(this);
    }
