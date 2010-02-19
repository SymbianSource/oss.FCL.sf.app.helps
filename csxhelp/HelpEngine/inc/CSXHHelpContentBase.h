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
* Description:  CCSXHHelpContentBase class declaration
*
*/


#ifndef INC_CSXHHELPCONTENTBASE_H_HEADER_INCLUDED_BC0553D0
#define INC_CSXHHELPCONTENTBASE_H_HEADER_INCLUDED_BC0553D0

#include <e32std.h>
#include <e32base.h>

const TInt KMaxHelpTopicName = 30;
/** 
* @class CCSXHGenericTOC1
* This class is the base class for GenericTOC1 & GenericTOC2
*/
class CCSXHHelpContentBase : public CBase
    {
  public:
  /** 
 * @function ~CCSXHHelpContentBase
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */ 
    virtual ~CCSXHHelpContentBase();

/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/    
    IMPORT_C virtual TUid GetViewID() const = 0;

/** 
*  @function GetName
*  @since S60 3.2
*  Get the name
*  @return Pointer which contains Name
*/
    IMPORT_C TPtr GetName() const;
    
    TInt32 Priority() const;

  protected:
/** 
*  @function CCSXHHelpContentBase
*  @since S60 3.2
*  Construct a CCSXHHelpContentBase 
*  @param aName 	- Name
*/  
    CCSXHHelpContentBase(const TDesC& name, const TInt32 aPriority = 0);

  private:
    // 1. For Generic TOC1: The TOC1 entry name
    // 2. For Generic TOC2: The TOC2 entry name
    // 3. For help database: Unused
    //TBuf<KMaxHelpTopicName> iName;
    //typedef TBuf<30> TCoeContextName;
    
    HBufC *iName;
    TInt32 iPriority;
    
    template <class T>
    friend TInt Orderer(const T& left, const T& right);

    template <class T>
	friend TInt OrdererWithPriority(const T& left, const T& right);
    };
/** 
*  @function Orderer
*  @since S60 3.2
*  template function used to compare between two objects
*  @param left 	- template object
*		  right	- template object
*  @return 0 if both are same, +ve or -ve Values as per Comparision
*/ 	
template <class T>
TInt Orderer(const T& left, const T& right) 
    {
    //return left.GetName().CompareC(right.GetName());
    return (left.iName)->CompareF(*right.iName);
	}

template <class T>
TInt OrdererWithPriority(const T& left, const T& right) 
    {
    if (left.iPriority < right.iPriority)
    	{
    	return -1;
    	}
    else if (left.iPriority == right.iPriority)
    	{
        return (left.iName)->CompareF(*right.iName);
    	}
    else
    	{
    	return 1;
    	}
	}

    
#endif /* INC_CSXHHELPCONTENTBASE_H_HEADER_INCLUDED_BC0553D0 */
