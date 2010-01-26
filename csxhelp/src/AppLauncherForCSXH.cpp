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
* Description:  AppLauncherForCSXH class definition
*
*/


#include "AppLauncherForCSXH.h"
#include "CSXHAppUi.h"
#include <cshelp.rsg>

#include <e32std.h>
#include <apgtask.h>
#include <apgcli.h> 
#include <utf.h>
#include <aknnotewrappers.h> 
#include <AknGlobalNote.h>
#include "csxhconstants.h"
#include <e32std.h> 


const TInt KBackSlashesInPrefix = 3;
#ifndef __SERIES60_30__ 
	const TUid KUidBrowserApplication = { 0x10008D39 };
#else
	const TUid KUidBrowserApplication = { 0x1020724D };
#endif

const TInt KKeyBlockTime = 100000;  

enum TAppLaunchMode
            {
            ENoAppLaunch = 0,   
            EVSAMode = 1,
            ECmdMode = 2,
            EAppLaunchFromInternet = 3,
            };
enum TAppLauchParams
    {
    EParamsInvalid = 0,
    EParamsNoMsg,
    EParamsWithMsg,
    };


AppLauncherForCSXH* AppLauncherForCSXH::NewL()
    {
    AppLauncherForCSXH* self = new(ELeave) AppLauncherForCSXH();
    self->ConstructL();
    return self;
    }

AppLauncherForCSXH::AppLauncherForCSXH()
    {}
    
void AppLauncherForCSXH::ConstructL()
    {
    iTimer = CPeriodic::NewL( CActive::EPriorityStandard ); 
    }
    
AppLauncherForCSXH::~AppLauncherForCSXH()
    {
    if(iTimer)
        delete iTimer;      
    }

// --------------------------------------------------------------------------
// Launches an application based on the Application Launch Link URL
// --------------------------------------------------------------------------
TBool AppLauncherForCSXH::LaunchAppL(const TDesC& aUrl,const TDesC& aCurrentUrl)
    {
    switch(GetAppLaunchModeL(aUrl,aCurrentUrl))
        {
        case ENoAppLaunch:
            return EFalse;
        case EAppLaunchFromInternet:
            HandleAppLauchFromInternetL();
            return ETrue;
        case EVSAMode:
            {
            if(!LaunchApp(aUrl,ETrue))
                HandleFailedAppLauchL();
            return ETrue;
            }
        case ECmdMode:
            {
            if(!LaunchApp(aUrl,EFalse))
                HandleFailedAppLauchL();
            return ETrue;
            }   
        default:
            return EFalse;
        }
    }

TInt AppLauncherForCSXH::GetAppLaunchModeL(const TDesC& aUrl,const TDesC& aCurrentUrl)
    {
    TInt Position = aUrl.Find(_L("app://vsa@localhost/"));
    if(KErrNotFound != Position && 0 == Position)
        {
        return CheckAppLaunchFromInternetL(EVSAMode,aCurrentUrl);
        }
    
    Position = aUrl.Find(_L("app://cmd@localhost/"));
    if(KErrNotFound != Position && 0 == Position)
        {
        return CheckAppLaunchFromInternetL(ECmdMode,aCurrentUrl);
        }
    
    return ENoAppLaunch;    
    }

