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
* Description:  CCSXHHTMLContentParser class declaration
*
*/


#ifndef __CCSXHHTMLCONTENTPARSER_H__
#define __CCSXHHTMLCONTENTPARSER_H__

#include "CSXHGenericTOC1.h"
#include "coehelp.h"
#include <f32file.h>
#include <barsc.h> 
#include <featurecontrol.h>

 
class TAppUid;
class CSenXmlReader;
class CCSXHXMLParseHandler;
class CCSXHHtmlTOC1;
class CCSXHHelpDataBase;
class CCSXHKywdTOC1;
class CCoeEnv;
class CCSXHHelpContentBase;
class CCSXHGenericTOC2;
class MSenContentHandlerClient;
class CCSXHXMLParseHandler_Kywd;
class CCSXHRuntimeIndexing;
class CCSXHXMLParseHandler_MetaFile;
/**
*  CCSXHHTMLContentParser class.
*  This class is used to parse XML files & generate Html data
*  @lib euser.lib SenXml.lib XMLFRAMEWORK.lib	ezip.lib HLPMODEL.LIB
*  @since 3.2
*/
class CCSXHHTMLContentParser : public CBase
{
	public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHHTMLContentParser 
*  and return a pointer to the created object
*  @param aCoeEnv 	- Control Environment
*  @return a CCSXHHTMLContentParser Pointer 
*/	
		static CCSXHHTMLContentParser* NewL(CCoeEnv* aCoeEnv);
/** 
*  @function NewLC
*  @since S60 3.2
*  Construct a CCSXHLegacyContentParser 
*  and return a pointer to the created object using two phase construction
*  @param aCoeEnv 	- Control Environment
*  @return a CCSXHLegacyContentParser Pointer 
*/		
		static CCSXHHTMLContentParser* NewLC(CCoeEnv *aCoeEnv);
/** 
*  @function GenerateTOC1ListL
*  @since S60 3.2
*  Generate Html TOC1 Objects 
*  @param aDataBase 	- Database class pointer
*/		void GenerateTOC1ListL(CCSXHHelpDataBase* aDataBase);
/** 
*  @function GenerateTOC2ListL
*  @since S60 3.2
*  Generate Html TOC2 Objects 
*  @param atoc1 	- generic toc1 class pointer
*		  aArray	- Database class pointer's array pointer 
*/
		void GenerateTOC2ListL(CCSXHGenericTOC1& atoc1, RPointerArray<CCSXHHelpContentBase>* aArray);		
/** 
*  @function GetTopicContentL
*  @since S60 3.2
*  Get the Buffer for a selected toc2  
*  @param atoc2 	- generic toc2 class pointer
*  @return pointer to a buffer
*/		
		TAny* GetTopicContentL(CCSXHGenericTOC2* atoc2);
/** 
*  @function GetContentsFromFileL
*  @since S60 3.2
*  Get the Buffer from a html file
*  @param htmlFile 	- html file path
*         aCoeEnv   - Control Environment
*  @return pointer to a buffer
*/		
		static HBufC8* GetContentsFromFileL(const TDesC& htmlFile,CCoeEnv* aCoeEnv,RFeatureControl &aFeatureControl);												   

/** 
*  @function GetHtmlFileL
*  @since S60 3.2
*  Get the html file path
*  @param aCoeEnv   - Control Environment
*         aDir      - Directory
*         TAppUid   - UID
*         aFileName - FileName(IN/OUT Argument)
*/		
		static void GetHtmlFileL(CCoeEnv* aCoeEnv,const short& aDir,
                                            const TAppUid& aUid,TBuf<KMaxFileName>& aFileName);
/** 
 * @function ~CCSXHHTMLContentParser
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */ 		
		~CCSXHHTMLContentParser();

/** 
*  @function GenerateKywdTOC1ListL
*  @since S60 3.2
*  Generate Html Keyword TOC1 Objects 
*  @param aDataBase 	- Database class pointer
*/		
		void GenerateKywdTOC1ListL(CCSXHHelpDataBase* aDataBase);
/** 
*  @function GenerateTOC2ListForKeywordSearchL
*  @since S60 3.2
*  Generate Html Keyword TOC2 Objects 
*  @param aDataBase 	- Database class pointer
*  @param akywdtoc1	- keyword toc1 class pointer
*/		
		void GenerateTOC2ListForKeywordSearchL(CCSXHHelpDataBase* aDataBase,
										CCSXHKywdTOC1* akywdtoc1);
/** 
*  @function GetContextTopicL
*  @since S60 3.2
*  Get the help Context object
*  @param aContextName - Help context object
*  @param aUid - Help content UID
*  @return Help content class pointer 
*/		
		CCSXHHelpContentBase* GetContextTopicL(CCSXHHelpDataBase *aDataBase, 
				                        TUid &aUid, TCoeContextName &aContextName);
/** 
*  @function GetHtmlTopicForUrlL
*  @since S60 3.2
*  Get the toc2 object in order to get Content
*  @param  aurl - Full File Path
*  @return pointer to database	
*/		
		CCSXHHelpContentBase* GetHtmlTopicForUrlL(const TDesC& aurl);

/** 
*  @function IsUidCategoryPresent
*  @since S60 3.2
*  Check for Existance of Appication Uid
*  @param aUid - Application Uid
*  @return  ETrue if exists, EFalse otherwise
*/		
		TBool IsUidCategoryPresent(const TUid& aUid);
		
		
		void InsertHTMLToc1L(const TDesC &appUidName,const TDesC &appName,const TInt& aDrive,
		                        CCSXHHelpDataBase* aDataBase, const TDesC &FeatureIds, TInt32 aPriority);
		
/** 
*  @function GetHTMLToc1
*  @since S60 3.2
*  Get the HTML TOC1 object
*  @param aUid 					- Application Uid
*		  aCurrentHtmlToc1	   	- HTMLTOC1 pointer
*  @return  ETrue if present, EFalse otherwise
*/	        
	TBool GetHTMLToc1(const TDesC& aUid,CCSXHXMLParseHandler* aParser);	        		                        

