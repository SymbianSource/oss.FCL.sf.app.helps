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
* Description:  CCSXHMainTopics class declaration
*
*/


#ifndef INC_CSXHMAINTOPICS_H_HEADER_INCLUDED_BC0568C5
#define INC_CSXHMAINTOPICS_H_HEADER_INCLUDED_BC0568C5

#include "CSXHGenericTOC1.h"
#include <apadef.h>

class CCSXHHelpDataBase;
/** 
* @class CCSXHMainTopics
* This class has info of TOC1 Objects
*/
class CCSXHMainTopics : public CCSXHGenericTOC1
    {
    public:
/** 
*  @function CCSXHMainTopics
*  @since S60 3.2
*  Construct a CCSXHMainTopics 
*  @param aCoeEnv 	- Control Environment 
*  @retrun a CCSXHMainTopics pointer
*/
    static CCSXHMainTopics* NewL(const TUid&, const TApaAppCaption& aAppCaption, const TInt32 aPriority = 0);
/** 
*  @function CCSXHMainTopics
*  @since S60 3.2
*  Construct a CCSXHMainTopics using two phase construction
*  @param aCoeEnv 	- Control Environment 
*  @retrun a CCSXHMainTopics pointer
*/  
    static CCSXHMainTopics* NewLC(const TUid&,const TApaAppCaption& aAppCaption, const TInt32 aPriority = 0);
    
/** 
 * @function ~CCSXHMainTopics
 * @since S60 3.2
 * Destroy the object and release all memory objects
 */    
    ~CCSXHMainTopics();
/** 
*  @function GetViewID
*  @since S60 3.2
*  Get ViewId to activate the View
*  @return View Id
*/    
    TUid GetViewID() const;
/** 
*  @function ResetChildList
*  @since S60 3.2
*  Delete Objects from the Array
*/    
    void ResetChildList();
    
    protected:
   
/** 
*  @function CCSXHMainTopics
*  @since S60 3.2
*  Construct a CCSXHMainTopics
*  @param aUid  - Application Uid
*         aName - TOC1 Object Name  
*/    
    CCSXHMainTopics(const TUid& aUid, const TDesC& aName, const TInt32 aPriority);
    	
    const TUid &iUid;
    };

#endif /* INC_CSXHMAINTOPICS_H_HEADER_INCLUDED_BC0568C5 */
