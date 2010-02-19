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
* Description:  CCSXHHtmlTOC1 class declaration
*
*/


#ifndef INC_CSXHHTMLTOC1_H_HEADER_INCLUDED_BC0568C5
#define INC_CSXHHTMLTOC1_H_HEADER_INCLUDED_BC0568C5

#include "CSXHGenericTOC1.h"

class CCSXHHelpDataBase;

/** 
* @class TAppUid
* This class stores the required information to store a folder name in string
* as integer and converts it back to a string when required. This class is mainly
* to save RAM space 
*/

class TAppUid
 	{
   	public:
   		TAppUid(const TDesC& aFolder);
   		const TUid& GetUid() const;
   		TBool IsValid() const;
    	void AppendUid(TBuf<KMaxFileName>& aStr) const;	
   	private:
   		TUid iUid;
   		TUint8 iLength;
    	};
    	

/** 
* @class CCSXHHtmlTOC1
* This class has the information of html TOC1 
*/
class CCSXHHtmlTOC1 : public CCSXHGenericTOC1
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHHtmlTOC1 
*  and return a pointer to the created object
*  @param aName 	- TOC1 Name
*		  aUid 		- Application Uid
*		  aDrive    - Drive Number
*  @return a CCSXHHtmlTOC1 Pointer 
*/
    static CCSXHHtmlTOC1* NewL(const TDesC& aName,const TDesC& aFolder,
    							const TInt& aDrive, TUid aViewId, TInt32 aPriority = 0);
/** 
 * @function ~CCSXHHtmlTOC1
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */    							
    ~CCSXHHtmlTOC1();

/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/    
    TUid GetViewID() const;

/** 
*  @function GetAppUid
*  @since S60 3.2
*  Get Application  uid
*  @return Application UId
*/    
    const TUid& GetAppUid() const;
/** 
*  @function GetContextTopic
*  @since S60 3.2
*  Get content for Context Name
*  @param  acontextName - Context Name 
*  @return pointer to database class
*/     
    CCSXHHelpContentBase* GetContextTopic(const TDesC& acontextName);
/** 
*  @function GetHelpFileL
*  @since S60 3.2
*  Get the file path
*  @param  aFilePath - File Path
*/     
    void GetHelpFileL(TBuf<KMaxFileName>& aFilePath) const;
/** 
*  @function GetHtmlTopicForFile
*  @since S60 3.2
*  Get the toc2 object in order to get Content
*  @param  aFile - toc2 Name
*/     
    CCSXHHelpContentBase* GetHtmlTopicForFile(const TDesC& aFile);
    
/** 
*  @function GetHelpFileL
*  @since S60 3.2
*  Get the file path
*  @return  ETrue, if it part of ROMdrive, EFalse otherwise
*/     
    TBool IsROMDrive();    
    
    protected:
/** 
*  @function CCSXHHtmlTOC1
*  @since S60 3.2
*  Construct a CCSXHHtmlTOC1 
*  @param aName 	- TOC1 Name
*		  aUid 		- Application Uid
*		  aDrive    - Drive Number
*/    
    CCSXHHtmlTOC1(const TDesC& aName, const TDesC& aFolder, const TInt& aDriveName, TUid aViewId, TInt32 aPriority);
/** 
*  @function InitChildList
*  @since S60 3.2
*  Allocate Memory to the array
*/    
    void InitChildList();
    
    TBool IsValid() const;
    
    
    private:	
    
	  
     TAppUid iAppUid;
      short iDrive;
    TUid iViewId;
    };



#endif /* INC_CSXHHTMLTOC1_H_HEADER_INCLUDED_BC0568C5 */
