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
* Description:  CCSXHAppUi class definition
*
*/


#include "CSXHAppUi.h"
#include "CSXHGenericView.h"
#include "CSXHHtmlTopicView.h"
#include "CSXHLegacyTopicView.h"
#include "CsHelp.hrh"
#include <cshelp.rsg>
#include "CSXHAppUi.h"
#include "CSXHDocument.h"

#include "CSXHHelpDataBase.h"
#include "CSXHViewIDs.h"

#include <avkon.hrh>
#include <utf.h>
#include <aknnotewrappers.h> 
#include <AknGlobalNote.h>
#include <akninputblock.h>
#include <hlplch.h>

#include "csxhconstants.h"

#ifdef FF_HELP_TUTORIAL_MERGE
#include <AiwServiceHandler.h>

#if defined(_DEBUG) && defined(__WINS__)
#include <f32file.h>
#include <flogger.h>
_LIT(KLogDir, "csxh");
_LIT(KLogFile, "csxh.txt");
inline void FPrint(const TRefByValue<const TDesC> aFmt, ...)
    {
    VA_LIST list;
    VA_START(list,aFmt);
    RFileLogger::WriteFormat(KLogDir, KLogFile, EFileLoggingModeAppend, aFmt, list);
    }
#define FLOG(a) {FPrint(a);}
#define FLOGF   FPrint
#else //_DEBUG
#define FLOG(a)
#define FLOGF
#endif //_DEBUG
#endif // FF_HELP_TUTORIAL_MERGE
CCSXHAppUi* CCSXHAppUi::iInstance = NULL;

// ConstructL is called by the application framework
void CCSXHAppUi::ConstructL()
    {
#ifndef __SERIES60_30__         
    BaseConstructL(EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible );
#else
    BaseConstructL(EAknEnableSkin);
#endif  
  
#ifdef FF_HELP_TUTORIAL_MERGE
    LoadTutorialService();
#endif // FF_HELP_TUTORIAL_MERGE
    SetKeyEventFlags( CAknAppUiBase::EDisableSendKeyShort | CAknAppUiBase::EDisableSendKeyLong );
    iTOC1View = CCSXHGenericView::NewL(KCSXHToc1ViewID,R_CSXH_TOC1);
    AddViewL(iTOC1View); 
    
    iTOC2View = NULL;
    iKywdTOC1View = NULL;
    iKywdTOC2View = NULL;
    iHtmlTopicView = NULL;
    iLegacyTopicView = NULL;
    iLegacyContextTopicView = NULL;
    iAppHelpsTOC1View = NULL;
    iAppHelpsTOC2View = NULL;
    
    SetDefaultViewL(*iTOC1View);  
        
    iRuntimeWatcher = CCSXHRuntimeAppWatcher::NewL();
    iRuntimeWatcher->SetObserver( this );
    iRuntimeWatcher->Start();
#ifdef _DEBUG
    RDebug::Print( _L("Help app launched") );
#endif
    }

CCSXHAppUi::CCSXHAppUi()                              
    {
    iInstance = this;
    }

CCSXHAppUi::~CCSXHAppUi()
    {
#ifdef FF_HELP_TUTORIAL_MERGE

    if(iServiceHandler)
        {
        delete iServiceHandler;
        }

#endif // FF_HELP_TUTORIAL_MERGE
#ifdef _DEBUG
    RDebug::Print(_L("Help app ended"));
#endif
    if ( iRuntimeWatcher )
        delete iRuntimeWatcher;

    }

// --------------------------------------------------------------------------
// Message Handler
// --------------------------------------------------------------------------
void CCSXHAppUi::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
        case EAknSoftkeyClose:
            Exit();
            break;
  
        case ECSXHOpenTopicText:
        case ECSXHOpenItemPropagated:
        case ECSXHOpenItem:        
            {
            CAknInputBlock* inputBlock = CAknInputBlock::NewLC();
            ActivateDisplayTopicViewL();
            CleanupStack::PopAndDestroy( inputBlock ); 
            }
            break;
        case ECSXHSearchText:
            {
            CCSXHDocument* doc = static_cast<CCSXHDocument*>(Document());
            doc->SetDisplayTopic(doc->GetHelpDataBase()->GetKywdTopics());
            ActivateDisplayTopicViewL();
            }
            break;
        default:
            break;
        }
    }
