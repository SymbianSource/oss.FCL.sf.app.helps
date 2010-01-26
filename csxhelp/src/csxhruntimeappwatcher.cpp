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
* Description:  CCSXHRuntimeAppWatcher class definition
*
*/


#include "csxhruntimeappwatcher.h"
#include "csxhconstants.h"

// ========================================================================
// CCSXHRuntimeAppWatcher
// ========================================================================
//
CCSXHRuntimeAppWatcher::CCSXHRuntimeAppWatcher()
                         : CActive( CActive::EPriorityIdle )
    {
    }
    
CCSXHRuntimeAppWatcher::~CCSXHRuntimeAppWatcher()
    {
    if ( IsActive() )
        Cancel();
    iFileSession.Close();
    }
    
CCSXHRuntimeAppWatcher* CCSXHRuntimeAppWatcher::NewL()
    {
    CCSXHRuntimeAppWatcher* self = new ( ELeave ) CCSXHRuntimeAppWatcher();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
void CCSXHRuntimeAppWatcher::ConstructL()
    {
    CActiveScheduler::Add( this );
    User::LeaveIfError( iFileSession.Connect() );
    }
    
void CCSXHRuntimeAppWatcher::RunL()
    {
    // iStatus equals to KErrNone means completion
    //
    if ( iStatus == KErrNone )
        {
        iObserver->RuntimeUpdateIndex();
        Start();
        }
        
    }
    
void CCSXHRuntimeAppWatcher::DoCancel()
    {
    iFileSession.NotifyChangeCancel( iStatus );
    }
    
void CCSXHRuntimeAppWatcher::Start()
    {
    // Requests a notification of change to files or directories
    // Addition or deletion of a directory entry, or changing or formatting a disk
    //
    TBuf<KMaxFileName> obsrvPath;
    obsrvPath.Append( TChar('?') );
    obsrvPath.Append( KInstallPath );
    iFileSession.NotifyChange( ENotifyEntry, iStatus, obsrvPath );
    
    // starts
    SetActive();
    }
    
void CCSXHRuntimeAppWatcher::SetObserver( MInstallationObserver* aObserver )
    {
    iObserver = aObserver;
    }
