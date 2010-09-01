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
* Description:  CCSXHApplication class declaration
*
*/



#ifndef INC_CSXHAPPLICATION_H__
#define INC_CSXHAPPLICATION_H__

#include <aknapp.h>


/*! 
  @class CCSXHApplication
  
  An instance of CCSXHApplication is the application part of the AVKON
  application framework for the Csxhelp application
  */
class CCSXHApplication : public CAknApplication
    {
public:  // from CAknApplication

/** 
  @function AppDllUid
  @since S60 3.2
  
  The application DLL UID value
  @return the UID of this Application/Dll
  */
    TUid AppDllUid() const;

protected: // from CAknApplication
/** 
  @function CreateDocumentL
  @since S60 3.2
  
  Create a CApaDocument object and return a pointer to it
  @return a pointer to the created document
  */
    CApaDocument* CreateDocumentL();
    };

#endif // INC_CSXHAPPLICATION_H__