void CCSXHAppUi::InitDisplayTopicViewL(TUid aViewID)
	{
	if(aViewID == KCSXHToc1ViewID)
		return;
	
	
  	if(aViewID == KCSXHContextLegacyTopicViewID && iLegacyContextTopicView == NULL)	
	 	{
        iLegacyContextTopicView = CCSXHLegacyTopicView::NewL (KCSXHContextLegacyTopicViewID,
                                                              R_CSHELP_TOPICFORCONTEXT,
                                                              ClientRect());
    	AddViewL(iLegacyContextTopicView);
	 	}	 	   
	else if(aViewID == KCSXHToc2ViewID && iTOC2View == NULL)
		{
		iTOC2View = CCSXHGenericView::NewL(KCSXHToc2ViewID,R_TOPICLIST);
    	AddViewL(iTOC2View); 
		}
	else if(aViewID == KCSXHToc2AppHelpsViewID && iAppHelpsTOC2View == NULL)
	    {
	    iAppHelpsTOC2View = CCSXHGenericView::NewL(KCSXHToc2AppHelpsViewID,R_TOPICLIST);
	    AddViewL(iAppHelpsTOC2View);
	    }
	 else if(aViewID == KCSXHKywdToc1ViewID && iKywdTOC1View == NULL)
	 	{
	 	iKywdTOC1View = CCSXHGenericView::NewL(KCSXHKywdToc1ViewID,R_KYWDTOC1LIST);
    	AddViewL(iKywdTOC1View);
	 	}
	 else if(aViewID == KCSXHKywdToc2ViewID && iKywdTOC2View == NULL)	
	 	{
	 	iKywdTOC2View = CCSXHGenericView::NewL(KCSXHKywdToc2ViewID,R_KYWDTOC2LIST);
    	AddViewL(iKywdTOC2View); 
	 	}
	 else if(aViewID == KCSXHLegacyTopicViewID && iLegacyTopicView == NULL)	
	 	{
        iLegacyTopicView = CCSXHLegacyTopicView::NewL(KCSXHLegacyTopicViewID,
                                                      R_TOPICTEXTLIST,ClientRect());
    	AddViewL(iLegacyTopicView);
	 	}	
	 	
	 else if(aViewID == KCSXHContextHtmlTopicViewID )	
	 	{
	 	if(!iHtmlTopicView)
	 		{
            iHtmlTopicView = CCSXHHtmlTopicView::NewL(KCSXHHtmlTopicViewID,
                                                      R_CSHELP_HTMLTOPICFORCONTEXT,
                                                      ClientRect());
    		AddViewL(iHtmlTopicView);	
	 		}
	 	else
	 		{
	 		//View is already created update the softkeys to Options and Close
	 		iHtmlTopicView->SetViewTypeL(CCSXHHtmlTopicView::EContextHtmlView);
	 		}
  	 	}	
	 else if(aViewID == KCSXHHtmlTopicViewID )	
	 	{
	 	if(!iHtmlTopicView)
	 		{
            iHtmlTopicView = CCSXHHtmlTopicView::NewL(KCSXHHtmlTopicViewID,
                                                      R_CSHELP_HTMLTOPICVIEW,
                                                      ClientRect());
    		AddViewL(iHtmlTopicView);
	 		}
	 	else
	 		{
	 		//View is already created update the softkeys to Options and Back
	 		iHtmlTopicView->SetViewTypeL(CCSXHHtmlTopicView::EHtmlView);
	 		}
	 	}
    else if ( aViewID == KCSXHToc1AppHelpsViewID && !iAppHelpsTOC1View )
        {
        iAppHelpsTOC1View = CCSXHGenericView::NewL( KCSXHToc1AppHelpsViewID, R_TOPICLIST );
        AddViewL( iAppHelpsTOC1View ); 
        }
	}