TBool AppLauncherForCSXH::LaunchApp(const TDesC& aUrl, TBool aVSAMode)
    {
    iParam1.Copy(KEmptyString);
    iParam2.Copy(KEmptyString);
    iParam3.Copy(KEmptyString);
    iParam4.Copy(KEmptyString);
    
    switch(CheckMsgParams(aUrl))
        {
        case EParamsInvalid:
            return EFalse;
        case EParamsNoMsg:
            {
            ExtractTwoParams(aUrl);
            if(aVSAMode)
                {
                TInt32 AId,VId;
                GetUid(iParam1,AId);
                GetUid(iParam2,VId);                
                TRAPD(res,CCSXHAppUi::GetInstance()->AppLaunchL(AId,VId));
                return (res == KErrNone) ? ETrue : EFalse;  
                }
            else
                {
                return LaunchCmdApp(EFalse);
                }
            }
        case EParamsWithMsg:
            {
            ExtractFourParams(aUrl);
            if(aVSAMode)
                {
                TInt32 AId,VId,MId;
                GetUid(iParam1,AId);
                GetUid(iParam2,VId);
                GetUid(iParam3,MId);
                TRAPD(res,CCSXHAppUi::GetInstance()->AppLaunchL(AId,VId,MId,iParam4));
                return (res == KErrNone) ? ETrue : EFalse;  
                }
            else
                {
                return LaunchCmdApp(ETrue);
                }
            }
        default:
            return EFalse;
        }
    }
TBool AppLauncherForCSXH::LaunchCmdApp(TBool aParamsMode)
    {
    TRAPD(res,LaunchCmdAppL(aParamsMode));
    if(res != KErrNone)
        return EFalse;
    return ETrue;
    }
TBool AppLauncherForCSXH::LaunchCmdAppL(TBool aParamsMode)
    {
    /*valid cases
    app://cmd@localhost/APP_NAME/APP_UID
    app://cmd@localhost/APP_NAME/APP_UID/MSG_UID/Parameters
    */
    TApaTask task = GetCmdAppTask();
    if (task.Exists())
        {//App is already running
        if(aParamsMode)
            SendMsgToCmdAppTaskL(task);
        
        task.BringToForeground();
        return ETrue;
        }
    //Application has to be started
    CApaCommandLine* cmdLine=CApaCommandLine::NewLC();
    cmdLine->SetExecutableNameL(iParam1);
    cmdLine->SetCommandL(EApaCommandRun);
    
    RApaLsSession lsSession;
    User::LeaveIfError(lsSession.Connect());
    CleanupClosePushL(lsSession);
    User::LeaveIfError(lsSession.StartApp(*cmdLine));
    CleanupStack::PopAndDestroy(&lsSession);
    CleanupStack::PopAndDestroy(cmdLine);   
    if(aParamsMode)
        {
        iCount = 0; 
        TCallBack timer_Callback_to_terminateApp( Timer_Callback_to_TerminateApp,this );
        StartTimer(timer_Callback_to_terminateApp);    
        }
    
    return ETrue;
    }
    
TInt AppLauncherForCSXH::Timer_Callback_to_TerminateApp(TAny* aPtr)
    {
    return static_cast<AppLauncherForCSXH*>( aPtr )->SendMsgToApplication();
    }
        
TInt AppLauncherForCSXH::SendMsgToApplication()
    {
    //If parameters has to be passed then the following is required
    TApaTask cmdTask = GetCmdAppTask();
    if (cmdTask.Exists())
        {
        TRAPD(res,SendMsgToCmdAppTaskL(cmdTask));       
        if(res != KErrNone)
            return 0;
        cmdTask.BringToForeground();
        iTimer->Cancel();
        }
    else if(++iCount > 20)
        {
        iTimer->Cancel();       
        }
        return 0;       
    }   
        
TApaTask AppLauncherForCSXH::GetCmdAppTask()    
    {
    TInt32 AId;
    GetUid(iParam2,AId);
    TApaTaskList taskList(CCSXHAppUi::GetInstance()->GetWsSession());
    TUid KtestAppUid( TUid::Uid( AId ) ); 
    return taskList.FindApp(KtestAppUid);
    }
void AppLauncherForCSXH::SendMsgToCmdAppTaskL(TApaTask& aTask)
    {
    
    TInt32 MId;
    GetUid(iParam3,MId);
    TUid KtestMsgUid( TUid::Uid( MId ) ); 
    HBufC8* params = CnvUtfConverter::ConvertFromUnicodeToUtf8L(iParam4);
    CleanupStack::PushL(params);
    aTask.SendMessage (KtestMsgUid, *params); 
    CleanupStack::PopAndDestroy(params);
    }
    
