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
* Description:  CCSXHHtmlTOC2 class definition
*
*/

#include "CSXHHtmlTOC2.h"
#include "CSXHHtmlTOC1.h"
#include "CSXHViewIDs.h"
#include "CSXHHTMLContentParser.h"
#include "CSXHHelpDataBase.h"
#include "csxhconstants.h"

#include <e32base.h>
#include <zipfile.h>
#include <bautils.h>
#include <coehelp.h>

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHtmlTOC2* CCSXHHtmlTOC2::NewL(CCSXHGenericTOC1* aParent,const TDesC& aName, 
                                   const TDesC& aHtmlHelpFile, TBool aKeyWordTopic )
    {
    CCSXHHtmlTOC2* self = CCSXHHtmlTOC2::NewLC(aParent, aName,aHtmlHelpFile,aKeyWordTopic);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHtmlTOC2* CCSXHHtmlTOC2::NewLC(CCSXHGenericTOC1* aParent, const TDesC& aName, 
                                    const TDesC& aHtmlHelpFile, TBool aKeyWordTopic)
    {
    CCSXHHtmlTOC2* self = new(ELeave) CCSXHHtmlTOC2(aParent, aName,aHtmlHelpFile,aKeyWordTopic);
    CleanupStack::PushL(self);
    return self;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHtmlTOC2::~CCSXHHtmlTOC2()
    {//No implementation required
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHHtmlTOC2::CCSXHHtmlTOC2(CCSXHGenericTOC1* aParent,const TDesC& aName, 
                            const TDesC& aHtmlHelpFile, TBool aKeyWordTopic) :
                            CCSXHGenericTOC2(aParent, aName),iHTMLFileName(aHtmlHelpFile),
                            iKeyWordTopic(aKeyWordTopic),iKywdParent(NULL)
    {//No implementation required
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
TUid CCSXHHtmlTOC2::GetViewID() const
    {
    return KCSXHHtmlTopicViewID;
    }   
   
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
const TBuf<KMaxFileName>&  CCSXHHtmlTOC2::GetFileName() const
    {
    return iHTMLFileName;
    }
   
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
EXPORT_C TBool CCSXHHtmlTOC2::GetHelpContext(TCoeHelpContext& aContext) const
    {
    
    CCSXHHtmlTOC1* parent = STATIC_CAST(CCSXHHtmlTOC1*,iParent);
    aContext.iMajor = parent->GetAppUid();
    if( KErrNotFound != iHTMLFileName.Find(KHtmlFileExtn) )
        {
        aContext.iContext.Append(iHTMLFileName.Left(iHTMLFileName.Find(KHtmlFileExtn)));
        }
#ifdef FF_HELP_TUTORIAL_MERGE
     else if( KErrNotFound != iHTMLFileName.Find(KTutorialFileExtn) )
        {
        aContext.iContext.Append(iHTMLFileName.Left(iHTMLFileName.Find(KTutorialFileExtn)));
        }
#endif // FF_HELP_TUTORIAL_MERGE
    return ETrue;
    }
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
EXPORT_C void CCSXHHtmlTOC2::GetHtmlUrlL(TBuf<KMaxFileName>& aFileUrl) const
    {
    aFileUrl = KFileUrlPrefix;
    
    aFileUrl.Append(iHTMLFilePath);
    aFileUrl.Append(iHTMLFileName);
    
    TInt pos = aFileUrl.Find(KFwdSlash) ;
    while (KErrNotFound != pos)
        {
        aFileUrl.Replace(pos,1,KBackSlash);
        pos = aFileUrl.Find(KFwdSlash) ;
        }
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
EXPORT_C TAny* CCSXHHtmlTOC2::GetTopicContentL()
    {
    return CCSXHHelpDataBase::GetInstance()->GetHtmlParser()->GetTopicContentL(this);
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
EXPORT_C HBufC8* CCSXHHtmlTOC2::GetContentsFromUrlL(const TDesC&  fileUrl,CCoeEnv* aCoeEnv, 
	RFeatureControl& aFeatureControl)
    {
    TBuf<KMaxFileName> htmlFile(fileUrl);
    htmlFile.Delete(0,KFileUrlPrefixLength);//Remove "file:///"
    //Replace "/" with "\\"
    TInt pos = htmlFile.Locate('/') ;
    while (KErrNotFound != pos)
        {
        htmlFile.Replace(pos,1,KFwdSlash);
        pos = htmlFile.Locate('/') ;
        }
    return CCSXHHTMLContentParser::GetContentsFromFileL(htmlFile,aCoeEnv,aFeatureControl);
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
EXPORT_C CCSXHKywdTOC1* CCSXHHtmlTOC2::GetKywdParent()
    {
    return iKywdParent;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
void CCSXHHtmlTOC2::SetKywdParent(CCSXHKywdTOC1* aKywdParent)
    {
    iKywdParent = aKywdParent;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
void CCSXHHtmlTOC2::SetHtmlFilePath(const TDesC&  aFilePath)
	{
	iHTMLFilePath = aFilePath;
	}

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    	
void CCSXHHtmlTOC2::GetHtmlFileName(TBuf<KMaxFileName>&  aFileName)	
	{
	aFileName.Copy(KEmptyString);
	aFileName.Append(iHTMLFilePath);	
	aFileName.Append(iHTMLFileName);	
	}
