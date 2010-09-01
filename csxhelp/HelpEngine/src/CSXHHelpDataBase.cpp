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
* Description:  CCSXHHelpDataBase class definition
*
*/

#include "CSXHHelpDataBase.h"
#include "CSXHHTMLContentParser.h"
#include "CSXHLegacyContentParser.h"
#include "CSXHViewIDs.h"
#include "CSXHGenericTOC2.h"
#include "CSXHKywdTOC1.h"
#include "CSXHMainTopics.h"
#include "CSXHKywdTopics.h"
#include "CsHelpCmdLineParser.h"
#include "CSXHContextTopic.h"
#include "coehelp.h" 

#include <eikprogi.h>
#include <utf.h>
#include <bautils.h>
#include <data_caging_path_literals.hrh>
#include <eikenv.h> 
#include <eikprogi.h>


#include <AknProgressDialog.h>

EXPORT_C CCSXHHelpDataBase* CCSXHHelpDataBase::NewL(CCoeEnv* aCoeEnv,
	const TApaAppCaption& aAppCaption, const TApaAppCaption& aAppHelpTopic)
    {
    CCSXHHelpDataBase* self = CCSXHHelpDataBase::NewLC(aCoeEnv,aAppCaption, aAppHelpTopic);
    CleanupStack::Pop(self);
    return self;    
    }

CCSXHHelpDataBase* CCSXHHelpDataBase::NewLC(CCoeEnv* aCoeEnv, const TApaAppCaption& aAppCaption, const TApaAppCaption& aAppHelpTopic)
    {
    CCSXHHelpDataBase *self = NULL;
    //Add stuff into TLS
    if(Dll::Tls() == NULL )
        {
        // TLS is still null, which means that no Singleton has
        // been instantiated yet. Do so now, and return that
        // instance:
        // Store a pointer to the new instance in thread local storage:
        self = new(ELeave) CCSXHHelpDataBase(aCoeEnv,aAppCaption, aAppHelpTopic);
        CleanupStack::PushL(self);
        self->ConstructL();
     
        TInt err = Dll::SetTls(self);
        if (err != KErrNone)
            {
            CleanupStack::Pop(self);            
            delete self;
            self = NULL;
            User::Leave(err);
            }
        }
     else
        {
        self = GetInstance();
        }

    return self;
    }
    
CCSXHHelpDataBase* CCSXHHelpDataBase::GetInstance()
    {
    // Singleton has been instantiated once already, so return
    // that instance:
    return static_cast<CCSXHHelpDataBase*>(Dll::Tls());
    }

CCSXHHelpDataBase::CCSXHHelpDataBase(CCoeEnv* aCoeEnv, const TApaAppCaption& aAppCaption, const TApaAppCaption& aAppHelpTopic)
             : iCoeEnv(aCoeEnv), iAppCaption(aAppCaption), iAppHelpTopic(aAppHelpTopic), iKeywordSearchCount(0)
             
    {
    }

CCSXHHelpDataBase::~CCSXHHelpDataBase()
    {
    delete iMainTOC1;
    delete iKywdTOC1;
    delete iLastContextTopic;   
    delete iHTMLContentParser;
    delete iLegacyContentParser;
    Dll::SetTls( NULL );
    }

void CCSXHHelpDataBase::ConstructL()
    {
    iHTMLContentParser = CCSXHHTMLContentParser::NewL(iCoeEnv);
    iLegacyContentParser = CCSXHLegacyContentParser::NewL(this);
    
    iMainTOC1 = CCSXHMainTopics::NewL(KCSXHToc1ViewID,iAppCaption);
    iKywdTOC1 = CCSXHKywdTopics::NewL(iAppCaption);
    
    //No need to delete iAppHelpsToc in destruct,
    //because it will be deleted when deleting iMainTOC1.
    //All third-party helps will be put to this topic
    TInt appHelpPriority = 20001;
    iAppHelpsToc = CCSXHMainTopics::NewL( KCSXHToc1AppHelpsViewID, iAppHelpTopic, appHelpPriority );
        
    //Build the TOC1 list here
    iMainTOC1->InsertChildWithPriority(iAppHelpsToc, EFalse);
    iHTMLContentParser->GenerateTOC1ListL(this);
    iLegacyContentParser->GenerateTOC1ListL(this);
    }
    
CCoeEnv* CCSXHHelpDataBase::GetCoeEnv()
    {
    return iCoeEnv;
    }

EXPORT_C CCSXHGenericTOC1* CCSXHHelpDataBase::GetMainTopics()
    {
    return iMainTOC1;   
    }
EXPORT_C CCSXHGenericTOC1* CCSXHHelpDataBase::GetKywdTopics()
    {
    return iKywdTOC1;
    }

EXPORT_C CCSXHGenericTOC1* CCSXHHelpDataBase::GetAppHelpsTopics()
    {
    return iAppHelpsToc;   
    }

EXPORT_C CCSXHHelpContentBase* CCSXHHelpDataBase::GetContextTopic(const TDesC8& aContextMessage)
    {
    TRAP_IGNORE(GetContextTopicL(aContextMessage));
    return iLastContextTopic;
    
    }
