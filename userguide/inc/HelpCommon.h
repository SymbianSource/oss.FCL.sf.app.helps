/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/

#ifndef HELPCOMMON_H
#define HELPCOMMON_H

#include <Qt>

// words
const char* const BACKSLASH = "/";
const char* const SPECIALCHAR = "+";
const char* const HASHMARK = "#";
const char* const COLON = ":";
// help content file name
const char* const XHTMLPATH = "resource/xhtml/";
const char* const CONTENTSZIP = "contents.zip";
const char* const INDEXXML = "index.xml";
const char* const KEYWORDXML = "keywords.xml";
const char* const METAXML = "meta.xml";

// path
const char* const PATH_ROOT = "z:/";
const char* const PATH_TRANSLATIONS = "Z:/resource/qt/translations/";

// URL
const char* const URL_HEADER_APP = "app://";
const char* const URL_HEADER_HTTP = "http://";
const char* const URL_HEADER_HTTPS = "https://";
const char* const URL_HEADER_FTP = "ftp://";
const char* const URL_HEADER_LOCAL = "file:///";
const char* const URL_LINK_SUPPORT= "Link to Nokia.com/support";

// text
const char* const TXT_BUTTON_ALL = "txt_common_button_all";
const char* const TXT_BUTTON_FIND = "txt_common_button_find";
const char* const TXT_BUTTON_LINK_SUPPORT = "txt_user_guide_button_link_to_nokiacomsupport";
const char* const TXT_SETLABEL_SEARCH = "txt_user_guide_setlabel_search";
const char* const TXT_SETLABEL_SEARCH_RESULTS = "txt_user_guide_setlabel_search_results";

// docml
const char* const QRC_DOCML_CATEGORY = ":/xml/categoryView.docml";
const char* const QRC_DOCML_CONTENTS = ":/xml/contentsView.docml";
const char* const QRC_DOCML_TOOLBAR = ":/xml/toolbar.docml";
const char* const DOCML_ACTION_ALL = "action_all";
const char* const DOCML_ACTION_SEARCH = "action_find";
const char* const DOCML_ACTION_LINK_NOKIA = "action_online_support";
const char* const DOCML_VIEW_CATEGORY = "view_category";
const char* const DOCML_VIEW_CONTENTS = "view_contents";
const char* const DOCML_LIST_CATEGORY_ALL = "list_category_all";
const char* const DOCML_LIST_CATEGORY_SEARCH = "list_category_search";
const char* const DOCML_SEARCH_PANEL_CATEGORY = "search_panel_category";
const char* const DOCML_TOOLBAR = "toolbar";
const char* const DOCML_BROWSER_CONTENTS = "browser_contents";
const char* const DOCML_BROWSER = "browser";
const char* const DOCML_LAYOUT_CATEGORY_ALL = "layout_category_all";
const char* const DOCML_LAYOUT_CATEGORY_SEARCH = "layout_category_find";
const char* const DOCML_LAYOUT_CATEGORY_SEARCH_NO_SRHPAL = "layout_category_find_no_searchpanel";
const char* const DOCML_GROUPBOX = "groupBox";
const char* const DOCML_LAYOUT_CATEGORY_SEARCH_NO_MATCH = "layout_category_find_no_match";
const char* const DOCML_LAYOUT_CATEGORY_SEARCH_NO_SRHPAL_NO_MATCH = "layout_category_find_no_searchpanel_no_match";
const char* const DOCML_NO_MATCH_LABEL = "no_match_label";


// enum

enum HelpViewName
{
    HelpViewCategory = 0,
    HelpViewContents
};

enum HelpItemRole
{
    UidRole = Qt::UserRole + 1,
	HrefRole,
    KeywordRole
};

#endif //HELPCOMMON_H

