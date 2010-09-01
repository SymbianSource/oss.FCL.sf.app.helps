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
* Description:  CCSXHLegacyTOC2 class declaration
*
*/


#ifndef INC_CSXHLEGACYTOC2_H_HEADER_INCLUDED_BC0535B9
#define INC_CSXHLEGACYTOC2_H_HEADER_INCLUDED_BC0535B9

#include "CSXHGenericTOC2.h"

class CCSXHGenericTOC1;
class CCSXHLegacyTOC1;
/** 
* @class CCSXHLegacyTOC2
* This class has the information of Legacy TOC2 Info
*/
class CCSXHLegacyTOC2 : public CCSXHGenericTOC2
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHLegacyTOC2 
*  and return a pointer to the created object
*  @param aParent   - TOC1 Object
*		  aName 	- Name of the TOC2 
*		  aKey  	- ETrue for KeywordTopic, EFalse otherwise
*  @return a CCSXHLegacyTOC2 Pointer 
*/ 
    static CCSXHLegacyTOC2* NewL(CCSXHGenericTOC1 *aParent,const TDesC&
    aName,TBool aKey=EFalse);

/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHLegacyTOC2 
*  and return a pointer to the created object using two phase constuction
*  @param aParent   - TOC1 Object
*		  aName 	- Name of the TOC2 
*		  aKey  	- ETrue for KeywordTopic, EFalse otherwise
*  @return a CCSXHLegacyTOC2 Pointer 
*/     
    static CCSXHLegacyTOC2* NewLC(CCSXHGenericTOC1 *aParent,const TDesC& 
    aName,TBool aKey=EFalse);

/** 
*  @function GetTopicContentL
*  @since S60 3.2
*  Get the topic to be displayed in Richtext format
*  @return RichText Pointer
*/    
    IMPORT_C TAny* GetTopicContentL();

/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/       	
    TUid GetViewID() const;
    
/** 
*  @function GetLegacyParent
*  @since S60 3.2
*  Get Parent Object
*  @return TOC1 Object(Parent) for present TOC2 Object
*/    
    IMPORT_C  CCSXHLegacyTOC1* GetLegacyParent();
		
	private:
/** 
*  @function CCSXHLegacyTOC2
*  @since S60 3.2
*  Construct a CCSXHLegacyTOC2 
*  @param aRect - TRect Object
*  @param aParent   - TOC1 Object
*		  aName 	- Name of the TOC2 
*		  aKey  	- ETrue for KeywordTopic, EFalse otherwise
*/	
		CCSXHLegacyTOC2(CCSXHGenericTOC1 *aParent, const TDesC& aName,TBool aKey);
		
		TBool iKeyWordTopic;
  };



#endif /* INC_CSXHLEGACYTOC2_H_HEADER_INCLUDED_BC0535B9 */
