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
* Description:  CCSXHXMLParseHandler class declaration
*
*/


#ifndef __CCSXHXMLPARSEHANDLER_H__
#define __CCSXHXMLPARSEHANDLER_H__

#include <coecntrl.h> // CCoeControl
#include <MSenContentHandlerClient.h>
#include <xml/taginfo.h>
#include <xml/attribute.h>

class CCSXHGenericTOC2;
class CCSXHGenericTOC1;
class CCSXHHtmlTOC1;
class CCSXHHelpDataBase;
class CCSXHHelpContentBase;
class CCSXHHTMLContentParser;
/** 
* @class CCSXHXMLParseHandler
* This class is used retrive the data from XML file
*/
class CCSXHXMLParseHandler : public CBase, public MSenContentHandlerClient
    {
	public:
/** 
*  @function CCSXHXMLParseHandler
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler 
*  @param aCoeEnv 	- Control Environment 
*/	
		CCSXHXMLParseHandler(CCoeEnv * aCoeEnv);
		
		virtual ~CCSXHXMLParseHandler();
/** 
*  @function StartDocument
*  @since S60 3.2
*  Receive notification of the beginning of a document. 
*  @return KErrNone or some of the system-wide Symbian error codes 
*/	
		TInt StartDocument(); 
/** 
*  @function EndDocument
*  @since S60 3.2
*  Receive notification of the end of a document. 
*  @return KErrNone or some of the system-wide Symbian error codes 
*/	
		TInt EndDocument();

/** 
*  @function SetDataBasePtr
*  @since S60 3.2
*  Sets DataBase Pointer
*/
		void  SetDataBasePtr(CCSXHHelpDataBase *aDataBase);		
/** 
*  @function GetApplicationName
*  @since S60 3.2
*  Get the Application Name from Meta.xml
*  @return Application Name
*/
		TDesC& GetApplicationName();
		
		TPtr GetFeatureIds();
 		
 	protected:
/** 
*  @function SetAppNameFlagL
*  @since S60 3.2
*  Set the Application Flag Based on local Name, ResourceID
*  @param alocalName - TagName read from XML file
*		  ResourceId - Used to Retrive the TagName from resource file   	
*/		
 		TBool SetAppNameFlag(const TDesC8 & alocalName,const TDesC& aAppTag);		
 		
 	protected:
		TBuf<KMaxFileName> iApplicationName;
		HBufC* iFeatureIds;		
		TBool iIsAppName;
		CCSXHHelpDataBase* iDataBase;
		CCoeEnv* iCoeEnv;		
		
	};


/** 
* @class CCSXHXMLParseHandler_MetaFile
* This class is used retrive the data from Meta.xml file
*/	
class CCSXHXMLParseHandler_MasterMetaFile : public CCSXHXMLParseHandler
	{
	public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_IndexFile 
*  @param aCoeEnv 	- Control Environment 
*  @retrun a CCSXHXMLParseHandler_IndexFile pointer
*/
		static CCSXHXMLParseHandler_MasterMetaFile* NewL(CCoeEnv * aCoeEnv);
	protected:
/** 
*  @function CCSXHXMLParseHandler_IndexFile
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_IndexFile 
*  @param aCoeEnv 	- Control Environment 
*/	
		CCSXHXMLParseHandler_MasterMetaFile(CCoeEnv * aCoeEnv);		
	public:	
/** 
*  @function StartElement
*  @since S60 3.2
*  Receive notification of the beginning of an element
*  @param aURI		 -  The Namespace URI, or the empty string 
						if the element has no Namespace URI or
						if Namespace processing is not being performed.  
		  aLocalName -  The local name (without prefix)  
		  aName		 -  The qualified name  
		  apAttrs	 -  The attributes attached to the element. 
		  				If there are no attributes, it shall be 
		  				an empty array.Namespaces declared in the current 
		  				element will be located in the array also 
*  @return KErrNone or some of the system-wide Symbian error codes 
*/	
		TInt StartElement (const TDesC8 &aURI, const TDesC8 &aLocalName, 
		const TDesC8 &aName, const RAttributeArray &apAttrs);
/** 
*  @function SetHtmlContentParserPtr
*  @since S60 3.2
*  Set the pointer of the content parser
*  @param aParser   -  Pointer to an html content parser
*/			 
		void SetHtmlContentParserPtr(CCSXHHTMLContentParser* aParser);
		
	private:
/** 
*  @function CreateTOC2ObjectsL
*  @since S60 3.2
*  Create TOC2 objects for Html TOC2 
*  @param aAttributes - The attributes attached to the element. 
*/	
		void CreateTOC1ObjectsL(const RAttributeArray & aAttributes);
	    
	    CCSXHHTMLContentParser *iParser;
	    
	    TInt iDrive;
	};

