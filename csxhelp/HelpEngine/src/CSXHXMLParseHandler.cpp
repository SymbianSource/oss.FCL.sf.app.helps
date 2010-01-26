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
* Description:  CCSXHXMLParseHandler class definition
*
*/

#include "CSXHXMLParseHandler.h"
#include "CSXHHtmlTOC2.h"
#include "CSXHKywdTopics.h"
#include "CSXHHelpDataBase.h"
#include "CSXHHtmlTOC1.h"
#include "csxhconstants.h"
#include "CSXHHTMLContentParser.h"

#include <aknnotewrappers.h> 
#include <utf.h> 
#include <eikenv.h> 
#include <bautils.h>


CCSXHXMLParseHandler::CCSXHXMLParseHandler(CCoeEnv*  aCoeEnv)
    {   
    iCoeEnv = aCoeEnv;
    }
    
CCSXHXMLParseHandler::~CCSXHXMLParseHandler()
	{
	if(iFeatureIds)
    	{
    	delete iFeatureIds;
    	iFeatureIds = NULL;
    	}
	}
TInt CCSXHXMLParseHandler::StartDocument()
    {
    return KErrNone;
    }

TInt CCSXHXMLParseHandler::EndDocument()
    {    
    return KErrNone;
    }

void CCSXHXMLParseHandler::SetDataBasePtr(CCSXHHelpDataBase* aDataBase)
    {
    iDataBase = aDataBase;
    } 
    
TDesC& CCSXHXMLParseHandler::GetApplicationName()
    {
    return iApplicationName;
    }
    
TPtr CCSXHXMLParseHandler::GetFeatureIds()
    {    
    return iFeatureIds->Des();	
    }
    
TBool CCSXHXMLParseHandler::SetAppNameFlag(const TDesC8&  alocalName,const TDesC& aAppTag)
    {   
    //R_TYPE_APPLICATION_TAG_NAME
    TBool Result = EFalse;
    TBuf<KMaxFileName> AppName;
    CnvUtfConverter::ConvertToUnicodeFromUtf8(AppName,alocalName);
    if(AppName.Compare(aAppTag) == 0)
        {
        iApplicationName.Copy(KEmptyString);        
        Result = ETrue;
        }
    else
        {
        Result = EFalse;
        }
    return Result;      
    } 

//-------------- CCSXHXMLParseHandler_Kywd -------------------

void CCSXHXMLParseHandler_Kywd::SetHtmlContentParserPtr(CCSXHHTMLContentParser* aParser)
    {
    iParser = aParser;
    }

CCSXHXMLParseHandler_Kywd* CCSXHXMLParseHandler_Kywd::NewL(CCoeEnv* aCoeEnv)
    {
    CCSXHXMLParseHandler_Kywd* self = new (ELeave)CCSXHXMLParseHandler_Kywd(aCoeEnv);   
    return self;
    }
    
CCSXHXMLParseHandler_Kywd::CCSXHXMLParseHandler_Kywd(CCoeEnv* aCoeEnv)
:CCSXHXMLParseHandler(aCoeEnv)                      
    {       
        iCurrentHtmlToc1 = NULL;
        iIsTargetName = EFalse;     
    }
    
