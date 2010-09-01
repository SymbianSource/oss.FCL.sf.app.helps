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
* Description:  header file.
*
*/



#ifndef _AIW_HELP_PROVIDER_H
#define _AIW_HELP_PROVIDER_H

#include <AiwServiceIfMenu.h>

class CAiwHelpProvider : public CAiwServiceIfMenu 
    {
    public: 
        static CAiwHelpProvider* NewL();
        ~CAiwHelpProvider();

    public: 
        virtual void InitialiseL(
            MAiwNotifyCallback& aFrameworkCallback,
            const RCriteriaArray& aInterest);

        virtual void HandleServiceCmdL(
            const TInt& aCmdId,
            const CAiwGenericParamList& aInParamList,
            CAiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MAiwNotifyCallback* aCallback = NULL);

        virtual void InitializeMenuPaneL(
            CAiwMenuPane& aMenuPane,
            TInt aIndex,
            TInt aCascadeId,
            const CAiwGenericParamList& aInParamList);

        virtual void HandleMenuCmdL(
            TInt aMenuCmdId,
            const CAiwGenericParamList& aInParamList,
            CAiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MAiwNotifyCallback* aCallback = NULL);

    private:
        CAiwHelpProvider();
    };
#endif
