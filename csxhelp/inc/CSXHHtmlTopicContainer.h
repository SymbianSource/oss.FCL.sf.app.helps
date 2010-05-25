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
* Description:  CCSXHHtmlTopicContainer class declaration
*
*/


#ifndef INC_CCSXHHTMLTOPICCONTAINER_H__
#define INC_CCSXHHTMLTOPICCONTAINER_H__
//If I use INC_CCSXHHTMLTOPICCONTAINER_H__, then CodeScanner fails 
//but if I use INC_CCSXH_HTML_TOPIC_CONTAINER_H__ code scanner 
//reports an error but produces results for other files

#include <coecntrl.h>
#include <brctllinkresolver.h>
#include <brctlinterface.h>
//RUNTIME
#include <featurecontrol.h>

class CBrCtlInterface;
class CCSXHDocument;
class CCSXHHtmlTOC2;
class CCSXHHtmlTopicView;
class AppLauncherForCSXH;
class CCSXHHelpContentBase;

typedef CBrCtlInterface* (*FuncPtr_CreateBrowserControlL) (CCoeControl* aParent, TRect aRect,
    TUint aBrCtlCapabilities, TUint aCommandIdBase = TBrCtlDefs::ECommandIdBase,
    MBrCtlSoftkeysObserver* aBrCtlSoftkeysObserver = NULL,
    MBrCtlLinkResolver* aBrCtlLinkResolver = NULL,
    MBrCtlSpecialLoadObserver* aBrCtlSpecialLoadObserver = NULL,
    MBrCtlLayoutObserver* aBrCtlLayoutObserver = NULL,
    MBrCtlDialogsProvider* aBrCtlDialogsProvider = NULL,
    MBrCtlWindowObserver* aBrCtlWindowObserver = NULL,
	MBrCtlDownloadObserver* aBrCtlDownloadObserver = NULL );
/** 
* @class CCSXHHtmlTopicContainer
* This class provides Browser functionality
*/

