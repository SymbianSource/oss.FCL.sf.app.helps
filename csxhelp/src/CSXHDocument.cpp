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
* Description:  CCSXHDocument class definition
*
*/


#include "CSXHDocument.h"
#include "CSXHAppUi.h"

#include "CSXHHelpDataBase.h"
#include "CSXHKywdTOC1.h"
#include "CSXHViewIDs.h"
#include <eikprogi.h>

#include <cshelp.rsg>
#include <AknProgressDialog.h>


// Standard Symbian OS construction sequence
CCSXHDocument* CCSXHDocument::NewL(CEikApplication& aApp)
    {
    CCSXHDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

CCSXHDocument* CCSXHDocument::NewLC(CEikApplication& aApp)
    {
    CCSXHDocument* self = new (ELeave) CCSXHDocument(aApp);
    CleanupStack::PushL(self);
    return self;
    }

CCSXHDocument::CCSXHDocument(CEikApplication& aApp) : CAknDocument(aApp)
    {// no implementation required
    }

CCSXHDocument::~CCSXHDocument()
    {
	delete iHelpDataBase;
    }

CEikAppUi* CCSXHDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object. ConstructL of the AppUi
    // object will be called by the Application Framework
    iHelpAppUi = new (ELeave) CCSXHAppUi;
    
    HBufC* appToc = CCSXHAppUi::GetCoeEnv()->AllocReadResourceLC( R_CSHELP_TOC_APP_HELPS );
    iHelpDataBase = CCSXHHelpDataBase::NewL(CCSXHAppUi::GetCoeEnv(),AppCaption(), *appToc);
    CleanupStack::PopAndDestroy( appToc );
	
	iDisplayTopic = iHelpDataBase->GetMainTopics();
	iPrevTopic = NULL;
	
	iProgressDialog = NULL;
    
    return iHelpAppUi;
    }
// --------------------------------------------------------------------------
// Sets the topic to be displayed next
// --------------------------------------------------------------------------
void CCSXHDocument::SetDisplayTopic(CCSXHHelpContentBase* aDisplayTopic)
	{
	iDisplayTopic = aDisplayTopic;
	iPrevTopic = NULL;
	
	if(iDisplayTopic == NULL)
		iDisplayTopic = iHelpDataBase->GetMainTopics();
	}
// --------------------------------------------------------------------------
// Sets the topic to be displayed next and the previous topic
// The previous topic is needed to highlight the correct item in the listbox
// --------------------------------------------------------------------------
void CCSXHDocument::SetDisplayAndPrevTopic(CCSXHHelpContentBase* aDisplayTopic)
	{
	iPrevTopic = iDisplayTopic;
	iDisplayTopic = aDisplayTopic;
	
	if(iDisplayTopic == NULL)
		iDisplayTopic = iHelpDataBase->GetMainTopics();
	
	}
	
void CCSXHDocument::SendMessageToAppUiL(TInt aCommand)
	{
	iHelpAppUi->ProcessCommandL(aCommand);
	}

// --------------------------------------------------------------------------
// Sets the context topic as the next topic to be displayed
// --------------------------------------------------------------------------
TBool CCSXHDocument::SetContextTopicAsDisplayTopicL(const TDesC8& aContextMessage)
	{
	CCSXHHelpContentBase* topic = iHelpDataBase->GetContextTopic(aContextMessage);
	if(topic)
		{
		SetDisplayTopic(topic);	
		return ETrue;
		}
	return EFalse;
	}
CCSXHHelpContentBase* CCSXHDocument::GetHtmlTopicForUrlL(const TDesC& url)
	{
	return iHelpDataBase->GetHtmlTopicForUrlL(url);
	}

void CCSXHDocument::InitProgressBarL()
	{

    iProgressDialog = new(ELeave)CAknProgressDialog(
                           REINTERPRET_CAST(CEikDialog**,&iProgressDialog));
    iProgressDialog->PrepareLC(R_CSHELP_SEARCH_PROGRESS_NOTE);
    
    CEikProgressInfo* pBar = iProgressDialog->GetProgressInfoL();
    iTotalKeywordsInResultView = iHelpDataBase->InitGenerateKeywordTOC2ListL(
    									STATIC_CAST(CCSXHKywdTOC1*,iDisplayTopic));
    pBar->SetFinalValue(iTotalKeywordsInResultView);
 	iIdle = CIdle::NewL(CActive::EPriorityLow);
    TCallBack callback(CallBack, this);
    iIdle->Start(callback);
    iProgressDialog->RunDlgLD(CAknNoteDialog::ENoTone);  
	}
	 	
void CCSXHDocument::FreeProgressBarL()
	{
	delete iIdle;
    iIdle = NULL;
    if(iProgressDialog)
    	{
    	iProgressDialog->ProcessFinishedL();
		iProgressDialog = NULL;
    	}
	}
		
TInt CCSXHDocument::CallBack(TAny* aThis)
    {
    TInt err(KErrNone);
    TBool res(EFalse);
    CCSXHDocument* self = STATIC_CAST(CCSXHDocument*, aThis);
    TRAP( err, res = self->DoCallBackL() );
    if(err != KErrNone )
    	{
    	TRAP_IGNORE(self->FreeProgressBarL());
    	}
    return res;
    }
TBool CCSXHDocument::DoCallBackL()
    {
    if(!iProgressDialog)
        {
        delete iIdle;
        iIdle = NULL;
        return EFalse;
        }
        
    CEikProgressInfo* pBar= iProgressDialog->GetProgressInfoL();
    pBar->SetAndDraw(iHelpDataBase->GetKeywordSearchCount());
    
    if(iHelpDataBase->GetKeywordSearchCount() >= iTotalKeywordsInResultView)
    	{
    	FreeProgressBarL();
    	return EFalse;
    	}
    
    return ETrue;    
    }    