// --------------------------------------------------------------------------
// Activates the view corresponding to the topic to be displayed
// It also sets for recreating the container dending on the view to be 
// activated, 
// 1) Search Pane in Search Result View will be cleared if user 
// opens the Search View.
// 2)Search Pane in Application Topics view will be cleared, if user opens 
//      a)Help Contents View 
//      b)Search View 
//      c)Context Senisitive view
// --------------------------------------------------------------------------
void CCSXHAppUi::ActivateDisplayTopicViewL()
    {
    CCSXHDocument* doc = static_cast<CCSXHDocument*>(Document());
    TUid viewId = doc->GetDisplayTopic()->GetViewID();
    InitDisplayTopicViewL(viewId);
    
    if(viewId == KCSXHKywdToc1ViewID )
        {
        if(iKywdTOC2View)
        	iKywdTOC2View->ResetContainer();
        if(iTOC2View)
        	iTOC2View->ResetContainer();
        if (iAppHelpsTOC2View)
        	iAppHelpsTOC2View->ResetContainer();
        }
	else if(viewId == KCSXHToc1ViewID ||
	        viewId == KCSXHContextHtmlTopicViewID ||
            viewId == KCSXHContextLegacyTopicViewID ||
            viewId == KCSXHToc1AppHelpsViewID)
	    {
	    if(iTOC2View)
			iTOC2View->ResetContainer();
	    if (iAppHelpsTOC2View)
	    	iAppHelpsTOC2View->ResetContainer();
	    }
/*	else if(viewId == KCSXHKywdToc1ViewID)  
		{
		doc->InitProgressBarL();
		}
*/	
	if(viewId == KCSXHContextHtmlTopicViewID)    
		viewId = KCSXHHtmlTopicViewID;
    ActivateLocalViewL(viewId);
    }
void CCSXHAppUi::HandleForegroundEventL(TBool aForeground)
	{
	// Call Base class method
 	CAknAppUi::HandleForegroundEventL(aForeground);

 	if(!aForeground && iView && 
 	iView->Id() != KCSXHHtmlTopicViewID
 	&& iHtmlTopicView)
 		{
 		//put NUll first in for bug ESXU-7JA9NS
 		iHtmlTopicView = NULL;
     	// We have lost the focus
     	//Check if the HTML view is the last active view. If it not delete it 
     	//to free the memory used by the browser control
     	RemoveView(KCSXHHtmlTopicViewID);
     	//delete iHtmlTopicView;
    	}
	}
// --------------------------------------------------------------------------
// It sets a flag in the View for recreating the container 
// --------------------------------------------------------------------------
void CCSXHAppUi::ResetTOC2ViewContainer()
    {
    if(iTOC2View)
    	iTOC2View->ResetContainer();	
    }
// --------------------------------------------------------------------------
// Context sensitive help launch handling. Help is not running
// --------------------------------------------------------------------------
TBool CCSXHAppUi::ProcessCommandParametersL(TApaCommand aCommand,
                                              TFileName& /* aDocumentName */,
                                              const TDesC8& aTail)
    {
    if (aCommand == EApaCommandOpen)
        { // when another app launches the help app
        HandleContextSensitiveLaunchL(aTail);
        }
    return EFalse;
    }

// --------------------------------------------------------------------------
// Context sensitive help launch handling. Help is already running
// --------------------------------------------------------------------------
MCoeMessageObserver::TMessageResponse CCSXHAppUi::HandleMessageL(
           TUint32 aClientHandleOfTargetWindowGroup,
           TUid aMessageUid,
           const TDesC8& aMessageParameters)
    {
    if (aMessageUid == KHlpAppWsMsg)
        {
        HandleContextSensitiveLaunchL(aMessageParameters);
        return EMessageHandled;
        }
    return CAknViewAppUi::HandleMessageL(aClientHandleOfTargetWindowGroup,
                                         aMessageUid,
                                         aMessageParameters);
    }

// --------------------------------------------------------------------------
// Opens the context topic. If no topic is avbl then toc1 view is activated
// --------------------------------------------------------------------------
void  CCSXHAppUi::HandleContextSensitiveLaunchL(const TDesC8& aContext )   
    {
    CCSXHDocument* doc = static_cast<CCSXHDocument*>(Document());
        
    if(doc->SetContextTopicAsDisplayTopicL(aContext))
        {
        TRAPD(res,ActivateDisplayTopicViewL());
        if(res == KErrNone)
            return;
        }
    
    //No topic avaliable, activate the TOC1 View & Pop a message
    doc->SetDisplayTopic(doc->GetHelpDataBase()->GetMainTopics());
    ActivateDisplayTopicViewL();
        
//    HBufC* ErrorMessage = iCoeEnv->AllocReadResourceLC(R_TYPE_NO_HELP_TOPICS);      
//    CAknGlobalNote* note = CAknGlobalNote::NewLC();
//    note->ShowNoteL(EAknGlobalInformationNote, *ErrorMessage);

//    CleanupStack::PopAndDestroy(note); 
//    CleanupStack::PopAndDestroy(ErrorMessage); 
    }

