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
* Description:  CCSXHGenericView class declaration
*
*/


#ifndef INC_CSXHGENERICVIEW_H_HEADER_INCLUDED_BC03B1F7
#define INC_CSXHGENERICVIEW_H_HEADER_INCLUDED_BC03B1F7

#include <aknview.h>
#include <e32std.h> 

//forward declarations
class CCSXHGenericContainer;

/** 
* @class CCSXHGenericView
* This class List View of the Csxhelp Application
*/
class CCSXHGenericView : public CAknView
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHGenericContainer 
*  and return a pointer to the created object
*  @param aUid   - View Id
*		  aFlags - Resource Id
*		  aRect  - TRect Object	
*  @return a CCSXHGenericView Pointer 
*/      
     	static CCSXHGenericView* NewL(const TUid& aUid, const TInt& aFlags);

/** 
*  @function ~CCSXHGenericView
*  @since S60 3.2
*  Destroy the object and release all memory objects
*/
	   	~CCSXHGenericView();

/** 
*  @function ResetContainer
*  @since S60 3.2
*  Reset the Flag, which helps to create a new Container 
*/
	   	void ResetContainer();

/** 
*  @function SetMiddleSoftKey
*  @since S60 3.2
*  This will set the Label for MSK
*/
    	void SetMiddleSoftKey(TBool aValue);
    	
    public:// Functions from base classes
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

    	
    protected: // Functions from base classes
/**
* From CAknView
* @function DoActivateL 
* activation handler
*
* @since 3.2
* @param aPrevViewId ID of previious view.
*/    
       	void DoActivateL(const TVwsViewId& aPrevViewId,TUid,const TDesC8& );

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
    
    private:
/** 
*  @function CCSXHGenericView
*  @since S60 3.2
*  Perform the first phase of two phase construction
*/    
       	CCSXHGenericView(const TUid&,const TInt&);

/** 
*  @function ConstructL
*  @since S60 3.2
*  Perform the second phase construction of a genericview object
*/	 	
       	void ConstructL(); 
    
    private:
        CCSXHGenericContainer *iContainer;
        TBool iResetContainerInActivate;
        
        const TUid &iUid;
        const TInt &iAppFlags;

    };

#endif /* INC_CSXHGENERICVIEW_H_HEADER_INCLUDED_BC03B1F7 */
