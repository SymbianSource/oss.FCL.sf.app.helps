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
* Description:  CCSXHGenericContainer class definition
*
*/


#include "CSXHGenericContainer.h"
#include "CSXHGenericView.h"
#include "CSXHDocument.h"
#include "CSXHAppUi.h"
#include "CSXHHelpDataBase.h"
#include "CSXHGenericTOC1.h"
#include "CSXHGenericTOC2.h"

#ifdef FF_HELP_TUTORIAL_MERGE
#include "CSXHHtmlTOC2.h"
#endif

#include "CsHelp.hrh"
#include "csxhconstants.h"

#include "CSXHViewIDs.h"
#include <eikclbd.h>
#include <aknlists.h>
#include <featmgr.h>
#include <AknPriv.hrh>

const TInt KFindNameLength = 40;
const TInt KComponentCount = 2;     

#ifdef FF_HELP_TUTORIAL_MERGE
CCSXHGenericContainer* CCSXHGenericContainer::NewL(const TRect& aRect,CCSXHDocument& aDocument,CCSXHGenericView* aView)
	{
	CCSXHGenericContainer* self = CCSXHGenericContainer::NewLC(aRect,aDocument,aView);
    CleanupStack::Pop(self);
    return self;
	}
#else
CCSXHGenericContainer* CCSXHGenericContainer::NewL(const TRect& aRect,CCSXHDocument& aDocument)
    {
    CCSXHGenericContainer* self = CCSXHGenericContainer::NewLC(aRect,aDocument);
    CleanupStack::Pop(self);
    return self;
    }
#endif
    
#ifdef FF_HELP_TUTORIAL_MERGE
CCSXHGenericContainer* CCSXHGenericContainer::NewLC(const TRect& aRect,CCSXHDocument& aDocument,CCSXHGenericView* aView)
    {
    CCSXHGenericContainer* self = new(ELeave) CCSXHGenericContainer(aDocument,aView);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }
#else
CCSXHGenericContainer* CCSXHGenericContainer::NewLC(const TRect& aRect,CCSXHDocument& aDocument)
    {
    CCSXHGenericContainer* self = new(ELeave) CCSXHGenericContainer(aDocument);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }
#endif

#ifdef FF_HELP_TUTORIAL_MERGE
CCSXHGenericContainer::CCSXHGenericContainer(CCSXHDocument& aDocument,CCSXHGenericView* aView) : 
iDocument(aDocument),iView(aView)
#else
CCSXHGenericContainer::CCSXHGenericContainer(CCSXHDocument& aDocument) : iDocument(aDocument)
#endif
    {//No implementation required
    }
    
CCSXHGenericContainer::~CCSXHGenericContainer()
    {
    delete iListBox;
    delete iFindPane;
    }
    
void CCSXHGenericContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

    CreateAndPopulateListL();

    iListBox->SetListBoxObserver(this); 
    SetRect(aRect);
    ActivateL();    
    }
// --------------------------------------------------------------------------
// Does the following
// 1) Creates and populates the listbox
// 2) Highlights an item in the listbox
// 3) Check for MSK
// --------------------------------------------------------------------------
void CCSXHGenericContainer::CreateAndPopulateListL()
    {
    // First phase construction
    iListBox = new (ELeave) CAknSingleStyleListBox;
    iListBox->SetContainerWindowL(*this);

    iListBox->ConstructL(this, 0);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
                                                CEikScrollBarFrame::EOff,
                                                CEikScrollBarFrame::EAuto);
    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL(ETrue); 
              
    CTextListBoxModel* model = iListBox->Model();  // not taking ownership
    model->SetOwnershipType (ELbmOwnsItemArray);
    CDesCArray* testArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
    
    CCSXHGenericTOC1* displayTopic = STATIC_CAST(CCSXHGenericTOC1*,iDocument.GetDisplayTopic());
    
    displayTopic->FillChildDataL(testArray);
    
    //Find panel Begin
    CAknSearchField::TSearchFieldStyle flags(CAknSearchField::ESearch);
    iFindPane = CAknSearchField::NewL(*this, flags, 
                                       NULL,KFindNameLength);

    STATIC_CAST(CAknFilteredTextListBoxModel*, iListBox->Model())
        ->CreateFilterL(iListBox, iFindPane);
    
    STATIC_CAST(CAknFilteredTextListBoxModel*, iListBox->Model())
            ->Filter()->HandleOfferkeyEventL();  
	//Corrections to TSW Error:  KMAI-6WNACA with New UI CR: EVIA-6XAJ46            
    if (FeatureManager::FeatureSupported(KFeatureIdJapanese) &&
        (User::Language() == ELangJapanese))
        {        
        iFindPane->Editor().SetAknEditorInputMode(EAknEditorHiraganaKanjiInputMode);
        }
    else
    	{
    	iFindPane->Editor().SetAknEditorCurrentInputMode(EAknEditorTextInputMode);
    	}

    HightLightItemL();    
    }
    
