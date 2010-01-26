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
* Description:  CCSXHHTMLContentParser class definition
*
*/

#include "CSXHHTMLContentParser.h"
#include "CSXHXMLParseHandler.h"
#include "CSXHHtmlTOC1.h"
#include "CSXHKywdTOC1.h"
#include "CSXHHtmlTOC2.h"
#include "CSXHHelpDataBase.h"
#include "csxhconstants.h"
#include "CSXHRuntimeIndexing.h"

#include <eikenv.h> 
#include <utf.h>
#include <data_caging_path_literals.hrh>
#include <zipfile.h>
#include <bautils.h>
#include <SenXmlReader.h> 
#include <f32file.h>
#include <pathinfo.h> 

//Input Language variation changes
#include <PtiEngine.h>

const TInt KOffset = 2;
        
CCSXHHTMLContentParser* CCSXHHTMLContentParser::NewL(CCoeEnv* aCoeEnv)
    {
    CCSXHHTMLContentParser* self = CCSXHHTMLContentParser::NewLC(aCoeEnv);
    CleanupStack::Pop(self);
    return self;
    }
    
CCSXHHTMLContentParser* CCSXHHTMLContentParser::NewLC(CCoeEnv* aCoeEnv)
    {
    CCSXHHTMLContentParser* self = new (ELeave) CCSXHHTMLContentParser(aCoeEnv);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
void CCSXHHTMLContentParser::ConstructL()
    {
    //RUNTIME
    GetSupportedFeatureListL();
    }
    
CCSXHHTMLContentParser::CCSXHHTMLContentParser(CCoeEnv* aCoeEnv) : iCoeEnv(aCoeEnv)
    {
    }
    
CCSXHHTMLContentParser::~CCSXHHTMLContentParser()
    {
    delete iXmlReader;
    
    //No need to delete the entries here, it will be done by the database
    iHtmlTOC1List.Close();
    
    //Delete all contents from the array.
    TInt count =    iDuplicateHelpTopicList.Count();
    CCSXHHtmlTOC1* toc1;
    for(TInt i=0;i<count;++i )
        {
        toc1 = iDuplicateHelpTopicList[i];
        delete toc1;
        }
    iDuplicateHelpTopicList.Reset();    
	iDuplicateHelpTopicList.Close();
	
    // Closes the array and frees all memory allocated to the array.
   // iSupportedFeatures.Close();
    
    delete iSupportedInputLanguages;
    delete iFeatureManager_FeatureIds;
    delete iIndexing;
    }
    
void CCSXHHTMLContentParser::GenerateTOC1ListL(CCSXHHelpDataBase* aDataBase)
    {       
    RFs& FileSession = iCoeEnv->FsSession();

    TDriveList DirList; 
    if(FileSession.DriveList(DirList) != KErrNone)
        return; 
    CCSXHXMLParseHandler_MetaFile* XMLParser = CCSXHXMLParseHandler_MetaFile::NewL(iCoeEnv); 
    CleanupStack::PushL(XMLParser);   
    InitializeReaderL(XMLParser);
    
      
    TBuf<KMaxFileName>* rootDir = new(ELeave) TBuf<KMaxFileName>;
    CleanupStack::PushL(rootDir);//5

    // begin runtime index XML generating
    //   
    iIndexing = CCSXHRuntimeIndexing::NewL();
    TInt driveListSize = DirList.Length();
    TBool dirExists = iIndexing->IndexFileExists();
    CDirScan* scanner=CDirScan::NewLC(FileSession);
#ifdef _DEBUG
    RDebug::Print(_L("runtime indexing object successfully build"));
#endif
    for(TInt dir(0); dir < driveListSize; dir++)
        {
        TInt error;
        TDriveInfo info;
        TChar driveLetter = '?';
        error = FileSession.Drive(info, dir);
            
        if  ( ( error == KErrNone ) && 
                (DirList[dir] && info.iType != EMediaNotPresent))
            {
            RFs::DriveToChar( dir, driveLetter );
            TInt dirChanged = iIndexing->CheckDirChange( driveLetter );
            rootDir->Delete( 0, rootDir->Length() );//Clear the previous contents                  
            rootDir->Append( driveLetter );

            if  ( dirExists 
                    && ( dirChanged == KNoChange
                        || dirChanged == KNoDirExist) )
                {
                // xml exist and no install/uninstall happened
                // while Z needs special process once its index file exists
#ifdef _DEBUG
                RDebug::Print(_L("Used to launch, scan drive number:  %d"), dir);
#endif
                if ( dirChanged == KNoChange )
                    {
                    // no modify helps, so just parse index.xml
                    //
                    HandleMasterMetaFileL( aDataBase, driveLetter, XMLParser );
                    }
                else // KNoDirExist
                    {
                    // help content folder removed due to some reason (uninstall/crash?)
                    // delete index file in case it exist
#ifdef _DEBUG
                    RDebug::Print( _L("no resource folder, delete index.xml if exists") );
#endif
                    iIndexing->DeleteIndexFile( driveLetter );
                    }
                }
            else
                {
                // xml not exist or install/uninstall not happend
                //
                if( GetHTMLContentPathForDriveL( rootDir,iCoeEnv ) )
                    {
#ifdef _DEBUG
                    RDebug::Print( _L("No index, scan folder for parsing, drive letter: %d"), dir );
#endif
                    iIndexing->BeginIndexFile( driveLetter );
                    scanner->SetScanDataL(*rootDir
                                    ,KEntryAttDir|KEntryAttMatchExclusive,
                                    ESortByName|EAscending, CDirScan::EScanDownTree);
                    ScanAndParseXMLfileToCreateTOC1ObjectL(FileSession,scanner,aDataBase,
                                                        dir,XMLParser);  
                                                        
                    iIndexing->FinishAndCloseIndexFileL();
                    
                    }
                
                }

            }
        }
    CleanupStack::PopAndDestroy( scanner );
    CleanupStack::PopAndDestroy(rootDir);
    CleanupStack::PopAndDestroy(XMLParser);
    
    ClearReader();
    }

TBool CCSXHHTMLContentParser::GetHTMLContentPathForDriveL(TBuf<KMaxFileName>* aContentDrive,CCoeEnv *aCoeEnv)
	{
    // get the path to language level
    // Input: aContentDrive == C or E or Z
    // Output: aContentDrive == C:\\resource\\xhtml\\01\\ (or Z:\\resource\\xhtml\\31\\)
    //
	aContentDrive->Append(KXhtmlFolder);
	TInt len = aContentDrive->Length();
	RFs& FileSession = aCoeEnv->FsSession();
	RArray<TLanguage> langs;
   	BaflUtils::GetDowngradePathL(FileSession,User::Language(),langs);
    		
    for(int i = 0; i < langs.Count(); ++i)
    	{
    	TLanguage lang = langs[i];
    	//TSW Error: UKAL-6YL72P
    	//Tool chain uses full symbian language id in 
    	//the directory name, which is not detected by help application.
    	//
    	if(lang < 10)
            {
            aContentDrive->AppendNumFixedWidth(lang,EDecimal,2);
            }
        else
            {
            aContentDrive->AppendNum(lang);
            }
    	
    	aContentDrive->Append(KFwdSlash);
    	if(BaflUtils::PathExists(FileSession,*aContentDrive))
    		{
    		langs.Reset();
    		return ETrue;
    		}
    	else
    		{
    		aContentDrive->Delete((len-1),aContentDrive->Length()-len);
    		}	
    	}
    	langs.Reset();
    	return EFalse;
	}
    
TBool CCSXHHTMLContentParser::HandleMasterMetaFileL(CCSXHHelpDataBase* aDataBase, 
                        TChar& aDrive, MSenContentHandlerClient *aPrevHandler)											   
    {
    RFs& FileSession = iCoeEnv->FsSession();
    TBuf<KMaxFileName> masterFile;

    iIndexing->GetPrivatePath( masterFile );
    masterFile.Append( aDrive );
    masterFile.Append( KFwdSlash );
    iIndexing->AppendLocale( masterFile );
    masterFile.Append( KMasterMetaFile );
    if(BaflUtils::FileExists( FileSession,masterFile ) )
        {
#ifdef _DEBUG
        RDebug::Print(_L("index exist begin to parse!    %c"), aDrive );
#endif
        CCSXHXMLParseHandler_MasterMetaFile* masterMetaHandler = 
        CCSXHXMLParseHandler_MasterMetaFile::NewL(iCoeEnv);
        masterMetaHandler->SetDataBasePtr(aDataBase);
        masterMetaHandler->SetHtmlContentParserPtr(this);
        CleanupStack::PushL(masterMetaHandler);      
    
        iXmlReader->SetContentHandler(*masterMetaHandler);
        // ParseL is not in async
        iXmlReader->ParseL(FileSession,masterFile);
        iXmlReader->SetContentHandler(*aPrevHandler);
        CleanupStack::PopAndDestroy(masterMetaHandler);
        }

    return ETrue;
    }
    
void CCSXHHTMLContentParser::GenerateTOC2ListL(CCSXHGenericTOC1& 
aGenericTOC1Object, RPointerArray<CCSXHHelpContentBase>* GenericTOC2List)
    {
    CCSXHXMLParseHandler_IndexFile* XMLParser = CCSXHXMLParseHandler_IndexFile::NewL(iCoeEnv);
    CleanupStack::PushL(XMLParser);      
    InitializeReaderL(XMLParser);
        
    XMLParser->SetArray(GenericTOC2List);
    XMLParser->SetGenericTOC1Object(aGenericTOC1Object);
    XMLParser->SetHtmlContentParserPtr(this);
    RFs& FileSession = iCoeEnv->FsSession();
    CCSXHHtmlTOC1* toc1 = STATIC_CAST(CCSXHHtmlTOC1*,&aGenericTOC1Object);
    TBuf<KMaxFileName> lookupindexfile;
    toc1->GetHelpFileL(lookupindexfile);
    XMLParser->SetPath(lookupindexfile);
    lookupindexfile.Append(KIndexFile);              
 
    if(BaflUtils::FileExists(FileSession,lookupindexfile))
        iXmlReader->ParseL (FileSession,lookupindexfile);
	
	TBuf<KMaxFileName>HelpContentName(toc1->GetName());
	int toc1Count = iDuplicateHelpTopicList.Count();
	
 	for(int i=0;i < toc1Count;i++)
	    {    
	    CCSXHHtmlTOC1* temptoc1 = iDuplicateHelpTopicList[i];
	        
	    if(temptoc1->GetName().CompareF(HelpContentName) == 0)
		    {    
		    TBuf<KMaxFileName> lookup;
		    temptoc1->GetHelpFileL(lookup);
		    XMLParser->SetPath(lookup);
		    lookup.Append(KIndexFile);              
		 
		    if(BaflUtils::FileExists(FileSession,lookup))
		        iXmlReader->ParseL (FileSession,lookup);
		    }
	    }
    
    CleanupStack::PopAndDestroy(XMLParser);
    ClearReader();
    }       
        
TAny* CCSXHHTMLContentParser::GetTopicContentL(CCSXHGenericTOC2* aTopic)
    {
    CCSXHHtmlTOC2* topic = STATIC_CAST(CCSXHHtmlTOC2*,aTopic);
    TBuf<KMaxFileName> htmlFile;
	topic->GetHtmlFileName(htmlFile);
    return GetContentsFromFileL(htmlFile,iCoeEnv,iFeatureControl);
    }
    

void CCSXHHTMLContentParser::ScanAndParseXMLfileToCreateTOC1ObjectL(RFs& FileSession,
                                                                 CDirScan* scanner,
                                                                 CCSXHHelpDataBase* aDataBase,
                                                                 const TInt& aDrive,
                                                                 CCSXHXMLParseHandler* XMLParser
                                                                  )
    {
     CDir* entryList = NULL;
    scanner->NextL(entryList);
    if(!entryList)
        return;
 
    CleanupStack::PushL(entryList);
    TInt entryCount = entryList->Count();           
     
    TLinearOrder<CCSXHHelpContentBase> anOrder(Orderer<CCSXHHelpContentBase>); 
    
    TBuf<KMaxFileName> lookup;
    

    while(entryCount--)
        {
        TEntry entry=(*entryList)[entryCount];
        
        //Clear the previous values
        lookup.Delete(0,lookup.Length());
        
        lookup.Append(scanner->FullPath());
        lookup.Append(entry.iName); 
        lookup.Append(KFwdSlash);
        lookup.Append(KMetaFile);
        
        //Check whether a file exists        
        if(!BaflUtils::FileExists(FileSession,lookup))
            continue;
        
        TRAPD(parserResult,iXmlReader->ParseL (FileSession,lookup ));
        if(parserResult == KErrNone)
            {
			if(CheckFeatureIDL(XMLParser->GetFeatureIds()))
            	{            
            	if(!IsAppUIdPresentAlready(entry.iName))
					{
		            CCSXHHtmlTOC1* CategoryObj = CCSXHHtmlTOC1::NewL(
		                                        XMLParser->GetApplicationName(),entry.iName,aDrive);
		            iIndexing->RuntimeGenerateIndexL( *CategoryObj, XMLParser->GetFeatureIds() );
		            if(CategoryObj && aDataBase->GetMainTopics()->InsertChild(CategoryObj,EFalse))
		            	iHtmlTOC1List.Append(CategoryObj);//Keep a local copy*/ 
					else
						{
						if(CategoryObj)  
		            		iDuplicateHelpTopicList.Append(CategoryObj);	
						}			            	
					}
            	}         
            }
        }       

    CleanupStack::PopAndDestroy(entryList);
    }
    
void CCSXHHTMLContentParser::InsertHTMLToc1L(
            const TDesC &appUidName,const TDesC &appName, 
            const TInt& aDrive , CCSXHHelpDataBase* aDataBase, const TDesC &FeatureIds)
    {
	if(CheckFeatureIDL(FeatureIds))
	    {
	    if(!IsAppUIdPresentAlready(appUidName)) 
		    {    
			CCSXHHtmlTOC1* CategoryObj = CCSXHHtmlTOC1::NewL(appName,appUidName,aDrive);
		    if(CategoryObj && aDataBase->GetMainTopics()->InsertChild(CategoryObj,EFalse))
		    	iHtmlTOC1List.Append(CategoryObj);//Keep a local copy*/ 
		    else
		    	{
		    	if(CategoryObj)  
	           		iDuplicateHelpTopicList.Append(CategoryObj);			    	
		    	}
		    }
	    }	
    }
    
HBufC8* CCSXHHTMLContentParser::GetContentsFromFileL(const TDesC& htmlFile,CCoeEnv* aCoeEnv,
			RFeatureControl& aFeatureControl) 
    {
    RFs& fileSession = aCoeEnv->FsSession();        
    TInt SlashPosition = htmlFile.LocateReverse('\\');    
    TBuf<KMaxFileName>* CompressedFile = new (ELeave) TBuf<KMaxFileName>; 
    CleanupStack::PushL(CompressedFile); //1
    CompressedFile->Copy(htmlFile.Mid(0,SlashPosition+1));
    CompressedFile->Append(KContentZipFile);    
    
    HBufC8* CssContent = CreateBufferForCSSContentL(aFeatureControl);
       
    if(!BaflUtils::FileExists(fileSession,*CompressedFile))
        {
        CleanupStack::PopAndDestroy(CompressedFile);
        
        HBufC8* buffer = GetContentsFromHTMLFileL(htmlFile,aCoeEnv);
        
        return MergeCssAndHTMLContentL(buffer,CssContent);
        }
        
    // Create an instance of CZipFile.
    CZipFile* zipFile = CZipFile::NewL(fileSession, *CompressedFile);    
    CleanupStack::PushL(zipFile); //2
        
    // Iterate and get the file name
    CZipFileMemberIterator* members = zipFile->GetMembersL();
    CleanupStack::PushL(members);//3
    CZipFileMember* member = members->NextL(); // Get the first file in zip
    
    TInt nCount=0;    
    const TBuf<KMaxFileName>*Fname;
    TBuf<KMaxFileName> *HtmlFileName = new (ELeave) TBuf<KMaxFileName>;
    CleanupStack::PushL(HtmlFileName); //4
    HtmlFileName->Copy(htmlFile.Mid(SlashPosition+1));
    for(nCount=0;member!=NULL;nCount++)
    	{    	 
    	 Fname = member->Name();    
	     if(Fname->Compare(*HtmlFileName)==0 )
	    	{
	    	break;
	    	}	    
	    delete member;	
    	member = members->NextL();    	
    	}

	if(NULL == member)
		{
		CleanupStack::PopAndDestroy(HtmlFileName);
		CleanupStack::PopAndDestroy(members);
	    CleanupStack::PopAndDestroy(zipFile);
	    CleanupStack::PopAndDestroy(CompressedFile);
	    
	    HBufC8* buffer = GetContentsFromHTMLFileL(htmlFile,aCoeEnv);
	    return MergeCssAndHTMLContentL(buffer,CssContent);
	    }
		
    CleanupStack::PushL(member);//5
    RZipFileMemberReaderStream* stream;
    zipFile->GetInputStreamL(member, stream);
    CleanupStack::PushL(stream);//6
        
    // Extracts aFileName to a buffer.
    // If the file is quite huge, then read the file in streaming mode.
    // For example, use 4KB buffer and read it in an active object.
    HBufC8* buffer = HBufC8::NewLC(member->UncompressedSize());//5
    TPtr8 bufferPtr(buffer->Des());
    User::LeaveIfError(stream->Read(bufferPtr, member->UncompressedSize()));

    // Release all the resources.   
    CleanupStack::Pop(buffer);
    CleanupStack::PopAndDestroy(stream); 
    CleanupStack::PopAndDestroy(member);
    CleanupStack::PopAndDestroy(HtmlFileName);
    CleanupStack::PopAndDestroy(members);
    CleanupStack::PopAndDestroy(zipFile);
    CleanupStack::PopAndDestroy(CompressedFile);
    
    return MergeCssAndHTMLContentL(buffer,CssContent);
    }

TBool CCSXHHTMLContentParser::HandleMasterKeywordFileL(CCSXHHelpDataBase* aDataBase)
    {
    RFs& FileSession = iCoeEnv->FsSession();
    CCSXHXMLParseHandler_MasterKywd* XMLParser_MasterKywd = CCSXHXMLParseHandler_MasterKywd::NewL(iCoeEnv);  
    CleanupStack::PushL(XMLParser_MasterKywd);  
    InitializeReaderL(XMLParser_MasterKywd);
    XMLParser_MasterKywd->SetHtmlContentParserPtr(this);
    XMLParser_MasterKywd->SetDataBasePtr(aDataBase);
    TBool bMasterKeywordFilePresent = EFalse;
    
    //Check for the availability of Master Keyword file
    TBuf<KMaxFileName>* Master_kywdPath = new(ELeave) TBuf<KMaxFileName>;
    CleanupStack::PushL(Master_kywdPath);//5
    Master_kywdPath->Append(PathInfo::RomRootPath().Left(1));
    GetHTMLContentPathForDriveL(Master_kywdPath,iCoeEnv);
    Master_kywdPath->Append(KMasterKywdFile);
    
    if(BaflUtils::FileExists(FileSession,*Master_kywdPath))
    	{    	
        TRAP_IGNORE(iXmlReader->ParseL(FileSession,*Master_kywdPath ));
        bMasterKeywordFilePresent = ETrue;
    	}    
    
    //KMasterKywdFile;
    CleanupStack::PopAndDestroy(Master_kywdPath);
    CleanupStack::PopAndDestroy(XMLParser_MasterKywd);
    ClearReader();	
    
    return bMasterKeywordFilePresent;
    }
    
void CCSXHHTMLContentParser::GenerateKywdTOC1ListL(CCSXHHelpDataBase* aDataBase)
    {
    if(!iHtmlTOC1List.Count())
        return;
    TBool bMasterKeywordFilePresent = HandleMasterKeywordFileL(aDataBase);
    
    CCSXHXMLParseHandler_Kywd* XMLParser = CCSXHXMLParseHandler_Kywd::NewL(iCoeEnv);  
    CleanupStack::PushL(XMLParser);  
    InitializeReaderL(XMLParser);
    
    XMLParser->SetDataBasePtr(aDataBase);
    XMLParser->SetIsTOC2(EFalse);
    XMLParser->SetHtmlContentParserPtr(this);
    
    //Parse keyword.xml file from Main Array list
    ParseKeywdFileAndCreatekywdTOC1ObjectsL(iHtmlTOC1List,XMLParser,bMasterKeywordFilePresent);
    
    //Parse keyword.xml file from duplicate Array list
    ParseKeywdFileAndCreatekywdTOC1ObjectsL(iDuplicateHelpTopicList,XMLParser,bMasterKeywordFilePresent);
    
    CleanupStack::PopAndDestroy(XMLParser);
    ClearReader();
    }
    
void CCSXHHTMLContentParser::GenerateTOC2ListForKeywordSearchL
(CCSXHHelpDataBase* aDataBase,CCSXHKywdTOC1* aKywdTOC1Object)
    {
    RPointerArray<CCSXHHtmlTOC1>* TOC1HtmlList = aKywdTOC1Object->GetHtmlTOC1List();
    if(!TOC1HtmlList)
        return;
    
    CCSXHXMLParseHandler_Kywd* XMLParser = CCSXHXMLParseHandler_Kywd::NewL(iCoeEnv);
    CleanupStack::PushL(XMLParser);      
    InitializeReaderL(XMLParser);
	
    int toc1Count = TOC1HtmlList->Count();
    CCSXHHtmlTOC1* toc1;

    RFs& FileSession = iCoeEnv->FsSession();
    
    XMLParser->SetDataBasePtr(aDataBase);
    XMLParser->SetIsTOC2(ETrue);
    XMLParser->SetHtmlContentParserPtr(this);
    
    TBuf<KMaxFileName>* lookup = new(ELeave)TBuf<KMaxFileName>;
    CleanupStack::PushL(lookup);//3
    XMLParser->SetTOC1Title(aKywdTOC1Object->GetName());
    for(int i = 0; i < toc1Count ; ++i )
        {
        toc1 = (*TOC1HtmlList)[i];
        //Get Corresponding toc1 from Main array and set toc1 object
        XMLParser->SetCurrentHtmlToc1(GetCorrespondingTOC1FromMainArray(toc1->GetName()));
        //Clear the previous values
        lookup->Delete(0,lookup->Length());
        toc1->GetHelpFileL(*lookup);
        XMLParser->SetPath(*lookup);
        lookup->Append(KKeywordsFile);
        TRAP_IGNORE(iXmlReader->ParseL (FileSession,*lookup ));
        aDataBase->IncrementKeywordSearchCount();
        }   
    CleanupStack::PopAndDestroy(lookup);
    CleanupStack::PopAndDestroy(XMLParser);  
    ClearReader();
    }
    
CCSXHHelpContentBase* CCSXHHTMLContentParser::GetContextTopic
(const TUid& aUid, const TDesC& contextName)
    {
    int toc1Count = iHtmlTOC1List.Count();
    CCSXHHtmlTOC1* toc1,*temptoc1;
    
    for(int i = 0; i < toc1Count; ++i)
        {
        toc1 = iHtmlTOC1List[i];
        if(aUid == toc1->GetAppUid())
            {
            return toc1->GetContextTopic(contextName);      
            }
        }
        
    toc1Count = iDuplicateHelpTopicList.Count();
	for(int i = 0; i < toc1Count; ++i)
		{
		toc1 = iDuplicateHelpTopicList[i];
        if(aUid == toc1->GetAppUid())
        	{
    		temptoc1 = GetCorrespondingTOC1FromMainArray(toc1->GetName());        		
    		return temptoc1->GetContextTopic(contextName);
        	}
		}    
    return NULL;
    }
    
CCSXHHelpContentBase* CCSXHHTMLContentParser::GetHtmlTopicForUrlL(const TDesC& aUrl)
    {
    CCSXHHelpContentBase* tocobj = GetObjectBasedonUrlL(iHtmlTOC1List,aUrl,ETrue);
    
    if(NULL == tocobj)
    	tocobj = GetObjectBasedonUrlL(iDuplicateHelpTopicList,aUrl,EFalse);
    
	return tocobj;    	
    }

CCSXHHelpContentBase* CCSXHHTMLContentParser::GetObjectBasedonUrlL(RPointerArray<CCSXHHtmlTOC1>& aTOC1ObjectsArray,
		const TDesC& aUrl, TBool aMainArrayList)    
    {
        /*For URLs of form 
        file:://<Drive Name>:/system/xhtml/<Lang ID>/<App UID>/Html name
      We can have help topics for other cases, help topic objects not possible   
    */
    TBuf<KMaxFileName> aFileUrl;
    
    int toc1Count = aTOC1ObjectsArray.Count();
    CCSXHHtmlTOC1* toc1,*temptoc1;
    for(int j = 0; j < toc1Count; ++j)
        {
        toc1 = aTOC1ObjectsArray[j];
        TBuf<KMaxFileName> htmlFile;
        toc1->GetHelpFileL(htmlFile);
        
        //Convert fwd slashes to back slashes
        TInt i = htmlFile.Find(KFwdSlash) ;
        while (KErrNotFound != i)
            {
            htmlFile.Replace(i,1,KBackSlash);
            i = htmlFile.Find(KFwdSlash) ;
            }
        
        i = aUrl.FindC(htmlFile);
        if(i != KErrNotFound)
            {
            TFileName fileName;
            i = aUrl.LocateReverseF('/');
            fileName = aUrl.Mid(i + 1);
            if(aMainArrayList)
            	temptoc1 = toc1;
            else
            	temptoc1 = GetCorrespondingTOC1FromMainArray(toc1->GetName());	
            
            return temptoc1->GetHtmlTopicForFile(fileName);
            }
        }
    return NULL;	
    }

void CCSXHHTMLContentParser::GetHtmlFileL(CCoeEnv* coeEnv,const short& aDir,
                                            const TAppUid& aUid,TBuf<KMaxFileName>& aFileName)
    {
    aFileName.Copy(KEmptyString);
    
    RFs& FileSession = coeEnv->FsSession();
    TDriveList DirList; 
    if( (FileSession.DriveList(DirList)<KErrNone) ||
        (aDir >= DirList.Length()))
        return; 
    
    TChar driveLetter = '?';
    TDriveInfo info;
    TInt error = FileSession.Drive(info, aDir);
    if  ((error == KErrNone) && 
        (DirList[aDir] && info.iType != EMediaNotPresent))
        {
        RFs::DriveToChar(aDir, driveLetter);

        aFileName.Append(driveLetter);
        GetHTMLContentPathForDriveL(&aFileName,coeEnv);
        aUid.AppendUid(aFileName);
        aFileName.Append(KFwdSlash);
        }
    }

void CCSXHHTMLContentParser::InitializeReaderL(CCSXHXMLParseHandler* aXMLParser)
    {
    if(iXmlReader == NULL )
        {       
        iXmlReader = CSenXmlReader::NewL();
        iXmlReader->SetContentHandler(*aXMLParser);
        }
    }
void CCSXHHTMLContentParser::ClearReader()
    {
    delete iXmlReader;
    iXmlReader = NULL;
    }
    
TBool CCSXHHTMLContentParser::IsUidCategoryPresent(const TUid& aUid)
    {
    int toc1Count = iHtmlTOC1List.Count();
    CCSXHHtmlTOC1* toc1;    
    for(int i = 0; i < toc1Count; ++i)
        {
        toc1 = iHtmlTOC1List[i];
        if(aUid == toc1->GetAppUid())
            return ETrue;
        }
    
    toc1Count = iDuplicateHelpTopicList.Count();
    for(int i = 0; i < toc1Count; ++i)
        {
        toc1 = iDuplicateHelpTopicList[i];
        if(aUid == toc1->GetAppUid())
            return ETrue;
        }    
    return EFalse;  
    }

TBool CCSXHHTMLContentParser::GetHTMLToc1(const TDesC& aUid,CCSXHXMLParseHandler* aParser)
	{
	TUint AppId;
	
	if(!aUid.Length())
		return EFalse;

    TLex FolderUid(aUid.Mid(KOffset));
    FolderUid.Val(AppId,EHex);
    TUid ApplicationUid = TUid::Uid((TInt)AppId);
	
	int toc1Count = iHtmlTOC1List.Count();
	
    CCSXHHtmlTOC1* toc1;    
    for(int i = 0; i < toc1Count; ++i)
        {
        toc1 = iHtmlTOC1List[i];
        if(ApplicationUid == toc1->GetAppUid())
        	{
        	STATIC_CAST(CCSXHXMLParseHandler_MasterKywd*,aParser)->SetCurrentHtmlToc1(toc1); 
        	return ETrue;
        	}            
        }
	
	toc1Count = iDuplicateHelpTopicList.Count();	        
	for(int i = 0; i < toc1Count; ++i)
        {
        toc1 = iDuplicateHelpTopicList[i];
        if(ApplicationUid == toc1->GetAppUid())
        	{
        	STATIC_CAST(CCSXHXMLParseHandler_MasterKywd*,aParser)->SetCurrentHtmlToc1(toc1); 
        	return ETrue;
        	}            
        }
                
    return EFalse;  
	}

//To get Corresponding toc1 from main array.
CCSXHHtmlTOC1* CCSXHHTMLContentParser::GetCorrespondingTOC1FromMainArray(const TDesC& aApplicationName)
	{
	TInt toc1Count = iHtmlTOC1List.Count();
	CCSXHHtmlTOC1* toc1;
	for(int i=0;i<toc1Count;++i)
		{
		toc1 = iHtmlTOC1List[i];
		if(toc1->GetName().CompareF(aApplicationName) == 0)
			return toc1;
		}
	
    return NULL;
	}

//Check UId is already present in the list.	
TBool CCSXHHTMLContentParser::IsAppUIdPresentAlready(const TDesC& aUid)
	{
	TBool result = EFalse;
	TInt toc1Count = iHtmlTOC1List.Count();
	CCSXHHtmlTOC1* toc1;
	
	TUint AppId;
    TLex FolderUid(aUid.Mid(KOffset));
    FolderUid.Val(AppId,EHex);
    TUid ApplicationUid = TUid::Uid((TInt)AppId);
	
	for(int i=0;i<toc1Count;++i)
		{
		toc1 = iHtmlTOC1List[i];
		if(ApplicationUid == toc1->GetAppUid())
			{
			result = ETrue;
			break;
			}			
		}
		
	toc1Count =    iDuplicateHelpTopicList.Count();	
	for(int i=0;i<toc1Count;++i)
		{
		toc1 = iDuplicateHelpTopicList[i];
		if(ApplicationUid == toc1->GetAppUid())
			{
			result = ETrue;
			break;
			}
		}		
		
	return result;	

	}
	
void CCSXHHTMLContentParser::ParseKeywdFileAndCreatekywdTOC1ObjectsL(
		RPointerArray<CCSXHHtmlTOC1>& aTOC1ObjectsArray,
		CCSXHXMLParseHandler_Kywd* XMLParser, TBool bMasterKeywordFilePresent)
	{
	int toc1Count = aTOC1ObjectsArray.Count();
    CCSXHHtmlTOC1* toc1;
     
    TBuf<KMaxFileName>* lookup = new(ELeave)TBuf<KMaxFileName>;
    CleanupStack::PushL(lookup);//1
    
    RFs& FileSession = iCoeEnv->FsSession();
	
	    for(int i = 0; i < toc1Count; ++i)
        {
        toc1 = aTOC1ObjectsArray[i];
        
        if(bMasterKeywordFilePresent && toc1->IsROMDrive())
        	continue;
        
        //Clear the previous values
        lookup->Delete(0,lookup->Length());
        
        toc1->GetHelpFileL(*lookup);
        lookup->Append(KKeywordsFile);
        
          //Check whether a file exists        
        if(!BaflUtils::FileExists(FileSession,*lookup))
            continue;
        
        XMLParser->SetCurrentHtmlToc1(toc1);
        TRAP_IGNORE(iXmlReader->ParseL(FileSession,*lookup ));
        }

    CleanupStack::PopAndDestroy(lookup);  
	}

//RUNTIME
void CCSXHHTMLContentParser::GetSupportedFeatureListL()
	{
	RFeatureUidArray SupportedFeatures;	
	TInt err = iFeatureControl.Connect();
	if ( err == KErrNone )
		{
         // ListSupportedFeatures() returns one of the Symbian error codes.
         err = iFeatureControl.ListSupportedFeatures( SupportedFeatures );
		 	
         // Remember to call CloseL after using RFeatureControl.
         // It disconnects the Feature Manager server.
         iFeatureControl.Close();  
		}
	//As per new req. add true, flase to id and create an array.
	
	iFeatureManager_FeatureIds = new (ELeave) CDesCArrayFlat(2); 
	
	TBuf<25>CurrFId;
	
	for(TInt i=0;i<SupportedFeatures.Count();i++)
		{
		CurrFId.Copy(KEmptyString);	
		CurrFId.AppendNum(SupportedFeatures[i].iUid);
		iFeatureManager_FeatureIds->AppendL(CurrFId);
		
		CurrFId.Copy(KTrue_StringtoAppend);	
		CurrFId.AppendNum(SupportedFeatures[i].iUid);
		iFeatureManager_FeatureIds->AppendL(CurrFId);
		
		CurrFId.Copy(KFalse_StringtoAppend);	
		CurrFId.AppendNum(SupportedFeatures[i].iUid);
		iFeatureManager_FeatureIds->AppendL(CurrFId);		
		}
		
	//Input Language variation changes
	//-----------------------------------------
	//Create an array with supported input language		
	CPtiEngine* ptiEngine = CPtiEngine::NewL();
    CleanupStack::PushL( ptiEngine );
    
    CArrayFix<TInt>* languageCodeArray = new(ELeave)CArrayFixFlat<TInt>(2);
    
    ptiEngine->GetAvailableLanguagesL( languageCodeArray );
	
	TInt nCount = languageCodeArray->Count();
	
	iSupportedInputLanguages = new (ELeave) CDesCArrayFlat(2); 
	
	//Codescanner gives error, if member variables are pushed.
	//CleanupStack::PushL( iSupportedInputLanguages );
    
    for(TInt i=0; i<nCount;i++)
    	{
    	TInt languageCode = languageCodeArray->At(i);
    	TBuf<25>Currlang(_L("LANGUAGE_"));
		if(languageCode < 10)
			Currlang.AppendNumFixedWidth(languageCode,EDecimal,2);
		else
			Currlang.AppendNum(languageCode);
		
		iSupportedInputLanguages->AppendL(Currlang);
    	}
    
    //CleanupStack::Pop(iSupportedInputLanguages);	
    CleanupStack::PopAndDestroy( ptiEngine );
    
    delete languageCodeArray;	
    //-----------------------------------------
    
    SupportedFeatures.Close();
	}
	
//RUNTIME
TBool CCSXHHTMLContentParser::CheckFeatureIDL(const TDesC& aFeatureIds)
	{
	if(/*KHexPrefixLength == aFeatureIds.Length() &&*/ 
		KErrNotFound != aFeatureIds.Find(KDefaultFeatureIdStringTemp))
		{
		return ETrue;
		}
			
	HBufC* Ids = HBufC::NewLC(aFeatureIds.Length());	
	TPtr CurrentFeatureIds = Ids->Des();	
	CurrentFeatureIds.Copy(aFeatureIds);
	CleanupStack::Pop(Ids);
		
	CDesCArray* FeatureIdList = new (ELeave) CDesCArrayFlat(2); ;
	TInt EndPos = CurrentFeatureIds.Locate(' ');
	
	CleanupStack::PushL(FeatureIdList);
				
    while (KErrNotFound != EndPos)
        {
				FeatureIdList->AppendL(CurrentFeatureIds.Mid(0,EndPos));// FId_Val);	        
        CurrentFeatureIds = CurrentFeatureIds.Mid(EndPos+1);
        EndPos = CurrentFeatureIds.Locate(' ') ;
        }

	if(KErrNotFound == EndPos && 0!= CurrentFeatureIds.Length())
		{
		FeatureIdList->AppendL(CurrentFeatureIds);
		}    
	
	CleanupStack::Pop(FeatureIdList); 
	 
	TInt position;		
	if(KErrNone == FeatureIdList->Find(KDefaultFeatureIdString,position) || 
	   KErrNone == FeatureIdList->Find(KDefaultFeatureIdStringTemp,position) )
		{
		delete FeatureIdList;
		delete Ids;
		return ETrue;
		}
		
	TInt nCount = FeatureIdList->MdcaCount();
	
	
	//FeatureManager list contains all enabled featureIds, appended with true, and false.
	//New Req. text associated with true_featureId, should be displayed, if Id is enabled.
	//else, if featureId is disabled, text associated with false_featureId should be displayed. 
	// 1. if featureId string contains "false" and it is not in the list, return ETrue.
	// 2. if featureId string does not contain "false" and it is present in the list, return ETrue.
	// 3. if featureId is part of input language, return ETrue.
	for(TInt j=0;j < nCount;j++)
		{
		if(
			(KErrNotFound != FeatureIdList->MdcaPoint(j).Find(KFalseString) &&
			KErrNone != iFeatureManager_FeatureIds->Find(FeatureIdList->MdcaPoint(j),position))||
		
			(KErrNotFound == FeatureIdList->MdcaPoint(j).Find(KFalseString) &&
			KErrNone == iFeatureManager_FeatureIds->Find(FeatureIdList->MdcaPoint(j),position)) ||
		
			KErrNone == iSupportedInputLanguages->Find(FeatureIdList->MdcaPoint(j),position)
		)						
			{
			delete FeatureIdList;
			delete Ids;
			return ETrue;
			}		
		}
				
	delete Ids;		
	delete FeatureIdList;	
	return EFalse;		
	}	
	
HBufC8* CCSXHHTMLContentParser::CreateBufferForCSSContentL(RFeatureControl& aFeatureControl)
	{	
	RFeatureUidArray SupportedFeatures;
	TInt err = aFeatureControl.Connect();
	if ( err == KErrNone )
		{
         // ListSupportedFeatures() returns one of the Symbian error codes.
         err = aFeatureControl.ListSupportedFeatures( SupportedFeatures );
		 	
         // Remember to call CloseL after using RFeatureControl.
         // It disconnects the Feature Manager server.
         aFeatureControl.Close();                    
		}
	
		//Input Language variation changes
	//-----------------------------------------

	CPtiEngine* ptiEngine = CPtiEngine::NewL();
    CleanupStack::PushL( ptiEngine );

    CArrayFix<TInt>* languageCodeArray = new(ELeave)CArrayFixFlat<TInt>(2);

    ptiEngine->GetAvailableLanguagesL( languageCodeArray );
    
	TInt nInputLangCount = languageCodeArray->Count();
	TInt nCount = SupportedFeatures.Count();	

	HBufC8* CssContent = HBufC8::NewLC(nCount * KMaxUnits * 6 + nInputLangCount * 3 + 400);
	TPtr8 bufferPtr(CssContent->Des());
	
	AppendStyleSheetContent_listitem(bufferPtr,nCount,SupportedFeatures,*languageCodeArray);
	AppendStyleSheetContent_paragraph(bufferPtr,nCount,SupportedFeatures,*languageCodeArray);
	AppendStyleSheetContent_ahref(bufferPtr,nCount,SupportedFeatures,*languageCodeArray);
	AppendStyleSheetContent_none(bufferPtr,nCount,SupportedFeatures);	
		
	SupportedFeatures.Close();				
	
	CleanupStack::Pop(CssContent);	
	
	//Input Language variation changes
	//-----------------------------------------
	CleanupStack::PopAndDestroy( ptiEngine );	

	delete languageCodeArray;
	//-----------------------------------------

	return CssContent;
	}
	
HBufC8* CCSXHHTMLContentParser::GetContentsFromHTMLFileL(const TDesC& htmlFile,CCoeEnv* aCoeEnv)
	{
	RFs& fsSession = aCoeEnv->FsSession();             
	RFile file; 
        
	TInt err = file.Open(fsSession,htmlFile,EFileRead|EFileShareReadersOrWriters);
    if(KErrNone == err)
	    {
	    TInt FileSize;
	    HBufC8* htmlFileContent = NULL;
	    if(KErrNone == file.Size(FileSize))
		    {
		    htmlFileContent = HBufC8::NewLC(FileSize);
			TPtr8 PtrContent = htmlFileContent->Des();
	        
		    file.Read(PtrContent);	
		    CleanupStack::Pop(htmlFileContent);			    
		    }		
	    file.Close();
		return htmlFileContent;
		}
	else
		{		
		return NULL;
		}
	}
	
HBufC8* CCSXHHTMLContentParser::MergeCssAndHTMLContentL(HBufC8* aHTMLBuffer, HBufC8* aCssContent)
	{
    TInt BufferLength=0;
    if(aHTMLBuffer)
    	BufferLength = aHTMLBuffer->Size();
    
    if(aCssContent)
    	BufferLength += aCssContent->Size();
    
    HBufC8* Htmlbuffer = HBufC8::NewLC(BufferLength);
    
    TPtr8 HtmlbufferPtr(Htmlbuffer->Des());
    
    if(aCssContent)
    	{
    	HtmlbufferPtr.Copy(aCssContent->Des());
    	delete aCssContent;
    	}
    	
    if(aHTMLBuffer)
	    {
	    HtmlbufferPtr.Append(aHTMLBuffer->Des());
	    delete aHTMLBuffer;	
	    }
    
    if(Htmlbuffer)
    	CleanupStack::Pop(Htmlbuffer);
    
    return Htmlbuffer;	
	}

void CCSXHHTMLContentParser::AppendStyleSheetContent_listitem(TPtr8& abufferptr,TInt aFeatureIdCount,
													RFeatureUidArray& aSupportedFeatures,
													CArrayFix<TInt>& alanguageCodeArray)
	{
	//List Items
	//---------------------------------------------------------
	abufferptr.Copy(KCsstext_First);
	
	for(TInt i=0;i<aFeatureIdCount;++i)
		{
		TBuf<KMaxFileName> FeatureId;
		FeatureId.Format(KtextFormat_true_l,aSupportedFeatures[i].iUid);
		//FeatureId.Append(KComma);
		abufferptr.Append(FeatureId);		
		}
		
	for(TInt i=0;i<alanguageCodeArray.Count();++i)
		{
		TBuf<KMaxFileName> FeatureId;
		if(alanguageCodeArray[i] < 10)
			FeatureId.Format(KtextFormat_lang_0l,alanguageCodeArray[i]);
		else
			FeatureId.Format(KtextFormat_lang_l,alanguageCodeArray[i]);
		
		abufferptr.Append(FeatureId);		
		}
		
	abufferptr.Append(KDefaultFeatureId);		
	abufferptr.Append(KCsstext_displayp_li);		
	abufferptr.Append(KCsstext_Last);
	//---------------------------------------------------------
	}

void CCSXHHTMLContentParser::AppendStyleSheetContent_paragraph(TPtr8& abufferptr,TInt aFeatureIdCount,
													RFeatureUidArray& aSupportedFeatures,
													CArrayFix<TInt>& alanguageCodeArray)
	{
	//Paragraph
	//---------------------------------------------------------
	abufferptr.Append(KCsstext_First);
	for(TInt i=0;i<aFeatureIdCount;++i)
		{
		TBuf<KMaxFileName> FeatureId;
		FeatureId.Format(KtextFormat_true_b,aSupportedFeatures[i].iUid);
		//FeatureId.Append(KComma);
		
		abufferptr.Append(FeatureId);		
		}
		
	for(TInt i=0;i<alanguageCodeArray.Count();++i)
		{
		TBuf<KMaxFileName> FeatureId;
		if(alanguageCodeArray[i] < 10)
			FeatureId.Format(KtextFormat_lang_0b,alanguageCodeArray[i]);
		else
			FeatureId.Format(KtextFormat_lang_b,alanguageCodeArray[i]);
		
		abufferptr.Append(FeatureId);		
		}
		
	abufferptr.Append(KDefaultFeatureId);
	abufferptr.Append(KCsstext_displayp_p);		
	abufferptr.Append(KCsstext_Last);
	
	//---------------------------------------------------------
	}

void CCSXHHTMLContentParser::AppendStyleSheetContent_ahref(TPtr8& abufferptr,TInt aFeatureIdCount,
												RFeatureUidArray& aSupportedFeatures,
												CArrayFix<TInt>& alanguageCodeArray)
	{
	//A href
	//---------------------------------------------------------
	abufferptr.Append(KCsstext_First);
	for(TInt i=0;i<aFeatureIdCount;++i)
		{
		TBuf<KMaxFileName> FeatureId;
		FeatureId.Format(KtextFormat_true_a,aSupportedFeatures[i].iUid);		
		abufferptr.Append(FeatureId);		
		}
	
	for(TInt i=0;i<alanguageCodeArray.Count();++i)
		{
		TBuf<KMaxFileName> FeatureId;
		if(alanguageCodeArray[i] < 10)
			FeatureId.Format(KtextFormat_lang_0a,alanguageCodeArray[i]);
		else
			FeatureId.Format(KtextFormat_lang_a,alanguageCodeArray[i]);
		abufferptr.Append(FeatureId);		
		}
		
	abufferptr.Append(KDefaultFeatureId);
	abufferptr.Append(KCsstext_displayp_a);		
	abufferptr.Append(KCsstext_Last);
	
	//---------------------------------------------------------
	}

void CCSXHHTMLContentParser::AppendStyleSheetContent_none(TPtr8& abufferptr,TInt aFeatureIdCount,
													RFeatureUidArray& aSupportedFeatures
													)
	{
	//False
	//---------------------------------------------------------
	abufferptr.Append(KCsstext_First);
	for(TInt i=0;i<aFeatureIdCount;++i)
		{
		TBuf<KMaxFileName> FeatureId;
		FeatureId.Format(KtextFormat_false,aSupportedFeatures[i].iUid,aSupportedFeatures[i].iUid,aSupportedFeatures[i].iUid);
		abufferptr.Append(FeatureId);		
		}
		
	abufferptr.Append(KDefaultFeatureId);
	abufferptr.Append(KCsstext_displayp_n);		
	abufferptr.Append(KCsstext_Last);
	
	//---------------------------------------------------------
	}
