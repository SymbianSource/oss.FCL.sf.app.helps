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
* Description:  CCSXHLegacyContentParser class definition
*
*/

#include "CSXHLegacyContentParser.h"
#include "CSXHLegacyTOC1.h"
#include "CSXHLegacyTOC2.h"
#include "CSXHKywdTOC1.h"
#include "CSXHHelpDataBase.h"

#include <coehelp.h>
#include <coemain.h>
#include <f32file.h> 
#include <badesca.h>
#include <txtrich.h>

CCSXHLegacyContentParser* CCSXHLegacyContentParser::NewL(CCSXHHelpDataBase
*aDataBase)
    {
    CCSXHLegacyContentParser* self = CCSXHLegacyContentParser::NewLC(aDataBase);
    CleanupStack::Pop(self);
    return self;
    }
    
CCSXHLegacyContentParser* CCSXHLegacyContentParser::NewLC(CCSXHHelpDataBase
*aDataBase)
    {
    CCSXHLegacyContentParser* self = new (ELeave) CCSXHLegacyContentParser(aDataBase);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
void CCSXHLegacyContentParser::ConstructL()
    {
    iModel = CHlpModel::NewL(CCoeEnv::Static()->FsSession(), this);     
    iModel->OpenL();
    }
    
CCSXHLegacyContentParser::CCSXHLegacyContentParser(CCSXHHelpDataBase* aDataBase)
                        :iTopic(NULL)
    {
    iDataBase = aDataBase;
    iHlpList = NULL;
    }
    
CCSXHLegacyContentParser::~CCSXHLegacyContentParser()
    {
    if(iTopic)
        delete iTopic;
    if(iHlpList)
        delete iHlpList;
    
    //No need to delete the entries here, it will be done by the database
    iLegacyTOC1List.Close();    
    
    if(iModel)  
        {
        TRAP_IGNORE(iModel->CloseL());
        delete iModel;  
        }
    }
// --------------------------------------------------------------------------
// Gets the available kywds from the legacy content and sends them to the
// database
// --------------------------------------------------------------------------    
void CCSXHLegacyContentParser::GenerateKywdTOC1ListL(CCSXHHelpDataBase* aDataBase)
    {
    CHlpList* HlpList; 
    HlpList = CHlpList::NewL();
    iModel->SearchL(EIndexList);
    iModel->LoadListL(HlpList); 
    
    TInt nCount(HlpList->MdcaCount());
    for (TInt i(0); i < nCount; i++)
        {           
        TBuf<KMaxFileName> appName;
        appName = HlpList->MdcaPoint(i);
        TRAP_IGNORE(aDataBase->InsertKeywordL(appName,i,HlpList->Item(i)->HelpFileUid()));
        }   
    delete HlpList;
    }
// --------------------------------------------------------------------------
// Gets the available TOC1 from the legacy content and sends them to the
// database. Local copy of the TOC1 objects is maintained for generating TOC2
// --------------------------------------------------------------------------    
void CCSXHLegacyContentParser::GenerateTOC1ListL(CCSXHHelpDataBase* aDataBase)
    {    
    const TInt twoColumnFlatArray = 2;
    CDesCArray* categoryList = new(ELeave) CDesCArrayFlat(twoColumnFlatArray);
    CleanupStack::PushL(categoryList);
    iModel->CategoryListL(categoryList);
    CCSXHLegacyTOC1* CategoryObj;
    TBuf<KMaxFileName> appName;
    TLinearOrder<CCSXHHelpContentBase> anOrder(Orderer<CCSXHHelpContentBase>);          
    for (TInt i(0); i < categoryList->Count(); i++)
        {           
        appName = categoryList->MdcaPoint(i);                           
        CategoryObj = CCSXHLegacyTOC1::NewL(appName);
        if ( aDataBase->GetAppHelpsTopics()->InsertChild(CategoryObj) )
                iLegacyTOC1List.Append(CategoryObj);//Keep a local copy*/       
        }
    CleanupStack::Pop(categoryList);            
    delete categoryList;
    }
// --------------------------------------------------------------------------
// Gets the available TOC2 objects for the TOC1 object
// --------------------------------------------------------------------------    
void CCSXHLegacyContentParser::GenerateTOC2ListL(CCSXHGenericTOC1& aGenericTOC1Object,
                                 RPointerArray<CCSXHHelpContentBase>* GenericTOC2List)
    {
    iModel->SearchL(ETopicListForCategory, aGenericTOC1Object.GetName());
    if(iHlpList)
        {
        delete iHlpList;
        //Next line is Needed for code scanner
        iHlpList = NULL;
        }
    iHlpList = CHlpList::NewL();
    iModel->LoadListL(iHlpList);
    
    TInt count(iHlpList->MdcaCount());
    CCSXHGenericTOC2* PLegacyTOC2;
    TLinearOrder<CCSXHHelpContentBase> anOrder(Orderer<CCSXHHelpContentBase>); 
    for (TInt i(0); i < count; i++)
        {    
        PLegacyTOC2 = CCSXHLegacyTOC2::NewL(&aGenericTOC1Object,iHlpList->MdcaPoint(i));
        if (KErrNone != GenericTOC2List->InsertInOrder(PLegacyTOC2,anOrder))
            delete PLegacyTOC2;
        }        
    }       
// --------------------------------------------------------------------------
// Gets the RTF content for the given topic
// --------------------------------------------------------------------------    
TAny* CCSXHLegacyContentParser::GetTopicContentL(CCSXHGenericTOC2* aTopic)
    {
    TInt nCount(iHlpList->MdcaCount());
    TInt nIndex(0);
    for(nIndex=0;nIndex < nCount; nIndex++)
        {
        TBuf<KMaxFileName> appName; 
        appName.Append(iHlpList->MdcaPoint(nIndex));
        if(appName.Compare(aTopic->GetName())==0)
            break;
        }
    if(iTopic)
        {
        delete iTopic;
        //Next line is Needed for code scanner
        iTopic = NULL;
        }
    iTopic = CHlpTopic::NewL();
    CHlpItem* item = iHlpList->Item(nIndex); 
    iModel->TopicSearchL(*item);    
    iModel->LoadTopicL(iTopic);    
    return iTopic->TopicText();    
    }

void CCSXHLegacyContentParser::HandleModelEventL(TInt aEvent)
    {
    iStatus = aEvent;   
    }

// --------------------------------------------------------------------------
// Gets the number of kywd subtopics for the given kywd
// --------------------------------------------------------------------------    
TInt CCSXHLegacyContentParser::InitGenerateTOC2ListForKeywordSearchL(CCSXHKywdTOC1* aKywdTOC1Object)
    {
    if(aKywdTOC1Object->LegacyIndex() == -1 || (!iLegacyTOC1List.Count()))      
        return 0;
    CHlpList* SearchList; 
    
    if(iHlpList)
        {
        delete iHlpList;
        //Next line is Needed for code scanner
        iHlpList = NULL;
        }
    iHlpList = CHlpList::NewL();
    SearchList = CHlpList::NewLC();
    iModel->SearchL(EIndexList);
    iModel->LoadListL(SearchList);  
    //Search Panel    
    CHlpItem* item = SearchList->Item(aKywdTOC1Object->LegacyIndex());
    iModel->IndexSearchL(*item);    
    iModel->LoadListL(iHlpList);        
    
    CleanupStack::PopAndDestroy(SearchList);
    return iHlpList->MdcaCount();
    }
    
    
// --------------------------------------------------------------------------
// Gets the available kywd subtopics for the given kywd
// --------------------------------------------------------------------------    
void CCSXHLegacyContentParser::GenerateTOC2ListForKeywordSearchL(CCSXHKywdTOC1* aKywdTOC1Object)
    {
    if(aKywdTOC1Object->LegacyIndex() == -1 || (!iLegacyTOC1List.Count()))      
        return;
    
    TInt nCount(iHlpList->MdcaCount());
    CCSXHGenericTOC2* PLegacyTOC2;
    TLinearOrder<CCSXHGenericTOC2> anOrder(Orderer<CCSXHGenericTOC2>); 
    for (TInt i(0); i < nCount; i++)
        {   
        PLegacyTOC2 = CCSXHLegacyTOC2::NewL(aKywdTOC1Object,iHlpList->MdcaPoint(i),ETrue);
        iDataBase->InsertKeywordTopic(aKywdTOC1Object,PLegacyTOC2);
        iDataBase->IncrementKeywordSearchCount();
        }    
  
    }
   
CCSXHLegacyTOC1* CCSXHLegacyContentParser::GetTOC1ForTOC2(CCSXHLegacyTOC2*)
    {
    TInt count = iLegacyTOC1List.Count();
    CCSXHLegacyTOC1* toc1; 
    for(TInt i = 0; i<count ; ++i)
        {
        toc1 = iLegacyTOC1List[i];
        if(iTopic->Category().Compare(toc1->GetName())==0)
            return toc1;
        }
    return NULL;    
    }
    
CCSXHHelpContentBase* CCSXHLegacyContentParser::GetContextTopic(TCoeHelpContext& aContext)
    {
        TRAPD(err,LoadContextTopicL(aContext));
        if(err != KErrNone || NULL == iTopic)
            return NULL;
        
        TInt count = iLegacyTOC1List.Count();
        CCSXHLegacyTOC1* toc1; 
        for(TInt i = 0; i<count ; ++i)
            {
            toc1 = iLegacyTOC1List[i];
            if(iTopic->Category().Compare(toc1->GetName())==0)
                {
                return toc1->GetContextTopic(iTopic->TopicTitle());
                }
            }
        return NULL;
    }

void CCSXHLegacyContentParser::LoadContextTopicL(TCoeHelpContext& aContext)
    {
    if(iTopic)
        {
        delete iTopic;
        //Next line is Needed for code scanner
        iTopic = NULL;
        }
    
    iModel->ContextSearchL(aContext);
    if(ETopicAvailable == iStatus )
        {
        iTopic = CHlpTopic::NewL();
        iModel->LoadTopicL(iTopic);     
        }
    }