void CCSXHGenericContainer::HandleListBoxEventL(CEikListBox* /*aListBox*/, TListBoxEvent aListBoxEvent)
    {
    switch ( aListBoxEvent )
        {
        case MEikListBoxObserver::EEventEnterKeyPressed:
        case MEikListBoxObserver::EEventItemSingleClicked:
#ifdef FF_HELP_TUTORIAL_MERGE
  	        if(IsShowTutorialItemSelectedL())
	        	{
                CCSXHAppUi::GetInstance()->CallTutorialAiwProviderL();
	        	}
	        else

#endif	        
	        	{
	        	iDocument.SendMessageToAppUiL( ECSXHOpenItemPropagated );
	        	}	        	   

            break;
        default:
            break;// do nothing with the rest of the events
        }       
    }
    
void CCSXHGenericContainer::SizeChanged()
    {
    iListBox->SetRect(Rect());
    AknFind::HandleFixedFindSizeChanged(this, iListBox, iFindPane);
    }
    
void CCSXHGenericContainer::FocusChanged(TDrawNow aDrawNow)
    {
    if (iListBox)
        {
        iListBox->SetFocus(IsFocused(), aDrawNow);
        }
    if ( iFindPane )
        {
        iFindPane->SetFocus( IsFocused(), aDrawNow );
        }
    }
        
TInt CCSXHGenericContainer::CountComponentControls() const
    {
    return KComponentCount;
    }
    
CCoeControl* CCSXHGenericContainer::ComponentControl(TInt aIndex) const
    {
    switch (aIndex)
        {
        case 0:
            return iListBox;
        case 1:
            return iFindPane;
        default:
            return NULL;
        }
    }
        
void CCSXHGenericContainer::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
    gc.Clear(aRect);
    }
    
TKeyResponse CCSXHGenericContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    TKeyResponse result(EKeyWasNotConsumed);
    
    TBool needRefresh;
    result = AknFind::HandleFindOfferKeyEventL(aKeyEvent, aType,
                        this, iListBox, iFindPane, EFalse, needRefresh);
    CheckForMSK();              
    if (result == EKeyWasNotConsumed)
        {
        result = iListBox->OfferKeyEventL (aKeyEvent, aType);
        }
    return result; 
    }
    
void CCSXHGenericContainer::HandleResourceChange(TInt aType)
    {
    CCSXHAppUi::GetInstance()->PropagateResourceChange(aType);

    if (KAknSplitInputEnabled == aType || KAknSplitInputDisabled == aType)
        {
    // Bug fixing, SetFocus can't be called with these aType, it will lead to infinite loop in fep.
        return;
        }
    
    if ( IsFocused() )
        {
        SetFocus( IsFocused(), EDrawNow );
        }

    }
    
void CCSXHGenericContainer::HandleResourceChangeImpl(TInt aType)
    {
    if (aType == KEikDynamicLayoutVariantSwitch)
        {
        iListBox->HandleResourceChange(aType);
        TRect mainRect; 
        SizeChanged();
        TRect statusPaneRect;
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,mainRect);
        SetRect(mainRect);
        DrawNow();
        }
    else
        {
        CCoeControl::HandleResourceChange(aType);   
        }
    }