	TBool CheckFeatureIDL(const TDesC& aFeatueIds);
	private:
/** 
*  @function CCSXHHTMLContentParser
*  @since S60 3.2
*  Construct a CCSXHHTMLContentParser
*  @param aCoeEnv  - Control Environment
*/ 	
		CCSXHHTMLContentParser(CCoeEnv* aCoeEnv);
/** 
*  @function InitializeReaderL
*  @since S60 3.2
*  Create XMLParse Objects
*  @param aXMLParser  - Pointer to XML parse Handler
*/ 
		void InitializeReaderL(CCSXHXMLParseHandler* aXMLParser);
/** 
*  @function InitializeParserAndReaderL
*  @since S60 3.2
*  Delete XMLParse Objects
*  @param aXMLParser  - Pointer to XML parse Handler
*/ 
		void ClearReader();
/** 
*  @function ConstructL
*  @since S60 3.2
*  perform the second phase construction
*/ 		
		void ConstructL();
/** 
*  @function ScanAndParseXMLfileToCreateTOC1ObjectL
*  @since S60 3.2
*  perform the second phase construction
*  @param aFileSession - FileSession
*		  ascanner	   - CDirScan pointer
*		  aDatabase	   - Database pointer
*		  aDrive	   - Drive Number
*		  aXMLParser   - XMLParserhandler pointer		
*/
		void ScanAndParseXMLfileToCreateTOC1ObjectL(RFs& aFileSession,CDirScan* ascanner,
												   CCSXHHelpDataBase* aDataBase,
												   const TInt& aDrive,
												   CCSXHXMLParseHandler_MetaFile* aXMLParser);		


		
		TBool IsRomBasedContentL( RFs& FileSession, const TDesC &aUid );
/** 
*  @function HandleMasterMetaFileL(
*  @since S60 3.2
*  perform the second phase construction
*  @param aFileSession - FileSession
*		  aDatabase	   - Database pointer
*		  aRootDir	   - Root directory
*         aPrevHandler  - Previous Content Handler
*  @return  ETrue if handled, EFalse otherwise
*/
	    TBool HandleMasterMetaFileL(CCSXHHelpDataBase* aDataBase, 
	        TChar& aDrive, MSenContentHandlerClient *aPrevHandler);

/** 
*  @function IsRedirectedL(
*  @param aDatabase	    - Database pointer
*		  aPath	        - Context root directory
*		  aUid          - App uid
*         aContextName  - file name
*  @return  ETrue if the context is redirected successfully, EFalse otherwise
*/
	    TBool IsRedirectedL(CCSXHHelpDataBase *aDataBase, 
	        			const TDesC &aPath, TUid &aUid, TCoeContextName &aContextName);
/** 
*  @function HandleMasterKeywordFileL(
*  @since S60 3.2
*  parse makster keywords file
*  @param aDatabase	   - Database pointer
*  @return  ETrue if handled, EFalse otherwise
*/		TBool HandleMasterKeywordFileL(CCSXHHelpDataBase* aDataBase);


