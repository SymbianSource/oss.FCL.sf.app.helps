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
* Description:  This file Contains Constants 
*
*/


_LIT(KEmptyString,"");
_LIT(KFwdSlash,"\\"); 
_LIT(KBackSlash,"/"); 
_LIT(KTabSpace,"\t");
_LIT(KHexPrefix,"0x");
_LIT(KZero,"0");
_LIT(KZipFileExtn,".zip");
_LIT(KHtmlFileExtn,".html"); 
_LIT(KContentZipFile,"contents.zip");
_LIT(KCsstext_First,"<style>\n");
// temp string. if xml file and help tool chain is ready.it can be deleted.
_LIT(KDefaultFeatureIdStringTemp,"-1");
_LIT(KDefaultFeatureIdString,"true-1");
_LIT(KTrue_StringtoAppend,"true_");
_LIT(KFalse_StringtoAppend,"false_");
_LIT(KFalseString,"false");
_LIT(KTrueString,"true");

_LIT(KtextFormat_true_a,"#true_%d_A,");
_LIT(KtextFormat_false,"#false_%d_A,#false_%d_B,#false_%d_L,");

_LIT(KtextFormat_true_b,"#true_%d_B,");
_LIT(KtextFormat_false_b,"#false_%d_B,");

_LIT(KtextFormat_true_l,"#true_%d_L,");
_LIT(KtextFormat_false_l,"#false_%d_L,");

_LIT(KtextFormat_lang_a,"#LANGUAGE_%d_A,");
_LIT(KtextFormat_lang_b,"#LANGUAGE_%d_B,");
_LIT(KtextFormat_lang_l,"#LANGUAGE_%d_L,");
_LIT(KtextFormat_lang_0a,"#LANGUAGE_0%d_A,");
_LIT(KtextFormat_lang_0b,"#LANGUAGE_0%d_B,");
_LIT(KtextFormat_lang_0l,"#LANGUAGE_0%d_L,");

_LIT(KDefaultFeatureId,"#true-1");
_LIT(KComma,",");
_LIT(KCsstext_displayp_li,"{display:list-item;}\n");
_LIT(KCsstext_displayp_p,"{display:block;}\n");
_LIT(KCsstext_displayp_a,"{display:inline;}\n");
_LIT(KCsstext_displayp_n,"{display:none;}\n");
_LIT(KCsstext_Last,"</style>\n");

#ifdef FF_HELP_TUTORIAL_MERGE
_LIT(KTutorialFileExtn,".tut");
#endif // FF_HELP_TUTORIAL_MERGE
#ifndef __SERIES60_30__
_LIT(KFileUrlPrefix,"file:///");
const TInt KFileUrlPrefixLength = 8;
#else
_LIT(KFileUrlPrefix,"file://");
const TInt KFileUrlPrefixLength = 7;
#endif

const TInt KHexPrefixLength = 2;

//xhtml folder name
//#define	qtn_app_xhtml_folder_text ":\\resource\\xhtml\\"
_LIT(KXhtmlFolder,":\\resource\\xhtml\\"); 

//masterkeywords.xml file name
_LIT(KMasterKywdFile,"keywords.xml"); 

//mastermeta.xml file name
_LIT(KMasterMetaFile,"index.xml"); 

//meta.xml file name
//#define	qtn_app_meta_file_text "meta.xml"
_LIT(KMetaFile,"meta.xml"); 

//index.xml file name
//#define	qtn_app_index_file_text	"index.xml"
_LIT(KIndexFile,"index.xml"); 

//Application name tag in meta.xml
//#define	qtn_app_application_name_tag_text "title"
_LIT(KAppNameTag,"title"); 

// toc1 drive information in master index.xml
//
_LIT( KDriveInfo, "drive");

// toc1 tag in master index.xml
// 
_LIT( KAppFeatureIDTag,"FeatureID" );

//toc2title tag in index.xml
//#define	qtn_app_toc2title_name_tag_text	"navtitle"
_LIT(KTOC2NameTag,"navtitle"); 

//html page tag in index.xml
//#define	qtn_app_htmlpage_name_tag_text "href"
_LIT(KHtmlPageTag,"href"); 

//#define	qtn_app_compress_name_tag_text "compressed"
_LIT(KCompressedTag,"compressed"); 

//#define	qtn_app_compress_name_tag_value_text "true"
_LIT(KCompressedTagText,"true"); 

//#define qtn_app_searchview_toc1title_tag_text "text"
_LIT(KSearchViewTOC1Tag,"text"); 
//#define qtn_app_searchview_toc2title_tag_text "target"
_LIT(KSearchViewTOC2Tag,"target"); 
//#define qtn_app_kywd_file_text "keywords.xml"
_LIT(KKeywordsFile,"keywords.xml"); 

_LIT(KMasterSearchTag,"keyword"); 

_LIT(KMasterCollectionTag,"collection"); 

_LIT(KMasterCollection_idTag,"id"); 

// Begin runtime indexing schema

const TInt KMaxIndexEntryExclude = 200;

const TInt KMaxUidLength = 10;

// sis install path
//
//_LIT( KInstallPath, "?:\\data\\");
_LIT( KInstallPath, ":\\resource\\xhtml\\");

// initial file's name
//
_LIT( KInitFileName, "helpinit.txt" );

// Index.xml version and encoding information
//
_LIT8( KIndexXMLVesion, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n" );

// Collection ends
//
_LIT8( KIndexCollectEnd, "</collections>" );

// Begin of collection entry
//
_LIT8( KIndexXMLEntryBegin, "  <collection ");

// End of collection entry
//
_LIT8( KIndexXMLEntryEnd, "\"></collection>\r\n");

// Prefix of quotation mark
//
_LIT8( KIndexQuoteBegin, "=\"");

// appendix of quotation mark
//
_LIT8( KIndexQuoteEnd, "\" ");


_LIT( KMasterMetaTmpFile,"tmp.xml" ); 