TInt  CCSXHXMLParseHandler_Kywd::StartElement (const TDesC8&  /*iURI*/ , const TDesC8&  
alocalName, const TDesC8&  /*iqualname*/, const RAttributeArray&  aAttributes) 
    {
    if(iFeatureIds)
    	{
    	delete iFeatureIds;
    	iFeatureIds = NULL;
    	}
    	
    
    TRAP_IGNORE(iFeatureIds = HBufC::NewL(2));
    TPtr FIdValue = iFeatureIds->Des();
    FIdValue.Copy(_L("-1"));  
    
    if(iIsTOC2View)
        {
        CreateTOC2ObjectsforKeywordSearch(alocalName,aAttributes);
        }
    else
        {               
        iIsAppName = SetAppNameFlag(alocalName,KSearchViewTOC1Tag); 
        
	    TInt nElements = aAttributes.Count();
	    TBuf<KMaxFileName> TagName; 
	    RAttribute AttributeInfo;
	    RTagInfo TagInformation;	    
    	CnvUtfConverter::ConvertToUnicodeFromUtf8(TagName,alocalName);
    
	    for(TInt i=0; i<nElements; i++)
	        {
	        AttributeInfo = aAttributes.operator[](i);             
	        TagInformation = AttributeInfo.Attribute();
	        
	        TBuf<KMaxUnits> LocalName;
	        //Read from resource file
	        CnvUtfConverter::ConvertToUnicodeFromUtf8(LocalName,TagInformation.LocalName().DesC());	        
	        if(LocalName.Compare(_L("FeatureID")) == 0 && TagName.Compare(_L("text")) == 0) 
	        	{
	        	if(iFeatureIds)
			    	{
			    	delete iFeatureIds;
			    	iFeatureIds = NULL;
			    	}
	        	
	        	TRAP_IGNORE(iFeatureIds = HBufC::NewL(AttributeInfo.Value().DesC().Length()));
	        	TPtr FIdValue = iFeatureIds->Des(); 
	        	CnvUtfConverter::ConvertToUnicodeFromUtf8(FIdValue,AttributeInfo.Value().DesC());
	        	}	        	
	        }       
        }   
    return KErrNone;
    }

TInt CCSXHXMLParseHandler_Kywd::EndElement  (  const TDesC8&  ,const TDesC8& ,const TDesC8& ) 
    {
	TRAP_IGNORE(CreateAndInsertKywdL());//Result can be ignored        
    iIsTargetName = EFalse;
    iIsAppName = EFalse;    
    return KErrNone;
    }
    
TInt CCSXHXMLParseHandler_Kywd::Characters (const TDesC8&  aBuff, const TInt /*aStart*/, const TInt /*aLength*/) 
    {
    TBuf<KMaxFileName> TitleName;   
    if(iIsAppName)
        {           
        CnvUtfConverter::ConvertToUnicodeFromUtf8(TitleName,aBuff);
        iApplicationName.Append(TitleName);     
        }
    else if(iIsTargetName)
        {
        CnvUtfConverter::ConvertToUnicodeFromUtf8(TitleName,aBuff);
        iKeywordSearchTOC2TitleName.Append(TitleName);              
        }
    return KErrNone;
    }

void CCSXHXMLParseHandler_Kywd::CreateTOC2ObjectsforKeywordSearch(const TDesC8& 
 alocalName, const RAttributeArray&  aAttributes)
    {
        iIsAppName = SetAppNameFlag(alocalName,KSearchViewTOC1Tag);
        
        TInt nElements = aAttributes.Count();
        TBuf<KMaxFileName>TagValue; 
        RAttribute AttributeInfo;
        RTagInfo TagInformation;
		
		TBuf<KMaxFileName> AppName;
        CnvUtfConverter::ConvertToUnicodeFromUtf8(AppName,alocalName);
        if(AppName.Compare(KSearchViewTOC2Tag) == 0)
            {
            iKeywordSearchTOC2TitleName.Copy(KEmptyString);
            iIsTargetName = ETrue;
            }
        else
            {
            iIsTargetName = EFalse;
            }
		
		if(!iIsTOC2View)            		               
			return;
		
        for(TInt i=0; i<nElements; i++)
            {
            AttributeInfo = aAttributes.operator[](i);             
            TagInformation = AttributeInfo.Attribute();
            
            TBuf<KMaxUnits> LocalName;
            //Read from resource file
            CnvUtfConverter::ConvertToUnicodeFromUtf8(LocalName,TagInformation.LocalName().DesC());
            if(LocalName.Compare(KHtmlPageTag) == 0)    
                {               
                CnvUtfConverter::ConvertToUnicodeFromUtf8(iHTMLPageName,
                	AttributeInfo.Value().DesC());
                }
            else if(LocalName.Compare(_L("FeatureID")) == 0) 
	            {
   	        	if(iFeatureIds)
			    	{
			    	delete iFeatureIds;
			    	iFeatureIds = NULL;
			    	}
	        	TRAP_IGNORE(iFeatureIds = HBufC::NewL(AttributeInfo.Value().DesC().Length()));
	        	TPtr FIdValue = iFeatureIds->Des(); 
	        	CnvUtfConverter::ConvertToUnicodeFromUtf8(FIdValue,AttributeInfo.Value().DesC());

	            }
            }
        
    }   
    
