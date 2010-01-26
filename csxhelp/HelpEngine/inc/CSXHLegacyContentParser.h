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
* Description:  CCSXHLegacyContentParser class declaration
*
*/


#ifndef __CCSXHLEGACYCONTENTPARSER_H__
#define __CCSXHLEGACYCONTENTPARSER_H__

#include <e32base.h>
#include <hlpmodel.h>

class CHlpModel;
class CHlpTopic;
class CCSXHGenericTOC1;
class CCSXHGenericTOC2;
class CCSXHLegacyTOC2;
class CCSXHHelpDataBase;
class CCSXHKywdTOC1;
class CCSXHLegacyTOC1;
class CCoeEnv;
class CCSXHHelpContentBase;
class TCoeHelpContext;

/**
*  CCSXHLegacyContentParser class.
*  This class is used to retrive Legacy content from .hlp file
*  @lib euser.lib
*  @since 3.2
*/

class CCSXHLegacyContentParser: public CBase , public MHlpModelObserver
	{
	public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHLegacyContentParser 
*  and return a pointer to the created object
*  @param aDataBase 	- Database class Pointer
*  @return a CCSXHLegacyContentParser Pointer 
*/  
		static CCSXHLegacyContentParser* NewL(CCSXHHelpDataBase* aDataBase);
/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHLegacyContentParser 
*  and return a pointer to the created object using two phase construction
*  @param aDataBase 	- Database class pointer
*  @return a CCSXHLegacyContentParser Pointer 
*/
		static CCSXHLegacyContentParser* NewLC(CCSXHHelpDataBase* aDataBase);
			
/** 
*  @function GenerateTOC1ListL
*  @since S60 3.2
*  Generate Legacy TOC1 Objects 
*  @param aDataBase 	- Database class pointer
*/
		void GenerateTOC1ListL(CCSXHHelpDataBase* aDataBase);
/** 
*  @function GenerateTOC2ListL
*  @since S60 3.2
*  Generate Legacy TOC2 Objects 
*  @param atoc1 	- generic toc1 class pointer
*		  aArray	- Database class pointer's array pointer 
*/
		void GenerateTOC2ListL(CCSXHGenericTOC1& atoc1,RPointerArray<CCSXHHelpContentBase>* aArray);
/** 
*  @function GetTopicContentL
*  @since S60 3.2
*  Get the richtext for a selected toc2  
*  @param atoc2 	- generic toc2 class pointer
*  @return richtext pointer
*/		
		TAny* GetTopicContentL(CCSXHGenericTOC2* atoc2);
/** 
*  @function GenerateKywdTOC1ListL
*  @since S60 3.2
*  Generate Legacy Keyword TOC1 Objects 
*  @param aDataBase 	- Database class pointer
*/		
		void GenerateKywdTOC1ListL(CCSXHHelpDataBase *aDataBase);

/** 
*  @function InitGenerateTOC2ListForKeywordSearchL
*  @since S60 3.2
*  Generate Legacy Keyword TOC2 Objects 
*  @param akywdtoc1	- keyword toc1 class pointer
*  @return TInt the number of keyword sub topics
*/
	TInt InitGenerateTOC2ListForKeywordSearchL(CCSXHKywdTOC1* akywdtoc1);

/** 
*  @function GenerateTOC2ListForKeywordSearchL
*  @since S60 3.2
*  Generate Legacy Keyword TOC2 Objects,InitGenerateTOC2ListForKeywordSearchL
*  has to be called first
*  @param akywdtoc1	- keyword toc1 class pointer
*/
		void GenerateTOC2ListForKeywordSearchL(CCSXHKywdTOC1* akywdtoc1);
/** 
*  @function GetTOC1ForTOC2
*  @since S60 3.2
*  Get Legacy TOC1 Object 
*  @param atoc2	- keyword toc2 class pointer
*  @return legacy toc1 class pointer
*/
		CCSXHLegacyTOC1* GetTOC1ForTOC2(CCSXHLegacyTOC2* atoc2);
/** 
 * @function ~CCSXHLegacyContentParser
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */  		
		~CCSXHLegacyContentParser();

/** 
*  @function GetContextTopic
*  @since S60 3.2
*  Get the help Context object
*  @param aContext - Help context object
*  @param database class pointer 
*/         
		CCSXHHelpContentBase* GetContextTopic(TCoeHelpContext &aContext);
		
	private:
/** 
*  @function CCSXHLegacyContentParser
*  @since S60 3.2
*  Construct a CCSXHLegacyContentParser
*  @param aDataBase  - Database class pointer
*/ 	
		CCSXHLegacyContentParser(CCSXHHelpDataBase *aDataBase);
/** 
*  @function ConstructL
*  @since S60 3.2
*  perform the second phase construction
*  @param aDataBase  - Database class pointer
*/ 		
		void ConstructL();
/** 
*  From MHlpModelObserver class	
*  @function HandleModelEventL
*  @since S60 3.2
*  handle event from CHlpModel
*  @param aEvent from CHlpModel
*/ 		
		void HandleModelEventL(TInt aEvent);
/** 
*  @function LoadContextTopicL
*  @since S60 3.2
*  Get the Richtext for context sensitive help
*  @param aContext Help context class object
*/ 		
		void LoadContextTopicL(TCoeHelpContext &aContext);
	
	private:
		CHlpModel* iModel;
		CHlpTopic* iTopic;
		CCSXHHelpDataBase* iDataBase;
		CHlpList* iHlpList; 
		TInt iStatus;
		//Store all the created Legacy TOC1 objects for future reference.
		RPointerArray<CCSXHLegacyTOC1> iLegacyTOC1List;
	};

#endif 