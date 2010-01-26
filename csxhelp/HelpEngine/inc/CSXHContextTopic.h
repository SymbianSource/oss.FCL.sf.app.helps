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
* Description:  CCSXHContextTopic class declaration
*
*/


#ifndef INC_CSXHCONTEXTTOPIC_H_HEADER_INCLUDED_BC050AFE
#define INC_CSXHCONTEXTTOPIC_H_HEADER_INCLUDED_BC050AFE

#include "CSXHHelpContentBase.h"

class CCSXHHelpDataBase;
/** 
* @class CCSXHContextTopic
* This class is used for Context Sensitivity   
*/
class CCSXHContextTopic : public CCSXHHelpContentBase
    {
  public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHContextTopic 
*  and return a pointer to the created object
*  @param aToc2 	- a toc2 class pointer type casted to database
*  @return a CCSXHContextTopic Pointer 
*/
	static CCSXHContextTopic* NewL(CCSXHHelpContentBase* aToc2);
/** 
 * @function ~CCSXHContextTopic
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */  	
    ~CCSXHContextTopic();
/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/      
    TUid GetViewID() const;
/** 
*  @function GetTopic
*  @since S60 3.2
*  Get the Object HTML / Legacy type cast to Database
*  @return View Id
*/      
    IMPORT_C CCSXHHelpContentBase* GetTopic();

  protected:
/** 
*  @function CCSXHContextTopic
*  @since S60 3.2
*  Construct a CCSXHContextTopic 
*  @param aToc2object 	- a toc2 class pointer type casted to database
*/  
  	CCSXHContextTopic(CCSXHHelpContentBase *aToc2object); 
  
  protected:
  	CCSXHHelpContentBase *iToc2;

  };



#endif /* INC_CSXHCONTEXTTOPIC_H_HEADER_INCLUDED_BC050AFE */