void AppLauncherForCSXH::HandleFailedAppLauchL()
    {
    HBufC* ErrorMessage = CCSXHAppUi::GetCoeEnv()->AllocReadResourceLC(
    				R_QTN_HELP_NOTE_UNABLE_TO_LAUNCH_APP);      
    CAknGlobalNote* note = CAknGlobalNote::NewLC();
    note->ShowNoteL(EAknGlobalInformationNote, *ErrorMessage);
    CleanupStack::PopAndDestroy(note); 
    CleanupStack::PopAndDestroy(ErrorMessage); 
    }
void AppLauncherForCSXH::HandleAppLauchFromInternetL()
    {
    HBufC* ErrorMessage = CCSXHAppUi::GetCoeEnv()->AllocReadResourceLC(
    				R_QTN_HELP_NOTE_APP_LAUNCH_NOT_ALLOWED);
    CAknGlobalNote* note = CAknGlobalNote::NewLC();
    note->ShowNoteL(EAknGlobalInformationNote, *ErrorMessage);
    CleanupStack::PopAndDestroy(note); 
    CleanupStack::PopAndDestroy(ErrorMessage);        
    }

TInt AppLauncherForCSXH::CheckAppLaunchFromInternetL(TInt aCurrentMode,const TDesC& /*aCurrentUrl*/ )
    {
       
    return aCurrentMode; 
    //BrowserNG is lauched for Externalhyperlinks, 
    //All the security issues are addresed by BrowserNG,    
    
    /*(0 == aCurrentUrl.Find(KFileUrlPrefix) ) ? 
                    aCurrentMode : EAppLaunchFromInternet;*/
    }

TInt AppLauncherForCSXH::CheckMsgParams(const TDesC& aUrl)
    {
    /*
    The following are valid cases
    app://vsa@localhost/APP_UID/VIEW_UID
    app://vsa@localhost/APP_UID/VIEW_UID/MSG_UID/Parameters
    app://cmd@localhost/APP_NAME/APP_UID
    app://cmd@localhost/APP_NAME/APP_UID/MSG_UID/Parameters
    
    That is, it should have exactly 4 backslashes or 
    >= 6 backslashes (considering the possibility that parameters
    can have embedded backslashes)
    */
    TBuf<KBackSlashLength> backSlash(KBackSlash);
    TInt count = 0;
    
    for(TInt i = 0 ; i < aUrl.Length() ; ++i)
        {
        if(KErrNotFound != backSlash.Find(&aUrl[i],KBackSlashLength))
            ++count;
        }
    
    if(count == 4)
        return EParamsNoMsg;
    else if (count >=6)
        return EParamsWithMsg;
    else    
        return EParamsInvalid;
    }

void AppLauncherForCSXH::ExtractTwoParams(const TDesC& aUrl)
    {
    /*Will be called for the following cases
    app://vsa@localhost/APP_UID/VIEW_UID
    app://cmd@localhost/APP_NAME/APP_UID
    */
    iParam1.Copy(KEmptyString);
    iParam2.Copy(KEmptyString);
    
    TBuf<KBackSlashLength> backSlash(KBackSlash);
    TInt count = 0;
    TInt pos;
    
    for(pos = 0; pos < aUrl.Length() ; ++pos)
        {
        if(KErrNotFound != backSlash.Find(&aUrl[pos],KBackSlashLength))
            ++count;
        if(count == KBackSlashesInPrefix)
            break;
        }
    TPtrC16 url = aUrl.Mid(pos + 1);
    pos = url.Find(KBackSlash);
    iParam1 = url.Left(pos);
    iParam2 = url.Mid(pos + 1);
    }

