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
* Description:  CCSXHKywdTopics class declaration
*
*/


#ifndef INC_CSXHKYWDTOPICS_H_HEADER_INCLUDED_BC0568C5
#define INC_CSXHKYWDTOPICS_H_HEADER_INCLUDED_BC0568C5

#include "CSXHGenericTOC1.h"
#include <apadef.h>

class CCSXHHelpDataBase;
class CCSXHHtmlTOC1;
class CCSXHKywdTOC1;

/** 
* @class CCSXHKywdTopics
* This class has the information of Keyword Objects
*/
class CCSXHKywdTopics : public CCSXHGenericTOC1
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHKywdTopics 
*  and return a pointer to the created object
*  @param aDataBase - database class pointer
*  @return a CCSXHKywdTopics Pointer 
*/      
    static CCSXHKywdTopics* NewL(const TApaAppCaption& aAppCaption);
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHKywdTopics 
*  and return a pointer to the created object using two phase construction
*  @param aDataBase - database class pointer
*  @return a CCSXHKywdTopics Pointer 
*/      
    static CCSXHKywdTopics* NewLC(const TApaAppCaption& aAppCaption);
/** 
*  @function ~CCSXHKywdTopics
*  @since S60 3.2
*  Destroy the object and release all memory objects
*/
    ~CCSXHKywdTopics();
/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/    
    TUid GetViewID() const;
/** 
*  @function InsertKywdL
*  @since S60 3.2
*  Insert into an array
*  @param aKywd	- Keyword Name
*/   
    void InsertKywdL(const TDesC& aKywd);
/** 
*  @function GetChildL
*  @since S60 3.2
*  Get objects for keyword
*  @param aKywd	- Keyword Name
*  @return database class pointer
*/   
    CCSXHHelpContentBase* GetChildL(const TDesC& aName);
    
/** 
*  @function GetSelectedTopic
*  @since S60 3.2
*  Get the pointer for a selected Keyword
*  @return keyword toc1 class pointer
*/   
    CCSXHKywdTOC1* GetSelectedTopic();
    
    protected:
/** 
*  @function CCSXHKywdTopics
*  @since S60 3.2
*  Construct a CCSXHKywdTopics
*  @param aName - keyword Name  
*/     
    	CCSXHKywdTopics(const TDesC& aName);
    	
/** 
*  @function InitChildList
*  @since S60 3.2
*  Allocate Memory for the Array 
*/   
    	void InitChildList() {}
/** 
*  @function CopyChildListL
*  @since S60 3.2
*  Copies content to Descriptor Array
*  @param aArray - Descriptor Array
*/   
  		void CopyChildListL(CDesCArray* aArray);
  		
  	private:
  		CDesCArray *iKywdTopics;
  		CCSXHKywdTOC1 *iCurrentTopic;
    };



/** 
* @class MCSXHKywdBuilder
* The HTML content parser operates on this class for inserting keywords
*/
class MCSXHKywdBuilder
    {
    public: // New functions
/** 
*  @function InsertKeywordL
*  @since S60 3.2
*  Insert keywords into an array, pure virtual function
*  @param aKwdTopics - keyword topics class pointer
*		  aKwd		 - keyword Name
*		  aToc1		 - html toc1 class pointer
*/ 
     virtual void InsertKeywordL(CCSXHKywdTopics* aKwdTopics, const TDesC& aKwd, 
    									CCSXHHtmlTOC1* aToc1) = 0; 
/** 
*  @function InsertKeywordL
*  @since S60 3.2
*  Insert keywords into an array, pure virtual function
*  @param aKwdTopics   - keyword topics class pointer
*		  aKwd		   - keyword Name
*		  aLegacyIndex - position of the object
*/     
     virtual void InsertKeywordL(CCSXHKywdTopics* aKwdTopics, const TDesC& aKwd, 
    									const TInt& aLegacyIndex) = 0; 
    };

#endif /* INC_CSXHKYWDTOPICS_H_HEADER_INCLUDED_BC0568C5 */
