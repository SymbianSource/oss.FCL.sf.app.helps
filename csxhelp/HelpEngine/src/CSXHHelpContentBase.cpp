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
* Description:  CCSXHHelpContentBase class definition
*
*/

#include "CSXHHelpContentBase.h"

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHelpContentBase::~CCSXHHelpContentBase()
    {
    delete iName;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
EXPORT_C  TPtr CCSXHHelpContentBase::GetName() const
    {
    return iName->Des();
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHelpContentBase::CCSXHHelpContentBase(const TDesC& name) : iName(name.Alloc())
    {//No implementation required
    }