class CCSXHHtmlTopicContainer : public CCoeControl, public MBrCtlLinkResolver, 
								public MBrCtlLinkContent, public MBrCtlLoadEventObserver,
								public MBrCtlSoftkeysObserver,public MBrCtlLayoutObserver
    {
public:

/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHHtmlTopicContainer 
*  and return a pointer to the created object
*  @param aRect - TRect Object
*		  aDocument - Document Object 
*		  aView		- HtmlTopicView Pointer
*  @return a CCSXHHtmlTopicContainer Pointer 
*/
    static CCSXHHtmlTopicContainer* NewL(const TRect& aRect,CCSXHDocument 
    &aDocument,CCSXHHtmlTopicView *aView);

/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHHtmlTopicContainer 
*  and return a pointer to the created object using two phase constuction
*  @param aRect - TRect Object
*		  aDocument - Document Object 
*		  aView		- HtmlTopicView Pointer
*  @return a CCSXHHtmlTopicContainer Pointer 
*/
    static CCSXHHtmlTopicContainer* NewLC(const TRect& aRect,CCSXHDocument
     &aDocument,CCSXHHtmlTopicView *aView);

/** 
*  @function CCSXHHtmlTopicContainer
*  @since S60 3.2
*  Construct a CCSXHHtmlTopicContainer 
*  and return a pointer to the created object using two phase constuction
*  @param aRect - TRect Object
*		  aDocument - Document Object 
*		  aView		- HtmlTopicView Pointer
*  @return a CCSXHHtmlTopicContainer Pointer 
*/
	CCSXHHtmlTopicContainer(CCSXHDocument &aDocument, CCSXHHtmlTopicView *aView);
/** 
 * @function ~CCSXHHtmlTopicContainer
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */
  ~CCSXHHtmlTopicContainer();
  
/** 
 * @function SetAndLoadInitialTopicL
 * @since S60 3.2
 * Destroy the object and release all memory objects
 * @param aTopic - Initial Topic to be displayed 
 */  
 void SetAndLoadInitialTopicL(CCSXHHtmlTOC2* aTopic);
     
public:  
/**
* From MBrCtlSoftkeysObserver class
*  Browser plug-in calls this method requests to update a softkey 
*  @function UpdateSoftkeyL    
*  @since S60 3.2
*  @Param aKeySoftkey	- Update the left softkey or the right softkey  
*		 aLabel			- The label associated with the softkey update  
*		 aCommandId		- The command to use if the softkey is selected by the user  
*		 aBrCtlSoftkeyChangeReason - The reason for the softkey change  
*/
void UpdateSoftkeyL(TBrCtlKeySoftkey aKeySoftkey,
                                    const TDesC& aLabel,
                                    TUint32 aCommandId,
                                    TBrCtlSoftkeyChangeReason aBrCtlSoftkeyChangeReason);

/**
* From MBrCtlLayoutObserver Class
*  Browser plug-in calls this method to Update the position of vertical scrollbar
*  @function UpdateBrowserVScrollBarL
*  @since S60 3.2    
*  @Param  aDocumentHeight - The total height of the markup page  
* 		   aDisplayHeight  - The height of the display  
* 		   aDisplayPosY    - The current Y position  
*/                                    
 void UpdateBrowserVScrollBarL(TInt aDocumentHeight, 
                                              TInt aDisplayHeight,
                                              TInt aDisplayPosY ) ;
                                              
/**
* From MBrCtlLayoutObserver Class
*  Browser plug-in calls this method to Update the position of horizontal scrollbar
*  @function UpdateBrowserHScrollBarL
*  @since S60 3.2    
*  @Param  aDocumentWidth - The total width of the markup page  
* 		   aDisplayWidth  - The width of the display  
* 		   aDisplayPosX    - The current X position  
*/                                              
void UpdateBrowserHScrollBarL(TInt aDocumentWidth, 
                                              TInt aDisplayWidth,
                                              TInt aDisplayPosX );
                                              
/**
* From MBrCtlLayoutObserver Class
*  Browser plug-in calls this method to Inform the layout of the page: right to left or left to right
*  @function NotifyLayoutChange    
*  @since S60 3.2
*  @Param  aNewLayout - RTL or LTR 
*/                                              
void NotifyLayoutChange( TBrCtlLayout aNewLayout );
  
/**
* From MBrCtlLayoutObserver Class
*  Browser plug-in calls this method to Inform the layout of the page: right to left or left to right
*  @function UpdateTitleL
*  @since S60 3.2    
*  @Param  aTitle - Title of the page 
*/        
void UpdateTitleL( const TDesC& aTitle );                                   

public:  // from CCoeControl
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
*  @function OfferKeyEventL
*  @since S60 3.2
*  Progress Key Event
*  @param aKeyEvent the key event
*  @param aType EEventKey | EEventKeyUp | EEventKeyDown
*  @return EKeyWasConsumed if key event is processed,
*         otherwise EKeyWasNotConsumed
*/
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

/** 
*  From MBrCtlLoadEventObserver class
*  @function HandleBrowserLoadEventL
*  @since S60 3.2
*  A load events notification
*  @param aLoadEvent - The load event  
*  		  aSize  	 - Size depends on the event  
*  		  aTransactionId  - The transaction id of the transaction that had this event  
*/	
	void HandleBrowserLoadEventL(TBrCtlDefs::TBrCtlLoadEvent aLoadEvent, 
								TUint aSize, TUint16 aTransactionId);

/** 
*  @function RefreshL
*  @since S60 3.2
*  Used to Refresh the Content
*  @param aTopic - CCSXHHtmlTOC2 Object  
*/	
	void RefreshL(CCSXHHtmlTOC2 *aTopic);
	
/** 
*  @function HandleBackKeyL
*  Handle Back Key
*  @since S60 3.2
*  @return ETrue, if the key is handled in the container, EFalse otherwise 
*/ 
	TBool HandleBackKeyL();

/** 
*  @function IsPrevHtmlPageAvblL
*  Check for Existance of the page in the Browser history stack
*  @since S60 3.2
*  @return ETrue, if page is available in Browser History Stack, EFalse otherwise 
*/ 
	TBool IsPrevHtmlPageAvblL();

/** 
*  @function SetSelectedFontSizeL
*  Sets BrowserFontsize
*  @since S60 3.2
*  @param aValue - Fontsize Value
*/	
	void SetSelectedFontSizeL(TInt aValue);
	
/** 
*  @function SetSelectedFontSizeL
*  Get Current Settings Value
*  @since S60 3.2
*  @param aValue - Any Setting 
*  @return Current Value used by Browser
*/	
	TInt GetCurrentValueL(TInt aSetting);

/** 
*  @function HandleCommandBackL
*  @since S60 3.2
*  Handles resource changes.
*  pass OpenLink command to the browser HandlecommandL 
*/
	void HandleCommandOpenHyperLinkL();

/** 
*  From CoeControl class
*  @function Draw
*  @since S60 3.2
*  Draw() for drawing the Background
*/
    void Draw(const TRect& aRect) const;
  
  	void ShowIndexHtmlPageL() const;
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
*  @function HandleCommandBackL
*  @since S60 3.2
*  Handles resource changes.
*  pass Back command to the browser HandlecommandL 
*/   	
	void HandleCommandBackL();
	
/**
* Browser plug-in calls this method when embedded link is found. Used with ECapabilityClientResolveEmbeddedURL
* @since 3.2
* @param aEmbeddedUrl The url of the embedded content
* @param aCurrentUrl The url of the current page
* @param aLoadContentType The expected type of the embedded content
* @param aEmbeddedLinkContent a callback interface to return the embedded content
* @return EFalse, browser will resolve the link
*/
    TBool ResolveEmbeddedLinkL(const TDesC& aEmbeddedUrl,
    		                               const TDesC& aCurrentUrl,
        	                               TBrCtlLoadContentType aLoadContentType, 
        	                               MBrCtlLinkContent& aEmbeddedLinkContent); 
		
/**
* Browser plug-in calls this method when the user requests to load content via selecting a link, or any other way. Used with ECapabilityClientNotifyURL 
* @since 3.2
* @param aUrl The requested url
* @param aCurrentUrl The url of the current page
* @param aBrCtlLinkContent a callback interface to return the embedded content
* @return EFalse, browser will resolve the link
*/
        TBool ResolveLinkL(const TDesC& aUrl, const TDesC& aCurrentUrl,
                                   MBrCtlLinkContent& aBrCtlLinkContent);

/**
* @function CancelFetchL
* Cancel all outstanding resolving operations
*/
        void CancelFetchL();
/**
* @function CancelAll
* Cancel all outstanding resolving operations
* @since 3.2
*/
        void CancelAll();
        
        
/**
* @function HandleResolveComplete
* @since 3.2	
* Resolver calls this method when content is resolved.
* @param aContentType The content type of the response
* @param aCharset The charset of the response. May be empty in case of image
* @param aContentBuf content data. Ownership is not transfered
* @return void
*/
        void HandleResolveComplete(const TDesC& aContentType,
                                           const TDesC& aCharset,
                                           const HBufC8* aContentBuf);

/**
* @function HandleResolveError
* @since 3.2
* This method is called if there is some error while resolving the content
* @param aError system wide error code.
*/
        void HandleResolveError(TInt aError);
/** 
*  @function CheckForMSK
*  @since S60 3.2
*  Checks Msk flag is Enabled or not, If Enabled, Sets MSK Label
*/      
		void CheckForMSK();
/** 
*  @function IsPrevPageAvbl
*  Check wether focus is on Hyperlink or not
*  @since S60 3.2
*  @return ETrue, if Focus is on Hyperlink , EFalse otherwise 
*/		
		TBool IsHyperLinkFocused();	
/** 
*  @function IsPrevPageAvbl
*  Check for Existance of the page
*  @since S60 3.2
*  @return ETrue, if page is available , EFalse otherwise 
*/		
	 	TBool IsPrevPageAvbl();
	 	
/** 
*  @function ClearHistoryL
*  Clear Browser history stack
*  @since S60 3.2
*/	 	
 		void  ClearHistoryL(); 
 		
/** 
*  @function GetCurrActiveObject
*  Get the Current Active Object 
*  @since S60 3.2
*  @return CCSXHHelpContentBase Pointer, 
*/ 		
 		CCSXHHelpContentBase* GetCurrActiveObject();

private:
 		
/**
* The state type of browser content.
* @see SetContentLoadState() and NeedRefresh.
*/
    enum TContentLoadState
        {
        ENoContent,
        EContentLoading,
        EContentLoadFinished
        };

/**
*  @function ConstructL
*  @since S60 3.2
*  @Perform the second phase construction of a HtmlTopicContainer object
*  @param aRect the rectangle this view will be drawn to
 */
    void ConstructL(const TRect& aRect);

/** 
*  @function LoadHtmlL
*  loads HTML page in the Browser
*  @since S60 3.2
*/
	void LoadHtmlL();

/** 
*  @function CheckForExternalLinkL
*  Check for external Hyperlink, if yes launch browserNG
*  @since S60 3.2
*  @return ETrue, if external hyperlink available, EFalse otherwise 
*/	
	TBool CheckForExternalLinkL(const TDesC& aUrl);
	
/**
* @function SetContentLoadingState
* Set the state of content loading and refresh the UI id needed
*/
    void SetContentLoadState( const TContentLoadState aLoadState );
        
/**
* @function NeedRefresh
* Judge whether we need to refresh the browser control
*/
    TBool NeedRefresh( const TContentLoadState aNewLoadState ) const;
	
private:
	CBrCtlInterface* iBrCtrl;    
	CCSXHHtmlTOC2* iTopic;
	CCSXHDocument& iDocument;
    CCSXHHelpContentBase *iNewTopic;
    
    //Check the impln comments in HandleBrowserLoadEventL function
    TBuf<KMaxFileName> iUrlNoAnchors; 
    
    CCSXHHtmlTopicView* iView;    
    AppLauncherForCSXH* iAppLauncher;
    RLibrary iBrLibrary;
    RFeatureControl iFeatureControl;
    TBool iLoadHtml;    
    TInt32 iPrevPageCount;
    TBool iBack;
    
    TContentLoadState iContentLoading;
	};


#endif 