void CCSXHXMLParseHandler_Kywd::CreateAndInsertKywdL()
    {    
    if(iParser->CheckFeatureIDL(iFeatureIds->Des()))
        {
        if(iIsAppName && !iIsTOC2View)
	        {
	        iDataBase->InsertKeywordL(iApplicationName,iCurrentHtmlToc1);	
	        }
        else if(iIsTargetName && iApplicationName.Compare(iTOC1Name)==0)
	        {
	        CCSXHHtmlTOC2* PXHTMLObject = 
	        CCSXHHtmlTOC2::NewL(iCurrentHtmlToc1,iKeywordSearchTOC2TitleName,iHTMLPageName,ETrue);
	        PXHTMLObject->SetKywdParent(STATIC_CAST(CCSXHKywdTopics*,iDataBase->GetKywdTopics())
	                                            ->GetSelectedTopic());
			STATIC_CAST(CCSXHHtmlTOC2*,PXHTMLObject)->SetHtmlFilePath(iFilePath);                                            
	        iDataBase->InsertKeywordTopicL(/*iTOC1Name,*/PXHTMLObject);	
	        }
        }
    }
    
void CCSXHXMLParseHandler_Kywd::SetTOC1Title(const TDesC& aName)
    {
    iTOC1Name = aName;  
    }   

void CCSXHXMLParseHandler_Kywd::SetCurrentHtmlToc1(CCSXHHtmlTOC1* aCurrentHtmlToc1)
    {
    iCurrentHtmlToc1 = aCurrentHtmlToc1;
    }
    
void  CCSXHXMLParseHandler_Kywd::SetIsTOC2(TBool aFlag)
    {
    iIsTOC2View = aFlag;
    }       
    
void CCSXHXMLParseHandler_Kywd::SetPath(const TDesC&  aFilePath)
	{
	iFilePath.Copy(KEmptyString);
	iFilePath.Append(aFilePath);
	}    

//-------------- CCSXHXMLParseHandler_MetaFile -------------------

CCSXHXMLParseHandler_MetaFile* CCSXHXMLParseHandler_MetaFile::NewL(CCoeEnv* aCoeEnv)
    {
    CCSXHXMLParseHandler_MetaFile* self = new (ELeave)CCSXHXMLParseHandler_MetaFile(aCoeEnv);   
    return self;
    }
    
CCSXHXMLParseHandler_MetaFile::CCSXHXMLParseHandler_MetaFile(CCoeEnv* aCoeEnv)
:CCSXHXMLParseHandler(aCoeEnv)
    {
    
    }
TInt  CCSXHXMLParseHandler_MetaFile::StartElement (const TDesC8&  , const TDesC8&  
alocalName, const TDesC8&  , const RAttributeArray& aAttributes) 
    {
    iIsAppName = SetAppNameFlag(alocalName,KAppNameTag);

    TInt nElements = aAttributes.Count();    
    TBuf<KMaxFileName> LocalTagName; 
    RAttribute AttributeInfo;
    RTagInfo TagInformation;
    
    CnvUtfConverter::ConvertToUnicodeFromUtf8(LocalTagName,alocalName);
    for(TInt i=0; i<nElements; i++)
        {
        AttributeInfo = aAttributes.operator[](i);             
        TagInformation = AttributeInfo.Attribute();
        
    	if(iFeatureIds)
	    	{
	    	delete iFeatureIds;
	    	iFeatureIds = NULL;
	    	}
    	
    	TRAP_IGNORE(iFeatureIds = HBufC::NewL(AttributeInfo.Value().DesC().Length()));
    	TPtr FIdValue = iFeatureIds->Des(); 
    	CnvUtfConverter::ConvertToUnicodeFromUtf8(FIdValue,AttributeInfo.Value().DesC());    	
        }
        
    if(LocalTagName.Compare(KAppNameTag) == 0 && nElements == 0)        
		{
		if(iFeatureIds)
	    	{
	    	delete iFeatureIds;
	    	iFeatureIds = NULL;
	    	}
		
		TRAP_IGNORE(iFeatureIds = HBufC::NewL(2));
	    TPtr FIdValue = iFeatureIds->Des();
	    FIdValue.Copy(_L("-1"));  
		}
        
    return KErrNone;
    }

