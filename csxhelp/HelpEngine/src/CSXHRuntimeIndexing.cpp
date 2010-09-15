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
* Description:  CCSXHRuntimeIndexing class definition
*
*/


#include <pathinfo.h>
#include <utf.h>
#include <bautils.h>
#include <eikenv.h>
#include <aknnotedialog.h> // for CAknNoteDialog
#include <aknnotewrappers.h>
#include <SenXmlUtils.h>
#include <SenXmlConstants.h>

#include "CSXHRuntimeIndexing.h"
#include "CSXHHtmlTOC1.h"
#include "csxhconstants.h"

// buffer length of Drive information in index file
//
const TInt KMaxDriveInfo = 100;

CCSXHRuntimeIndexing::CCSXHRuntimeIndexing()
    {
    iCoeEnv = CCoeEnv::Static();
    }
    
CCSXHRuntimeIndexing::~CCSXHRuntimeIndexing()
    {
    delete iFullDirName;
    iFile.Close();
    }
    
CCSXHRuntimeIndexing* CCSXHRuntimeIndexing::NewL()
    {
    CCSXHRuntimeIndexing* self = new ( ELeave ) CCSXHRuntimeIndexing();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
void CCSXHRuntimeIndexing::ConstructL()
    {
    RFs& fileSession = iCoeEnv->FsSession();
    iFullDirName = HBufC::NewL( KMaxFileName );

    TFileName fullDirName;
    
    GetPrivatePath( fullDirName );
    if ( !BaflUtils::PathExists( fileSession, fullDirName ) )
        {
        fileSession.CreatePrivatePath( EDriveC );
        }
    
    TInt len = fullDirName.Length();
    TDriveList dirList; 
    if( fileSession.DriveList( dirList ) == KErrNone )
        {
        TDriveInfo info;
        TInt driveListLen = dirList.Length();
        
        for ( TInt i = 0; i < driveListLen; i++ )
            {
            // make separate folder for C/D/E/... drive
            // e.g. C:\\private\\10005234\\C\\

            TInt err = fileSession.Drive( info, i );
            if ( err == KErrNone &&
                    info.iType != EMediaNotPresent )
                {
                TChar driveChar;
                RFs::DriveToChar( i, driveChar );
                fullDirName.Append( driveChar );
                fullDirName.Append( KFwdSlash );
                fileSession.MkDir( fullDirName );
                fullDirName.Delete( len, 3 );
                }
            }
        
        }
    }
    
TBool CCSXHRuntimeIndexing::IndexFileExistsL()
    {
    RFs& fileSession = iCoeEnv->FsSession();
    TBuf<KMaxFileName> fileName;
    GetPrivatePath( fileName );
    fileName.Append( _L("Z\\") );
    AppendLocaleL( fileName );
    fileName.Append( KMasterMetaFile );
    
    // Now the dirName is C:\\private\\<app-uid>\\Z\\index.xml
    // check if it exists, if yes, runtime indexing must be launched before
    // since Z based Help content must be delivered with cell phone
    //
    return BaflUtils::FileExists( fileSession, fileName );
    
    }
    
void CCSXHRuntimeIndexing::BeginIndexFileL( const TChar& aDrive )
    {
    RFs& fileSession = iCoeEnv->FsSession();
    TPtr des = iFullDirName->Des();
    iDriveLetter = aDrive;
    des.Zero();
    GetPrivatePath( des );
    des.Append( aDrive );
    des.Append( KFwdSlash );
    AppendLocaleL( des );
    if ( !BaflUtils::PathExists( fileSession, des ) )
        {
        // generate C:\\private\\<help-uid>\\<aDrive>\\ folder
        //
        fileSession.MkDir( des );
        }
    
    // for temporary usage, once it's finished successfully,
    // rename to index.xml. otherwise, the old index file still exists.
    //
    des.Append( KMasterMetaTmpFile );
    TInt err = KErrNone;
    err = iFile.Replace( fileSession, des, EFileWrite );
    if ( KErrNone != err || 0 == iFile.SubSessionHandle() )
        {
        return;
        }
    
    HBufC8* driveInfo = HBufC8::NewLC( KMaxDriveInfo );
    TPtr8 ptr = driveInfo->Des();
    ptr.Append( KIndexXMLVesion );
    ptr.Append( _L( "<collections drive=\"" ) );
    ptr.Append( aDrive);
    ptr.Append( _L( "\">\r\n" ) );
    iFile.Write( *driveInfo );
    CleanupStack::PopAndDestroy( driveInfo );
    }
    
void CCSXHRuntimeIndexing::FinishAndCloseIndexFileL()
    {
    if ( 0 == iFile.SubSessionHandle() )
        {
        return;
        }
    RFs& fileSession = iCoeEnv->FsSession();
    iFile.Write( KIndexCollectEnd );
    iFile.Close();  
      
    TTime time;
    // Replace the index.xml with temporary file, in case it fails, will have no impact
    // the original index.xml, it's a backup solution
    //
    TBuf<KMaxFileName> newName;
    TInt err = KErrNone;
    newName.Copy( *iFullDirName );
    TInt pos = newName.LocateReverse( TChar('\\') );
    newName.Delete( pos + 1, newName.Length() - pos - 1 ); // remove the temp name
    newName.Append( KMasterMetaFile ); //  and append index.xml
    
    err = fileSession.Replace( *iFullDirName, newName );
    
    if ( err == KErrNone )
        {
        // set the modified date of master index.xml
        // to be the same with related help content folder
        // e.g. C:\\resource\\xhtml\\01\\ modified time is 2009.3.1
        // C:\\private\\10005234\\C\\01\\index.xml will be 2009.3.1
        // this will make sure once help content folder changed, index.xml can be
        // determined to change.
        TBuf<KMaxFileName> helpDir;
        helpDir.Append( iDriveLetter );  
        helpDir.Append( KInstallPath );
        // append locale information
        AppendLocaleL( helpDir );
        
        fileSession.Modified( helpDir, time );
        err = fileSession.SetModified( newName, time );
        
        }
    
    }
      
void CCSXHRuntimeIndexing::RuntimeGenerateIndexL( const CCSXHHtmlTOC1& aToc1, const TDesC& aFeature )
    {
    if ( 0 == iFile.SubSessionHandle() )
        {
        return;
        }
    // Form into an entry, like
    // <collection FeatureID="-1" id="0x10005951" navtitle="Bluetooth"></collection>
    HBufC8* appName = CnvUtfConverter::ConvertFromUnicodeToUtf8L( aToc1.GetName() );
    CleanupStack::PushL(appName);
    TRAPD( err, SenXmlUtils::LeaveOnXmlEscapesL( *appName ) );
    TInt length = KMaxIndexEntryExclude;
    TBuf8<KMaxUidLength> buffUid;
    
    buffUid.Append( KHexPrefix );
    buffUid.AppendNumFixedWidth( aToc1.GetAppUid().iUid, EHex, 8 );
    length += appName->Length();
    length += aFeature.Length();
    HBufC8* entry = HBufC8::NewLC( length );
    TPtr8 des = entry->Des();
    // Append entry tag
    des.Append( KIndexXMLEntryBegin );
    // Append feature id attribute "FeatureID"
    des.Append( KAppFeatureIDTag );
    des.Append( KIndexQuoteBegin );
    // Append feature id
    des.Append( aFeature );
    des.Append( KIndexQuoteEnd );
    // Append app id attribute "id"
    des.Append( KMasterCollection_idTag );
    des.Append( KIndexQuoteBegin );
    // Append app id
    des.Append( buffUid );
    des.Append( KIndexQuoteEnd );
    // Append app priority 
    des.Append( KPriorityTag );
    des.Append( KIndexQuoteBegin );
    des.AppendNum(aToc1.Priority());
    des.Append( KIndexQuoteEnd );
    // Append app name attribut "navtitle"
    des.Append( KTOC2NameTag );
    des.Append( KIndexQuoteBegin );
    if ( err == KErrSenInvalidCharacters )
        {
        // special handling to the XML-Escaped char
        HBufC8* newElement = SenXmlUtils::EncodeHttpCharactersLC( *appName );
        des.Append( *newElement );
        CleanupStack::PopAndDestroy( newElement );
        }
    else
        {
        des.Append( *appName );
        }

    des.Append( KIndexXMLEntryEnd );
    iFile.Write( *entry );
    
    CleanupStack::PopAndDestroy( entry );
    CleanupStack::PopAndDestroy( appName );
    
    }
    
void CCSXHRuntimeIndexing::GetPrivatePath( TDes& aName )
    {
    RFs& fileSession = iCoeEnv->FsSession();
    TBuf<KMaxFileName> privatePath;
    TChar drive;
    RFs::DriveToChar( EDriveC, drive );
    
    fileSession.PrivatePath( privatePath );
    aName.Append( drive );
    aName.Append( TChar(':') ); 
    aName.Append( privatePath );
    }

void CCSXHRuntimeIndexing::AppendLocaleL( TDes& aDir )
    {
    RFs& fileSession = iCoeEnv->FsSession();
    RArray<TLanguage> langs;
    BaflUtils::GetDowngradePathL( fileSession, User::Language(), langs );
    TInt len = aDir.Length();
    
    if ( len > 0 )
        {
        TLanguage lang = langs[0];
        if(lang < 10)
            {
            aDir.AppendNumFixedWidth( lang, EDecimal,2 );
            }
        else
            {
            aDir.AppendNum( lang );
            }
        }
    else
        {
        // rarely case, if no current language, use English
        aDir.Append( _L("01") );
        }
        
    aDir.Append( KFwdSlash );
    langs.Reset();
    }
    
void CCSXHRuntimeIndexing::DeleteIndexFileL( const TChar& aDrive )
    {
    RFs& fileSession = iCoeEnv->FsSession();
    TBuf<KMaxFileName> des;
    GetPrivatePath( des );
    des.Append( aDrive );
    des.Append( KFwdSlash );
    AppendLocaleL( des );
    // C:\\private\\<app-uid>\\<drive letter>\\<language-id>\\index.xml
    des.Append( KMasterMetaFile );
    
    if ( BaflUtils::FileExists( fileSession, des ) )
        {
        fileSession.Delete( des );
        }

    }
    
TInt CCSXHRuntimeIndexing::CheckDirChangeL( const TChar& aDrive )
    {
    RFs& fsSession = iCoeEnv->FsSession();
    
    // Begin to search help content folder to see if there are date modify
    // meanwhile, compare to the related folder in private directory
    TFileName dirName;
    dirName.Append( aDrive );
    dirName.Append( KInstallPath );

    // Begin to append locale, note this will append current locale,
    // thru which, app can determine if it's caused by user changed locales
    AppendLocaleL( dirName );

    TTime dirModified(0);
    TTime fileModified(0);
    
    if ( BaflUtils::PathExists( fsSession, dirName ) )
        {
        // here is modified time of help content folder
        fsSession.Modified( dirName, dirModified );
        }
    else
        {
        // fast return, no need to go on checking
        return KNoDirExist;
        }

    TFileName filename;
    GetPrivatePath( filename );
    filename.Append( aDrive );
    filename.Append( KFwdSlash );
    AppendLocaleL( filename );
    filename.Append( KMasterMetaFile );
                
    if ( BaflUtils::FileExists( fsSession, filename ) )
        {
        // here is modified time of index.xml
        //
        fsSession.Modified( filename, fileModified );
        }
    else
        {
        return KNoIndexExist;
        }
                    
    if ( fileModified != dirModified )
        {
        // folder changed, return it
        TInt ret = KErrNotFound;
        RFs::CharToDrive( aDrive, ret );
        return ret;
        }
    
    return KNoChange;
    }
