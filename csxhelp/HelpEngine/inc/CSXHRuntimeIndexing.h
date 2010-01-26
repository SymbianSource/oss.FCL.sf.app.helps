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
* Description:  This is a utility class that provides functionality
*                for scanning app help folder, generate index XML
*                at runtime, and also update it if any install/uninstall        
*
*/

#ifndef CSXHRUNTIMEINDEXING_H
#define CSXHRUNTIMEINDEXING_H

// INCLUDES
#include <e32base.h>
#include <f32file.h>

// CONSTANTS
// the help content folder doesn't exist
const TInt KNoDirExist = -3;
// there is no changes in help content folder (like install/uninstall)
const TInt KNoChange = -2;
// there is no index.xml file (due to no help content at previous time)
const TInt KNoIndexExist = -1;

// FORWARD CLASS DECLARATION
class CCSXHHtmlTOC1;
class CCSXHKywdTOC1;
class CCoeEnv;

/*
 * runtime indexing handling class
 * responsible for generating index file, maintaining it.
 * this class also create CS X Help private folder if not exist
 * 
 */
class CCSXHRuntimeIndexing : public CBase
    {
public:

    /*
     * constructor
     */
    CCSXHRuntimeIndexing();
    /*
    
     * destructor
     */
    ~CCSXHRuntimeIndexing();
    
    /*
     * Two-phase constructor
     */
    static CCSXHRuntimeIndexing* NewL();
    
    /*
     * Generate the master index file content
     * @param aToc1: TOC1 object which contains main topic information
     * @param aFeature: Feature information, supported by Feature Mgr
     */
    void RuntimeGenerateIndexL( const CCSXHHtmlTOC1& aToc1, const TDesC& aFeature );
    
    /*
     * Check whether master index exist
     * Since Z based helps must exist
     * Usage: to determine Help was launched before
     */
    TBool IndexFileExists();
    
    /*
     * create temporary index file
     * this file will be saved to C:\\private\\<help-uid>\\<aDrive>\\<lang-id>\\
     * @param: aDrive Drive information
     */
    void BeginIndexFile( const TChar& aDrive );
    
    /*
     * close the temporary index file, and rename it to index.xml formally
     * tmp.xml -> index.xml
     * this function can backup index.xml, once exception happens
     */
    void FinishAndCloseIndexFileL();
    
    /*
     * Get help application private path in C drive
     * @param aName[out] return help app private path in C
     */
    void GetPrivatePath( TDes& aName );
    
    /*
     * Get proper locale, and append it to descriptor
     * for more information about proper locale
     *   please refer to BaflUtils::GetDowngradePathL
     * @param aDir[out] append proper locale
     */
    void AppendLocale( TDes& aDir );
    
    /*
     * Delete master index file, particular in C: or E: or F: (user disk)
     * this will only occur when user remove all custom helps, no more helps folder
     *   in this case, custom index mustn't exist.
     * @param aDrive the file to be deleted
     */
    void DeleteIndexFile( const TChar& aDrive );
    
    /*
     * Check if helps folder changed
     * changes include: 
     *        A: install new application
     *        B: uninstall (if unistall all helps, custom helps folder will also removed)
     *        C: user changed the language
     *        D: user restored the cell phone
     * @param aName[out] return help app private path in C
     */
    TInt CheckDirChange( const TChar& aDrive );
    
private:

    /*
     * two-phase constructor, private
     */
    void ConstructL();
    
private:

    /*
     * buff current index file name
     * owned
     */
    HBufC* iFullDirName;
    
    /*
     * CoeEnv
     * not owned
     */
    CCoeEnv * iCoeEnv;
    
    /*
     * current index file handle
     */
    RFile iFile;
    
    /*
     * actually it's iFullDirName's drive letter
     * for more convenient, keep it as a class member
     */
    TChar iDriveLetter;
    
    };
    
#endif // CSXHRUNTIMEINDEXING_H