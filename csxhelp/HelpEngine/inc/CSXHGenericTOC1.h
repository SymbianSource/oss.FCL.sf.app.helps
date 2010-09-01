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
* Description:  CCSXHGenericTOC1 class declaration
*
*/


#ifndef INC_CSXHGENERICTOC1_H_HEADER_INCLUDED_BC050AFE
#define INC_CSXHGENERICTOC1_H_HEADER_INCLUDED_BC050AFE

#include "CSXHHelpContentBase.h"
#include <badesca.h>


class CCSXHHelpDataBase;
/** 
* @class CCSXHGenericTOC1
* This class has the information of TOC1 
*/
class CCSXHGenericTOC1 : public CCSXHHelpContentBase
    {
  public:
/** 
 * @function ~CCSXHGenericTOC1
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */ 
    ~CCSXHGenericTOC1();
/** 
 * @function FillChildDataL
 * @since S60 3.2
 * Fill the array
 * @param aArray - A pointer to a Descriptor Array	
 */
   	IMPORT_C virtual void FillChildDataL(CDesCArray* aArray);
/** 
 * @function GetChildL
 * @since S60 3.2
 * Fill the array
 * @param aName - toc1 Name	
 * @return pointer to toc2 type casted to database class  
 */
  	IMPORT_C virtual CCSXHHelpContentBase* GetChildL(const TDesC& aName);

/** 
 * @function InsertChild
 * @since S60 3.2
 * Insert toc2 objects
 * @param atoc2 - toc2 objects	
 * @param aDeleteObject - boolean, default True
 * @return ETrue Successful, EFalse otherwise
 */  	
  	TBool InsertChild(CCSXHHelpContentBase* atoc2,TBool aDeleteObject = ETrue);
  	
  	TBool InsertChildWithPriority(CCSXHHelpContentBase* aChild, TBool aDeleteObject = ETrue);
/** 
 * @function ResetChildList
 * @since S60 3.2
 * clear Array, delete the objects
 * @return ETrue Successful, EFalse otherwise
 */  	
  	virtual void ResetChildList();
  	
  
  protected:
/** 
*  @function CCSXHGenericTOC1
*  @since S60 3.2
*  Construct a CCSXHGenericTOC1 
*  @param aName 	- TOC1 Name
*/
  	CCSXHGenericTOC1(const TDesC& aName, const TInt32 aPriority = 0); 
  	
/** 
*  @function InitChildList
*  @since S60 3.2
*  Allocate the Memory for Array
*/
  	virtual void InitChildList();
/** 
*  @function CopyChildListL
*  @since S60 3.2
*  Copy the content to Descriptor Array
*  @param aArray - Descriptor Array
*/
  	virtual void CopyChildListL(CDesCArray* aArray);

/** 
*  @function ConstructChildList
*  @since S60 3.2
*  Fill the toc2 objects in the array
*  calls ConstructChildListL internally	
*  @return ETrue on Success, EFalse otherwise
*/
  	TBool ConstructChildList();
  	
  private:	
/** 
*  @function ConstructChildListL
*  @since S60 3.2
*  Fill the toc2 objects in the array
*/
  	void ConstructChildListL();
  	
  protected:

    RPointerArray<CCSXHHelpContentBase> *iChildList;

  };



#endif /* INC_CSXHGENERICTOC1_H_HEADER_INCLUDED_BC050AFE */
