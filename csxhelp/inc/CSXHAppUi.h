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
* Description:  CCSXHAppUi class declaration
*
*/


#ifndef INC_CSXHAPPUI_H_HEADER_INCLUDED_BC054CBF
#define INC_CSXHAPPUI_H_HEADER_INCLUDED_BC054CBF

#include <aknViewAppUi.h>
#include "AppLauncherForCSXH.h"
#include "csxhruntimeappwatcher.h"

//Forward declarations
class CCSXHHtmlTopicView;
class CCSXHLegacyTopicView;
class CCSXHGenericView;
#ifdef FF_HELP_TUTORIAL_MERGE
class CAiwGenericParamList;
class CAiwServiceHandler;
#endif // FF_HELP_TUTORIAL_MERGE

// CLASS DECLARATION

/** 
* @class CCSXHAppUi
* Application UI class
* Provides support for the following features:
* - view architecture
*/
class CCSXHAppUi : public CAknViewAppUi,
                   public MInstallationObserver
    {
  public:
/** 
 * @function ConstructL
 * @since S60 3.2
 * Perform the second phase construction of a CCSXHAppUi object this needs
 * to be public due to the way the framework constructs the AppUi      
 */
    void ConstructL();
/** 
 * @function CCSXHAppUi
 * @since S60 3.2
 * Perform the first phase of two phase construction. This needs to be
 * public due to the way the framework constructs the AppUi
 */    
    CCSXHAppUi();
/** 
 * @function ~CCSXHAppUi
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */    
    ~CCSXHAppUi();

/** 
 * @function ~CCSXHAppUi
 * @since S60 3.2  
 * From CEikAppUi, takes care of command handling.
 * @param aCommand command to be handled
 */    
    void HandleCommandL(TInt aCommand);
    
/** 
 *  @function PropagateResourceChange
 *  @since S60 3.2
 *  Propagates the resource change event to all views
 *  @param aType - Type of resource change event
 */		
	void PropagateResourceChange(TInt aType);     
    
/** 
 * @function GetInstance
 * @since S60 3.2  
 * returns Application UI class pointer 
 * @return Application UI class pointer 
 */     
    static CCSXHAppUi* GetInstance();

/** 
 * @function GetWsSession
 * @since S60 3.2  
 * returns WsSession Pointer
 * @return WsSession pointer
 */    
	static RWsSession& GetWsSession();

/** 
 * @function GetCoeEnv
 * @since S60 3.2  
 * returns Control Environment pointer
 * @return Control Environment pointer
 */	
	static CCoeEnv* GetCoeEnv();

/** 
 * @function AppLaunchL
 * @since S60 3.2  
 * Launch the Application based on Uid & ViewId
 * @param aAppId Application Uid
 * 		 aVId   ViewId 
 */	    
    void AppLaunchL(TInt32 &aAppId,TInt32 &aVId);
    
/** 
 * @function AppLaunchL
 * @since S60 3.2  
 * Launch the Application based on Uid,ViewId, MessageId & Message
 * @param aAppId   Application Uid
 * 		 aVId     ViewId
 * 		 aMsgId   MessageId
 * 		 aParams  Message
 */    
    void AppLaunchL(TInt32 &aAppId,TInt32 &aVId,TInt32 &aMsgId,
    									const TBuf<KMaxParamLength>& aParams);
    
/** 
 * @function ResetTOC2ViewContainer
 * @since S60 3.2  
 * Sets a flag in the View for recreating the container 
 */    
    void ResetTOC2ViewContainer();
/**
* From MInstallationObserver
* @function RuntimeUpdateIndex 
* call back function once install/uninstall happens
*
* @since 5.0
*/
    void RuntimeUpdateIndex();

#ifdef FF_HELP_TUTORIAL_MERGE
    /**
    *  Generates parameter list for AIW calls
    *  @since S60 3.2
    *  @param aContext The help content ready to Tutorial
    *  @return The Aiw Generic Param List
    */
    CAiwGenericParamList* NewParamListLC(const TCoeHelpContext& aContext);

    /**
    *  If tutorial exists,call play presentation command
    *  @since S60 3.2
    *  @param None
    *  @return None
    */
    void CallTutorialAiwProviderL();

    /**
    *  If tutorial exists,call play presentation command
    *  @since S60 3.2
    *  @param aTCoeHelpContext The Help Content stored
    *  @return None
    */
    void SetHelpContext(TCoeHelpContext aTCoeHelpContext);

    /**
    *  If the presentation requsted exist or not
    *  @since S60 3.2
    *  @param None
    *  @return A boolean value to indicate the presentation exist or not
    */
     TBool IsApplicationPresentInTutorialL();

#endif // FF_HELP_TUTORIAL_MERGE
	protected: 
	// Functions from base calsses
	/**
    * Handles changes in keyboard focus when an application switches to, 
    * or from, the foreground.
    * @param aForeground @c ETrue if the application is in the foreground,
    * otherwise @c EFalse.
    */
	void HandleForegroundEventL(TBool aForeground);
/** 
 * @function ProcessCommandParametersL
 * From CEikAppUi.
 * Take care of command parameter
 * @since 3.2
 * @param aTail should be contexts in TDesC8& form
 * @return whether or not processing was successful
 */
        TBool ProcessCommandParametersL(TApaCommand aCommand,
                                        TFileName& /* aDocumentName */,
                                        const TDesC8& aTail);

/**
 * @function HandleMessageL 
 * From MCoeMessageObserver
 * Take care of messages from other app.
 * @since 3.2
 * @param aMessageParameters should be contexts in TDesC8&
 * @return always EMessageHandled.
 */
    MCoeMessageObserver::TMessageResponse HandleMessageL(
                                  TUint32 aClientHandleOfTargetWindowGroup,
                                  TUid aMessageUi,
                                  const TDesC8& aMessageParameters); 
  private:
/**
 * @function ActivateDisplayTopicViewL   
 * Take care of Recreating the Container Class for Specific Views
 * @since 3.2
 * @param aMessageParameters should be contexts in TDesC8&
 * @return always EMessageHandled.
 */  
  	void ActivateDisplayTopicViewL();
  	
  	void InitDisplayTopicViewL(TUid aViewID);
/**
 * @function HandleContextSensitiveLaunchL   
 * Take care of Context Sensitive Launch.
 * @since 3.2
 * @param aMessageParameters should be contexts in TDesC8&
 * @return always EMessageHandled.
 */  	
  	void HandleContextSensitiveLaunchL(const TDesC8& aContext );   
#ifdef FF_HELP_TUTORIAL_MERGE
        /**
        *  Load Service Handler and attach the interest
        *  @since S60 3.2
        *  @param None
        *  @return None
        */
        void LoadTutorialServiceL();
#endif
  private:
    
    CCSXHGenericView* iTOC1View;
    CCSXHGenericView* iKywdTOC1View;
    
    CCSXHLegacyTopicView* iLegacyTopicView;
    CCSXHLegacyTopicView* iLegacyContextTopicView;
    
    //This class will be used for both normal and context html topics    
    CCSXHHtmlTopicView* iHtmlTopicView;
	
	CCSXHGenericView* iKywdTOC2View;
	CCSXHGenericView* iTOC2View;
	CCSXHGenericView* iAppHelpsTOC1View;
	CCSXHGenericView* iAppHelpsTOC2View;
	
	static CCSXHAppUi* iInstance;
	CCSXHRuntimeAppWatcher* iRuntimeWatcher;
	
#ifdef FF_HELP_TUTORIAL_MERGE

        //AIW Service Handler
        //Owned
        CAiwServiceHandler* iServiceHandler;

        //Tutorial exist or not
        TBool iTutorialExists;

        //Help Content Text
        TCoeHelpContext iTCoeHelpContext;
#endif
    };



#endif /* INC_CSXHAPPUI_H_HEADER_INCLUDED_BC054CBF */