TInt CCSXHXMLParseHandler_MetaFile::EndElement  (  const TDesC8&  ,const TDesC8& ,const TDesC8& ) 
    {
    iIsAppName = EFalse;
    return KErrNone;
    }
    
TInt CCSXHXMLParseHandler_MetaFile::Characters (const TDesC8&  aBuff, const TInt /*aStart*/, const TInt /*aLength*/) 
    {
    TBuf<KMaxFileName> TitleName;   
    if(iIsAppName)
        {           
        CnvUtfConverter::ConvertToUnicodeFromUtf8(TitleName,aBuff);
        iApplicationName.Append(TitleName);     
        }
    return KErrNone;
    }
    

//-------------- CCSXHXMLParseHandler_MasterMetaFile -------------------
CCSXHXMLParseHandler_MasterMetaFile* CCSXHXMLParseHandler_MasterMetaFile::NewL(CCoeEnv* aCoeEnv)
    {
    CCSXHXMLParseHandler_MasterMetaFile* self = new (ELeave)CCSXHXMLParseHandler_MasterMetaFile(aCoeEnv); 
    return self;
    }
    
CCSXHXMLParseHandler_MasterMetaFile::CCSXHXMLParseHandler_MasterMetaFile(CCoeEnv* aCoeEnv)
:CCSXHXMLParseHandler(aCoeEnv)
    {
        
    }
    
TInt  CCSXHXMLParseHandler_MasterMetaFile::StartElement (const TDesC8&  , const TDesC8&  
, const TDesC8&  , const RAttributeArray&  aAttributes) 
    {
    TRAP_IGNORE(CreateTOC1ObjectsL(aAttributes));
    return KErrNone;
    }
    

        
void CCSXHXMLParseHandler_MasterMetaFile::CreateTOC1ObjectsL(const
 RAttributeArray&  aAttributes)
    {
    TInt nElements = aAttributes.Count();
    TBuf<KMaxFileName> appUid; 
    TBuf<KMaxFileName> appName; 
    RAttribute AttributeInfo;
    RTagInfo TagInformation;
    TLinearOrder<CCSXHHelpContentBase> anOrder(Orderer<CCSXHHelpContentBase>);      
	
    for(TInt i=0; i<nElements; i++)
        {
        AttributeInfo = aAttributes.operator[](i);             
        TagInformation = AttributeInfo.Attribute();
        
        TBuf<KMaxUnits> LocalName;
        //Read from resource file
        CnvUtfConverter::ConvertToUnicodeFromUtf8(LocalName,TagInformation.LocalName().DesC());
        
        if ( LocalName.Compare( KDriveInfo ) == 0 )
            {
            TChar driveLetter(AttributeInfo.Value().DesC()[0]);
            RFs::CharToDrive( driveLetter, iDrive );
            }
            
        if(LocalName.Compare(KMasterCollection_idTag) == 0)
            {
            CnvUtfConverter::ConvertToUnicodeFromUtf8(appUid,AttributeInfo.Value().DesC());                    
            }
                    
        if(LocalName.Compare(KTOC2NameTag) == 0)
            {
            CnvUtfConverter::ConvertToUnicodeFromUtf8(appName,AttributeInfo.Value().DesC());
            if(2 == nElements)
            	{
            	if(iFeatureIds)
			    	{
			    	delete iFeatureIds;
			    	iFeatureIds = NULL;
			    	}
            	
            	iFeatureIds = HBufC::NewL(2);
			    TPtr FIdValue = iFeatureIds->Des();
			    FIdValue.Copy(_L("-1"));  
            	}
            	
            
            iParser->InsertHTMLToc1L( appUid, appName, iDrive, iDataBase, iFeatureIds->Des() );                            	
            }
            
        if(LocalName.Compare(_L("FeatureID")) == 0) 
            {
        	if(iFeatureIds)
		    	{
		    	delete iFeatureIds;
		    	iFeatureIds = NULL;
		    	}
        	
        	iFeatureIds = HBufC::NewL(AttributeInfo.Value().DesC().Length());
        	TPtr FIdValue = iFeatureIds->Des(); 
        	CnvUtfConverter::ConvertToUnicodeFromUtf8(FIdValue,AttributeInfo.Value().DesC());
            }            
        }        
    } 
    
