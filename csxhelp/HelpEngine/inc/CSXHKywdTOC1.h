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
* Description:  CCSXHKywdTOC1 class declaration
*
*/


#ifndef INC_CSXHKYWDTOC1_H_HEADER_INCLUDED_BC0568C5
#define INC_CSXHKYWDTOC1_H_HEADER_INCLUDED_BC0568C5

#include "CSXHGenericTOC1.h"

class CCSXHHtmlTOC1;
/** 
* @class CCSXHLegacyTOC1
* This class has the information of keyword TOC1 
*/
class CCSXHKywdTOC1 : public CCSXHGenericTOC1
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHKywdTOC1 
*  and return a pointer to the created object
*  @param aName 	- Name of the TOC1
*  @return a CCSXHKywdTOC1 Pointer 
*/     
	    static CCSXHKywdTOC1* NewL(const TDesC& aName);
/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHKywdTOC1 
*  and return a pointer to the created object using two phase contruction
*  @param aName 	- Name of the TOC1
*  @return a CCSXHKywdTOC1 Pointer 
*/  	    
    	static CCSXHKywdTOC1* NewLC(const TDesC& aName);
/** 
 * @function ~CCSXHKywdTOC1
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */     	
  		~CCSXHKywdTOC1();
/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/
	    TUid GetViewID() const;
/** 
*  @function LegacyIndex
*  @since S60 3.2
*  Get the position
*  @return position
*/	    
	    TInt& LegacyIndex();
/** 
*  @function AppendHtmlTOC1L
*  @since S60 3.2
*  Insert html toc1 object
*  @param atoc1 - html toc1 class pointer
*/	    
	    void AppendHtmlTOC1L(CCSXHHtmlTOC1* aToc1);

/** 
*  @function GetHtmlTOC1List
*  @since S60 3.2
*  Get the Array
*  @return pointer to the Array of html toc1 objects
*/	    
	    RPointerArray<CCSXHHtmlTOC1>* GetHtmlTOC1List(); 
  	
    protected:
/** 
*  @function CCSXHKywdTOC1
*  @since S60 3.2
*  Construct a CCSXHKywdTOC1
*  @param aName  - kywd toc1 name
*/     
    	CCSXHKywdTOC1(const TDesC& aName);
/** 
*  @function InitChildList
*  @since S60 3.2
*  Allocate Memory for toc1htmllist array
*/     
       	void InitChildList();	
  	private:	
       	TInt iLegacyIndex;//Needed for getting the TOC2 kywd legacy contents
		RPointerArray<CCSXHHtmlTOC1> *iTOC1HtmlList; //Needed for getting the TOC2 kywd HTML contents
     	
    };
    
 
    
#endif /* INC_CSXHKYWDTOC1_H_HEADER_INCLUDED_BC0568C5 */