		static	TBool GetHTMLContentPathForDriveL(TBuf<KMaxFileName>* aContentDrive,CCoeEnv *aCoeEnv);

/** 
*  @function IsAppUIdPresentAlready(
*  @since S60 3.2
*  Check duplicate Uids
*  @param aUid	   - Application Uid
*  @return  ETrue if dupicate is identified, EFalse otherwise
*/		
		TBool IsAppUIdPresentAlready(const TDesC& aUid);

/** 
*  @function GetCorrespondingTOC1FromMainArray(
*  @since S60 3.2
*  Get TOC1 object from the main array, which is used to display Main topic view.
*  @param aApplicationName	   - Application Name
*  @return  pointer to HTML toc1.
*/		
		CCSXHHtmlTOC1* GetCorrespondingTOC1FromMainArray(const TDesC& aApplicationName);

/** 
*  @function ParseKeywdFileAndCreatekywdTOC1Objects
*  @since S60 3.2
*  Parse keyword.xml file and Create kywd TOC1 objects.
*  @param aTOC1ObjectsArray	   - Array of HTMLToc1Objects
*		  XMLParser			   - pointer to XML parse handler.
*		  bMasterKeywordFilePresent - Boolean parameter	  	
*/		
		
		void ParseKeywdFileAndCreatekywdTOC1ObjectsL(RPointerArray<CCSXHHtmlTOC1>& aTOC1ObjectsArray,
				CCSXHXMLParseHandler_Kywd* XMLParser, TBool bMasterKeywordFilePresent);
				
/** 
*  @function ParseKeywdFileAndCreatekywdTOC1Objects
*  @since S60 3.2
*  Parse keyword.xml file and Create kywd TOC1 objects.
*  @param aTOC1ObjectsArray	   - Array of HTMLToc1Objects
*		  aUrl			   	   - html file path
*/		

		CCSXHHelpContentBase* CCSXHHTMLContentParser::GetObjectBasedonUrlL(RPointerArray<CCSXHHtmlTOC1>& aTOC1ObjectsArray,
							  const TDesC& aUrl,TBool aMainArrayList);    
		
/** 
*  @function GetSupportedFeatureListL
*  @since S60 3.2
*  Get enabled featureIds in the current build
*/		
		void GetSupportedFeatureListL();

/** 
*  @function CreateBufferForCSSContentL
*  @since S60 3.2
*  Create CSS Content using enabled featureIds
*  @param aFeatureControl	   - RFeatureControl instance
*/		
		static HBufC8* CreateBufferForCSSContentL(RFeatureControl& aFeatureControl);

/** 
*  @function GetContentsFromHTMLFileL
*  @since S60 3.2
*  Read the contents from HTML file
*  @param htmlFile	   - HTML file name
*		  aCoeEnv	   - Control Environment
*/		
		
		static HBufC8* GetContentsFromHTMLFileL(const TDesC& htmlFile, CCoeEnv* aCoeEnv);

/** 
*  @function MergeCssAndHTMLContentL
*  @since S60 3.2
*  Merge CSS and HTML Content 
*  @param aHTMLBuffer	   - HTML file content
*		  aCssContent	   - CSS content
*/		
		
		static HBufC8* MergeCssAndHTMLContentL(HBufC8* aHTMLBuffer, HBufC8* aCssContent);

		static void AppendStyleSheetContent_listitem(TPtr8& abufferptr,TInt aFeatureIdCount,
										RFeatureUidArray& aSupportedFeatures,CArrayFix<TInt>& alanguageCodeArray);
		static void AppendStyleSheetContent_paragraph(TPtr8& abufferptr,TInt aFeatureIdCount,
										RFeatureUidArray& aSupportedFeatures,CArrayFix<TInt>& alanguageCodeArray);
		static void AppendStyleSheetContent_ahref(TPtr8& abufferptr,TInt aFeatureIdCount,
										RFeatureUidArray& aSupportedFeatures,CArrayFix<TInt>& alanguageCodeArray);
		static void AppendStyleSheetContent_none(TPtr8& abufferptr,TInt aFeatureIdCount,
										RFeatureUidArray& aSupportedFeatures);													   
	private:		
		CSenXmlReader *iXmlReader;
		
		//Store all the created HTML TOC1 objects for future reference.
		RPointerArray<CCSXHHtmlTOC1> iHtmlTOC1List,iDuplicateHelpTopicList;
		CCoeEnv *iCoeEnv;

		RFeatureUidArray iSupportedFeatures;
		RFeatureControl iFeatureControl;
		//Input Language variation changes
		CDesCArray* iSupportedInputLanguages;
		CDesCArray* iFeatureManager_FeatureIds;
		CCSXHRuntimeIndexing* iIndexing;

};

#endif 
