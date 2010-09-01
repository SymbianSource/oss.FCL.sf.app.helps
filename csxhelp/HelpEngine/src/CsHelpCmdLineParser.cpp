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
* Description: 
*     This is a utility class that provides functionality
*     for parsing command line arguments passed into the
*     Cshelp application when launched by another application.
*
*
*/


// INCLUDE FILES
#include <hlplch.h>

#include "CsHelpCmdLineParser.h"
const TInt KArrSize = 2;
// ================= MEMBER FUNCTIONS =======================

// C++ constructor
CCsHlpCmdLineParser::CCsHlpCmdLineParser()
: CBase()
    {
    }


// ---------------------------------------------------------
// CCsHlpCmdLineParser::ConstructL(...)
// (other items were commented in a header).
// ---------------------------------------------------------
void CCsHlpCmdLineParser::ConstructL(const TDesC8& aCmdLine)
    {
    iCmdLine = CBufFlat::NewL(aCmdLine.Size());
    iCmdLine->InsertL(0, aCmdLine.Ptr(), aCmdLine.Size());
    }


// ---------------------------------------------------------
// CCsHlpCmdLineParser::~CCsHlpCmdLineParser()
// ---------------------------------------------------------
CCsHlpCmdLineParser::~CCsHlpCmdLineParser()
    {
    delete iCmdLine;
    }

// ---------------------------------------------------------
// CCsHlpCmdLineParser::ContextListL()
// (other items were commented in a header).
// ---------------------------------------------------------
CArrayFix<TCoeHelpContext>* CCsHlpCmdLineParser::ContextListL()
    {
    CArrayFixFlat<TCoeHelpContext>* array =
                                  new(ELeave)CArrayFixFlat<TCoeHelpContext>(KArrSize);
    CleanupStack::PushL(array);
    TInt count(0);

    iCmdLine->Read(0, &count, sizeof(TInt));
    iPos += sizeof(TInt);
    for (TInt i(0); i < count; i++)
        {
        TCoeHelpContext context;
        GetContext(context);
        array->AppendL(context);
        }
    CleanupStack::Pop(array); // array
    return array;
    }

// ---------------------------------------------------------
// CCsHlpCmdLineParser::GetContext(...)
// (other items were commented in a header).
// ---------------------------------------------------------
void CCsHlpCmdLineParser::GetContext(TCoeHelpContext& aContext)
    {
    TInt length(0);
    iCmdLine->Read(iPos, &length, sizeof(TInt));
    iPos += sizeof(TInt);
    TPtr8 ptr((TText8*)aContext.iContext.Ptr(), length);
    iCmdLine->Read(iPos, ptr, length);
    aContext.iContext.SetLength(length/KArrSize);
    iPos += length;

    iCmdLine->Read(iPos, &aContext.iMajor, sizeof(TUid));

    iPos += sizeof(TUid);
    }

// End of file
