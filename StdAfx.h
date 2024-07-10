// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3767FA76_575A_11D2_9A26_00403354E42C__INCLUDED_)
#define AFX_STDAFX_H__3767FA76_575A_11D2_9A26_00403354E42C__INCLUDED_

#define _CRT_SECURE_NO_DEPRECATE 1
#pragma warning(disable:4995)
#pragma warning(disable:4996)

#define _SETUPAPI_VER 0x0501

// Modify the following defines if you have to target an OS before the ones
// specified in the following code. See MSDN for the latest information
// about corresponding values for different operating systems.
#ifndef WINVER		// Permit use of features specific to Windows 95 and Windows NT 4.0 or later.
#define WINVER 0x0501	// Change this to the appropriate value to target
#endif                     // Windows 98 and Windows 2000 or later.

// _WIN32_WINNT to be #defined at 0x0500 for vs2010
#ifndef _WIN32_WINNT	// Permit use of features specific to Windows NT 4.0 or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target
#endif		         // Windows 98 and Windows 2000 or later.

#ifndef _WIN32_WINDOWS	      // Permit use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410    // Change this to the appropriate value to target
#endif			      // Windows Millennium Edition or later.

#ifndef _WIN32_IE		// Permit use of features specific to Internet Explorer 4.0 or later.
#define _WIN32_IE 0x0600   // Change this to the appropriate value to target
#endif			// Internet Explorer 5.0 or later.

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <vector>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdlgs.h>
#include <afxtempl.h>

#include <afxsock.h>		// MFC socket extensions

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3767FA76_575A_11D2_9A26_00403354E42C__INCLUDED_)
