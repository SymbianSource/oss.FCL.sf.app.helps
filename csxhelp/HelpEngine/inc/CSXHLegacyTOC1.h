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
* Description:  CCSXHLegacyTOC1 class declaration
*
*/


#ifndef INC_CSXHLEGACYTOC1_H_HEADER_INCLUDED_BC050894
#define INC_CSXHLEGACYTOC1_H_HEADER_INCLUDED_BC050894

#include "CSXHGenericTOC1.h"

class CCSXHGenericContentParser;
class CCSXHHelpDataBase;
/** 
* @class CCSXHLegacyTOC1
* This class has the information of Legacy TOC1 
*/
class CCSXHLegacyTOC1 : public CCSXHGenericTOC1
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHLegacyTOC1 
*  and return a pointer to the created object
*  @param aName 	- Name of the TOC1
*  @return a CCSXHLegacyTOC1 Pointer 
*/    
    static CCSXHLegacyTOC1* NewL(const TDesC& aName);
    
/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHLegacyTOC1 
*  and return a pointer to the created object using two phase contruction
*  @param aName 	- Name of the TOC1
*  @return a CCSXHLegacyTOC1 Pointer 
*/      
    static CCSXHLegacyTOC1* NewLC(const TDesC& aName);
     
/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/ 
    TUid GetViewID() const;
    
/** 
*  @function GetContextTopic
*  @since S60 3.2
*  Get toc2 Object 
*  @return CCSXHHelpContentBase (Leagcytoc2) class Pointer
*/  
    CCSXHHelpContentBase* GetContextTopic(const TDesC &aContextName);

    private: 	
/** 
*  @function CCSXHLegacyTOC1
*  @since S60 3.2
*  Construct a CCSXHLegacyTOC1 
*/    
    	CCSXHLegacyTOC1(const TDesC& aName);

/** 
*  @function InitChildList
*  @since S60 3.2
*  Allocates Memory in order to store Objects
*/    
    	void InitChildList();
    
    };



#endif /* INC_CSXHLEGACYTOC1_H_HEADER_INCLUDED_BC050894 */
