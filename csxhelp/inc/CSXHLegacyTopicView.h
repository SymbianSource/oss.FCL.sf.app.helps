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
* Description:  CCSXHLegacyTopicView class declaration
*
*/


#include <aknview.h>
#include <aknlists.h>

class CCSXHLegacyTopicContainer;

// CLASS DECLARATION

/** 
* @class CCSXHLegacyTopicView
* This class handles RichTextView of the Csxhelp Application
*/
class CCSXHLegacyTopicView : public CAknView
    {
    public:   
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHLegacyTopicView 
*  and return a pointer to the created object
*  @param aUid   - View Id
*		  aFlags - Resource Id
*		  aRect  - TRect Object	
*  @return a CCSXHLegacyTopicView Pointer 
*/
     
    static CCSXHLegacyTopicView* NewL(const TUid& aUid, const TInt& 
			aFlags,const TRect& aRect);
/**
* From CAknView
* @function DoDeactivate 
* de-activation handler
*
* @since 3.2
* @param aPrevViewId ID of previious view.
*/    
    void DoDeactivate();
    
/** 
*  @function ~CCSXHLegacyTopicView
*  @since S60 3.2
*  Destroy the object and release all memory objects
*/
        virtual ~CCSXHLegacyTopicView();
    protected:
/** 
*  @function CCSXHLegacyTopicView
*  @since S60 3.2
*  Perform the first phase of two phase construction
*/        CCSXHLegacyTopicView(const TUid&, const TInt&);

/** 
*  @function ConstructL
*  @since S60 3.2
*  Perform the second phase construction of a legacytopicview object
*  @param aRect the rectangle this view will be drawn to
*/ 
       void ConstructL(const TRect& aRect);          

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
 * @function ViewActivatedL
 * @since S60 3.2  
 * Takes any action required when this view is deactivated. 
 * Handles this view deactivation event from view server 
 */
	   void ViewDeactivated();


    private: 
         
		CCSXHLegacyTopicContainer* iTopicContainer;
		TVwsViewId iPrevious;
		
		const TUid& iUid;
      	const TInt& iAppFlags;
  
    };