CCSXHAppUi* CCSXHAppUi::GetInstance()
    {
    return iInstance;
    }
RWsSession& CCSXHAppUi::GetWsSession()
    {
    return iInstance->iCoeEnv->WsSession();
    }
CCoeEnv* CCSXHAppUi::GetCoeEnv()
    {
    return iInstance->iCoeEnv;
    }

// --------------------------------------------------------------------------
// Launches an application based on the Application UID and a View ID
// --------------------------------------------------------------------------
void CCSXHAppUi::AppLaunchL(TInt32& aAppId,TInt32& aVId)
    {
    //Convert from TInt to TUid
    TUid KtestAppUid( TUid::Uid( aAppId ) ); 
    TUid KtestViewUid( TUid::Uid( aVId ) ); 
      
    //Launch Application
    TVwsViewId AppLaunch(KtestAppUid,KtestViewUid);
    ActivateViewL(AppLaunch);
    }

// --------------------------------------------------------------------------
// Launches an application based on the Application UID and a View ID
// --------------------------------------------------------------------------
void CCSXHAppUi::AppLaunchL(TInt32& aAppId,TInt32& aVId,
                            TInt32& aMsgId,
                            const TBuf<KMaxParamLength>& aParams)
    {
    //Convert from TInt to TUid
    TUid AplnUid( TUid::Uid( aAppId ) ); 
    TUid AplnViewUid( TUid::Uid( aVId ) ); 
    TUid MsgUid( TUid::Uid( aMsgId ) ); 
      
    //Launch Application
    TVwsViewId AppLaunch(AplnUid,AplnViewUid);
    
    HBufC8* params = CnvUtfConverter::ConvertFromUnicodeToUtf8L(aParams);
    CleanupStack::PushL(params);
    ActivateViewL(AppLaunch,MsgUid,*params);
    CleanupStack::PopAndDestroy(params);
    }
 
void CCSXHAppUi::PropagateResourceChange(TInt aType)
    {
    iTOC1View->ResourceChangeHdl(aType);
    if(iKywdTOC1View)
    	iKywdTOC1View->ResourceChangeHdl(aType);
    if(iKywdTOC2View)
    	iKywdTOC2View->ResourceChangeHdl(aType);
    if(iTOC2View)
		iTOC2View->ResourceChangeHdl(aType);    
    if (iAppHelpsTOC2View)
    	iAppHelpsTOC2View->ResourceChangeHdl(aType);
    if (iAppHelpsTOC1View)
        iAppHelpsTOC1View->ResourceChangeHdl(aType);
    if(iHtmlTopicView)
    	iHtmlTopicView->ResourceChangeHdl(aType);
    if(iLegacyTopicView)
    	iLegacyTopicView->ResourceChangeHdl(aType);
    if(iLegacyContextTopicView)
    	iLegacyContextTopicView->ResourceChangeHdl(aType);
    }
 
void CCSXHAppUi::RuntimeUpdateIndex()
    {
    CAknNoteDialog* dlg = new CAknNoteDialog();
    if (dlg != NULL)
        {
        dlg->SetTimeout( CAknNoteDialog::EShortTimeout );
        dlg->SetTone( CAknNoteDialog::ENoTone );
        TRAP_IGNORE(dlg->ExecuteLD(R_CSHELP_INSTALL_UNINSTALL_NOTE));
        }
    }
   
       
#ifdef FF_HELP_TUTORIAL_MERGE
// --------------------------------------------------------------------------
// CCSXHAppUi::NewParamListLC
// Create paramlist for provider.
// --------------------------------------------------------------------------
CAiwGenericParamList* CCSXHAppUi::NewParamListLC(
                                            const TCoeHelpContext& aContext)
    {
    TAiwVariant variant1;
    TAiwVariant variant2;
    variant1.Set(aContext.iMajor);
    TAiwGenericParam param1(EGenericParamHelpItem, variant1);
    variant2.Set(aContext.iContext);
    TAiwGenericParam param2(EGenericParamHelpItem, variant2);
    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();
    list->AppendL(param1);
    list->AppendL(param2);
    return list;
    }

