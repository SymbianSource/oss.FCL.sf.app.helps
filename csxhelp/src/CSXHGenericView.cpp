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
* Description:  CCSXHGenericView class definition
*
*/


#include "CSXHGenericView.h"
#include "CSXHGenericContainer.h"
#include "CSXHAppUi.h"
#include "CSXHDocument.h"
#include <cshelp.rsg>
#include "CsHelp.hrh"

#include "CSXHHelpContentBase.h"
#include "CSXHViewIDs.h"
#include "CSXHHelpDataBase.h"

#include <akntitle.h> 
#include <coemain.h>
#include <eikmenup.h>
#include <eikbtgpc.h>

#ifdef FF_HELP_TUTORIAL_MERGE

#include <AiwMenu.h>
#include <AiwCommon.h>
#include <AiwServiceHandler.h>
#include <AknGlobalNote.h>

#endif // FF_HELP_TUTORIAL_MERGE

CCSXHGenericView* CCSXHGenericView::NewL(const TUid& aUid, 
                                         const TInt& aFlags)
    {
    CCSXHGenericView* self = new(ELeave) CCSXHGenericView(aUid,aFlags);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
CCSXHGenericView::CCSXHGenericView(const TUid& aUid, const TInt& aFlags)
            :iResetContainerInActivate(EFalse),iUid(aUid),iAppFlags(aFlags)
    {//No Implementation required
    }
    
void CCSXHGenericView::ConstructL() 
    {
    BaseConstructL(iAppFlags);

    }
    
CCSXHGenericView::~CCSXHGenericView()
    {
    if(iContainer)
    	{
    	AppUi()->RemoveFromStack(iContainer); 
    	delete iContainer;
    	}
    }
    
TUid CCSXHGenericView::Id() const
    {
    return iUid;
    }

// --------------------------------
// Message Handler
// --------------------------------    
void CCSXHGenericView::HandleCommandL(TInt aCommand)    
    {
    switch(aCommand)
        {           
        case EAknSoftkeyBack:
            {
            CCSXHDocument* doc = static_cast<CCSXHDocument*>(AppUi()->Document());
            if(iUid == KCSXHToc2ViewID)
            	{
                doc->SetDisplayAndPrevTopic(doc->GetHelpDataBase()->GetMainTopics());
            	}
            else if(iUid == KCSXHKywdToc2ViewID)
            	{
                doc->SetDisplayAndPrevTopic(doc->GetHelpDataBase()->GetKywdTopics());
            	}
            else if ( iUid == KCSXHToc1AppHelpsViewID )
            	{
                doc->SetDisplayAndPrevTopic( doc->GetHelpDataBase()->GetMainTopics() );
            	}
            else if ( iUid == KCSXHToc2AppHelpsViewID )
            	{
                doc->SetDisplayAndPrevTopic( doc->GetHelpDataBase()->GetAppHelpsTopics() );
            	}
            else if ( iUid == KCSXHKywdToc1ViewID )
            	{
                doc->SetDisplayAndPrevTopic( doc->GetHelpDataBase()->GetMainTopics() );
            	}
            	
            
            AppUi()->HandleCommandL(ECSXHOpenItem); 
            }
            break;
        case ECSXHOpenItemPropagated: //Will be called for AppUi    
            iContainer->SetDisplayTopicL();
            break;
        case ECSXHTOC1ListView:
            {
            CCSXHDocument* doc = static_cast<CCSXHDocument*>(AppUi()->Document());              
            doc->SetDisplayAndPrevTopic(doc->GetHelpDataBase()->GetMainTopics());
            AppUi()->HandleCommandL(ECSXHOpenItem);
            }
            break;      
        case ECSXHOpenItem:
        case ECSXHOpenTopicText:    
#ifdef FF_HELP_TUTORIAL_MERGE

            if(iContainer->IsShowTutorialItemSelectedL())
                {
                if(CCSXHAppUi::GetInstance()->IsApplicationPresentInTutorialL())
                    {
                    CCSXHAppUi::GetInstance()->CallTutorialAiwProviderL();
                    }
                else
                    {
                    HBufC* ErrorMessage = iCoeEnv->AllocReadResourceLC(
                                                        R_TYPE_NO_HELP_TOPICS);
                    CAknGlobalNote* note = CAknGlobalNote::NewLC();
                    note->ShowNoteL(EAknGlobalInformationNote, *ErrorMessage);

                    CleanupStack::PopAndDestroy(note);
                    CleanupStack::PopAndDestroy(ErrorMessage);
                    }
                }
            else

#endif // FF_HELP_TUTORIAL_MERGE
	        	{        	
	            iContainer->SetDisplayTopicL();
	            AppUi()->HandleCommandL(aCommand);  
	            }
            break; 
        default:
            AppUi()->HandleCommandL(aCommand);  
            break;              
        }
    }
// --------------------------------------------------------------------------
// Handler for view activation. If required the container object will be 
// recreated. The recreation can be due to 1) Resetting the search pane
// 2) Changing the selection of the items in the list box 3) Repopulating the
// contents of the list box
// --------------------------------------------------------------------------
void CCSXHGenericView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,TUid,const TDesC8& )
    {
    CCSXHDocument* doc = static_cast<CCSXHDocument*>(AppUi()->Document());
    if(iResetContainerInActivate)
        {
        delete iContainer;
        iContainer = NULL;
        iResetContainerInActivate = EFalse;
        }
    if(!iContainer)
    	{
   	    if(iUid == KCSXHKywdToc2ViewID)
			doc->InitProgressBarL();
   	    
#ifdef FF_HELP_TUTORIAL_MERGE
        iContainer = CCSXHGenericContainer::NewL(ClientRect(),*doc,this);
#else // !FF_HELP_TUTORIAL_MERGE
        iContainer = CCSXHGenericContainer::NewL(ClientRect(),*doc);
#endif // FF_HELP_TUTORIAL_MERGE

        /*if(iUid == KCSXHKywdToc2ViewID)
			doc->FreeProgressBarL();*/
    	}
    iContainer->HightLightItemL();	
    iContainer->MakeVisible(ETrue);
    iContainer->DrawNow();
    iContainer->SetMopParent(this);
    //iContainer->SetRect(ClientRect());   
    iContainer->SetCurrentView(this);
    AppUi()->AddToStackL(*this, iContainer);
    
    if(iUid == KCSXHToc1ViewID ||
       iUid == KCSXHKywdToc1ViewID)
        {
        doc->GetHelpDataBase()->ClearAllTOC1Contents();
        doc->ClearPrevTopic();
        }
    
    iContainer->CheckForMSK();
    
    //Set the title of the view
    CEikStatusPane* sp = StatusPane();
    CAknTitlePane* title = STATIC_CAST(CAknTitlePane*, 
    sp->ControlL(TUid::Uid(EEikStatusPaneUidTitle)));
    title->SetTextL(doc->GetDisplayTopic()->GetName());
    }
    