// --------------------------------------------------------------------------
// Sets the next topic to be displayed based on the selected item
// --------------------------------------------------------------------------    
void CCSXHGenericContainer::SetDisplayTopicL()
    {
    TInt index = iListBox->CurrentItemIndex();
    if(KErrNotFound == index)
        return;
        
    TBuf<KMaxFileName> selectedTOC2 = iListBox->Model()->ItemText(index);
    selectedTOC2 = selectedTOC2.Mid(1);
    
    CCSXHGenericTOC1* displayTopic = STATIC_CAST(CCSXHGenericTOC1*,iDocument.GetDisplayTopic());
    if( displayTopic->GetChildL(selectedTOC2) )
        {
    	iDocument.SetDisplayAndPrevTopic(displayTopic->GetChildL(selectedTOC2));
        }
    }

TInt CCSXHGenericContainer::GetNumberOfListItems()
    {
    return iListBox->Model()->NumberOfItems();
    }
    
void CCSXHGenericContainer::SetCurrentView(CCSXHGenericView* aView)
    {
    iView = aView;
    }
    
void CCSXHGenericContainer::HightLightItemL()    
    {
    CCSXHHelpContentBase* prevTopic = iDocument.GetPrevTopic();    
    if(!prevTopic)
        return;
        
    TInt visibleItemcount = GetNumberOfListItems();
    for (TInt index = 0; index < visibleItemcount ; ++index )
        {
        TBuf<KMaxFileName> txt = iListBox->Model()->ItemText(index);
        txt = txt.Mid(1);//remove the '/t'
        if(txt.Compare(prevTopic->GetName()) == 0)
            {
            if(iListBox->CurrentItemIndex() != index)
                iListBox->SetCurrentItemIndex(index);
            return;
            }
        }
    }
    
void CCSXHGenericContainer::CheckForMSK()   
    {
#ifndef __SERIES60_30__ 
    if(AknLayoutUtils::MSKEnabled())
        {   
        if(iListBox->Model()->NumberOfItems())
            {
            iView->SetMiddleSoftKey(ETrue);         
            }
        else
            {
            iView->SetMiddleSoftKey(EFalse);                
            }
        }
#endif      
    }
      
#ifdef FF_HELP_TUTORIAL_MERGE
// --------------------------------------------------------------------------
// CCSXHAppUi::IsShowTutorialItemSelectedL
// If the selected item is a tutorial item.
// --------------------------------------------------------------------------
TBool CCSXHGenericContainer::IsShowTutorialItemSelectedL()
    {
    TBool bShowTutorial = EFalse;
    //TODO:
    // if keyword page supported tutorial, the comments should be opened
    if( 0 != GetNumberOfListItems /*&&  KCSXHKywdToc2ViewID != iView->Id() */)
        {
        TInt index = iListBox->CurrentItemIndex();
        if(KErrNotFound == index)
            return bShowTutorial;
        //Get Next View
        TBuf<KMaxFileName> selectedTOC2 = iListBox->Model()->ItemText(index);

        selectedTOC2 = selectedTOC2.Mid(1);
        CCSXHGenericTOC1* displayTopic = STATIC_CAST(CCSXHGenericTOC1*,
                                               iDocument.GetDisplayTopic());
        TUid NextViewId = displayTopic->GetChildL(selectedTOC2)->GetViewID();

        //Check Tutorial is present for Application topic [TOC2]
        if(NextViewId == KCSXHHtmlTopicViewID)
            {
            CCSXHHtmlTOC2* HTMLTOC2Ptr = STATIC_CAST(CCSXHHtmlTOC2*,
                                      displayTopic->GetChildL(selectedTOC2));
            TFileName filename;
            HTMLTOC2Ptr->GetHtmlUrlL( filename );
            if( KErrNotFound != filename.Find( KTutorialFileExtn ))
                {
                bShowTutorial = ETrue;
                TCoeHelpContext helpContext;
                HTMLTOC2Ptr->GetHelpContext(helpContext);
                CCSXHAppUi::GetInstance()->SetHelpContext( helpContext );
                }
            }
        }
    return bShowTutorial;
    }

#endif // FF_HELP_TUTORIAL_MERGE


void CCSXHGenericContainer::MakeVisible(TBool aVisible)
    {
    CCoeControl::MakeVisible(aVisible);
    iListBox->ScrollBarFrame()->ComponentControl(0)->MakeVisible(aVisible);
    }
