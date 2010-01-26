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
* Description:  CCSXHHelpDataBase class declaration
*
*/


#ifndef INC_CSXHHELPDATABASE_H_HEADER_INCLUDED_BC054F58
#define INC_CSXHHELPDATABASE_H_HEADER_INCLUDED_BC054F58

#include "CSXHHelpContentBase.h"
#include "CSXHGenericTOC1.h"
#include <f32file.h>
#include <apadef.h>
class 	CAknProgressDialog;

class CCSXHHelpDataBase;
class CCSXHHTMLContentParser;
class CCSXHLegacyContentParser;
class CCSXHGenericTOC2;
class CCSXHKywdTOC1;
class CCSXHHtmlTOC1;
class CCoeEnv;
class CCSXHMainTopics;
class CCSXHKywdTopics;
class MCSXHKywdBuilder;

/** 
* @class CCSXHHelpDataBase
* This class has all pointers & used to generate the required data from Application
*/
class CCSXHHelpDataBase : public CBase
    {
  public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHHelpDataBase 
*  and return a pointer to the created object
*  @param aCoeEnv 	- Control Environment
*  @return a CCSXHHelpDataBase Pointer 
*/
    IMPORT_C static CCSXHHelpDataBase* NewL(CCoeEnv* aCoeEnv,const TApaAppCaption& aAppCaption);
/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHHelpDataBase 
*  and return a pointer to the created object using two phase construction
*  @param aCoeEnv 	- Control Environment
*  @return a CCSXHHelpDataBase Pointer 
*/
    static CCSXHHelpDataBase* NewLC(CCoeEnv *aCoeEnv,const TApaAppCaption& aAppCaption);

/** 
*  @function GetInstance
*  @since S60 3.2
*  Get the instance of database class
*  @return database pointer 
*/
    static CCSXHHelpDataBase* GetInstance();

 /** 
 * @function ~CCSXHHelpDataBase
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */ 
    IMPORT_C ~CCSXHHelpDataBase();
/** 
*  @function GetMainTopics
*  @since S60 3.2
*  Get the instance of Main toc1
*  @return toc1 pointer 
*/  
    IMPORT_C CCSXHGenericTOC1* GetMainTopics();
/** 
*  @function GetKywdTopics
*  @since S60 3.2
*  Get the instance of keyword toc1
*  @return toc1 pointer 
*/  
    IMPORT_C CCSXHGenericTOC1* GetKywdTopics();
/** 
*  @function ClearAllTOC1Contents
*  @since S60 3.2
*  Delete all the objects from the Array
*/   
    IMPORT_C void ClearAllTOC1Contents();
/** 
*  @function GetContextTopic
*  @since S60 3.2
*  Delete all the objects from the Array
*  @param aContextMessage - Message Sent by other Application
*  @return Database pointer 
*/ 	
	IMPORT_C CCSXHHelpContentBase* GetContextTopic(const TDesC8& aContextMessage);
/** 
*  @function GetHtmlTopicForUrlL
*  @since S60 3.2
*  Delete all the objects from the Array
*  @param aurl - Html file path
*  @return Database pointer 
*/ 	
	IMPORT_C CCSXHHelpContentBase* GetHtmlTopicForUrlL(const TDesC& aurl);
	
/** 
*  @function IncrementProgressBar
*  @since S60 3.2
*  Increments the progress info 
*  @return progress info pointer
*/ 	
	void IncrementKeywordSearchCount(TInt aUnits = 1);
/** 
*  @function GetLegacyParser
*  @since S60 3.2
*  Get LegacyContentparser
*  @return LegacyContentParser pointer 
*/	
	CCSXHLegacyContentParser* GetLegacyParser();
/** 
*  @function GetHtmlParser
*  @since S60 3.2
*  Get htmlContentparser
*  @return HtmlContentParser pointer 
*/	
	CCSXHHTMLContentParser* GetHtmlParser();
/** 
*  @function InsertKeywordL
*  @since S60 3.2
*  Insert into array
*  @param aKywdName - Keyword Name
*		  aToc1		- Toc1 Object
*/	
	void InsertKeywordL(const TDesC& aKywdName, CCSXHHtmlTOC1* aToc1);
/** 
*  @function InsertKeywordL
*  @since S60 3.2
*  Insert into Array
*  @param aKywdName 		- Keyword Name
*		  aLegacyKywdIndex	- Position 
*		  aCatUid			- Application Uid
*/	
	void InsertKeywordL(const TDesC& aKywdName, TInt aLegacyKywdIndex, const TUid& aCatUid);


/** 
*  @function InitGenerateKeywordTOC2ListL
*  @since S60 3.2
*  Initializes the generation of kywd toc2 objects
*  @param aKywdToc1 		- Keyword toc1 object
*/	
	IMPORT_C TInt InitGenerateKeywordTOC2ListL(CCSXHKywdTOC1* aKywdToc1);

/** 
*  @function GenerateKeywordTOC2ListL
*  @since S60 3.2
*  Generate the toc2 objects,InitGenerateKeywordTOC2ListL has to be called first
*  @param aKywdToc1 		- Keyword toc1 object
*/	
	void GenerateKeywordTOC2ListL(CCSXHKywdTOC1* aKywdToc1);

/** 
*  @function InsertKeywordTopicL
*  @since S60 3.2
*  The caller should allocate memory for the aTOC2 object, if it is not successfully 
*  added to the topic, then this function will free the memory
*  @param 
*  		  aToc2		- toc2 object
*/	
	CCSXHKywdTOC1* InsertKeywordTopicL(CCSXHGenericTOC2* aToc2);
	
/** 
*  @function InsertKeywordTopic
*  @since S60 3.2
*  The caller should allocate memory for the aTOC2 object, if it is not successfully 
*  added to the topic, then this function will free the memory
*  @param aToc1 - toc1 object
*  		  aToc2	- toc2 object
*/	
	void InsertKeywordTopic(CCSXHKywdTOC1* aToc1, CCSXHGenericTOC2* aToc2);

/** 
*  @function FillKeywordTopicsL
*  @since S60 3.2
*  Fills Keyword objects
*  @param aBuilder - MCSXHKywdBuilder object
*/	
	void FillKeywordTopicsL(MCSXHKywdBuilder* aBuilder);

/** 
*  @function GetCoeEnv
*  @since S60 3.2
*  Get the Control Environment
*  @return pointer to control environment
*/
	CCoeEnv* GetCoeEnv();
/** 
*  @function GetKeywordSearchCount
*  @since S60 3.2
*  Gets the current count of keywords for search result view
*  @return TInt count
*/
IMPORT_C	TInt GetKeywordSearchCount();
	
  private:
/** 
*  @function ConstructL
*  @since S60 3.2
*  perform the second phase construction
*/
    void ConstructL();
/** 
*  @function CCSXHHelpDataBase
*  @since S60 3.2
*  Construct a CCSXHHelpDataBase
*  @param aCoeEnv  - Control Environment
*/
    CCSXHHelpDataBase(CCoeEnv* aCoeEnv,const TApaAppCaption& aAppCaption);

/** 
*  @function GetContextTopicL
*  @since S60 3.2
*  Get the topic for the Context
*  @param aContextMessage  - Message sent by other Application
*  @return Database pointer	
*/    
    CCSXHHelpContentBase* GetContextTopicL(const TDesC8& aContextMessage);
    
    // List of TOC1 entries
    CCSXHMainTopics *iMainTOC1;
    // List of keyword entries
    CCSXHKywdTopics *iKywdTOC1;
    
    
    CCSXHHTMLContentParser *iHTMLContentParser;
    CCSXHLegacyContentParser *iLegacyContentParser;
    
	CCoeEnv *iCoeEnv;
	
	CCSXHHelpContentBase *iLastContextTopic;
	
	MCSXHKywdBuilder *iKwydBuilder;
	
	TApaAppCaption iAppCaption;
	
	TInt iKeywordSearchCount;
	
    };



#endif /* INC_CSXHHELPDATABASE_H_HEADER_INCLUDED_BC054F58 */