void CCSXHXMLParseHandler_MasterMetaFile::SetHtmlContentParserPtr(CCSXHHTMLContentParser* aParser)
    {
    iParser = aParser;
    }

    
//-------------- CCSXHXMLParseHandler_IndexFile -------------------

CCSXHXMLParseHandler_IndexFile* CCSXHXMLParseHandler_IndexFile::NewL(CCoeEnv* aCoeEnv)
    {
    CCSXHXMLParseHandler_IndexFile* self = new (ELeave)CCSXHXMLParseHandler_IndexFile(aCoeEnv); 
    return self;
    }
    
CCSXHXMLParseHandler_IndexFile::CCSXHXMLParseHandler_IndexFile(CCoeEnv* aCoeEnv)
:CCSXHXMLParseHandler(aCoeEnv)
    {
        
    }
    
TInt  CCSXHXMLParseHandler_IndexFile::StartElement (const TDesC8&  , const TDesC8&  
, const TDesC8&  , const RAttributeArray&  aAttributes) 
    {
    if(iFeatureIds)
    	{
    	delete iFeatureIds;
    	iFeatureIds = NULL;
    	}
            	
    TRAP_IGNORE(iFeatureIds = HBufC::NewL(2));
    TPtr FIdValue = iFeatureIds->Des();
    FIdValue.Copy(_L("-1"));  

    TRAP_IGNORE(CreateTOC2ObjectsL(aAttributes));
    return KErrNone;
    }
    
void CCSXHXMLParseHandler_IndexFile::SetArray(RPointerArray<CCSXHHelpContentBase>* aHTMLlist)
    {
    iTOC2Array = aHTMLlist;
    }
    
void CCSXHXMLParseHandler_IndexFile::SetGenericTOC1Object(CCSXHGenericTOC1& aParent)
    {
    iParent = &aParent;
    }
    
void CCSXHXMLParseHandler_IndexFile::SetHtmlContentParserPtr(CCSXHHTMLContentParser* aParser)
    {
    iParser = aParser;
    }
    
        
void CCSXHXMLParseHandler_IndexFile::CreateTOC2ObjectsL(const
 RAttributeArray&  aAttributes)
    {
    TInt nElements = aAttributes.Count();
    TBuf<KMaxFileName>TagValue; 
    RAttribute AttributeInfo;
    RTagInfo TagInformation;
    TLinearOrder<CCSXHHelpContentBase> anOrder(Orderer<CCSXHHelpContentBase>);      

    for(TInt i=0; i<nElements; i++)
        {
        AttributeInfo = aAttributes.operator[](i);             
        TagInformation = AttributeInfo.Attribute();
        
        TBuf<KMaxUnits> LocalName;
        //Read from resource file
        CnvUtfConverter::ConvertToUnicodeFromUtf8(LocalName,TagInformation.LocalName().DesC());
                    
        if(LocalName.Compare(KTOC2NameTag) == 0)
            {
            if(iParser->CheckFeatureIDL(iFeatureIds->Des()) || 2 == nElements)
	            {            	
	            CnvUtfConverter::ConvertToUnicodeFromUtf8(TagValue,AttributeInfo.Value().DesC());
	            
	            CCSXHGenericTOC2* PXHTMLObject = CCSXHHtmlTOC2::NewL(iParent,
	            								 TagValue,iHTMLPageName);

				STATIC_CAST(CCSXHHtmlTOC2*,PXHTMLObject)->SetHtmlFilePath(iFilePath);
				
	            if (KErrNone != iTOC2Array->InsertInOrder(PXHTMLObject,anOrder))
	                delete PXHTMLObject;
	            }            
            }
        else if(LocalName.Compare(KHtmlPageTag) == 0)   
            {               
            CnvUtfConverter::ConvertToUnicodeFromUtf8(iHTMLPageName,
            AttributeInfo.Value().DesC());
            }
        else if(LocalName.Compare(KCompressedTag) == 0)
            {
            CnvUtfConverter::ConvertToUnicodeFromUtf8(iCompressString,
			AttributeInfo.Value().DesC());
            }
        else if(LocalName.Compare(_L("FeatureID")) == 0)
        	{
        	if(iFeatureIds)
		    	{
		    	delete iFeatureIds;
		    	iFeatureIds = NULL;
		    	}
        	
        	iFeatureIds = HBufC::NewL(AttributeInfo.Value().DesC().Length());
        	TPtr FIdValue = iFeatureIds->Des(); 
        	CnvUtfConverter::ConvertToUnicodeFromUtf8(FIdValue,AttributeInfo.Value().DesC());
            }        
        }        
    } 
    
