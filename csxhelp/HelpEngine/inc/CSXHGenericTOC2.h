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
* Description:  CCSXHGenericTOC2 class declaration
*
*/

#ifndef INC_CSXHGENERICTOC2_H_HEADER_INCLUDED_BC05356D
#define INC_CSXHGENERICTOC2_H_HEADER_INCLUDED_BC05356D

#include "CSXHHelpContentBase.h"

class CCSXHGenericTOC1;
/** 
* @class CCSXHGenericTOC2
* This class has the information of TOC2 
*/
class CCSXHGenericTOC2 : public CCSXHHelpContentBase
    {
  public:
/** 
 * @function GetParent
 * @since S60 3.2
 * Get the Parent object
 * @return pointer to toc1 class object
 */
    IMPORT_C CCSXHGenericTOC1* GetParent();
/** 
 * @function GetTopicContentL
 * @since S60 3.2
 * Get topic  to display
 * @return pointer to Buffer
 */
    virtual TAny* GetTopicContentL() = 0;
    
  protected:
/** 
*  @function CCSXHGenericTOC2
*  @since S60 3.2
*  Construct a CCSXHGenericTOC2 
*  @param aParent 	- TOC1 object
*         aName 	- toc2 Name
*/  
    CCSXHGenericTOC2(CCSXHGenericTOC1* aParent, const TDesC& aName);
  	CCSXHGenericTOC1* iParent;

    };



#endif /* INC_CSXHGENERICTOC2_H_HEADER_INCLUDED_BC05356D */
