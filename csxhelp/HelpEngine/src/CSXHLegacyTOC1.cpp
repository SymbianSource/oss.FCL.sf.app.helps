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
* Description:  CCSXHLegacyTOC1 class definition
*
*/

#include "CSXHLegacyTOC1.h"
#include "CSXHViewIDs.h"
#include "CSXHLegacyContentParser.h"
#include "CSXHHelpDataBase.h"

#include <e32base.h>

CCSXHLegacyTOC1* CCSXHLegacyTOC1::NewL(const TDesC& aName)
    {
    CCSXHLegacyTOC1* self = CCSXHLegacyTOC1::NewLC(aName);
    CleanupStack::Pop(self);
    return self;
    }

CCSXHLegacyTOC1* CCSXHLegacyTOC1::NewLC(const TDesC& aName)
    {
    CCSXHLegacyTOC1* self = new(ELeave) CCSXHLegacyTOC1(aName);
    CleanupStack::PushL(self);
    return self;
    }

CCSXHLegacyTOC1::CCSXHLegacyTOC1(const TDesC& aName) :
CCSXHGenericTOC1(aName)
    {//No implementation required
    }

TUid CCSXHLegacyTOC1::GetViewID() const
    {
    return KCSXHToc2ViewID;
    }
    
CCSXHHelpContentBase* CCSXHLegacyTOC1::GetContextTopic(const TDesC& aContextName)
    {
    TBool resetChildList = EFalse;
    if(iChildList ==NULL)
        {
        InitChildList();//Generates child list if required.     
        resetChildList = ETrue;
        }
        
    TInt count =    iChildList->Count();
    CCSXHHelpContentBase* toc2 = NULL;
    for(TInt i=0;i<count;++i )
        {
        toc2 = (*iChildList)[i];
        if(toc2->GetName().Compare(aContextName) == 0)
            return toc2;//Child list should not be reset
        }
    if(resetChildList)
        ResetChildList();
    return NULL;    
    }

void CCSXHLegacyTOC1::InitChildList()
    {
    if(iChildList != NULL)
        return;
    ConstructChildList();
    
    TRAP_IGNORE(CCSXHHelpDataBase::GetInstance()->
    GetLegacyParser()->GenerateTOC2ListL(*this,iChildList));
    }
    
