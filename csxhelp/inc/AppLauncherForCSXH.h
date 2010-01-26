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
* Description:  AppLauncherForCSXH class declaration
*
*/


#ifndef INC_APPLAUNCHERFORCSXH_H_HEADER_INCLUDED_BC03B1F7
#define INC_APPLAUNCHERFORCSXH_H_HEADER_INCLUDED_BC03B1F7

#include <e32std.h> 
#include <apgtask.h> 

class TApaTask;

const TInt KMaxParamLength = 32;
const TInt KMaxVSAPrefix = 32;
const TInt KMaxVSAScheme = 88;
const TInt KBackSlashLength = 1;
/**
 *  AppLauncherForCSXH 
 *  This class implements launching applications from help
 *  @since S60 v3.2
 */
class AppLauncherForCSXH 
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a AppLauncherForCSXH 
*  and return a pointer to the created object
*  @return a AppLauncherForCSXH Pointer 
*/
    static AppLauncherForCSXH* NewL();
    
/** 
*  @function ~AppLauncherForCSXH
*  @since S60 3.2
*  Destroy the object and release all memory objects
*/    
      ~AppLauncherForCSXH();
/** 
*  @function LaunchAppL
*  @since S60 3.2
*  Launches Application
*  @param aUrl  - Buffer which contains iformation about Application Uid,MessageId, ViewID 
*         aCurrentUrl - Currently displayed file path info
*  @return ETrue, if Application Launches Successfully, EFalse otherwise	
*/      
      TBool LaunchAppL(const TDesC& aUrl,const TDesC& aCurrentUrl); 
/** 
*  @function LaunchBrowserNGL
*  @since S60 3.2
*  Launches BrowserNG Application
*  @param aUrl  - external hyperlink info.
*  @return ETrue, if Application Launches Successfully, EFalse otherwise	
*/      
      void LaunchBrowserNGL(const TDesC& aUrl);     	     	
    private:

/** 
*  @function AppLauncherForCSXH
*  @since S60 3.2
*  Perform the first phase of two phase construction
*/ 
     AppLauncherForCSXH();
     
/** 
*  @function ConstructL
*  @since S60 3.2
*  Perform the second phase construction of a legacytopicview object
*/     
     void ConstructL();
/** 
*  @function GetAppLaunchModeL
*  @since S60 3.2
*  Get the Mode Application Launch Command / View Switch Architecture
*  @param 	aUrl  - Buffer which contains iformation about Application Uid,MessageId, ViewID 
*   		aCurrentUrl - Currently displayed file path info
*  @return  Mode of Launch
*/    
      TInt GetAppLaunchModeL(const TDesC& aUrl,const TDesC& aCurrentUrl);       
/** 
*  @function LaunchApp
*  @since S60 3.2
*  Launch the Application
*  @param 	aUrl  - Buffer which contains iformation about Application Uid,MessageId, ViewID 
*   		aVSAMode - Etrue for View Switch Architecture - EFalse for Command Launch
*  @return  ETrue, Application Launch Successful, EFalse otherwise
*/      
      TBool LaunchApp(const TDesC& aUrl, TBool aVSAMode = ETrue);
/** 
*  @function LaunchCmdApp
*  @since S60 3.2
*  Launch the Application 
*  @param 	aParamsMode  - ETrue, If Application has to be launched with Message. 
*  @return  ETrue, Application Launch Successful, EFalse otherwise
*/      
      TBool LaunchCmdApp(TBool aParamsMode = ETrue);
/** 
*  @function LaunchCmdAppL
*  @since S60 3.2
*  Launch the Application using Command line APIs with parameters
*  @param 	aParamsMode  - ETrue, If Application has to be launched with Message. 
*  @return  ETrue, Application Launch Successful, EFalse otherwise
*/     
      TBool LaunchCmdAppL(TBool aParamsMode);
  /** 
*  @function GetCmdAppTask
*  @since S60 3.2
*  Launch the Application using Command line APIs with parameters
*  @return  Get the TApaTask of the Launched Application
*/      
      TApaTask GetCmdAppTask();
/** 
*  @function SendMsgToCmdAppTaskL
*  @since S60 3.2
*  Send the Message to the Launched Application
*  @param 	aTask  - TApaTask of the launched Application 
*/      
      void SendMsgToCmdAppTaskL(TApaTask &aTask);

/** 
*  @function HandleFailedAppLauchL
*  @since S60 3.2
*  Gives Information Note if Application Launch Failed.
*/      
      void HandleFailedAppLauchL();

/** 
*  @function HandleAppLauchFromInternetL
*  @since S60 3.2
*  Gives Information Note if user try to Launch the application from the external link
*/      
      void HandleAppLauchFromInternetL();
/** 
*  @function CheckAppLaunchFromInternetL
*  @since S60 3.2
*  Checks wether using is trying to launch the application from  external link
*  @param aCurrentMode - VSA / Cmd
*		  aCurrentUrl  - Currently displayed file path info
*/  
      TInt CheckAppLaunchFromInternetL(TInt aCurrentMode,const TDesC& aCurrentUrl );

/** 
*  @function CheckMsgParams
*  @since S60 3.2
*  Checks wether parameters are Valid / Invalid
*  @param Buffer which contains iformation about Application Uid,MessageId, ViewID 
*  @return EParamsWithMsg / EParamsInvalid / EParamsNoMsg
*/      
      TInt CheckMsgParams(const TDesC& aUrl);

/** 
*  @function CheckMsgParams
*  @since S60 3.2
*  Copies Uid & View Id into Variables
*  @param Buffer which contains information about Application Uid,ViewID 
*/ 
      void ExtractTwoParams(const TDesC& aUrl);

/** 
*  @function ExtractFourParams
*  @since S60 3.2
*  Copies Uid, View Id, MessageId & Message into Variables
*  @param aUrl - Buffer which contains information about Application Uid,ViewID, MessageId &  
*/ 
      void ExtractFourParams(const TDesC& aUrl);     
/** 
*  @function GetUid
*  @since S60 3.2
*  Coverts Buffer Content to TInt
*  @param aUid - Application Uid 
*		  aResult - Application Uid	
*/
      void GetUid(TPtrC aUid, TInt32 &aResult);       

/** 
*  @function SendMsgToBrowserNGL
*  @since S60 3.2
*  Send the Message to the BrowserNG
*  @param aUrl - External Hyper Link
*  @return ETrue MessageSent Successfully, EFalse otherwise
*/      
	  TBool SendMsgToBrowserNGL(const HBufC* aUrl);		
/** 
*  @function StartTimer
*  @since S60 3.2
*  Starts the timer
*  @param aCallBack - Callback function for timer
*/      
	  void StartTimer(TCallBack aCallBack);

/** 
*  @function Timer_Callback_to_TerminateApp
*  @since S60 3.2
*  Timer callback function
*  @param aPtr - AppLauncherForCSXH class pointer
*  @return 0
*/      
      static TInt Timer_Callback_to_TerminateApp(TAny* aPtr);

/** 
*  @function SendMsgToApplication
*  @since S60 3.2
*  SendMessage to BrowserNG Application
*  @return 0
*/     
      TInt SendMsgToApplication();    		 
      
    private:
	  CPeriodic* iTimer;
	  TInt iCount;
	  TBuf<KMaxFileName> iUrl;      
      TBuf<KMaxParamLength> iParam1;
      TBuf<KMaxParamLength> iParam2;
      TBuf<KMaxParamLength> iParam3;
      TBuf<KMaxParamLength> iParam4;	  
    };    
		

#endif /* INC_APPLAUNCHERFORCSXH_H_HEADER_INCLUDED_BC03B1F7 */