/** 
* @class CCSXHXMLParseHandler_MetaFile
* This class is used retrive the data from Meta.xml file
*/	
class CCSXHXMLParseHandler_MetaFile : public CCSXHXMLParseHandler
	{
	public:
/** 
*  @function CCSXHXMLParseHandler_MetaFile
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_MetaFile 
*  @param aCoeEnv 	- Control Environment 
*  @retrun a CCSXHXMLParseHandler_MetaFile pointer
*/	
		static CCSXHXMLParseHandler_MetaFile* NewL(CCoeEnv * aCoeEnv);
	protected:
/** 
*  @function CCSXHXMLParseHandler_MetaFile
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_MetaFile 
*  @param aCoeEnv 	- Control Environment 
*/
		CCSXHXMLParseHandler_MetaFile(CCoeEnv * aCoeEnv);
	public:	
/** 
*  @function StartElement
*  @since S60 3.2
*  Receive notification of the beginning of an element
*  @param aURI		 -  The Namespace URI, or the empty string 
						if the element has no Namespace URI or
						if Namespace processing is not being performed.  
		  aLocalName -  The local name (without prefix)  
		  aName		 -  The qualified name  
		  apAttrs	 -  The attributes attached to the element. 
		  				If there are no attributes, it shall be 
		  				an empty array.Namespaces declared in the current 
		  				element will be located in the array also 
*  @return KErrNone or some of the system-wide Symbian error codes 
*/	
		TInt StartElement (const TDesC8 &aURI, const TDesC8 &aLocalName, 
		const TDesC8 &aName, const RAttributeArray &apAttrs);
/** 
*  @function EndElement
*  @since S60 3.2
*  Receive notification of the end of an element
*  @param aURI		 -  The Namespace URI, or the empty string 
						if the element has no Namespace URI or
						if Namespace processing is not being performed.  
		  aLocalName -  The local name (without prefix)  
		  aName		 -  The qualified name  
*  @return KErrNone or some of the system-wide Symbian error codes 
*/		
		TInt EndElement  (  const TDesC8 &aURI,const TDesC8 &aLocalName,const TDesC8 &aName);
/** 
*  @function Characters
*  @since S60 3.2
*  Receive notification inside an element
*  @param aBuff   -  The characters.  
*		  aStart  -  The start position in the character buffer.  
*		  aLength -  The number of characters to use from the character buffer.  
*  @return KErrNone or some of the system-wide Symbian error codes 
*/		
		TInt Characters (const TDesC8 &aBuff, const TInt aStart,
			 const TInt aLength); 					
	};

