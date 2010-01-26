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
* Description:  CCSXHHtmlTOC2 class declaration
*
*/


#ifndef INC_CSXHHTMLTOC2_H_HEADER_INCLUDED_BC052432
#define INC_CSXHHTMLTOC2_H_HEADER_INCLUDED_BC052432

#include "CSXHGenericTOC2.h"

class CCSXHGenericTOC1;
class CCSXHKywdTOC1;
class CCoeEnv;
class TCoeHelpContext;
class RFeatureControl;
/** 
* @class CCSXHHtmlTOC2
* This class has the information of html TOC2 
*/
class CCSXHHtmlTOC2 : public CCSXHGenericTOC2
    {
    public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHHtmlTOC2 
*  and return a pointer to the created object
*  @param aparent 	- TOC1 object
*		  aName  	- toc2 Name
*		  aHtmlFileName - FileName
*		  aKey		    - ETrue for Keyword toc2, EFalse otherwise	
*  @return a CCSXHHtmlTOC2 Pointer 
*/
static CCSXHHtmlTOC2* NewL(CCSXHGenericTOC1* aparent,const TDesC& aName,
const TDesC& aHtmlFileName,TBool aKey=EFalse);
/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHHtmlTOC2 
*  and return a pointer to the created object using two phase construction
*  @param aparent 	- TOC1 object
*		  aName  	- toc2 Name
*		  aHtmlFileName - FileName
*		  aKey		    - ETrue for Keyword toc2, EFalse otherwise	
*  @return a CCSXHHtmlTOC2 Pointer 
*/
static CCSXHHtmlTOC2* NewLC(CCSXHGenericTOC1*aparent,const TDesC& aName,
const TDesC& aHtmlFileName,TBool aKey=EFalse);

/** 
 * @function ~CCSXHHtmlTOC2
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */    
    ~CCSXHHtmlTOC2();

/** 
 * @function ~CCSXHHtmlTOC2
 * @since S60 3.2
 * Get the full path of the HTML file
 * @param aFileUrl - Buffer in order to get the file path	
 */    
    IMPORT_C void GetHtmlUrlL(TBuf<KMaxFileName>& aFileUrl) const;

/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/    
    TUid GetViewID() const;
/** 
*  @function GetTopicContentL
*  @since S60 3.2
*  Get the content to display in the browser 
*  @return Pointer to the buffer
*/   
    TAny* GetTopicContentL();
       	
/** 
*  @function GetFileName
*  @since S60 3.2
*  Get html file Name
*  @return buffer which contains html file Name
*/
    const TBuf<KMaxFileName>& GetFileName() const;
    
/** 
*  @function GetHelpContext
*  @since S60 3.2
*  Constructs a Help Context corresponding to the topic
*  @returns true on success
*/
    IMPORT_C TBool GetHelpContext(TCoeHelpContext& aContext) const;

/** 
*  @function GetContentsFromUrlL
*  @since S60 3.2
*  Get content from a html file
*  @param aUrl    - Full file path
*		  aCoeEnv - Control Environment
*  @return pointer to buffer which contains HTML file content
*/
    IMPORT_C static HBufC8* GetContentsFromUrlL(const TDesC& aUrl,
    				CCoeEnv* aCoeEnv,RFeatureControl& aFeatureControl);
   
/** 
*  @function SetKywdParent
*  @since S60 3.2
*  Set the parent object
*  @param aParent    - keyword toc1 class pointer
*/ 
    void SetKywdParent(CCSXHKywdTOC1* aParent);
/** 
*  @function GetKywdParent
*  @since S60 3.2
*  Get the parent 
*  @return poniter to keywordtoc1 (parent) class
*/
    IMPORT_C  CCSXHKywdTOC1* GetKywdParent();

/** 
*  @function SetHtmlFilePath
*  @since S60 3.2
*  Set Html file path
*  @param aFilePath - Html file path
*/
    void SetHtmlFilePath(const TDesC&  aFilePath);
    
/** 
*  @function GetHtmlFileName
*  @since S60 3.2
*  Get html file
*  @param aFileName - Full path with html file name 
*/
    void GetHtmlFileName(TBuf<KMaxFileName>&  aFileName);	

	private:
/** 
*  @function CCSXHHtmlTOC2
*  @since S60 3.2
*  Construct a CCSXHHtmlTOC2 
*  @param aparent 	- TOC1 object
*		  aName  	- toc2 Name
*		  aHtmlFileName - FileName
*		  aKey		    - ETrue for Keyword toc2, EFalse otherwise	
*/	
		CCSXHHtmlTOC2(CCSXHGenericTOC1* aparent,const TDesC& aName,
		const TDesC& aHtmlFileName,TBool aKeyWord = false);
	
	private:
    	TBuf<KMaxFileName> iHTMLFileName,iHTMLFilePath;
    	TBool iKeyWordTopic;
    	CCSXHKywdTOC1 *iKywdParent;    	
    };



#endif /* INC_CSXHHTMLTOC2_H_HEADER_INCLUDED_BC052432 */
