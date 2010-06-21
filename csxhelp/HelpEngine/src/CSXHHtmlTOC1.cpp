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
* Description:  CCSXHHtmlTOC1 class definition
*
*/

#include "CSXHHtmlTOC1.h"
#include "CSXHHtmlTOC2.h"
#include "CSXHViewIDs.h"
#include "CSXHHTMLContentParser.h"
#include "CSXHHelpDataBase.h"
#include "csxhconstants.h"

#include <eikenv.h> 
#include <utf.h>
#include <data_caging_path_literals.hrh>
#include <zipfile.h>
#include <bautils.h>
#include <SenXmlReader.h> 
#include <f32file.h>
#include <pathinfo.h>

const TInt KBufSize = 35;

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHtmlTOC1* CCSXHHtmlTOC1::NewL(const TDesC& aName, const TDesC& aFolder,const TInt& aDrive, TUid aViewId, TInt32 aPriority)
    {
    CCSXHHtmlTOC1* self = new (ELeave) CCSXHHtmlTOC1(aName, aFolder, aDrive, aViewId, aPriority);
    return self;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHtmlTOC1::CCSXHHtmlTOC1(const TDesC& aName, const TDesC& aFolder,const TInt& aDrive, TUid aViewId, TInt32 aPriority):
                    CCSXHGenericTOC1(aName, aPriority), iAppUid(aFolder), iDrive(aDrive), iViewId(aViewId)
    {//No implementation required
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHtmlTOC1::~CCSXHHtmlTOC1()
    {//No implementation required
    }   

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
TUid CCSXHHtmlTOC1::GetViewID() const
    {
    return iViewId;
    }
    
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
void CCSXHHtmlTOC1::GetHelpFileL(TBuf<KMaxFileName>&  aFileName ) const
    {
    //return iHelpFile;
    return CCSXHHTMLContentParser::GetHtmlFileL(
                CCSXHHelpDataBase::GetInstance()->GetCoeEnv(),iDrive,iAppUid,aFileName);
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
const TUid& CCSXHHtmlTOC1::GetAppUid() const
    {
    return iAppUid.GetUid();
    }
    
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
TBool CCSXHHtmlTOC1::IsROMDrive() 
    {
    TInt ROMDrive;
    RFs::CharToDrive( PathInfo::RomRootPath()[0], ROMDrive );
    if(ROMDrive == iDrive)
    	return ETrue;
    else
    	return EFalse;
    }
    
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHelpContentBase* CCSXHHtmlTOC1::GetContextTopic(const TDesC&  contextName)
    {
    TBool resetChildList = EFalse;
    if(iChildList == NULL)
        {
        InitChildList();//Generates child list  
        resetChildList = ETrue;
        }
    
    TInt count =    iChildList->Count();
    CCSXHHtmlTOC2* toc2 = NULL;
    for(TInt i=0;i<count;++i )
        {
        toc2 = STATIC_CAST(CCSXHHtmlTOC2*, (*iChildList)[i]);
        TBuf<KBufSize> contextFile = contextName;
        contextFile.Append(KHtmlFileExtn);
        if(contextFile.Compare(toc2->GetFileName()) == 0)
            return toc2;//Child list should not be reset
        }
    if(resetChildList)
        ResetChildList();
    return NULL;    
    }

// ---------------------------------------------------------
// Items commented in header.
// --------------------------------------------------------- 
CCSXHHelpContentBase* CCSXHHtmlTOC1::GetHtmlTopicForFile(const TDesC&  aFile)
    {
    TBool resetChildList = EFalse;
    if(iChildList ==NULL)
        {
        InitChildList();//Generates child list if required.     
        resetChildList = ETrue;
        }
        
    TInt count = iChildList->Count();
    CCSXHHtmlTOC2* toc2 = NULL;
    for(TInt i=0;i<count;++i )
        {
        toc2 = STATIC_CAST(CCSXHHtmlTOC2*, (*iChildList)[i]);
        if(toc2->GetFileName().Compare(aFile) == 0)
            return toc2;//Child list should not be reset
        }
    if(resetChildList)
        ResetChildList();
    return NULL;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
void CCSXHHtmlTOC1::InitChildList()
    {
    if(iChildList != NULL)
        return;
    ConstructChildList();
    TRAP_IGNORE(CCSXHHelpDataBase::GetInstance()->GetHtmlParser()->
                                                        GenerateTOC2ListL(*this,iChildList));
    }
TBool CCSXHHtmlTOC1::IsValid() const
	{
	return iAppUid.IsValid();
	}
	
	
TAppUid::TAppUid(const TDesC& aFolder)
	{
	TLex lex(aFolder.Mid(KHexPrefixLength));
	TUint appId = 0;
	lex.Val(appId,EHex);
	
	if(appId)
		{
		iUid = TUid::Uid((TInt)appId);
		iLength = aFolder.Length();		
		}
	else
		{
		iLength = 0;
		}
	}

const TUid& TAppUid::GetUid() const
	{
	return iUid;
	}

TBool TAppUid::IsValid() const
	{
	return iLength ? ETrue : EFalse;
	}
void TAppUid::AppendUid(TBuf<KMaxFileName>& aStr) const
	{
	TInt initialLen = aStr.Length();
	aStr.Append(KHexPrefix);
	aStr.AppendNum(iUid.iUid,EHex);
	TInt finalLen = aStr.Length();
	
	while(iLength > (finalLen - initialLen))
		{
		aStr.Insert(initialLen+KHexPrefixLength,KZero);
		++finalLen;
		}
	
	}

