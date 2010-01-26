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
* Description:  CCSXHGenericContainer class declaration
*
*/


#ifndef INC_CSXHGENERICCONTAINER_H_HEADER_INCLUDED_BC039735
#define INC_CSXHGENERICCONTAINER_H_HEADER_INCLUDED_BC039735

#include <coecntrl.h> // CCoeControl
#include <eiklbo.h> // MEikListBoxObserver
#include <aknsfld.h>

// FORWARD DECLARATIONS
class CAknSingleStyleListBox;
class CCSXHDocument;
class CCSXHGenericView;

#ifdef FF_HELP_TUTORIAL_MERGE
class CCSXHGenericTOC1;
#endif

// CLASS DECLARATION
/**
* @class CCSXHGenericContainer
* This class provides listbox container control functionality
*/

class CCSXHGenericContainer : public CCoeControl, public MEikListBoxObserver
    {
    public:
    /**
    *  @function NewL
    *  @since S60 3.2
    *  Construct a CCSXHGenericContainer
    *  and return a pointer to the created object
    *  @param aRect - TRect Object
    *         aDocument - Document Object
    *  @return a CCSXHGenericContainer Pointer
    */
#ifdef FF_HELP_TUTORIAL_MERGE
        static CCSXHGenericContainer* NewL(const TRect& aRect,CCSXHDocument &aDocument
        ,CCSXHGenericView* aView);
#else // !FF_HELP_TUTORIAL_MERGE
        static CCSXHGenericContainer* NewL(const TRect& aRect,CCSXHDocument &aDocument);
#endif // FF_HELP_TUTORIAL_MERGE
    /**
    *  @function NewLC
    *  @since S60 3.2
    *  Construct a CCSXHGenericContainer first phase of two phase constuction
    *  @param aRect - TRect Object
    *         aDocument - Document Object
    */
#ifdef FF_HELP_TUTORIAL_MERGE
        static CCSXHGenericContainer* NewLC(const TRect& aRect, CCSXHDocument &aDocument
        ,CCSXHGenericView* aView);
#else // !FF_HELP_TUTORIAL_MERGE
        static CCSXHGenericContainer* NewLC(const TRect& aRect, CCSXHDocument &aDocument);
#endif // FF_HELP_TUTORIAL_MERGE
    /**
    *  @function ~CCSXHGenericContainer
    *  @since S60 3.2
    *  Destroy the object and release all memory objects
    */
    ~CCSXHGenericContainer();

/** 
*  @function HandleResourceChange
*  @since S60 3.2
*  Handles resource changes.
*  This function is called by framework when when e.g skin or locale is changed.
*  @param aType resource change type
*/    	
    	void HandleResourceChange(TInt aType);

/** 
*  @function HandleResourceChangeImpl
*  @since S60 3.2
*  Handles resource changes.
*  Implementation function for resource change
*  @param aType resource change type
*/    	
    	void HandleResourceChangeImpl(TInt aType);

/** 
*  @function SetDisplayTopicL
*  @since S60 3.2
*  Using Selected Item, set next object for display  
*/    	
    	void SetDisplayTopicL();

/** 
*  @function GetNumberOfListItems
*  @since S60 3.2
*  Get the number of items present in the List
*  @return Number of Items in the List
*/    	
    	TInt GetNumberOfListItems();

/** 
*  @function SetCurrentView
*  @since S60 3.2
*  Sets View Pointer
*  @param aView - Sets View pointer which is using this Container
*/    	
    	void SetCurrentView(CCSXHGenericView* aView);
/** 
*  @function CheckForMSK
*  @since S60 3.2
*  Checks Msk flag is Enabled or not, If Enabled, Sets MSK Label
*/    	
    	void CheckForMSK();	
/** 
*  @function HightLightItemL
*  @since S60 3.2
*  Sets the selected item in the list box
*/
		void HightLightItemL();		

#ifdef FF_HELP_TUTORIAL_MERGE
        /**
        *  If the selected item is a tutorial item
        *  @since S60 3.2
        *  @param None
        *  @return A boolean value to indicate the item is tutorial or not
        *  Sets the selected item in the list box
        */
		TBool IsShowTutorialItemSelectedL();
#endif // FF_HELP_TUTORIAL_MERGE
		

        void MakeVisible(TBool aVisible);

    protected: // From base classes.
/**
 * When focus is changed, called.
 * Set focus editor(viewer).
 * @function FocusChanged
 * @since 3.2
 * @param aDrawNow used to determine whether or not
 *        redraw should occur now, or later
 */
        void FocusChanged(TDrawNow aDrawNow);			
    	
	private:
	
/** 
*  @function CCSXHGenericContainer
*  @since S60 3.2
*  Perform the second phase construction of a CCSXHDocument object
*  @param aDocument - CCSXHDocument Object
*/
#ifdef FF_HELP_TUTORIAL_MERGE
        CCSXHGenericContainer(CCSXHDocument &aDocument,CCSXHGenericView* aView);
#else // !FF_HELP_TUTORIAL_MERGE
        CCSXHGenericContainer(CCSXHDocument &aDocument);
#endif // FF_HELP_TUTORIAL_MERGE
/** 
*  @function ConstructL
*  @since S60 3.2
*  Perform the first phase of two phase construction
*  @param aRect - TRect Object
*/
    	void ConstructL(const TRect& aRect);    	

/** 
*  @function CreateAndPopulateListL
*  @since S60 3.2
*  Creates a AVKON List & Inserts items into it.
*/
		void CreateAndPopulateListL();	
		
		
	private: 
/** 
*  From MEikListBoxObserver class
*  @function HandleListBoxEventL
*  @since S60 3.2
*  This function handles List box Events
*  @param aListBox 		- ListBox Pointer
*		  aListBoxEvent - Event to be handled 	
*/
       	void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aListBoxEvent);	

	private: // from CoeControl
/** 
*  From CoeControl class
*  @function SizeChanged
*  @since S60 3.2
*  re-allocates and re-sizes components.
*/
       	void SizeChanged();
/** 
*  From CoeControl class
*  @function CountComponentControls
*  @since S60 3.2
*  @return Number of child controls this has.  
*/       	
       	TInt CountComponentControls() const;

/** 
*  From CoeControl class
*  @function ComponentControl
*  @since S60 3.2
* @param aIndex index of child controls
* @retrun pointer to child control
*/       	
       	CCoeControl* ComponentControl(TInt aIndex) const;

/** 
*  From CoeControl class
*  @function Draw
*  @since S60 3.2
*  Draw() for drawing the Background
*/       	
       	void Draw(const TRect& aRect) const;
/** 
*  From CoeControl class
*  @function OfferKeyEventL
*  @since S60 3.2
*  Progress Key Event
*  @param aKeyEvent the key event
*  @param aType EEventKey | EEventKeyUp | EEventKeyDown
*  @return EKeyWasConsumed if key event is processed,
*         otherwise EKeyWasNotConsumed
*/       	
       	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);	
            	
    private:	
       	CCSXHDocument& iDocument;
	   	CAknSingleStyleListBox* iListBox;
       	CAknSearchField* iFindPane;
       	CCSXHGenericView* iView;

       	
	};



#endif /* INC_CSXHTOC2CONTAINER_H_HEADER_INCLUDED_BC039735 */
