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
* Description:  CCSXHLegacyTopicContainer class declaration
*
*/


// INCLUDES
#include <aknview.h>
#include <eikedwob.h>
#include <txtfrmat.h>


// FORWARD DECLARATIONS
class CEikRichTextEditor;
class CEikGlobalTextEditor;
class CRichText;
class CAknsBasicBackgroundControlContext;
class CCSXHDocument;
class CCSXHLegacyTOC2;
// CLASS DECLARATION

/**
*  CCsHelpTopicContainer
*  container control class for legacy topic view
*  @lib etext.lib, eikcoctl.lib
*  @since 3.2
*/
class CCSXHLegacyTopicContainer : public CCoeControl
    {
    public: // Constructors and destructor
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHLegacyTopicContainer 
*  and return a pointer to the created object
*  @param aRect - TRect Object
*		  aDocument - Document Object 
*		  aTopic	- LegacyTOC2 Pointer	
*  @return a CCSXHLegacyTopicContainer Pointer 
*/
        static CCSXHLegacyTopicContainer* NewL(const TRect& aRect,CCSXHDocument
        &aDocument,CCSXHLegacyTOC2 *aTopic);
        
/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHLegacyTopicContainer 
*  and return a pointer to the created object using two phase constuction
*  @param aRect - TRect Object
*		  aDocument - Document Object 
*		  aTopic	- LegacyTOC2 Pointer	
*  @return a CCSXHLegacyTopicContainer Pointer 
*/        
		static CCSXHLegacyTopicContainer* NewLC(const TRect& aRect,CCSXHDocument
		&aDocument,CCSXHLegacyTOC2 *aTopic);
		
/** 
*  @function CCSXHLegacyTopicContainer
*  @since S60 3.2
*  Construct a CCSXHLegacyTopicContainer 
*  and return a pointer to the created object using two phase constuction
*  @param aDocument - Document Object 
*		  aTopic	- LegacyTOC2 Pointer	
*  @return a CCSXHHtmlTopicContainer Pointer 
*/		
		CCSXHLegacyTopicContainer(CCSXHDocument &aDocument,CCSXHLegacyTOC2 *aTopic);

/** 
*  @function RefreshL
*  @since S60 3.2
*  Used to Refresh the Content
*  @param aTopic - CCSXHLegacyTOC2 Object  
*/
		void RefreshL(CCSXHLegacyTOC2 *aTopic);
			
 /**
  @function ConstructL
  @since S60 3.2
  Perform the second phase construction of a LegacyTopicContainer object
  @param aRect the rectangle this view will be drawn to
  */      void ConstructL(const TRect& aRect);
        
        /**
        * @The Mopsupply object gives the skin context to 
        * @the scrollbar, so that the scrollbar is skinned
        * @according the skin parameters and look transparent
          */
        TTypeUid::Ptr MopSupplyObject(TTypeUid aId);

/** 
 * @function ~CCSXHLegacyTopicContainer
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */
      virtual ~CCSXHLegacyTopicContainer();		
		
    public: // New functions
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
        
	
    protected: // New functions
/** 
*  @function SetTextL
*  @since S60 3.2
*  Handles resource changes.
*  set topic's text to a rich text editor
*  @param aText help document
*/         
        void SetTextL(CRichText* aText);

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


    private: // new function
/**
 * Get rich text format from resource.
 * @function GetTextFormat
 * @since 3.2
 */
        void GetTextFormat();

/*
* Format the rich text help data
* @function FormatRichTextL
* @since 3.2
* @param aText instance of the CRichText object to format
*/
        void FormatRichTextL(CRichText& aText);

    private: // Functions from base classes
/** 
*  From CoeControl class
*  @function SizeChanged
*  @since S60 3.2
*  re-allocates and re-sizes components.
*/
       void SizeChanged();

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
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
                                    TEventCode aType);

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
        

    private: //data
        // to show help document
        CEikEdwin* iEdwin;
		
        // rich text format
        TCharFormat iCharFormat;
        TCharFormatMask iCharFormatMask;
		CAknsBasicBackgroundControlContext* iSkinContext; // skin data
		CRichText* iText; // skin data	
		CCSXHDocument& iDocument;	
		CCSXHLegacyTOC2* iTopic;
    };
// End of File