void CCSXHXMLParseHandler_IndexFile::SetPath(const TDesC&  aFilePath)
	{
	iFilePath.Copy(KEmptyString);
	iFilePath.Append(aFilePath);
	}
    
//-------------- CCSXHXMLParseHandler_MasterKeywordFile -------------------

CCSXHXMLParseHandler_MasterKywd* CCSXHXMLParseHandler_MasterKywd::NewL(CCoeEnv* aCoeEnv)
    {
    CCSXHXMLParseHandler_MasterKywd* self = new (ELeave)CCSXHXMLParseHandler_MasterKywd(aCoeEnv); 
    return self;
    }
    
CCSXHXMLParseHandler_MasterKywd::CCSXHXMLParseHandler_MasterKywd(CCoeEnv* aCoeEnv)
:CCSXHXMLParseHandler(aCoeEnv)
    {
        
    }    

TInt  CCSXHXMLParseHandler_MasterKywd::StartElement (const TDesC8&  , const TDesC8&  
, const TDesC8&  , const RAttributeArray&  aAttributes) 
    {
    TRAP_IGNORE(CreateTOC1ObjectsL(aAttributes));
    return KErrNone;
    }
    
void CCSXHXMLParseHandler_MasterKywd::SetHtmlContentParserPtr(CCSXHHTMLContentParser* aParser)
    {
    iParser = aParser;
    }

void CCSXHXMLParseHandler_MasterKywd::SetCurrentHtmlToc1(CCSXHHtmlTOC1* aCurrentHtmlToc1)
    {
    iCurrentHtmlToc1 = aCurrentHtmlToc1;
    }
         
void CCSXHXMLParseHandler_MasterKywd::CreateTOC1ObjectsL(const
 RAttributeArray&  aAttributes)
    {
    TInt nElements = aAttributes.Count();
    TBuf<KMaxFileName> appUid; 
    TBuf<KMaxFileName> KeywordName; 
    RAttribute AttributeInfo;
    RTagInfo TagInformation;
	    
    TLinearOrder<CCSXHHelpContentBase> anOrder(Orderer<CCSXHHelpContentBase>);      

    for(TInt i=0; i<nElements; i++)
        {
        AttributeInfo = aAttributes.operator[](i);             
        TagInformation = AttributeInfo.Attribute();
        
        TBuf<KMaxUnits> LocalName;
        //Read from resource file
        CnvUtfConverter::ConvertToUnicodeFromUtf8(LocalName,TagInformation.LocalName().DesC());
                    
        if(LocalName.Compare(KMasterSearchTag) == 0)
            {
            CnvUtfConverter::ConvertToUnicodeFromUtf8(KeywordName,AttributeInfo.Value().DesC());
            }
                    
        if(LocalName.Compare(KMasterCollectionTag) == 0)
            {
            CnvUtfConverter::ConvertToUnicodeFromUtf8(appUid,AttributeInfo.Value().DesC());
            if(2 == nElements)
            	{
				if(iParser->GetHTMLToc1(appUid,this))      
	            	iDataBase->InsertKeywordL(KeywordName,iCurrentHtmlToc1);           	            	
            	}
            }       
            
		if(LocalName.Compare(_L("FeatureID")) == 0)    
			{
        	if(iFeatureIds)
		    	{
		    	delete iFeatureIds;
		    	iFeatureIds = NULL;
		    	}
        	
        	iFeatureIds = HBufC::NewL(AttributeInfo.Value().DesC().Length());
        	TPtr FIdValue = iFeatureIds->Des(); 
        	CnvUtfConverter::ConvertToUnicodeFromUtf8(FIdValue,AttributeInfo.Value().DesC());

			if(iParser->CheckFeatureIDL(iFeatureIds->Des()))
				{
				if(iParser->GetHTMLToc1(appUid,this))      
	            	iDataBase->InsertKeywordL(KeywordName,iCurrentHtmlToc1);           	
				}
			}
        }        
    }