class CCSXHXMLParseHandler_IndexFile : public CCSXHXMLParseHandler
	{
	public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_IndexFile 
*  @param aCoeEnv 	- Control Environment 
*  @retrun a CCSXHXMLParseHandler_IndexFile pointer
*/
		static CCSXHXMLParseHandler_IndexFile* NewL(CCoeEnv * aCoeEnv);
	protected:
/** 
*  @function CCSXHXMLParseHandler_IndexFile
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_IndexFile 
*  @param aCoeEnv 	- Control Environment 
*/	
		CCSXHXMLParseHandler_IndexFile(CCoeEnv * aCoeEnv);		
	public:	
/** 
*  @function StartElement
*  @since S60 3.2
*  Receive notification of the beginning of an element
*  @param aURI		 -  The Namespace URI, or the empty string 
						if the element has no Namespace URI or
						if Namespace processing is not being performed.  
		  aLocalName -  The local name (without prefix)  
		  aName		 -  The qualified name  
		  apAttrs	 -  The attributes attached to the element. 
		  				If there are no attributes, it shall be 
		  				an empty array.Namespaces declared in the current 
		  				element will be located in the array also 
*  @return KErrNone or some of the system-wide Symbian error codes 
*/	
		TInt StartElement (const TDesC8 &aURI, const TDesC8 &aLocalName, 
		const TDesC8 &aName, const RAttributeArray &apAttrs);
/** 
*  @function SetArray
*  @since S60 3.2
*  Get the pointer & fill the contents from Index.xml
*  @param aHTMLlist   -  Pointer to an Array
*/			 
		void SetArray(RPointerArray<CCSXHHelpContentBase> *aHTMLlist);	
/** 
*  @function SetGenericTOC1Object
*  @since S60 3.2
*  Set GenericTOC1 Object
*  @param aParent   -  Generic TOC1 Object
*/			 
		void SetGenericTOC1Object(CCSXHGenericTOC1& aParent); 		
/** 
*  @function SetPath
*  @since S60 3.2
*  Set Html file path
*  @param aFilePath   -  Set Html file path
*/		
		void SetPath(const TDesC&  aFilePath);
		
		void SetHtmlContentParserPtr(CCSXHHTMLContentParser* aParser);		
	private:
/** 
*  @function CreateTOC2ObjectsL
*  @since S60 3.2
*  Create TOC2 objects for Html TOC2 
*  @param aAttributes - The attributes attached to the element. 
*/	
		void CreateTOC2ObjectsL(const RAttributeArray & aAttributes);
		TBuf<KMaxFileName> iCompressString,iHTMLPageName,iFilePath; 					
		RPointerArray<CCSXHHelpContentBase>* iTOC2Array; 
		CCSXHGenericTOC1* iParent;	
		CCSXHHTMLContentParser *iParser;
	};

	
class CCSXHXMLParseHandler_Kywd : public CCSXHXMLParseHandler
	{
	public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_Kywd 
*  @param aCoeEnv 	- Control Environment 
*  @retrun a CCSXHXMLParseHandler_Kywd pointer
*/
		static CCSXHXMLParseHandler_Kywd* NewL(CCoeEnv * aCoeEnv);
	protected:
/** 
*  @function CCSXHXMLParseHandler_Kywd
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_Kywd 
*  @param aCoeEnv 	- Control Environment 
*/	
		CCSXHXMLParseHandler_Kywd(CCoeEnv * aCoeEnv);	
	public:
/** 
*  @function StartElement
*  @since S60 3.2
*  Receive notification of the beginning of an element
*  @param aURI		 -  The Namespace URI, or the empty string 
						if the element has no Namespace URI or
						if Namespace processing is not being performed.  
		  aLocalName -  The local name (without prefix)  
		  aName		 -  The qualified name  
		  apAttrs	 -  The attributes attached to the element. 
		  				If there are no attributes, it shall be 
		  				an empty array.Namespaces declared in the current 
		  				element will be located in the array also 
*  @return KErrNone or some of the system-wide Symbian error codes 
*/	
		TInt StartElement (const TDesC8 &aURI, const TDesC8 &aLocalName, 
		const TDesC8 &aName, const RAttributeArray &apAttrs);
/** 
*  @function EndElement
*  @since S60 3.2
*  Receive notification of the end of an element
*  @param aURI		 -  The Namespace URI, or the empty string 
						if the element has no Namespace URI or
						if Namespace processing is not being performed.  
		  aLocalName -  The local name (without prefix)  
		  aName		 -  The qualified name  
*  @return KErrNone or some of the system-wide Symbian error codes 
*/		
		TInt EndElement  (  const TDesC8 &aURI,const TDesC8 &aLocalName,const TDesC8 &aName);
/** 
*  @function Characters
*  @since S60 3.2
*  Receive notification inside an element
*  @param aBuff   -  The characters.  
*		  aStart  -  The start position in the character buffer.  
*		  aLength -  The number of characters to use from the character buffer.  
*  @return KErrNone or some of the system-wide Symbian error codes 
*/		
		TInt Characters (const TDesC8 &aBuff, const TInt aStart,
			 const TInt aLength); 

/** 
*  @function SetTOC1Title
*  @since S60 3.2
*  Set TOC1 Name 
*  @param aName   -  TOC1 Name  
*/
 		void SetTOC1Title(const TDesC& aName);
/** 
*  @function SetCurrentHtmlToc1
*  @since S60 3.2
*  Set HtmlTOC1 Object
*  @param aCurrentHtmlToc1   -  Pointyer to HtmlTOC1 Object  
*/
		void SetCurrentHtmlToc1(CCSXHHtmlTOC1 *aCurrentHtmlToc1);
/** 
*  @function SetIsTOC2
*  @since S60 3.2
*  Set the flag if parsing in order to get TOC2 Objects, EFalse otherwise
*  @param aCurrentHtmlToc1   -  Pointer to HtmlTOC1 Object  
*/
		void SetIsTOC2(TBool aFlag);		

/** 
*  @function SetPath
*  @since S60 3.2
*  Set Html file path
*  @param aFilePath   -  Set Html file path
*/		
		void SetPath(const TDesC&  aFilePath);
		
		void SetHtmlContentParserPtr(CCSXHHTMLContentParser* aParser);
		
	private:
/** 
*  @function CreateTOC2ObjectsforKeywordSearch
*  @since S60 3.2
*  Get the Info In order to Create TOC2 objects for Html Keyword 
*  @param aName  - TagName
*		  aAttrs - The attributes attached to the element. 
*/
		void CreateTOC2ObjectsforKeywordSearch(const TDesC8 &aName,const RAttributeArray& aAttrs); 		
/** 
*  @function CreateTOC2ObjectsforKeywordSearchL
*  @since S60 3.2
*  Creates TOC2 objects for Html Keyword And Insert into Database  
*/
		void CreateAndInsertKywdL();		
 		TBuf<KMaxFileName> iHTMLPageName,iKeywordSearchTOC2TitleName; 					
 		TBuf<KMaxFileName> iTOC1Name,iFilePath;
 		CCSXHHtmlTOC1 *iCurrentHtmlToc1;
 		TBool iIsTargetName,iIsTOC2View;
 		CCSXHHTMLContentParser* iParser;
	};	

