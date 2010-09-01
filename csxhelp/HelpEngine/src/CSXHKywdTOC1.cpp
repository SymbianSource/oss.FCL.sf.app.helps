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
* Description:  CCSXHKywdTOC1 class definition
*
*/

#include "CSXHKywdTOC1.h"
#include "CSXHViewIDs.h"
#include "CSXHHelpDataBase.h"

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHKywdTOC1* CCSXHKywdTOC1::NewL(const TDesC& aName)
    {
    CCSXHKywdTOC1* self = CCSXHKywdTOC1::NewLC(aName);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHKywdTOC1* CCSXHKywdTOC1::NewLC(const TDesC& aName)
    {
    CCSXHKywdTOC1* self = new(ELeave) CCSXHKywdTOC1(aName);
    CleanupStack::PushL(self);
    return self;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
CCSXHKywdTOC1::CCSXHKywdTOC1(const TDesC& aName):
                CCSXHGenericTOC1(aName),iLegacyIndex(-1),iTOC1HtmlList(NULL)
    {
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
CCSXHKywdTOC1::~CCSXHKywdTOC1()
    {
    if(iTOC1HtmlList)
        {
        iTOC1HtmlList->Close();
        delete iTOC1HtmlList;   
        }
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
TUid CCSXHKywdTOC1::GetViewID() const
    {
    return KCSXHKywdToc2ViewID;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
TInt& CCSXHKywdTOC1::LegacyIndex()
    {
    return iLegacyIndex;    
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
void CCSXHKywdTOC1::AppendHtmlTOC1L(CCSXHHtmlTOC1* aToc1)
    {
    if(NULL == iTOC1HtmlList)
        iTOC1HtmlList = new(ELeave) RPointerArray<CCSXHHtmlTOC1>;
    iTOC1HtmlList->Append(aToc1);
    }
    
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
RPointerArray<CCSXHHtmlTOC1>* CCSXHKywdTOC1::GetHtmlTOC1List()
    {
    return iTOC1HtmlList;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
void CCSXHKywdTOC1::InitChildList()
    {
    if(iChildList != NULL)
        return;
    ConstructChildList();
    TRAP_IGNORE(CCSXHHelpDataBase::GetInstance()->GenerateKeywordTOC2ListL(this));
    }