void CCSXHGenericView::DoDeactivate()
    {
    if(iContainer)
        {
        iContainer->MakeVisible(EFalse);
        AppUi()->RemoveFromStack(iContainer);
        }
    }
    
void CCSXHGenericView::ResetContainer()
    {
    iResetContainerInActivate = ETrue;
    }
// --------------------------------------------------------------------------
// Dynamically changing the items in the Options Menu
// --------------------------------------------------------------------------

void CCSXHGenericView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {    
    if ( aResourceId == R_CSXH_MENU_TOPICLIST &&
         iContainer->GetNumberOfListItems()== 0 )
        {
        aMenuPane->DeleteMenuItem(ECSXHOpenTopicText);
        }
        
    if ( aResourceId == R_CSXH_MENU_KYWDTOC1LIST &&
         iContainer->GetNumberOfListItems()== 0 )
        {
        aMenuPane->DeleteMenuItem(ECSXHOpenTopicText);
        }
        
    if ( aResourceId == R_CSXH_MENU &&
         iContainer->GetNumberOfListItems()== 0 )
        {
        aMenuPane->DeleteMenuItem(ECSXHOpenItem);
        }
        
    if ( aResourceId == R_CSXH_MENU_KYWDTOC2LIST &&
         iContainer->GetNumberOfListItems()== 0 )
        {
        aMenuPane->DeleteMenuItem(ECSXHOpenTopicText);
        }
    }    
// --------------------------------------------------------------------------
// MSK Handler Handling 
// --------------------------------------------------------------------------
void CCSXHGenericView::SetMiddleSoftKey(TBool aValue)
    {    
    Cba()->MakeCommandVisible(ECSXHOpenTopicText,aValue);
    Cba()->DrawDeferred();  
    }
void CCSXHGenericView::ResourceChangeHdl(TInt aType)
    {
    if(iContainer)
        iContainer->HandleResourceChangeImpl(aType);
    }


