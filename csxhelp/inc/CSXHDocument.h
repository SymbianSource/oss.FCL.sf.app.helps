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
* Description:  CCSXHDocument class declaration
*
*/


#ifndef INC_CSXHDOCUMENT_H_HEADER_INCLUDED_BC0121B7
#define INC_CSXHDOCUMENT_H_HEADER_INCLUDED_BC0121B7

#include <AknDoc.h>

// Forward references
class CAknAppUi;
class CEikApplication;
class CCSXHHelpDataBase;
class CCSXHHelpContentBase;
class CEikAppUi;
class CEikProgressInfo;
class CAknProgressDialog;
class CIdle;
/** 
  @class CCSXHDocument
* An instance of class CCSXHDocument is the Document part of the AVKON
* application framework for the Csxhelp application
*/

class CCSXHDocument : public CAknDocument
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHDocument for the AVKON application aApp 
*  construction, and return a pointer to the created object
*  @param aApp - Application Object
*  @return a CCSXHDocument Pointer 
*/
	static CCSXHDocument* NewL(
	// application creating this document
	CEikApplication& aApp);
    // Construct a CCSXHDocument for the AVKON application aApp using two phase
    // construction, and return a pointer to the created object

/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHDocument for the AVKON application aApp using two phase
*  construction, and return a pointer to the created object
*  @param aApp - Application Object
*  @return a CCSXHDocument Pointer 
*/
    static CCSXHDocument* NewLC(
	// application creating this document
	CEikApplication& aApp);


/** 
*  @function ~CCSXHDocument
*  @since S60 3.2
*  Destroy the object and release all memory objects
*/
	    ~CCSXHDocument();

	public: // from CAknDocument
/** 
*  @function CreateAppUiL
*  @since S60 3.2
*  Create a CCSXHAppUi object and return a pointer to it
*  @return CCSXHAppUi class pointer
*/		
  	    CEikAppUi* CreateAppUiL();
	    
  	public:
/** 
*  @function SetDisplayTopic
*  @since S60 3.2
*  @param aDisplayTopic - HelpContentbase Class pointer
*/  	
		void SetDisplayTopic(CCSXHHelpContentBase *aDisplayTopic);
/** 
*  @function SetDisplayAndPrevTopic
*  @since S60 3.2
*  @param aDisplayTopic - HelpContentbase Class pointer
*/  	
		void SetDisplayAndPrevTopic(CCSXHHelpContentBase *aDisplayTopic);
/** 
*  @function GetDisplayTopic
*  @since S60 3.2
*  @return HelpContentBase Class pointer
*/ 
   inline  CCSXHHelpContentBase* GetDisplayTopic() const 
   {return iDisplayTopic;}
/** 
*  @function GetPrevTopic
*  @since S60 3.2
*  @return HelpContentBase Class pointer
*/		
	inline CCSXHHelpContentBase* GetPrevTopic() const
	{return iPrevTopic;}
/** 
*  @function ClearPrevTopic
*  @since S60 3.2
*  @Clears the previous topic
*/		
	inline void ClearPrevTopic()
	{iPrevTopic = NULL;}
/** 
*  @function GetHelpDataBase
*  @since S60 3.2
*  @return HelpContentBase Class pointer
*/		
 	inline CCSXHHelpDataBase* GetHelpDataBase() const
 	{return iHelpDataBase;}

/** 
*  @function GetHelpDataBase
*  @since S60 3.2
*  Sends a Command to Application UI Class
*  @param aCommand - Command to be Handled
*/		
	 	void SendMessageToAppUiL(TInt aCommand);

/** 
*  @function SetContextTopicAsDisplayTopicL
*  @since S60 3.2
*  Parse the Message, Get the topic for Display & set this Object as Current Display Object
*  @param aContextMessage - Message from Other Application for Context Sensitive launch
*  @return  ETrue for Topic Found /EFalse for topic Not found
*/	 	
	 	TBool SetContextTopicAsDisplayTopicL(const TDesC8& aContextMessage);

/** 
*  @function GetHtmlTopicForUrlL
*  @since S60 3.2
*  Get the Object for HTML topic using url Name
*  @param url - url Name
*  @return Pointer to the HelpDataBase which contains HTMLtopic info
*/	 	
	 	CCSXHHelpContentBase* GetHtmlTopicForUrlL(const TDesC &url);

/** 
*  @function InitProgressBar
*  @since S60 3.2
*  Initializes the progress bar
*/	 	
	    void InitProgressBarL();

/** 
*  @function FreeProgressBar
*  @since S60 3.2
*  Frees the progress bar
*/	 	
	 	void FreeProgressBarL();
	 	
/**
        * function for TCAllBack protocol.
        * called directly
        * @since 1.1
        * @param aThis is the pointer to the actual instance
        * @return Whether finished or not
        */
        static TInt CallBack(TAny* aThis);
        
        /**
        * call back function.
        * called by CallBackL()
        * @since 1.1
        * @return Whether finished or not
        */
        TInt DoCallBackL();	 	
	 	
	private:
/** 
*  @function CCSXHDocument
*  @since S60 3.2
*  Perform the first phase of two phase construction
*/
	    CCSXHDocument(
	// application creating this document
	CEikApplication& aApp);
  
  	private:  
    	CCSXHHelpDataBase *iHelpDataBase;
    	CAknAppUi *iHelpAppUi;
    	
    	CCSXHHelpContentBase *iDisplayTopic;
    	CCSXHHelpContentBase *iPrevTopic;
    	
    	CAknProgressDialog *iProgressDialog;
    	
    	CIdle *iIdle;
    	
    	TInt iTotalKeywordsInResultView;
    };



#endif /* INC_CSXHDOCUMENT_H_HEADER_INCLUDED_BC0121B7 */
