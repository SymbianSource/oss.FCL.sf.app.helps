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
* Description:  This is a utility class that provides functionality
*				 for parsing command line arguments passed into the
*				 Cshelp application when launched by another application.		
*
*/


#ifndef CSHELPCMDLINEPARSER_H
#define CSHELPCMDLINEPARSER_H


// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class TCoeHelpContext;

// CLASS DECLARATION
/**
*  CCsHlpCmdLineParser class.
*  to convert a message - that is passed from other app - to contexts
*  @lib euser.lib
*  @since 3.2
*/
class CCsHlpCmdLineParser : public CBase
    {
    public:

/** 
*  @function CCsHlpCmdLineParser
*  @since S60 3.2
*  Perform the first phase of two phase construction
*/        CCsHlpCmdLineParser();

/** 
*  @function ConstructL
*  @since S60 3.2
*  Perform the second phase construction of a CCsHlpCmdLineParser object
*/        void ConstructL(const TDesC8& aCmdLine);
    
/** 
*  @function ~CCsHlpCmdLineParser
*  @since S60 3.2
*  Destroy the object and release all memory objects
*/
        ~CCsHlpCmdLineParser();

    public:
/** 
*  Get contexts array
*  @function ContextListL
*  @since S60 3.2
*  @return help context array
*/        
        CArrayFix<TCoeHelpContext>* ContextListL();

    private:
/** 
*  Get the help Context object
*  @function GetContext
*  @since S60 3.2
*  @param aContext - Help context object
*/         
        void GetContext(TCoeHelpContext& aContext);
    
    private:
        // Parsed data to help context array.
        CBufFlat* iCmdLine;

        // Position of iCmdLine
        TInt iPos;
    };

#endif // CSHELPCMDLINEPARSER_H

// End of File
