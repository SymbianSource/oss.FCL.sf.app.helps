/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  source file.
*                Write all source code here, all the implementations
*
*/


#include <eikmenup.h>
#include "AiwHelpProvider.h"
#include <AiwMenu.h>
#include <barsread.h>
#include <eikenv.h>
#include <AiwHelpProviderPlugin.rsg>
#include <ImplementationProxy.h>
#include "AiwHelpProvider.rh"
#include <AiwServiceIfMenu.h>
#include <AiwCommon.h>
#include <hlplch.h>
#include <coemain.h>
#include <avkon.hrh>


_LIT(KResFileName, "\\resource\\apps\\AiwHelpProviderPlugin.rsc");

#define KImplementationId 0x10207465



/******************************************************************
===================================================================
						NewL of the Provider
===================================================================
*******************************************************************/						
CAiwHelpProvider* CAiwHelpProvider::NewL()
    {
    return new (ELeave) CAiwHelpProvider();
    }


    
/******************************************************************
===================================================================
						Constructor of the Provider
===================================================================
*******************************************************************/
CAiwHelpProvider::CAiwHelpProvider()
    {
    }



/******************************************************************
===================================================================
					Destructor of the Provider
===================================================================
*******************************************************************/
CAiwHelpProvider::~CAiwHelpProvider()
    {
    }
    
    
    
/**
        * Called by the Handler framework to initialise provider with
        * necessary information from Handler.
        * @param aFrameworkCallback Framework provided callback for
        *        provider to send events to framework
        * @param aInterest List of criteria items which invoked the provider.
        * @exception Symbian OS error code
        */
void CAiwHelpProvider::InitialiseL(MAiwNotifyCallback& /*aFrameworkCallback*/,
    const RCriteriaArray& /*aInterest*/)
    {
    
    }


/**
        * Executes generic service commands included in criteria.
        * @param aCmdId Command to be executed
        * @param aInParamList Input parameters, can be an empty list
        * @param aOutParamList Output parameters, can be an empty list
        * @param aCmdOptions Options for the command, see KAiwOpt* constants in AiwCommon.hrh.
        * @param aCallback callback for asynchronous command handling, parameter checking, etc.
        * @see enum TAiwServiceCmdOptions in AiwCommon.hrh
        * @exception KErrArgument if callback is missing when required.
        * @exception KErrNotSupported if no provider support service
        */
void CAiwHelpProvider::HandleServiceCmdL(const TInt& /*aCmdId*/,
    const CAiwGenericParamList& /*aInParamList*/,
    CAiwGenericParamList& /*aOutParamList*/,
    TUint aCmdOptions,
    const MAiwNotifyCallback* /*aCallback*/)
    {
 	// Cancel bit must always be checked. 
    if(aCmdOptions & KAiwOptCancel)
        {
        return;
        }
    }
/**
        * Initialises menu pane by adding provider specific menu items.
        * The AIW Framework gives the parameters to be used in addition.
        * @param aMenuPane Menu pane handle
        * @param aIndex position of item where to add menu items.
        * @param aCascadeId ID of cascade menu item.
        * @param aInParamList input parameter list for provider's parameters checking
        */
        
void CAiwHelpProvider::InitializeMenuPaneL(CAiwMenuPane& aMenuPane,
    TInt aIndex,
    TInt /*aCascadeId*/,
    const CAiwGenericParamList& /*aInParamList*/)
    {
    TFileName resFile(KResFileName);
    TFileName dllName;
    Dll::FileName(dllName);
    TBuf<2> drive = dllName.Left(2);
    resFile.Insert(0, drive);
    aMenuPane.AddMenuItemsL(resFile, R_AIWHELPPROVIDER_MENU, KAiwCmdHelp, aIndex);
    }

/**
        * Handle a menu command invoked by the Handler.
        * @param aMenuCmdId Command ID for the menu command,
        *        defined by the provider when adding the menu commands.
        * @param aInParamList Input parameters, could be empty list
        * @param aOutParamList Output parameters, could be empty list
        * @param aCmdOptions Options for the command, see KAiwCmdOpt* constants.
        * @param aCallback callback if asynchronous command handling is wanted by consumer.
        *    The provider may or may not support this, leaves with KErrNotSupported, it not.
        */
void CAiwHelpProvider::HandleMenuCmdL(TInt aMenuCmdId,
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList& /*aOutParamList*/,
    TUint aCmdOptions,
    const MAiwNotifyCallback* /*aCallback*/)
    {
    // Cancel bit must always be checked.
    if(aCmdOptions & KAiwOptCancel)
        {
        return;
        }
           
    switch (aMenuCmdId)
	    {
	    case EAiwShowHelpCmd:
	    case EAknCmdHelp:
		    {
		    TCoeHelpContext helpContext;
		    TInt index;
		        
		    // Get the UID of the Help context i.e. the UID of the CS Help file containing 
		    // the topic information.

		    index = 0;
		    const TAiwGenericParam* uidParam = aInParamList.FindFirst(index, EGenericParamHelpItem, 
		        EVariantTypeTUid);

		    if(index >= 0)
		        {
		        // Get the data.
		        const TAiwVariant& val = uidParam->Value();
		        helpContext.iMajor = val.AsTUid();
		        }
		    else
		        {
		        // UID param not found. Error handling could be added here.
		        }            
		        
		    // Get the context of Help i.e. the name of the help context. This is the literal 
		    // descriptor generated from a context string by the CSHelp compiler.          

		    index = 0;
		    const TAiwGenericParam* contextParam = aInParamList.FindFirst(index, EGenericParamHelpItem, 
		        EVariantTypeDesC);

		    if(index >= 0)
		        {
		        // Get the data.
		        const TAiwVariant& val = contextParam->Value();
		        helpContext.iContext = val.AsDes();
		        }
		    else
		        {
		        // Context param not found. Error handling could be added here.
		        }             
		                            
		    CArrayFix<TCoeHelpContext>* contextList = new (ELeave) CArrayFixFlat<TCoeHelpContext>(1);
		    contextList->AppendL(helpContext);
		    
		    //Get the Window server session
		    RWsSession wsSession;
		    User::LeaveIfError(wsSession.Connect());
		    CleanupClosePushL(wsSession);
		    HlpLauncher::LaunchHelpApplicationL(wsSession,contextList);
		    CleanupStack::PopAndDestroy();
		    break;
		    }
        default:
            break;
        }
    }

//
// Rest of the file is for ECom initialization. 
//

// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] =
    {
        IMPLEMENTATION_PROXY_ENTRY(KImplementationId, CAiwHelpProvider::NewL)
    };

// ---------------------------------------------------------
// Exported proxy for instantiation method resolution
// ---------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