void AppLauncherForCSXH::ExtractFourParams(const TDesC& aUrl)
    {
    /*Will be called for the following cases
    app://vsa@localhost/APP_UID/VIEW_UID/MSG_UID/Parameters
    app://cmd@localhost/APP_NAME/APP_UID/MSG_UID/Parameters
    */
    iParam1.Copy(KEmptyString);
    iParam2.Copy(KEmptyString);
    iParam3.Copy(KEmptyString);
    iParam4.Copy(KEmptyString);
    
    TBuf<KBackSlashLength> backSlash(KBackSlash);
    TInt count = 0;
    TInt pos;
    
    for(pos = 0; pos < aUrl.Length() ; ++pos)
        {
        if(KErrNotFound != backSlash.Find(&aUrl[pos],KBackSlashLength))
            ++count;
        if(count == KBackSlashesInPrefix)
            break;
        }
        
    TPtrC16 url = aUrl.Mid(pos + 1);
    pos = url.Find(KBackSlash);
    iParam1 = url.Left(pos);
    
    TPtrC16 url2 = url.Mid(pos + 1);
    pos = url2.Find(KBackSlash);
    iParam2 = url2.Left(pos);

    TPtrC16 url3 = url2.Mid(pos + 1);
    pos = url3.Find(KBackSlash);
    iParam3 = url3.Left(pos);
    
    iParam4 = url3.Mid(pos+1);
    }

void AppLauncherForCSXH::GetUid(TPtrC aUid, TInt32& aResult)
    {
    TInt hexPos = aUid.Find(_L("x"));
    if(KErrNotFound == hexPos)
        TLex(aUid).Val(aResult); 
    else
        {       
        TPtrC16 uidt = aUid.Mid(hexPos+1);
        TInt64 res;
        //During this operation sometimes values exceed TInt32, but the end result is always
        //fits into TInt32
        TLex(uidt).Val(res,EHex); 
        aResult = res;
        }
    }

void AppLauncherForCSXH::LaunchBrowserNGL(const TDesC& aUrl)
    {    
    _LIT( KBrowserCmdFetchUrl, "4 " );
    HBufC* param = HBufC::NewLC( KBrowserCmdFetchUrl().Length() + 
                                 aUrl.Length() );
	TPtr paramPtr = param->Des();
    paramPtr.Copy( KBrowserCmdFetchUrl );
    paramPtr.Append( aUrl );
    
    if(!SendMsgToBrowserNGL(param))
	    {    
		RApaLsSession appArcSession;
	    User::LeaveIfError( appArcSession.Connect() );
	    CleanupClosePushL<RApaLsSession>( appArcSession );
	    
	    TThreadId id;
	    User::LeaveIfError
	        (
	            appArcSession.StartDocument(*param,KUidBrowserApplication,id )
	        );
	    CleanupStack::PopAndDestroy( &appArcSession );
	    }
	    
	CleanupStack::PopAndDestroy( param );
    }
    
TBool AppLauncherForCSXH::SendMsgToBrowserNGL(const HBufC* aUrl)
    {
    TApaTaskList taskList(CCSXHAppUi::GetInstance()->GetWsSession());
    TApaTask task = taskList.FindApp(KUidBrowserApplication);       
    if(task.Exists())
        {   
        // 8-bit buffer is required.
        HBufC8* param8 = HBufC8::NewLC( aUrl->Length() );
        param8->Des().Copy( *aUrl );
        task.SendMessage( TUid::Uid( 0 ), *param8 ); // Uid is not used
        CleanupStack::PopAndDestroy( param8 );
        return ETrue;           
        }
    else
        return EFalse;
    }
    
void AppLauncherForCSXH::StartTimer(TCallBack aCallBack)
    {
    if( iTimer->IsActive() )
        {
            iTimer->Cancel();
        }

    iTimer->Start(
        TTimeIntervalMicroSeconds32( KKeyBlockTime ),
        TTimeIntervalMicroSeconds32( KKeyBlockTime ),
        aCallBack );
    }    