CCSXHHelpContentBase* CCSXHHelpDataBase::GetContextTopicL(const TDesC8& aContextMessage)
    {
    if(iLastContextTopic)
        {
        delete iLastContextTopic;
        iLastContextTopic = NULL;
        }
    //TSW Error Correction:Help: Application crashes 
    //after trying to re-open context-dependent help from application
    //Clear childlist of all (TOC1)parents.    
	ClearAllTOC1Contents();    
	
    CArrayFix<TCoeHelpContext>* contextList;
    CCsHlpCmdLineParser* parser = new(ELeave) CCsHlpCmdLineParser;
    CleanupStack::PushL(parser);
    parser->ConstructL(aContextMessage);
    contextList = parser->ContextListL();
    CleanupStack::PopAndDestroy(parser); // parser
    CleanupStack::PushL(contextList);
    CCSXHHelpContentBase* contextTopic;
    TInt numberOfContextList = contextList->Count();
    TCoeHelpContext* context;
    for (TInt i(0); i < numberOfContextList; i++)
        {
        context = &(contextList->At(i));
        
        contextTopic = iHTMLContentParser->GetContextTopicL(this, context->iMajor,context->iContext);
        if(contextTopic)
            {
            CleanupStack::PopAndDestroy(contextList);
            iLastContextTopic = CCSXHContextTopic::NewL(contextTopic);
            return iLastContextTopic;
            }
        }

    for (TInt i(0); i < numberOfContextList; i++)
        {
        context = &(contextList->At(i));
        
        contextTopic = iLegacyContentParser->GetContextTopic(*context);
        if(contextTopic)
            {
            CleanupStack::PopAndDestroy(contextList);
            iLastContextTopic = CCSXHContextTopic::NewL(contextTopic);
            return iLastContextTopic;
            }
        }
        
        
    CleanupStack::PopAndDestroy(contextList);    
    return NULL;
    }

EXPORT_C void CCSXHHelpDataBase::ClearAllTOC1Contents()
    {
    iMainTOC1->ResetChildList();
    iKywdTOC1->ResetChildList();
    }

CCSXHLegacyContentParser* CCSXHHelpDataBase::GetLegacyParser() 
    {
    return iLegacyContentParser;        
    }
    
CCSXHHTMLContentParser* CCSXHHelpDataBase::GetHtmlParser() 
    {
    return iHTMLContentParser;      
    }

void CCSXHHelpDataBase::InsertKeywordL(const TDesC& aKywdName, 
	TInt aLegacyKywdIndex, const TUid& aCatUid)
    {
    if(!iHTMLContentParser->IsUidCategoryPresent(aCatUid))
        iKwydBuilder->InsertKeywordL(iKywdTOC1,aKywdName,aLegacyKywdIndex);
    }
    
void CCSXHHelpDataBase::InsertKeywordL(const TDesC& aKywdName, CCSXHHtmlTOC1* aToc1)
    {
    iKwydBuilder->InsertKeywordL(iKywdTOC1,aKywdName,aToc1);
    }

CCSXHKywdTOC1* CCSXHHelpDataBase::InsertKeywordTopicL(CCSXHGenericTOC2* aToc2)
    {
    CCSXHKywdTOC1* toc1 = iKywdTOC1->GetSelectedTopic();
    InsertKeywordTopic(toc1,aToc2);
    return toc1;
    }
    
void CCSXHHelpDataBase::InsertKeywordTopic(CCSXHKywdTOC1* aToc1, CCSXHGenericTOC2* aToc2)
    {
    aToc1->InsertChild(aToc2);
    }

EXPORT_C TInt CCSXHHelpDataBase::InitGenerateKeywordTOC2ListL(CCSXHKywdTOC1* aTopic)
    {
    iKeywordSearchCount = 0;
    RPointerArray<CCSXHHtmlTOC1>* TOC1HtmlList = aTopic->GetHtmlTOC1List();
    TInt htmlCount = TOC1HtmlList ? TOC1HtmlList->Count() : 0;
    TInt legacyCount = iLegacyContentParser->InitGenerateTOC2ListForKeywordSearchL(aTopic);
    return (htmlCount + legacyCount);
    }
    
void CCSXHHelpDataBase::GenerateKeywordTOC2ListL(CCSXHKywdTOC1* aTopic)
    {
    iHTMLContentParser->GenerateTOC2ListForKeywordSearchL(this,aTopic);
 	iLegacyContentParser->GenerateTOC2ListForKeywordSearchL(aTopic);
    }
    
EXPORT_C CCSXHHelpContentBase* CCSXHHelpDataBase::GetHtmlTopicForUrlL(const TDesC& url)
    {
    return iHTMLContentParser->GetHtmlTopicForUrlL(url);
    }
void CCSXHHelpDataBase::FillKeywordTopicsL(MCSXHKywdBuilder* aBuilder)
    {
    iKwydBuilder = aBuilder;
    iHTMLContentParser->GenerateKywdTOC1ListL(this);
    iLegacyContentParser->GenerateKywdTOC1ListL(this);
    }
    

void CCSXHHelpDataBase::IncrementKeywordSearchCount(TInt aUnits)
	{
	iKeywordSearchCount += aUnits;
	}

EXPORT_C TInt CCSXHHelpDataBase::GetKeywordSearchCount()
	{
	return iKeywordSearchCount;
	}
	