class CCSXHXMLParseHandler_MasterKywd : public CCSXHXMLParseHandler
	{
	public:
/** 
*  @function NewL
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_Kywd 
*  @param aCoeEnv 	- Control Environment 
*  @retrun a CCSXHXMLParseHandler_Kywd pointer
*/
		static CCSXHXMLParseHandler_MasterKywd* NewL(CCoeEnv * aCoeEnv);
	protected:
/** 
*  @function CCSXHXMLParseHandler_Kywd
*  @since S60 3.2
*  Construct a CCSXHXMLParseHandler_Kywd 
*  @param aCoeEnv 	- Control Environment 
*/	
		CCSXHXMLParseHandler_MasterKywd(CCoeEnv * aCoeEnv);	
		
	public:
/** 
*  @function StartElement
*  @since S60 3.2
*  Receive notification of the beginning of an element
*  @param aURI		 -  The Namespace URI, or the empty string 
						if the element has no Namespace URI or
						if Namespace processing is not being performed.  
		  aLocalName -  The local name (without prefix)  
		  aName		 -  The qualified name  
		  apAttrs	 -  The attributes attached to the element. 
		  				If there are no attributes, it shall be 
		  				an empty array.Namespaces declared in the current 
		  				element will be located in the array also 
*  @return KErrNone or some of the system-wide Symbian error codes 
*/	
		TInt StartElement (const TDesC8 &aURI, const TDesC8 &aLocalName, 
		const TDesC8 &aName, const RAttributeArray &apAttrs);
/** 
*  @function CreateTOC1ObjectsL
*  @since S60 3.2
*  Create TOC1 objects for Keywords
*  @param aAttributes - The attributes attached to the element. 
*/	
		void CreateTOC1ObjectsL(const RAttributeArray&  aAttributes);

/** 
*  @function SetCurrentHtmlToc1
*  @since S60 3.2
*  Set HtmlTOC1 Object
*  @param aCurrentHtmlToc1   -  Pointyer to HtmlTOC1 Object  
*/
		void SetCurrentHtmlToc1(CCSXHHtmlTOC1 *aCurrentHtmlToc1);
		
/** 
*  @function SetHtmlContentParserPtr
*  @since S60 3.2
*  Set the pointer of the content parser
*  @param aParser   -  Pointer to an html content parser
*/			 
		void SetHtmlContentParserPtr(CCSXHHTMLContentParser* aParser);		
		

	private:
 		CCSXHHtmlTOC1 *iCurrentHtmlToc1; 		
 		CCSXHHTMLContentParser *iParser;
	};        
#endif 
