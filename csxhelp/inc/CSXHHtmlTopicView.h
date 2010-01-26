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
* Description:  CCSXHHtmlTopicView class declaration
*
*/


#ifndef INC_CCSXHHTMLTOPICVIEW_H__
#define INC_CCSXHHTMLTOPICVIEW_H__

// INCLUDES
#include <aknview.h>
#include <aknlists.h>

class CCSXHHtmlTopicContainer;
class TCoeHelpContext;
// CLASS DECLARATION

/** 
* @class CCSXHHtmlTopicView
* This class handles BrowserView of the Csxhelp Application
*/
class CCSXHHtmlTopicView : public CAknView
    {
    public: 
    
    enum THtmlViewType
    	{
    	EHtmlView = 0,
    	EContextHtmlView
    	};    
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHHtmlTopicView 
*  and return a pointer to the created object
*  @param aUid   - View Id
*		  aFlags - Resource Id
*		  aRect  - TRect Object	
*  @return a CCSXHHtmlTopicView Pointer 
*/
    static CCSXHHtmlTopicView* NewL(const TUid&, const TInt&,const TRect& aRect);
    
/** 
*  @function ~CCSXHHtmlTopicView
*  @since S60 3.2
*  Destroy the object and release all memory objects
*/
    virtual ~CCSXHHtmlTopicView();

/**
*  @function SetViewType
*  @since S60 3.2
*  Sets the view type as either normal or context view
*/        
	void SetViewTypeL(TInt viewType); 
	
/** 
*  @function GetViewType
*  @since S60 3.2
*  Gets the view type as either normal or context view
*/        
	TInt GetViewType(); 	
        
    protected:
/** 
*  @function CCSXHHtmlTopicView
*  @since S60 3.2
*  Perform the first phase of two phase construction
*/  
    CCSXHHtmlTopicView(const TUid&,const TInt&);

/** 
*  @function ConstructL
*  @since S60 3.2
*  Perform the second phase construction of a htmltopicview object
*/  
    void ConstructL(const TRect& aRect);

/**
* From MEikMenuObserver
* @function DynInitMenuPaneL 
* used to manage the menu when
* no item are in the list box.
* We need to delete the "Open"
* menu item in this case.
*
* @since 3.2
* @param aResource resource ID of menu pane
* @param aMenuPane instance of menupane
*/         
        void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
      

    public: // Functions from base classes
/**
* From CAknView, return view id of this view
*
* @function Id
* @since 3.2
* @return view id
*/
       TUid Id() const;

/** 
 * @function HandleCommandL
 * @since S60 3.2  
 * takes care of command handling.
 * @param aCommand command to be handled
 */
       void HandleCommandL(TInt aCommand);
       
/** 
 * @function ResourceChangeHdl
 * @since S60 3.2  
 * Handler for resource change
 * @param aType - Type of resource change event
 */
     void ResourceChangeHdl(TInt aType);             

/** 
 * @function ViewActivatedL
 * @since S60 3.2  
 * Takes any action required when this view is activated. 
 * Handles this view activation event from view server
 * @param aPrevViewId  		- Specifies the view previously active.  
 *		  aCustomMessageId  - Specifies the message type.  
 *  	  aCustomMessage    - Activation message 
 */
       void ViewActivatedL(const TVwsViewId& aPrevViewId,
	                             TUid aCustomMessageId,
	                             const TDesC8& aCustomMessage);
	                             
	                             
/** 
 * @function SetSoftKeysL
 * @since S60 3.2  
 * Sets SoftKeys 
 * @param aSoftKeys Resource Id
 */	                             
	   void SetSoftKeysL(TInt aSoftKeys);
	   
/** 
 * @function GetCurrentFontSize
 * @since S60 3.2  
 * SetSoftKeysL 
 * @return Current FontSize 
 */	   
	   TInt GetCurrentFontSize();
/** 
*  @function SetMiddleSoftKey
*  @since S60 3.2
*  This will set the Label for MSK
*/	   
	   void SetMiddleSoftKey(TBool aValue);
	   
/**
* From CAknView
* @function DoDeactivate 
* de-activation handler
*
* @since 3.2
* @param aPrevViewId ID of previious view.
*/  
      void DoDeactivate();	   
#ifdef FF_HELP_TUTORIAL_MERGE
    /**
    *  Lauch tutorial if exist, else popup a error message
    *  @since S60 3.2
    *  @param None
    *  @return None
    */
    void LaunchTutorial();
#endif // FF_HELP_TUTORIAL_MERGE

    protected: // functions from base classes
/**
* From CAknView
* @function DoActivateL 
* activation handler
*
* @since 3.2
* @param aPrevViewId ID of previious view.
*/
       void DoActivateL(const TVwsViewId& aPrevViewId,
                             TUid /* aCustomMessageId */,
                             const TDesC8& /* aCustomMessage */);


        
   private: 
        // previous view
      TVwsViewId iPrevious;
	  CCSXHHtmlTopicContainer* iBCContainer;
	  
	  const TUid& iUid;
      const TInt& iAppFlags;
      
	  static TInt iFontSize;
	  
	  TBool iContextTopicView;		
    };

#endif // INC_CCSXHHTMLTOPICVIEW_H__

// End of File
