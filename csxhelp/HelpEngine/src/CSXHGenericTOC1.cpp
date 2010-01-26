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
* Description:  CCSXHGenericTOC1 class definition
*
*/

#include "CSXHGenericTOC1.h"
#include "csxhconstants.h"

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHGenericTOC1::CCSXHGenericTOC1(const TDesC& aName):
                    CCSXHHelpContentBase(aName),iChildList(NULL)
    {//No Implementation required
    }
    
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
CCSXHGenericTOC1::~CCSXHGenericTOC1()
    {
    ResetChildList();
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
void CCSXHGenericTOC1::ResetChildList()
    {
    if(iChildList == NULL)
        return;
    TInt count =    iChildList->Count();
    CCSXHHelpContentBase* toc2;
    for(TInt i=0;i<count;++i )
        {
        toc2 = (*iChildList)[i];
        delete toc2;
        }
    iChildList->Reset();
    delete iChildList;
    iChildList = NULL;
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
void CCSXHGenericTOC1::ConstructChildListL()
    {
    iChildList = new(ELeave) RPointerArray<CCSXHHelpContentBase>();
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
EXPORT_C CCSXHHelpContentBase* CCSXHGenericTOC1::GetChildL(const TDesC& aName)
    {
    if(iChildList == NULL)
        return NULL;
    
    TInt count = iChildList->Count();
    CCSXHHelpContentBase* child = NULL;
    for(TInt i=0;i<count;++i )
        {
        child = (*iChildList)[i];
        if(aName.Compare(child->GetName())==0)
            return child;
        }
    return NULL;            
    }
    
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
EXPORT_C void CCSXHGenericTOC1::FillChildDataL(CDesCArray* aArray)
    {
    //Using Template Pattern. Both the functions below are virtual
    InitChildList();
    CopyChildListL(aArray);     
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------
TBool CCSXHGenericTOC1::InsertChild(CCSXHHelpContentBase* aChild,
TBool aDeleteObject)
    {
    ConstructChildList();
    TLinearOrder<CCSXHHelpContentBase> anOrder(Orderer<CCSXHHelpContentBase>);  
    if (KErrNone != iChildList->InsertInOrder(aChild,anOrder))
        {
        if(aDeleteObject)
        	delete aChild;
        
        return EFalse;
        }
    return ETrue;           
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
void CCSXHGenericTOC1::CopyChildListL(CDesCArray* aArray)
    {
    TInt count =    iChildList->Count();
    CCSXHHelpContentBase* child;
    for(TInt i=0;i<count;++i )
        {
        child = (*iChildList)[i];
        TBuf<KMaxFileName> toc2Entry(KTabSpace);
        toc2Entry.Append(child->GetName());
        aArray->AppendL(toc2Entry);
        }       
    }

// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
void CCSXHGenericTOC1::InitChildList()
    {
    ConstructChildList();
    }
    
// ---------------------------------------------------------
// Items commented in header.
// ---------------------------------------------------------    
TBool CCSXHGenericTOC1::ConstructChildList()
    {
    if(iChildList != NULL)
        return ETrue;
        
    TRAPD(err,ConstructChildListL());
    if(err == KErrNone)
        return ETrue;
    else
        {
        iChildList = NULL;
        return EFalse;
        }
    }
