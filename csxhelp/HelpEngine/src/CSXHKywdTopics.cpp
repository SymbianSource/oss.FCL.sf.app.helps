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
* Description:  CCSXHKywdTopics class definition
*
*/

#include "CSXHKywdTopics.h"
#include "CSXHHTMLContentParser.h"
#include "CSXHHelpDataBase.h"
#include "CSXHViewIDs.h"
#include "csxhconstants.h"
#include "CSXHKywdTOC1.h"

#include <eikenv.h> 
#include <e32base.h>

//Implementation class. This class will be used when all the keywords has to be displayed   
class MCSXHKywdTopicsBuilder : public MCSXHKywdBuilder
    {
    public:
    static MCSXHKywdTopicsBuilder* NewL()
        {
        MCSXHKywdTopicsBuilder* self = MCSXHKywdTopicsBuilder::NewLC();
        CleanupStack::Pop(self);
        return self;
        }
    static MCSXHKywdTopicsBuilder* NewLC()
        {
        MCSXHKywdTopicsBuilder* self = new(ELeave) MCSXHKywdTopicsBuilder();
        CleanupStack::PushL(self);
        return self;
        }
    
    void InsertKeywordL(CCSXHKywdTopics* aKwdTopics, const TDesC& aKwd,CCSXHHtmlTOC1*)
        {aKwdTopics->InsertKywdL(aKwd);}
    
    void InsertKeywordL(CCSXHKywdTopics* aKwdTopics, const TDesC& aKwd,const TInt&)
        {aKwdTopics->InsertKywdL(aKwd);}
    
    private:
    MCSXHKywdTopicsBuilder(){}
    };

    
//Implementation class. This class will be used when user has selected a keyword and
// wants to navigate to the search result view
class MCSXHKywdSelectedItemBuilder : public MCSXHKywdBuilder
    {
    public:
    static MCSXHKywdSelectedItemBuilder* NewL(CCSXHKywdTOC1* aCurrentTopic)
        {
        MCSXHKywdSelectedItemBuilder* self = MCSXHKywdSelectedItemBuilder::NewLC(aCurrentTopic);
        CleanupStack::Pop(self);
        return self;
        }
    static MCSXHKywdSelectedItemBuilder* NewLC(CCSXHKywdTOC1* aCurrentTopic)
        {
        MCSXHKywdSelectedItemBuilder* self = new(ELeave) 
        	MCSXHKywdSelectedItemBuilder(aCurrentTopic);
        CleanupStack::PushL(self);
        return self;
        }
    void InsertKeywordL(CCSXHKywdTopics*, const TDesC& aKwd,CCSXHHtmlTOC1* aToc1) 
        {
        if(iCurrentTopic->GetName().Compare(aKwd) == 0)
            iCurrentTopic->AppendHtmlTOC1L(aToc1);
        }
    void InsertKeywordL(CCSXHKywdTopics*, const TDesC& aKwd,const TInt& aLegacyIndex)
        {
        if(iCurrentTopic->GetName().Compare(aKwd) == 0)
            iCurrentTopic->LegacyIndex() = aLegacyIndex;
        }
    
    private:
    MCSXHKywdSelectedItemBuilder(CCSXHKywdTOC1* aCurrentTopic)
                                :iCurrentTopic(aCurrentTopic){}
    CCSXHKywdTOC1* iCurrentTopic;
    };

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHKywdTopics* CCSXHKywdTopics::NewL(const TApaAppCaption& aAppCaption)
    {
    CCSXHKywdTopics* self = CCSXHKywdTopics::NewLC(aAppCaption);
    CleanupStack::Pop(self);
    return self;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
CCSXHKywdTopics* CCSXHKywdTopics::NewLC(const TApaAppCaption& aAppCaption)
    {
    CCSXHKywdTopics* self = new(ELeave) CCSXHKywdTopics(aAppCaption);
    CleanupStack::PushL(self);
    return self;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
CCSXHKywdTopics::CCSXHKywdTopics(const TDesC& aName) : CCSXHGenericTOC1(aName)
    {}
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
CCSXHKywdTopics::~CCSXHKywdTopics()
    {
    if(iCurrentTopic)
        delete iCurrentTopic;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
TUid CCSXHKywdTopics::GetViewID() const
    {
    return KCSXHKywdToc1ViewID;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------       
void CCSXHKywdTopics::CopyChildListL(CDesCArray* aArray)    
    {
    iKywdTopics = aArray;
    MCSXHKywdTopicsBuilder* builder = NULL;
    builder = MCSXHKywdTopicsBuilder::NewL();
    CCSXHHelpDataBase::GetInstance()->FillKeywordTopicsL(builder);
    delete builder;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
void CCSXHKywdTopics::InsertKywdL(const TDesC& aKywd)
    {
    TBuf<KMaxFileName> child(KTabSpace);
    child.Append(aKywd);
    iKywdTopics->InsertIsqL(child);
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
CCSXHHelpContentBase* CCSXHKywdTopics::GetChildL(const TDesC& aName)
    {
    if(iCurrentTopic)
        {
        delete iCurrentTopic;
        iCurrentTopic = NULL;
        }
        
    iCurrentTopic = CCSXHKywdTOC1::NewL(aName);
    
    MCSXHKywdSelectedItemBuilder* builder = NULL;
    builder = MCSXHKywdSelectedItemBuilder::NewL(iCurrentTopic);

    CCSXHHelpDataBase::GetInstance()->FillKeywordTopicsL(builder);

    delete builder;
    return iCurrentTopic;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
CCSXHKywdTOC1* CCSXHKywdTopics::GetSelectedTopic()
    {
    return iCurrentTopic;
    }



