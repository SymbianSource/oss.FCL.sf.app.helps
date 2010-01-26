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
* Description:  observer if any install/unistall happens at help app
*                 runtime
*
*/

#ifndef CSXHRUNTIMEAPPWATCHER_H
#define CSXHRUNTIMEAPPWATCHER_H

#include <e32base.h>
#include <f32file.h>

/*
 * Base class of Installation/Uninstallation
 * 
 */
class MInstallationObserver
    {
public:
    /*
     * call back function, to handle update index
     */
    virtual void RuntimeUpdateIndex() = 0;
    };

/*
 * Runtime indexing wather
 * keep watching if user install/uninstall/modify any helps
 * 
 */
class CCSXHRuntimeAppWatcher : public CActive
    {
public:
    
    /*
     * default constructor
     */
    CCSXHRuntimeAppWatcher();
    
    /*
     * destructor
     */
    ~CCSXHRuntimeAppWatcher();
    
    /*
     * two-phase constructor
     */
    static CCSXHRuntimeAppWatcher* NewL();
    
    /*
     * from base class CActive
     * Handles an active object's request completion event
     */
    void RunL();
    
    /*
     * from base class CActive
     * Implements cancellation of an outstanding request
     */
    void DoCancel();
    
    /*
     * start this active object
     */
    void Start();
    
    /*
     * Set observer
     * @param aObserver: App UI, notify user helps modified
     * 
     */
    void SetObserver( MInstallationObserver* aObserver );
    
private:
    
    /*
     * two-phase constructor
     */
    void ConstructL();
    
private:

    /*
     * file session
     */
    RFs iFileSession;
    
    /*
     * observer
     * not owned
     */
    MInstallationObserver* iObserver;
    };
    
#endif // CSXHRUNTIMEAPPWATCHER_H