// --------------------------------------------------------------------------
// CCSXHAppUi::LoadTutorialService
// Load service handler and attach interest for using tutorial service command.
// --------------------------------------------------------------------------
void CCSXHAppUi::LoadTutorialServiceL()
    {
    iServiceHandler = CAiwServiceHandler::NewL();

    // Create AIW interest
    RCriteriaArray interest;

    iServiceHandler->AttachL(R_AIWTUTORIALAPP_INTEREST);

    //Tutorial AIW provider exists or not
    iTutorialExists = EFalse;
    iServiceHandler->GetInterest(interest);
    if(interest.Count() > 0)
        {
        FLOGF(_L("CCSXHAppUi::LoadTutorialService() - interest.Count()=%d"), interest.Count());
        CAiwCriteriaItem* testCriteria = interest[0];
        if(iServiceHandler->NbrOfProviders(interest[0]) > 0)
            {
            FLOGF(_L("CCSXHAppUi::LoadTutorialService() - NbrOfProviders()=%d"), iServiceHandler->NbrOfProviders(interest[0]));
            iTutorialExists = ETrue;
            }
        }
    interest.Close();
    }

// --------------------------------------------------------------------------
// CCSXHAppUi::CallTutorialAiwProviderL
// If tutorial exists,call play presentation command.
// --------------------------------------------------------------------------
void CCSXHAppUi::CallTutorialAiwProviderL()
    {
    CAiwGenericParamList* inlist = NewParamListLC( iTCoeHelpContext );

    iServiceHandler->ExecuteServiceCmdL( KPlayPresentation,
                                         *inlist,
                                         iServiceHandler->OutParamListL() );
    CleanupStack::PopAndDestroy();   //inlist
    }

// --------------------------------------------------------------------------
// CCSXHAppUi::IsApplicationPresentInTutorialL
// If tutorial exists,identify the presentation needed exist or not.
// --------------------------------------------------------------------------
TBool CCSXHAppUi::IsApplicationPresentInTutorialL()
    {
    TBool Result = EFalse;
    //__ASSERT_DEBUG( iTutorialExists, User::Panic(_L("TutAiwHlp"),-1));
    if(iTutorialExists )
        {
        CAiwGenericParamList* inlist = NewParamListLC(iTCoeHelpContext) ;

        CAiwGenericParamList* outlist = CAiwGenericParamList::NewLC();

        TUid maj = iTCoeHelpContext.iMajor;
        TCoeContextName con = iTCoeHelpContext.iContext;

        FLOGF(_L("CCSXHAppUi::IsApplicationPresentInTutorialL() - 0x%08x %S"),maj,&con);

        //checks if the application specific presentation exists.
        //Parameter list inlist must contain AppUid which refers to presentation.
        // The other parameters are not used.
        //The result is returned in outlist parameter list.
        //First parameter in the outlist contains ETrue if there is presentation
        //available and EFalse if there is not.
        iServiceHandler->ExecuteServiceCmdL( KPresentationExists,
                                             *inlist, *outlist );

        FLOG(_L("CCSXHAppUi::IsApplicationPresentInTutorialL() - cp-1"));

        //__ASSERT_DEBUG( outlist->Count() > 0, User::Panic(_L("TutAiwHlpO"),-1));
        __ASSERT_DEBUG(outlist->Count(), User::Panic(_L("outlist"),0));

        FLOG(_L("CCSXHAppUi::IsApplicationPresentInTutorialL() - cp-2"));
        if( outlist->Count() > 0)
            {
            FLOG(_L("CCSXHAppUi::IsApplicationPresentInTutorialL() - cp-2.4"));
            TAiwGenericParam param = (*outlist)[0];
            TAiwVariant variant = param.Value();
            TInt32 val(0);

            if(variant.Get(val) && val)
                {
                Result = ETrue;
                }
            FLOG(_L("CCSXHAppUi::IsApplicationPresentInTutorialL() - cp-3"));
            __ASSERT_DEBUG( variant.Get(val) && val, User::Panic(_L("TutAiwHlpV"),-1));
            }
        CleanupStack::PopAndDestroy();   //outlist
        CleanupStack::PopAndDestroy();   //inlist
        }
    return Result;
    }

// --------------------------------------------------------------------------
// CCSXHAppUi::SetHelpContext
// If tutorial exists,identify the presentation needed exist or not.
// --------------------------------------------------------------------------
void CCSXHAppUi::SetHelpContext(TCoeHelpContext aTCoeHelpContext)
    {
    iTCoeHelpContext = aTCoeHelpContext;
    }
#endif // FF_HELP_TUTORIAL_MERGE
