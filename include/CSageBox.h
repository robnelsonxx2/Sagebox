// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project from the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

//#pragma warning( disable : 26444) 
#pragma once

#if !defined (_CSageBox_H_)
#define _CSageBox_H_
//#pragma warning( disable : 4996) 

//#include "SageIncludes.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "Sage.h"
#include "SageOpt.h"
#include "CSageTypes.h"
#include "CWindow.h"
#include "CTextWidget.h"
#include "CMemClass.h"
#include "CSageMacros.h"
#include "CDialog.h"
#include "CSageTools.h"
#include "StyleManifest.h"
#include "CQuickControls.h"
#include "CComplex.h"
#include "CQuickDialog.h"

// -------------------
// Main CSageBox Class
// -------------------
//
// CSageBox needs to be instantiated as the first object in SageBox.  For quick console-mode functions, only CSageBox is needed. 
//
// Once CSageBox is created, many utility functions become available, and visible windows (CWindow class) can be created that
// can host output, input, controls widgets, etc.
//
// SageBox has a few modes, depending on the type of application.
//
// Console
//
// SageBox can be used in console mode to supplement console-mode programmging with quick dialog and button functions,
// and Windows that can host the entire set of SageBox functions while still in console-mode.
//
// SandBox
//
// The SandBox mode allows regular C/C++ programming in the context of a window class, mixing
// console-mode style programming with windows functionality. 
// 
// In this mode, the console mode window can also be available so that the window(s) created by SageBox and the Console mode
// window an be used simultaneously.
//
// The SandBox window acts just like a Windows programs, with controls like sliders, buttons, etc., and can also be
// used in a more console-mode style setting with various console functions such as printf, write(), and various i/o
// routines, including "in" and "out" that recplicate C++ cin and cout, but with more functionality.
//
// Regular Windows -- Event Drive and Procedural (i.e. regular C/C++)
//
// SageBox applications can be written in a regular C/C++ procedural style without the use of event callbacks or overriding classes, as well as regular windows.
// A mix of the two is also possible, the idea being to start with an easy procedural program and then later migrate some or all of it to a more event-drive
// structure as needed.
//
// With Windows, not all functionality needs to be event-driven to be the most efficient, but in some cases that may be useful dependonding on the application.
//
// For most applications, start-up projects, and prototypush programs, simple procedural code with the use of EventLoop() works just fine and creates
// a bonafide windows application.
//
// As the program grows, it is an easy transition to making some or all of the program work in an event-driven source-code structure.
//
// Event-Driven, Canonical Windows
//
// Sagebox also facilitates writing canonical windows programs, with overrides and classes (i.e. Message Handlers) to intercept all Windows messages
// with or without overriding the window or control class. 
//
// With a Message Handler, you can specify where to send Windows messages without overriding the class, or the entire window or control can be overriden for more
// flexibility.
//
// SageBox also provides a large array of functions to work with the Windows operating system, as well as keeping track of event states as they occur.
//
// SageBox Tools
//
// This is still being implemented.  The tools in Sagebox extend beyond windows functionality and into general tools, such as image-processing, neural networking, and other tools that
// have been developed over the last few years.
//
// This is still being developed.  See functions available in CSageTools as well as CRawBitmap, Sage::CString, and so-forth.
//
namespace Sage
{

// Empty values useful for returning null objects of various types when errors occur. 

static constexpr RGBColor_t EmptyRGB{};
static constexpr HSLColor_t EmptyHSL{};
static constexpr POINT		EmptyPoint{};
static constexpr SIZE		EmptySize{};
static constexpr RECT       EmptyRect {};
static constexpr SizeRect   EmptySizeRect{};

class CSageBox;
class CProcessWindow;
class CHandleWindowMsg;
using CSagebox = CSageBox;          // Allow CSagebox to be used, as well, since I keep putting CSagebox cSagebox in videos. 
class CConsoleIO;
struct WinConio; 

class CSageBox
{
    friend CHandleWindowMsg;
    friend WinConio;
public:


	CWindow * m_cWin = nullptr;
    
    // Process Window Start Mode
    //
    //   Default,        // Shows title bar in upper-right corner that expands when mouseover
    //   Debug,          // Shows full window including debug output window
    //   Hidden,         // Hidden until debug is output first time, mouse is held in upper-right corner, or user turns it on through sysmenu
    //
    enum class ProcessWinStartMode
    {
        Default,        // Shows title bar in upper-right corner that expands when mouseover
        Debug,          // Shows full window including debug output window
        Hidden,         // Hidden until debug is output first time, mouse is held in upper-right corner, or user turns it on through sysmenu

    };



private:
	void Postamble(CWindow & cWin);
	void Preamble(CWindow & cWin,bool bHidden = false);
	void CreateDefaultWindow();
    bool InitDevControls();   // Initialize default Dev Controls Window -- added only if used. 
    static void CreateProcessAccelerator();       // Creates ^C accelerator

    HANDLE m_hWinThread = nullptr;
    CDevControls * m_cDevControls   = nullptr;          // Not created until first used. 
    bool m_bConsoleHidden           = true;             // true if the Console Window was hidden
    HWND m_hConsoleWindow           = (HWND) nullptr;   // HWND of the Console window if we're in console mode. 
    bool m_bConsoleDontRestore      = false;            // When true and the console is hidden, the console window is not 
                                                        // restored on program exit.  Otherwise, a hidden console window is restored
                                                        // When the program ends.

    // Process-Wide singleton

    static CProcessWindow * m_cProcessWindow;           // Process Watch Window & low-level debug.  One per process. 
    static bool             m_bProcessWinActive;
    static bool             m_bAcceptCtrlCBreak;        // When true, ^C offers to terminate the process. 

	int m_iConsoleWinNum = 0;		// Used for positioning windows when ConsoleWin() is called
    CPasWindow * m_cWinCore = nullptr;
    void Init(const char * sAppName);
public:

    /// <summary>
    /// Console Output Functions
    /// <para></para>&#160;&#160;&#160;
    /// conio provides a set of useful functions for console output that allow for different text colors, font sizes, background colors, and so-forth.
    /// <para></para>
    /// You can use conio.printf() or conio.write() with "{}" markers to specify colors.
    /// <para></para>&#160;&#160;&#160;For example, printf("This is {red}red{/} and this is {b}blue") prints "red in red" and "blue" in blue.
    /// <para></para>Note the use of {b} as a shortcut for blue.  Colors can be "r" for "Red" and so-forth, and "db" for "darkblue", etc. Use  conio.GetColorNames() for a full list.
    /// <para></para>&#160;&#160;&#160;
    /// Background colors can also be set with "bg=r", or the entire line with "lbg=blue", for example.
    /// <para></para>&#160;&#160;&#160;
    /// --> "u" for underline, and "rev" as quick way to reverse text. Combine types, such as conio.printf("This {bg=blue}{y}word{/}{/} is written in a blue background with yellow text color")
    /// </summary>
    static WinConio conio; 

    // Public values to be removed from the release version at some later date (kSagePrivPublic)

kSagePrivPublic:

    // Some external variables used in initialization where the Sagebox object has not been created yet
    // or is not in the scope of a low-level processing setting up a configuration

    static WORD m_wC; 
    static const char * m_sC; 
    static const char * m_sT; 
    static bool _bIsSagebox; 

public:
	// The main SageBox Class
	//
	// note: Only instantiate one per-application.  Pass a reference or a pointer, or set
	// a pointer member variable to re-use in other functions and classes.
	//
	// Sagebox contains a number of console-based functions that can help with windowing functions
	// without creating a Window.  Use the CSageBox object to access these.
	//
	// All CSageBox functions are available through windows created by CSageBox
	//
	// For windows-based applications, the CSageBox object is not needed once it is instantiated, but
	// cannot be deleted until the end of the program, as it closes and destroys all windows, controls, widgets,
	// and memory created by any part of SageBox
	//
	CSageBox(const char * sAppName = nullptr);
	CSageBox(const char * sAppName,SageboxInit & stInit);
	CSageBox(SageboxInit & stInit);
	~CSageBox();
    void SetWinThread(HANDLE hThread);

    // Quick Console Window functions (same as NewWindow, just smaller)
	//
	// This creates quick small window (around 800x600), where NewWindow creates a much larger window. 
	// There isn't any other difference
	//
	CWindow & ConsoleWin(int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());
    
    // Quick Console Window functions (same as NewWindow, just smaller)
	//
	// This creates quick small window (around 800x600), where NewWindow creates a much larger window. 
	// There isn't any other difference
	//
	CWindow & ConsoleWin(int iX,int iY,const cwfOpt & cwOpt);

	// Quick Console Window functions (same as NewWindow, just smaller)
	//
	// This creates quick small window (around 800x600), where NewWindow creates a much larger window. 
	// There isn't any other difference
	//
	CWindow & ConsoleWin(const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// Quick Console Window functions (same as NewWindow, just smaller)
	//
	// This creates quick small window (around 800x600), where NewWindow creates a much larger window. 
	// There isn't any other difference
	//
	CWindow & ConsoleWin(cwfOpt & cwOpt);


	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const char * sWinTitle,const cwfOpt & cwOpt = cwfOpt());
    static CSagebox * m_cStaticSagebox;

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	CWindow & NewWindow(CWindow * cWin,const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	CWindow & NewWindow(CWindow * cWin,const cwfOpt & cwOpt);

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const char * sWinTitle, const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	CWindow & NewWindow(const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    CWindow & NewWindow(cwfOpt & cwOpt);

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // Note when using NewWindow(SIZE(),..) varations, InnerSize() is assumed and the window size supplied will be the interior size.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    CWindow & NewWindow(SIZE szSize,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // Note when using NewWindow(SIZE(),..) varations, InnerSize() is assumed and the window size supplied will be the interior size.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    CWindow & NewWindow(SIZE szSize,const cwfOpt & cwOpt); 

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    CWindow & NewWindow(CPoint pLoc,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    CWindow & NewWindow(CPoint pLoc,const cwfOpt & cwOpt); 

    // AutoWindow() -- Uses m_cStaticSagebox to creat a new window
    //
    // AutoWindow() returns a CWindow object.  if m_cStaticSagebox doesn't exist, it is created.
    //
    // AutoWindow() is used for when you want to quickly start a Sagebox session with a window -- this allows a quick
    // way to create the window without the need to create Sagebox first. 
    //
    // Use GetStaticSagebox() to get the static Sagebox object.
    //
    // m_cStaticSagebox will then be the reference CSagebox for the entire application.
    // When a window is created, CSagebox is typically not required as most functions can be accessed through the created Window.
    // For specific CSagebox functions, use GetStaticSagebox() to retrieve the object.
    //
    // ** Do not create another CSagebox after using AutoWindow().  This should be ok, but it is not recommended **
    //
	static CWindow & AutoWindow(int iX,int iY,int iWidth,int iHeight,const char * sWinTitle,const cwfOpt & cwOpt = cwfOpt());

    // AutoWindow() -- Uses m_cStaticSagebox to creat a new window
    //
    // AutoWindow() returns a CWindow object.  if m_cStaticSagebox doesn't exist, it is created.
    //
    // AutoWindow() is used for when you want to quickly start a Sagebox session with a window -- this allows a quick
    // way to create the window without the need to create Sagebox first. 
    //
    // Use GetStaticSagebox() to get the static Sagebox object.
    //
    // m_cStaticSagebox will then be the reference CSagebox for the entire application.
    // When a window is created, CSagebox is typically not required as most functions can be accessed through the created Window.
    // For specific CSagebox functions, use GetStaticSagebox() to retrieve the object.
    //
    // ** Do not create another CSagebox after using AutoWindow().  This should be ok, but it is not recommended **
    //
	static CWindow & AutoWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    // AutoWindow() -- Uses m_cStaticSagebox to creat a new window
    //
    // AutoWindow() returns a CWindow object.  if m_cStaticSagebox doesn't exist, it is created.
    //
    // AutoWindow() is used for when you want to quickly start a Sagebox session with a window -- this allows a quick
    // way to create the window without the need to create Sagebox first. 
    //
    // Use GetStaticSagebox() to get the static Sagebox object.
    //
    // m_cStaticSagebox will then be the reference CSagebox for the entire application.
    // When a window is created, CSagebox is typically not required as most functions can be accessed through the created Window.
    // For specific CSagebox functions, use GetStaticSagebox() to retrieve the object.
    //
    // ** Do not create another CSagebox after using AutoWindow().  This should be ok, but it is not recommended **
    //
    static CWindow & AutoWindow(const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // AutoWindow() -- Uses m_cStaticSagebox to creat a new window
    //
    // AutoWindow() returns a CWindow object.  if m_cStaticSagebox doesn't exist, it is created.
    //
    // AutoWindow() is used for when you want to quickly start a Sagebox session with a window -- this allows a quick
    // way to create the window without the need to create Sagebox first. 
    //
    // Use GetStaticSagebox() to get the static Sagebox object.
    //
    // m_cStaticSagebox will then be the reference CSagebox for the entire application.
    // When a window is created, CSagebox is typically not required as most functions can be accessed through the created Window.
    // For specific CSagebox functions, use GetStaticSagebox() to retrieve the object.
    //
    // ** Do not create another CSagebox after using AutoWindow().  This should be ok, but it is not recommended **
    //
    static CWindow & AutoWindow(const cwfOpt & cwOpt);

    // AutoWindow() -- Uses m_cStaticSagebox to creat a new window
    //
    // AutoWindow() returns a CWindow object.  if m_cStaticSagebox doesn't exist, it is created.
    //
    // AutoWindow() is used for when you want to quickly start a Sagebox session with a window -- this allows a quick
    // way to create the window without the need to create Sagebox first. 
    //
    // Use GetStaticSagebox() to get the static Sagebox object.
    //
    // m_cStaticSagebox will then be the reference CSagebox for the entire application.
    // When a window is created, CSagebox is typically not required as most functions can be accessed through the created Window.
    // For specific CSagebox functions, use GetStaticSagebox() to retrieve the object.
    //
    // ** Do not create another CSagebox after using AutoWindow().  This should be ok, but it is not recommended **
    //
    static CWindow & AutoWindow(SIZE szSize,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // AutoWindow() -- Uses m_cStaticSagebox to creat a new window
    //
    // Note when using AutoWindow(SIZE(),..) varations, InnerSize() is assumed and the window size supplied will be the interior size.
    //
    // AutoWindow() returns a CWindow object.  if m_cStaticSagebox doesn't exist, it is created.
    //
    // AutoWindow() is used for when you want to quickly start a Sagebox session with a window -- this allows a quick
    // way to create the window without the need to create Sagebox first. 
    //
    // Use GetStaticSagebox() to get the static Sagebox object.
    //
    // m_cStaticSagebox will then be the reference CSagebox for the entire application.
    // When a window is created, CSagebox is typically not required as most functions can be accessed through the created Window.
    // For specific CSagebox functions, use GetStaticSagebox() to retrieve the object.
    //
    // ** Do not create another CSagebox after using AutoWindow().  This should be ok, but it is not recommended **
    //
    static CWindow & AutoWindow(SIZE szSize,const cwfOpt & cwOpt);

    // AutoWindow() -- Uses m_cStaticSagebox to creat a new window
    //
    // Note when using AutoWindow(SIZE(),..) varations, InnerSize() is assumed and the window size supplied will be the interior size.
    //
    // AutoWindow() returns a CWindow object.  if m_cStaticSagebox doesn't exist, it is created.
    //
    // AutoWindow() is used for when you want to quickly start a Sagebox session with a window -- this allows a quick
    // way to create the window without the need to create Sagebox first. 
    //
    // Use GetStaticSagebox() to get the static Sagebox object.
    //
    // m_cStaticSagebox will then be the reference CSagebox for the entire application.
    // When a window is created, CSagebox is typically not required as most functions can be accessed through the created Window.
    // For specific CSagebox functions, use GetStaticSagebox() to retrieve the object.
    //
    // ** Do not create another CSagebox after using AutoWindow().  This should be ok, but it is not recommended **
    //
    static CWindow & AutoWindow(CPoint pLoc,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // AutoWindow() -- Uses m_cStaticSagebox to creat a new window
    //
    // AutoWindow() returns a CWindow object.  if m_cStaticSagebox doesn't exist, it is created.
    //
    // AutoWindow() is used for when you want to quickly start a Sagebox session with a window -- this allows a quick
    // way to create the window without the need to create Sagebox first. 
    //
    // Use GetStaticSagebox() to get the static Sagebox object.
    //
    // m_cStaticSagebox will then be the reference CSagebox for the entire application.
    // When a window is created, CSagebox is typically not required as most functions can be accessed through the created Window.
    // For specific CSagebox functions, use GetStaticSagebox() to retrieve the object.
    //
    // ** Do not create another CSagebox after using AutoWindow().  This should be ok, but it is not recommended **
    //
    static CWindow & AutoWindow(CPoint pLoc,const cwfOpt & cwOpt);

    // GetStaticSagebox() -- Returns the object for m_cStaticSagebox created when AutoWindow() is used. 
    //
    static CSagebox * GetStaticSagebox();


    // Main() -- Creates a new window from a newly created class and runs the windows Main() function.
	//
	// This is designed to be used with the QuickConsole macro, where the defined classname is used in the Main() function
	//
	// For example, Main(new MyWindowClass,100,200) will create the MyWindowClass object and run MyWindowClass.Main()
	//
	// Do not delete the MyWindowClass object (notice that there is no object saved).  Once attached to a new window
	// through CSageBox, SageBox manages it and deletes the object when the window is destroyed.
	//
	int Main(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// Main() -- Creates a new window from a newly created class and runs the windows Main() function.
	//
	// This is designed to be used with the QuickConsole macro, where the defined classname is used in the Main() function
	//
	// For example, Main(new MyWindowClass,100,200) will create the MyWindowClass object and run MyWindowClass.Main()
	//
	// Do not delete the MyWindowClass object (notice that there is no object saved).  Once attached to a new window
	// through CSageBox, SageBox manages it and deletes the object when the window is destroyed.
	//
	int Main(CWindow * cWin,const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    static int AutoMain(CWindow * cWin,const cwfOpt & cwOpt);
    static int AutoMain(CWindow * cWin,const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());
 	static int AutoMain(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	int RegisterWidget(int & iRegistryID);
#if 0
    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	CDialog & NewDialog(int iX,int iY,int iWidth = 0,int iHeight = 0,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	CDialog & NewDialog(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	CDialog & NewDialog(int iX,int iY,const char * sTitle,const cwfOpt & cwOpt = cwfOpt());

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	CDialog & NewDialog(int iX,int iY,const cwfOpt & cwOpt = cwfOpt());
#endif
    /// <summary>
    /// Creates a Quick Dialog Window for creating quick dialog windows for user input, such as radio buttons, checkboxes, listboxes, etc.
    /// <para></para>
    /// This creates a dialog window with an Ok and Cancel button, to which you can add controls.  It returns a CQuickDialog object where you can add the controls and display the dialog box.
    /// <para></para>&#160;&#160;&#160;
    /// Use functions such as AddCheckboxes(), AddListbox() and other functions to add controls.  These functions will return the control (or Control Group, in the case of multiple radio buttons and checkboxes) object
    /// that can then be querired for input.
    /// <para></para>
    /// --> Use CQuickDialog::WaitforClose() to wait for the user to press OK or Cancel, after which values can be retrieved from all controls. Returns status of button pressed.
    /// <para></para>
    /// --> Use CQuickDialog::Show() to show the dialog box.  After this, you can use the GetEvent() loop or SageEvent message in your message handler to query CQuickDialog::StatusChanged() to determine of the user has pressed a button or closed
    /// the window.   This method allows you to dynamically look at control events while the user is using the dialog box (i.e. such as reacting to Checkbox presses, etc.)
    /// --> To re-use the dialog box, use CQuickDialog.ResetStatus() and then re-use WaitforClose() or Show()
    /// --> Use CDialog::Close() to close the dialog box.
    /// --> Use Modal() option to make dialog modal (disables parent window until user closes the window by pressing a button or closing it with the 'X' button)
    /// </summary>
    /// <param name="sHeader">(optional) top informational text header in the window.  This may be multi-line.  Set first character in first line as '+' to set title of window.</param>
    /// <param name="cwOpt">(optional).  Options passed to QuickDialog (i.e. Modal(), Title()</param>
    /// <returns></returns>
    CQuickDialog & QuickDialog(const char * sHeader = nullptr,const cwfOpt & cwOpt = CWindow::cwNoOpt);

    /// <summary>
    /// Creates a Quick Dialog Window for creating quick dialog windows for user input, such as radio buttons, checkboxes, listboxes, etc.
    /// <para></para>
    /// This creates a dialog window with an Ok and Cancel button, to which you can add controls.  It returns a CQuickDialog object where you can add the controls and display the dialog box.
    /// <para></para>&#160;&#160;&#160;
    /// Use functions such as AddCheckboxes(), AddListbox() and other functions to add controls.  These functions will return the control (or Control Group, in the case of multiple radio buttons and checkboxes) object
    /// that can then be querired for input.
    /// <para></para>
    /// --> Use CQuickDialog::WaitforClose() to wait for the user to press OK or Cancel, after which values can be retrieved from all controls. Returns status of button pressed.
    /// <para></para>
    /// --> Use CQuickDialog::Show() to show the dialog box.  After this, you can use the GetEvent() loop or SageEvent message in your message handler to query CQuickDialog::StatusChanged() to determine of the user has pressed a button or closed
    /// the window.   This method allows you to dynamically look at control events while the user is using the dialog box (i.e. such as reacting to Checkbox presses, etc.)
    /// --> To re-use the dialog box, use CQuickDialog.ResetStatus() and then re-use WaitforClose() or Show()
    /// --> Use CDialog::Close() to close the dialog box.
    /// --> Use Modal() option to make dialog modal (disables parent window until user closes the window by pressing a button or closing it with the 'X' button)
    /// </summary>
    /// <param name="sHeader">(optional) top informational text header in the window.  This may be multi-line.  Set first character in first line as '+' to set title of window.</param>
    /// <param name="cwOpt">(optional).  Options passed to QuickDialog (i.e. Modal(), Title()</param>
    /// <returns></returns>
    CQuickDialog & QuickDialog(const cwfOpt & cwOpt);

	// dialog functions. See documentation by pressing "dialog." for function list, or see CWindow for documentation
	//
	CWindow::WinDialog dialog;

    // EventLoop() -- Wait for a user event.  Even though the main SageBox window is not visible, all other controls
	// run through this EventLoop.
    //
    // This is the Main Event Loop for procedurally-driven programs to capture events without using event callbacks.
    //
    // EventLoop() returns for relavent events from the user.  It does not return for all Windows events, only those that affect the running of the
    // program.  All Windows message events can be intercepted by subclassing the window or using SetMessageHandler() to capture all messages.
    //
    //
    // EventLoop() returns if the window is closing with the WaitEvent::WindowClosing status. 
	// For the main (hidden) SageBox window, this must be exited to close the program since the window
	// does not display and there is now way to close it by choice.
    // 
    // Important Note:  Make sure EventLoop() does not return until it sees events.  With empty windows or corrupted windows, EventLoop()
    // can enter a processor-using wild loop.   When testing, it is a good idea to have printfs() to the window or console to make sure
    // only events are returned and it is not caught in a spining loop. 
    //
	bool EventLoop(Sage::WaitEvent * eStatus = nullptr); 
    
    // GetEvent() -- Wait for a user event.  Even though the main SageBox window is not visible, all other controls
	// run through this EventLoop.
    //
    // This is the Main Event Loop for procedurally-driven programs to capture events without using event callbacks.
    //
    // EventLoop() returns for relavent events from the user.  It does not return for all Windows events, only those that affect the running of the
    // program.  All Windows message events can be intercepted by subclassing the window or using SetMessageHandler() to capture all messages.
    //
    //
    // EventLoop() returns if the window is closing with the WaitEvent::WindowClosing status. 
	// For the main (hidden) SageBox window, this must be exited to close the program since the window
	// does not display and there is now way to close it by choice.
    // 
    // Important Note:  Make sure EventLoop() does not return until it sees events.  With empty windows or corrupted windows, EventLoop()
    // can enter a processor-using wild loop.   When testing, it is a good idea to have printfs() to the window or console to make sure
    // only events are returned and it is not caught in a spining loop. 
    //
    bool GetEvent(Sage::WaitEvent * eStatus = nullptr);

    // QuickButton() -- Put up a quick button and information line. 
    //
    // This is used for quick Input-and-Go buttons, to stop program execution and get a quick button to continue.
    // QuickButton() simply puts up a "press to continue" message with an Ok button.
    //
    // The text can be is changed by setting sText.
    // The ProgramName (if it is set) is set in the title bar, unless sTitleBar is set other text
    //
    // QuickInfoButton() is similar to dialog.Info(), but uses a smaller font.
    // 
    // Multiple lines may be entered with '\n', and long lines are automatically broken into multiple lines
    //
	dllvirt void QuickButton(const char * sText = nullptr,const char * sTitleBar = nullptr) dllvoid;

    // WinMessageBox() -- Bring up a standard Windows Message Box. 
    //
    // This follows the same format as ::MessageBox() in windows and simply calls it in the same way. 
    //
    // This allows more "Windows-like" messages, information, warnings, etc.
    //
    // Example: WinMessageBox("This is the message","This is the title",MB_OK | MB_ICONINFO)
    //
	int WinMessageBox(const char * sMessage,const char * sTitle = nullptr,unsigned int dwFlags = MB_OK);

    // Sets the Program/Application name.  This can also be set when initiating Sagebox, i.e. CSageBox("My Application"); 
    //
    // The Application name is used in various window title bars in SageBox.  With no Program Name set, either the window title is blank or a default
    // text string is used.
    //
	void SetProgramName(const char * sAppName); 

    // Gets the program name set in SageBox through either the SageBox constructor (i.e. CSageBox("My Application") or through GetProgramName();
    //
    const char * GetProgramName();

    // GetInteger() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter an integer
    //
    // Pressing <CR> or the window closing will return from GetInteger().  Empty input (ESC or empty line) is treated as a 0 unless it is disabled with options listed below.
    //
        // GetInteger() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> iDefault -- the iDefault parameter sets the default value for input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
	int GetInteger(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());

    // GetInteger() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter an integer
    //
    // Pressing <CR> or the window closing will return from GetInteger().  Empty input (ESC or empty line) is treated as a 0 unless it is disabled with options listed below.
    //
        // GetInteger() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> iDefault -- the iDefault parameter sets the default value for input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
	int GetInteger(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());

    // console.GetFloat() -- Get a floating-point value from the user, in an inline console-style input (i.e. as part of a text line rather than a windowed box)
    //
    // This function will get a small set of input from the user (typical a window of 15 characters or so) and return the floating point value once the user presses return.
    //
    // Basic Forms:
    //
    // --> double MyValue = GetFloat().                        -- Get a float value with default options
    // --> double MyValue = GetFloat(123.56).                -- Get a float value with default set to 123.56
    // --> double MyValue = GetFloat(<<options>>).            -- Get a float value with an option string and no default (see examples below)
    // --> double MyValue = GetFloat(123.56,<<options>>).    -- Get a float value with a default set to 123.56 and also specify options (see examples below)
    //
    // Inputs (all optional):
    //
    //        fDefault        - Default value that will be placed in the box when it first appears.  The user can then change it
    //      sOptions        - Text-based options to control the input and output
    //        cwOptions        - opt::Option()-style options -- you can use Text or opt::-style options, depending on your preference.
    //
    // --> Example: double MyValue = GetFloat(123.56,"range=-100,100,colorsBW")
    //              double Myalue = GetFloat(123.56,opt::Range(-100,100) | opt::ColorsBW())
    //
    // Both of these examples perform the same function: Set a default of "123.56", set a range of -100,100, and tell the box to have a white background with black text
    //
    //--> Available Options
    //
    // Range            -- Range of the value (i.e. Range(-100,100)).  If the input is outside the range, an error message appears telling the user to enter a number within the range specified.
    // MinValue            -- Sets the minimum value (i.e. MinValue(-100)).  If the input is less than the MinValue, an error message appears telling the user to enter a number above the MinValue
    // MaxValue            -- Sets the Maximum Value allowed.  If the input is greater than the MaxValue, an error message appears elling the user to enter a number less then the MinValue
    // ColorsBW            -- Sets the background of the box to white, with black text.  This can make the box more visible.  Otherwise, the box is the windows background color and current text color
    // NoBorder            -- Default is a thin border for visibility.  the "NoBorder" option will remove the border so the text looks more like normal text in the overall text output line.
    // ThickBorder        -- Provides a recessed window that looks more like an input window, but still in the text line.
    // NoBlankLine        -- Does not allow the user to enter a blank line.  When this is off (default), when the user presses enter with a blank line, the default value is returned (or 0 when here was no default set)
    // 
    // --> Examples
    //
    // --> double MyValue = console.GetFloat()                                                 -- Gets a float with default display set and no default alue
    // --> double MyValue = console.GetFloat(123.56)                                           -- Gets a float with default display set and with default value = 123.56
    // --> double MyValue = console.GetFloat(123.56,"NoBorder,MinValue=-100");                 -- string-style options
    // --> double MyValue = console.GetFloat(123.56,opt::NoBorder() | opt::MinValue(-100))     -- opt::-style options
    //
    // ** You can also use in >> MyValue to use C++ input stream style
    //
    // --> Same examples as above (assuming MyValue is defined as "double MyValue" ahead of the call):
    //
    // --> in >> MyValue                                                                       -- Gets a float with default display set and no default alue
    // --> in >> opt::Default(123.56) >> MyValue                                               -- Gets a float with default display set and with default value = 123.56
    // --> in >> opt::Default(123.56) >> opt::NoBorder() >> opt::MinValue(-100) >> MyValue;    -- opt::-style options
    // --> in >> Default(123.56) >> NoBorder() >> MinValue(-100) >> MyValue;                   -- opt::-style options (when "using namespace opt" is specified)
    //
    // After a certain point, it makes sense to use console.GetFloat() (over stream-style "in >> MyValue) when a large amount of options are used.  
    // For simple calls and prototyping, using "in >> MyValue" or things like "in >> opt::ColorwsBW >> MyValue" can be much quicker and easier to read, but
    // can be more clear using console.GetFLoat() when many options are being used (see above examples).
    //
    // There are 4 different function prototypes so that options can be omitted when not required.
    //
	dllvirt double GetFloat(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt()) dllfloat;

    // console.GetFloat() -- Get a floating-point value from the user, in an inline console-style input (i.e. as part of a text line rather than a windowed box)
    //
    // This function will get a small set of input from the user (typical a window of 15 characters or so) and return the floating point value once the user presses return.
    //
    // Basic Forms:
    //
    // --> double MyValue = GetFloat().                        -- Get a float value with default options
    // --> double MyValue = GetFloat(123.56).                -- Get a float value with default set to 123.56
    // --> double MyValue = GetFloat(<<options>>).            -- Get a float value with an option string and no default (see examples below)
    // --> double MyValue = GetFloat(123.56,<<options>>).    -- Get a float value with a default set to 123.56 and also specify options (see examples below)
    //
    // Inputs (all optional):
    //
    //        fDefault        - Default value that will be placed in the box when it first appears.  The user can then change it
    //      sOptions        - Text-based options to control the input and output
    //        cwOptions        - opt::Option()-style options -- you can use Text or opt::-style options, depending on your preference.
    //
    // --> Example: double MyValue = GetFloat(123.56,"range=-100,100,colorsBW")
    //              double Myalue = GetFloat(123.56,opt::Range(-100,100) | opt::ColorsBW())
    //
    // Both of these examples perform the same function: Set a default of "123.56", set a range of -100,100, and tell the box to have a white background with black text
    //
    //--> Available Options
    //
    // Range            -- Range of the value (i.e. Range(-100,100)).  If the input is outside the range, an error message appears telling the user to enter a number within the range specified.
    // MinValue            -- Sets the minimum value (i.e. MinValue(-100)).  If the input is less than the MinValue, an error message appears telling the user to enter a number above the MinValue
    // MaxValue            -- Sets the Maximum Value allowed.  If the input is greater than the MaxValue, an error message appears elling the user to enter a number less then the MinValue
    // ColorsBW            -- Sets the background of the box to white, with black text.  This can make the box more visible.  Otherwise, the box is the windows background color and current text color
    // NoBorder            -- Default is a thin border for visibility.  the "NoBorder" option will remove the border so the text looks more like normal text in the overall text output line.
    // ThickBorder        -- Provides a recessed window that looks more like an input window, but still in the text line.
    // NoBlankLine        -- Does not allow the user to enter a blank line.  When this is off (default), when the user presses enter with a blank line, the default value is returned (or 0 when here was no default set)
    // 
    // --> Examples
    //
    // --> double MyValue = console.GetFloat()                                                 -- Gets a float with default display set and no default alue
    // --> double MyValue = console.GetFloat(123.56)                                           -- Gets a float with default display set and with default value = 123.56
    // --> double MyValue = console.GetFloat(123.56,"NoBorder,MinValue=-100");                 -- string-style options
    // --> double MyValue = console.GetFloat(123.56,opt::NoBorder() | opt::MinValue(-100))     -- opt::-style options
    //
    // ** You can also use in >> MyValue to use C++ input stream style
    //
    // --> Same examples as above (assuming MyValue is defined as "double MyValue" ahead of the call):
    //
    // --> in >> MyValue                                                                       -- Gets a float with default display set and no default alue
    // --> in >> opt::Default(123.56) >> MyValue                                               -- Gets a float with default display set and with default value = 123.56
    // --> in >> opt::Default(123.56) >> opt::NoBorder() >> opt::MinValue(-100) >> MyValue;    -- opt::-style options
    // --> in >> Default(123.56) >> NoBorder() >> MinValue(-100) >> MyValue;                   -- opt::-style options (when "using namespace opt" is specified)
    //
    // After a certain point, it makes sense to use console.GetFloat() (over stream-style "in >> MyValue) when a large amount of options are used.  
    // For simple calls and prototyping, using "in >> MyValue" or things like "in >> opt::ColorwsBW >> MyValue" can be much quicker and easier to read, but
    // can be more clear using console.GetFLoat() when many options are being used (see above examples).
    //
    // There are 4 different function prototypes so that options can be omitted when not required.
    //
    virtual double GetFloat(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());


    // GetString() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter text an press return.
    //
    // Pressing <CR> or the window closing will return from GetString().  Empty input is defined by a "" string.  Check return value [0] == 0 for no string entry. Unlike some functions, NULLPTR is not returned. 
    //
    // Pressing ESC will clear the text entry, remaining in the edit box for more input.  When there is a default set of text, the text will revert to the original default.
    //
    // GetString() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> sDefaut -- the sDefault parameter sets the default text for the text input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> Width()          -- Sets the width of the window in pixels.  The default sets the input window to the remainer of the window width from 
    //                         the start point (current console write position), which may seem too big.  "Width=100" sets the box with to 100 pixels, "With=50" sets it to 50, etc. 
    // --> CharWidth()      -- Sets the width of the window to number of chars in "CharWidth" based on average character width for the current font.
    //                         For example, "CharWidth=10" sizes the edit box to the width of 10 characters for the current font. This setting does not affect the number of characters
    //                         entered in the box, unless the "noscroll" option is set. 
    //                         If the "noscroll" option is set, input stops at the end of the box. Otherwise, the text will scroll horizontally as more text is entered
    // --> MaxChars()       -- Sets maximum number of characters that can be entered, regardless of window size (overrules "CharWidth" when "CharWidth" allows more characters
    //                         than "MaxChars".  For example, "MaxChars=10" will allow no more than 10 characters to be entered, regardless of input box size.
    // --> AllowScroll()    -- Allows horizontal scrolling in the input box
    //                         box.  Default behavior is to scroll text horizontally so that any number of characters can be entered in any size input box.
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
    // Examples:
    //
    // auto MyString = MyWindow.Console.GetString();    -- Gets a string using defaults
    // auto MyString = MyConsole.GetString(SageOpt_Colors("bw") SageOpt_CharWidth(10) SageOpt_BgColor("Yellow") SageOpt_BlankDefault);
    //
	CString GetString(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());

    // GetString() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter text an press return.
    //
    // Pressing <CR> or the window closing will return from GetString().  Empty input is defined by a "" string.  Check return value [0] == 0 for no string entry. Unlike some functions, NULLPTR is not returned. 
    //
    // Pressing ESC will clear the text entry, remaining in the edit box for more input.  When there is a default set of text, the text will revert to the original default.
    //
    // GetString() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> sDefaut -- the sDefault parameter sets the default text for the text input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> Width()          -- Sets the width of the window in pixels.  The default sets the input window to the remainer of the window width from 
    //                         the start point (current console write position), which may seem too big.  "Width=100" sets the box with to 100 pixels, "With=50" sets it to 50, etc. 
    // --> CharWidth()      -- Sets the width of the window to number of chars in "CharWidth" based on average character width for the current font.
    //                         For example, "CharWidth=10" sizes the edit box to the width of 10 characters for the current font. This setting does not affect the number of characters
    //                         entered in the box, unless the "noscroll" option is set. 
    //                         If the "noscroll" option is set, input stops at the end of the box. Otherwise, the text will scroll horizontally as more text is entered
    // --> MaxChars()       -- Sets maximum number of characters that can be entered, regardless of window size (overrules "CharWidth" when "CharWidth" allows more characters
    //                         than "MaxChars".  For example, "MaxChars=10" will allow no more than 10 characters to be entered, regardless of input box size.
    // --> AllowScroll()    -- Allows horizontal scrolling in the input box
    //                         box.  Default behavior is to scroll text horizontally so that any number of characters can be entered in any size input box.
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
    // Examples:
    //
    // auto MyString = MyWindow.Console.GetString();    -- Gets a string using defaults
    // auto MyString = MyConsole.GetString(SageOpt_Colors("bw") SageOpt_CharWidth(10) SageOpt_BgColor("Yellow") SageOpt_BlankDefault);
    //
	CString GetString(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    void getline(CString & cString,const cwfOpt & cwOpt = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
	void getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt());

	// Dialog Info/Question Windows

	void InfoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
	bool YesNoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
	Sage::DialogResult YesNoCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
	bool OkCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

	// Please Wait Window Functions

    // PleaseWaitWindow() - Open Please Wait Window
    //
    // Input String:
    //
    // --> Input String can be text-based, i.e. PleaseWaitWindow("ProgressBar,HideCancel") or Opt Strings, i.e. PleaseWaitWindow(opts::ProgressBar() | opt::HideCancel());
    // ----> One string puts a subtitle under "Please Wait...", i.e. PleaseWaitWindow("System is doing something"); 
    // ----> Two Strings replaces "Please Wait.." with text, then puts sub-title for second line, i.e. PleaseWaitWindow("Wait a few seconds...\nSystem is doing something");
    // ----> Inserting a Sring with '+' changes title bar text, i.e. PleaseWaitWindow('+My Program\nWait a few seconds...\nSystem is doing something") sets "My Program" as small window title.
    //
    // Options:
    //
    // ----> CancelOk            - Adds a Cancel button.  This can be chacked with PleaseWaitCancelled()
    // ----> HideCancel            - Adds a hidden cancel button that can be shown or re-hidden.  Can be used to delay cancel ability. Also Useful with PleaseWaitGetOk().
    // ----> ProgressBar        - Adds a Progress Bar that can be updated with SetPleaseWaitProgress()
    //
    // Examples:
    // ----> PleaseWaitWindow();    -- Opens a simple please wait window, with no button.  Closed with ClosePleaseWaitWindow();
    // ----> PleaseWaitWindow(opt::ProgressBar()) or PleaseWaitWindow("progressbar") -- Opens a Please Wait Window with a progress bar
    // ----> PleaseWaitWindow("System is Calculating Information",opt::CancelOk()) -- Opens a Please Wait Window with a subtitle and cancel button (which may be checked with PleaseWaitCancelled())
    // ----> Typically accessed as a dialog function, i.e. MyWindow->dialog.PleaseWaitWindow();
    //
	void PleaseWaitWindow(const char * sText = nullptr,const char * sOptions = nullptr);

    // PleaseWaitWindow() - Open Please Wait Window
    //
    // Input String:
    //
    // --> Input String can be text-based, i.e. PleaseWaitWindow("ProgressBar,HideCancel") or Opt Strings, i.e. PleaseWaitWindow(opts::ProgressBar() | opt::HideCancel());
    // ----> One string puts a subtitle under "Please Wait...", i.e. PleaseWaitWindow("System is doing something"); 
    // ----> Two Strings replaces "Please Wait.." with text, then puts sub-title for second line, i.e. PleaseWaitWindow("Wait a few seconds...\nSystem is doing something");
    // ----> Inserting a Sring with '+' changes title bar text, i.e. PleaseWaitWindow('+My Program\nWait a few seconds...\nSystem is doing something") sets "My Program" as small window title.
    //
    // Options:
    //
    // ----> CancelOk            - Adds a Cancel button.  This can be chacked with PleaseWaitCancelled()
    // ----> HideCancel            - Adds a hidden cancel button that can be shown or re-hidden.  Can be used to delay cancel ability. Also Useful with PleaseWaitGetOk().
    // ----> ProgressBar        - Adds a Progress Bar that can be updated with SetPleaseWaitProgress()
    //
    // Examples:
    // ----> PleaseWaitWindow();    -- Opens a simple please wait window, with no button.  Closed with ClosePleaseWaitWindow();
    // ----> PleaseWaitWindow(opt::ProgressBar()) or PleaseWaitWindow("progressbar") -- Opens a Please Wait Window with a progress bar
    // ----> PleaseWaitWindow("System is Calculating Information",opt::CancelOk()) -- Opens a Please Wait Window with a subtitle and cancel button (which may be checked with PleaseWaitCancelled())
    // ----> Typically accessed as a dialog function, i.e. MyWindow->dialog.PleaseWaitWindow();
    //
	void PleaseWaitWindow(const char * sText,const cwfOpt cwOptions);

    // PleaseWaitWindow() - Open Please Wait Window
    //
    // Input String:
    //
    // --> Input String can be text-based, i.e. PleaseWaitWindow("ProgressBar,HideCancel") or Opt Strings, i.e. PleaseWaitWindow(opts::ProgressBar() | opt::HideCancel());
    // ----> One string puts a subtitle under "Please Wait...", i.e. PleaseWaitWindow("System is doing something"); 
    // ----> Two Strings replaces "Please Wait.." with text, then puts sub-title for second line, i.e. PleaseWaitWindow("Wait a few seconds...\nSystem is doing something");
    // ----> Inserting a Sring with '+' changes title bar text, i.e. PleaseWaitWindow('+My Program\nWait a few seconds...\nSystem is doing something") sets "My Program" as small window title.
    //
    // Options:
    //
    // ----> CancelOk            - Adds a Cancel button.  This can be chacked with PleaseWaitCancelled()
    // ----> HideCancel            - Adds a hidden cancel button that can be shown or re-hidden.  Can be used to delay cancel ability. Also Useful with PleaseWaitGetOk().
    // ----> ProgressBar        - Adds a Progress Bar that can be updated with SetPleaseWaitProgress()
    //
    // Examples:
    // ----> PleaseWaitWindow();    -- Opens a simple please wait window, with no button.  Closed with ClosePleaseWaitWindow();
    // ----> PleaseWaitWindow(opt::ProgressBar()) or PleaseWaitWindow("progressbar") -- Opens a Please Wait Window with a progress bar
    // ----> PleaseWaitWindow("System is Calculating Information",opt::CancelOk()) -- Opens a Please Wait Window with a subtitle and cancel button (which may be checked with PleaseWaitCancelled())
    // ----> Typically accessed as a dialog function, i.e. MyWindow->dialog.PleaseWaitWindow();
    //
	void PleaseWaitWindow(const cwfOpt cwOptions);

    // ClosePlaseWait() -- Hides and Closes opened Please Wait Window
    //
	void ClosePleaseWait();

    // PleaseWaitCancelled() -- Returns true if the Cancel button was pressed on the Please Wait Window.  False, if not or Please Wait Window is not active.
    //
	bool PleaseWaitCancelled(Peek peek = Peek::No);

    // PleaseWaitGetOk() -- Changes "Cancel" button to "Ok" button and waits for the button to be pressed.  This is useful when a function has completed and you want to
    //                      make sure the user knows it is finish.
    //
    // This function returns TRUE when the butto is pressed, if the Cancel button exists (i.e. either "CancelOk" or "HideCancel" was specified).  Otherwise, TRUE is returned immediately
    //
	bool PleaseWaitGetOk();

    // PleaseWaitShowCancel() -- Shows (or Hides) the Cancel button.   If opt::CancelOk() or opt::HideCancel() is set in the PleaseWaitWindow() call, PleaseWaitShowCancel() will
    // hide or show the cancel button. 
    //
    // Input:
    //
    // PleaseWaitShowCancel(true) -- Shows the cancel button
    // PleaseWaitShowCancel(false) -- Hide the cancel button
    //
	bool PleaseWaitShowCancel(bool bShow = true);

    // SetPleaseWaitProgress() -- Sets the progress bar percent on the Please Wait Window
    //
    // This will set the bar to the percentage specified. For example, PleaseWaitProgress(57) will set the progress bar to 57% and print "57%" in the middle of the progress bar.
    //
    // Input:
    // --> iPercent: Percent completed, from 0-100
    // --> sMessage: When specified, replaces the percent display with the message.  This is useful for initialization or completion, for example:
    //      SetPleaseWaitProgress(100,"Finished.") will place "finished" on the progress bar instead of 100%
    //
    // Example:
    //    ----> SetPleaseWaitProgress(10,"Finished.");
    //    ----> PleaseWaitGetOk(); 
    //
    // --> These two lines show the progress bar full with the label "finished" within the progress bar.  The second line then changes the "Cancel" button to "Ok" and waits for the user to press it.
    //
    void SetPleaseWaitProgress(int iPercent,char * sMessage = nullptr);


    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
	CString GetOpenFile(Sage::stOpenFileStruct & stFile);

    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
    CString GetOpenFile(const char * sTypes = nullptr);
	
    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
	bool GetOpenFile(Sage::stOpenFileStruct & stFile,CString & csFilename);
    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
	bool GetOpenFile(const char * sTypes,CString & csFilename);

    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
	bool GetOpenFile(CString & csFilename);

    // GetOpenFileStruct() -- Return an stOpenFileStruct to set for using an GetOpenFile() or GetSaveFile() dialog.
    //
    // This is a shortcut for declaring a structure directly, so that auto& stStruct = GetOpenFileStruct() can be used
    // more easily.
    //
    Sage::stOpenFileStruct GetOpenFileStruct();

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
    bool QuickThumbnail(Sage::RawBitmap_t & stBitmap,int iWidth,int iHeight,Sage::ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
	bool QuickThumbnail(Sage::RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle);

        // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
    bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
	bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle);


    CBitmap QuickResize(RawBitmap_t & stBitmap,int iWidth,int iHeight,ResizeType eType = ResizeType::BestFit);
    CBitmap QuickResize(RawBitmap_t & stBitmap,SIZE szSize,ResizeType eType = ResizeType::BestFit);

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	CWindow & BitmapWindow(Sage::RawBitmap_t & stBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	CWindow & BitmapWindow(CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	CWindow & BitmapWindow(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	CWindow & BitmapWindow(int iX,int iY,Sage::RawBitmap_t & stBitmap,const cwfOpt & cwOpt = cwfOpt());

    CBitmap ReadImageFile(const char * sPath,bool * bSuccess = nullptr);
    ImageStatus GetLastImageStatus();


    // ReadJpegFile -- Read a jpeg file and store it into a CBitmap. 
    // This reads standard 8-bit jpeg (3-channel or monochrome).
    // If the file does not exist or is in an unsupported format, CBitmap will come back empty.
    // Use GetJpegError() to get the status of the last ReadJpegFile() call, which will
    // give information on why the Bitmap came back empty.
    //
    // if (bSuccess) is supplied, it is filled with true for a successful read, otherwise false.
    //
	CBitmap ReadJpegFile(const char * sPath,bool * bSuccess = nullptr);

    // ReadJpegFile -- Read a jpeg file and store it into a CBitmap. 
    // This reads standard 8-bit jpeg (3-channel or monochrome).
    // If the file does not exist or is in an unsupported format, CBitmap will come back empty.
    // Use GetJpegError() to get the status of the last ReadJpegFile() call, which will
    // give information on why the Bitmap came back empty.
    //
    // if (bSuccess) is supplied, it is filled with true for a successful read, otherwise false.
    //
	CBitmap ReadJpegMem(const unsigned char * sData,int iDataLength,bool * bSuccess);

    // Returns the last Jpeg status.  This will return CJpeg::Status::Ok if there was no error, 
    // or an indication of what went wrong with the last call, such as CJpeg::Status::EmptyFilePath or CJpeg::Status::FileNotFound
    //
    CJpeg::Status GetJpegError();

    // DevControls -- passed through to default CDevControls so that adding buttons, sliders, etc. is easy. 
    // For more controls (i.e. other Dev Control windows, use DevControlsWindow() to create new ones. 

 	// DevButton() -- Add a button to the Default Dev Control Window.  This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
    CButton & DevButton(const char * sButtonName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // DevCheckbox() -- Add a checkbox to the DevWindow. 
    //
    // All usual options apply to the checkbox, such as providing fgColor, Font, etc. 
    // If no text is supplied, a name for the checkbox is chosen automatically, such as "Checkbox 1", Checkbox 2", etc.
    //
    // --> If the checkbox name is preceded by '~' (i.e. "~My Checkbox"), this tells the DevWindow
    //     to place multiple checkboxes side-by-side (2 per-line) in the DevWindow to save space.
    //
    //     If the '~' is omitted (i.e. "My Checkbox"), all checkboxes are placed in the next vertical space
    //     in the DevWindow
    //
    //     note: to use '~' as the first character of the checkbox name, i.e. ("~Display Image"), use a double "~~". 
    //           For example, "~~Display Image" will display as "~Display Image"
    //
    CButton & DevCheckbox(const char * sCheckboxName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // QuickSlider() -- Add a slider to the Default Dev Controls Window.  The default width is 200 with a 0-100 range.  
    // The Range can be changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
    CSlider & DevSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // DevEditBox() -- Add an EditBox to the default Dev control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal EditBox options
	//
    // Note: DevEditBox() and DevInputBox() are the same.  The term Editbox is deprecated and is used for consitency with Windows control names.
    //
    CEditBox & DevEditBox(const char * sEditBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt());
    
    // DevInputBox() -- Add an InputBox to the default Dev control Window.  The sInputBoxName, while optional, will provide a
	// label to the left of the Input Box.  The default width is 150 pixels or so, but can be changed with normal InputBox options
	//
    // Note: DevEditBox() and DevInputBox() are the same.  The term Editbox is deprecated and is used for consitency with Windows control names.
    //
     CEditBox & DevInputBox(const char * sInputBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt()) { return DevEditBox(sInputBoxName,cwOpt); } // $$moveme
  
	CComboBox & DevComboBox(const char * sComboBoxName,const cwfOpt & cwOpt = cwfOpt());
	CWindow & DevWindow(const char * sTitle,int iNumlines,const cwfOpt & cwOpt = cwfOpt());
	CWindow & DevWindow(int iNumLines,const cwfOpt & cwOpt = cwfOpt());
	CWindow & DevWindow(const cwfOpt & cwOpt = cwfOpt());
	CWindow & DevWindow(const char * sTitle);

    CTextWidget & DevText(const char * sText,const cwfOpt & cwOpt  = cwfOpt());
    CTextWidget & DevText(const char * sText,int iHeight,const cwfOpt & cwOpt  = cwfOpt());
    CTextWidget & DevText(int iHeight,const cwfOpt & cwOpt  = cwfOpt());
    CTextWidget & DevText(const cwfOpt & cwOpt  = cwfOpt());


 	// DevAddSection() -- Adds a text section to the default Dev Controls window, to separate types of controls.
	// You can use opt::fgColor() to set the text color of the section name.
	//
    bool DevAddSection(const char * sSectionName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // GetDevControlsPtr() -- returns the pointer to the default CDevControls object. 
    // *** Important note *** -- this will return NULLPTR until a control is created with
    // QuickButton(), QuickSlider(), etc. The window is not created until a control is created in order
    // to save memory.
    //
    // With the pointer, any CDevContgrols() function can be used, though most are replicated directly
    // through CSageBox functions for ease of use.
    //
    CDevControls * GetDevControlsPtr();

    // DevGetWindow() -- Returns the Window (i.e. CWindow) of the default DevWindow.  This can be used to access window functions for the 
    // the default DevWindow. 
    //
    // For user-created Dev Control Windows, use the CDevControls::GetWindow() function, i.e. cMyDevWindow->GetWindow(); 
    //
    CWindow * DevGetWindow();

    // DevGetGroup() -- returns the group (i.e. WinGroup) substructure for the default Dev Window.  This can be used to perform group functions.
    // For example, when using multiple DevSlider() calls with a declared common group (i.e. DevSlider("MySlider",opt::Group(100,1)), group.SliderMoved(100)
    // can be used to determine if any slider in the group was moved, the slider ID and the slider position.
    //
    // Groups created in DevControls must be used through that Dev Controls Window's group function, which is why DevGroup is provided.
    //
    // For example, using DevGetGroup()->SliderMoved(MyGroup,..) will check any sliders within the given group within the DevWindow.
    //
    // This function only works for the default DevWindow.  For user-created DevWindows, use the CDevControls::group() function, i.e.
    // cMyDevWindow->group().SliderMoved(MyGroup,...)
    //
    CWindow::WinGroup * DevGetGroup();
    
    // DevControlsWindow() -- Create a CDevControls Window, allowing for quick creation and automatic placement of
    // controls (buttons, slider, editboxes, etc.) in the window.   This allows quick prototyping and development of non
    // GUI functions with GUI controls. 
    // 
    // See documentationn in CQuickControls.h for more information
    //
    // ** Important note ** the object pointer returned MUST BE DELETED, as it is not a managed object. 
    // Example code tends to use Obj<CDevControls> cDevControls = DevControlsWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
    CDevControls * DevControlsWindow(int iX = -1,int iY = -1,const cwfOpt & cwOpt = cwfOpt());

    // DevControlsWindow() -- Create a CDevControls Window, allowing for quick creation and automatic placement of
    // controls (buttons, slider, editboxes, etc.) in the window.   This allows quick prototyping and development of non
    // GUI functions with GUI controls. 
    // 
    // See documentationn in CQuickControls.h for more information
    //
    // ** Important note ** the object pointer returned MUST BE DELETED, as it is not a managed object. 
    // Example code tends to use Obj<CDevControls> cDevControls = DevControlsWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
    CDevControls * DevControlsWindow(const cwfOpt & cwOpt);

    // DevControlsTopmost() -- Sets the Default Dev controls windows as a 'topmost' window, 
    // preventing other windows from overlapping and obscuring the Dev Controls Window.
    // This is the equivalent of GetDevControlsPtr()->SetTopmost(), or using SetTopMost() in a 
    // user-created window (see DevControlsWindow() to create more Dev Control Windows).
    // -->
    // Setting bTopMost = false sets the default Dev Controls Window to normal behavior, allowing
    // other windows to overlap when they gain focus and are overlapping the default Dev Controls Window
    //
    bool DevControlsTopmost(bool bTopmost = true);

    // ExitButton() -- Create a modal button (i.e. a button that needs input before continuing) with a message.  This can be used
    // prior to exiting a console-mode program so that the windows and controls remain intact until the user pressed
    // the "OK" button.  This is the same as QuickInfoButton() but uses a default "program ending" message when there is
    // no text included.
    // --> sText sets the message to display to the user.  When sText is omitted a message declaring the program has ended
    //     is displayed as the default message.
    // 
    // Exit Button returns 0. 
    //
    int ExitButton(const char * sText = nullptr); 

    // WaitforClose() -- Displays a message box notifying the user the program has ended.
    // Since Sagebox has no window, WaitforClose() calls ExitButton() and returns the code given. 
    //
    // WaitforClose() in included for parity with CWindow::WaitforClose() (which simply waits for the close with no button, since it has a window)
    //
    // See ExitButton(), which allows a personalized message. 
    //
    // ReturnCode is returned from WaitforClose() to allow an easy return from main() as return WaitforClose(); 
    // If WaitforClose() fails for any reason, it returns -1
    //
    int WaitforClose(int iReturnCode = 0);

   // ReadPgrBitmap() -- Reads a Bitmap or JPEG file from a .PGR file (or PGR Memory) and returns a 
    // CBitmap.
    //
    // This function is used to quick access to BMP or JPEG files embedded in PGR file without 
    // opening the PGR file and searching for the image.
    //
    // ReadPgrBitmap() opens the PGR, searches for the image (BMP, compress BMP (TPC), or JPEG) and loads it
    // if found.  Otherwise, an empty CBitmap is returned.
    //
    // if bSucess is passed, this is filled with TRUE if an image was found, and FALSE if there was no image or an 
    // error occurred loading the image.
    //
    // --> Examples:
    // --> auto cBitmap = ReadPgrBitmap("ImageName","myPgrFile.pgr");
    // --> auto cBitmap = ReadPgrBitmap("Bitmaps:Image1",PgrMem); 
    //
	CBitmap ReadPgrBitmap(const char * sImageTitle,const char * sPgrFile,bool * bSuccess = nullptr);

    // ReadPgrBitmap() -- Reads a Bitmap or JPEG file from a .PGR file (or PGR Memory) and returns a 
    // CBitmap.
    //
    // This function is used to quick access to BMP or JPEG files embedded in PGR file without 
    // opening the PGR file and searching for the image.
    //
    // ReadPgrBitmap() opens the PGR, searches for the image (BMP, compress BMP (TPC), or JPEG) and loads it
    // if found.  Otherwise, an empty CBitmap is returned.
    //
    // if bSucess is passed, this is filled with TRUE if an image was found, and FALSE if there was no image or an 
    // error occurred loading the image.
    //
    // --> Examples:
    // --> auto cBitmap = ReadPgrBitmap("ImageName","myPgrFile.pgr");
    // --> auto cBitmap = ReadPgrBitmap("Bitmaps:Image1",PgrMem); 
    //
	CBitmap ReadPgrBitmap(const char * sImageTitle,const unsigned char * sPGRMemory,bool * bSuccess = nullptr);

    // GetBitmapStruct() -- Returns a RawBitmap_t struct with memory for the Width and height.
    // If Height is omitted, a Bitmap structure of height 1 is returned.
    //
    // This is returned as an aligned 8-bit-per-channel bitmap.
    //
    // [[nodiscard]] -- Important note: This returns a RawBitmap_t structure which has allocate memory.
    // RawBitmap_t::Delete() must be called to delete this memory.
    //
    // Assign this to CBitmap, such as CBitmap cBitmap = GetWindowBitmap(); 
    // CBitmap will delete this memory automatically.  Otherwise, call RawBitmap_t::Delete() to make sure the memory is deleted
    //
    CBitmap CreateBitmap(int iWidth,int iHeight = 1);

    // GetBitmapStruct() -- Returns a RawBitmap_t struct with memory for the Width and height.
    // If Height is omitted, a Bitmap structure of height 1 is returned.
    //
    // This is returned as an aligned 8-bit-per-channel bitmap.
    //
    // [[nodiscard]] -- Important note: This returns a RawBitmap_t structure which has allocate memory.
    // RawBitmap_t::Delete() must be called to delete this memory.
    //
    // Assign this to CBitmap, such as CBitmap cBitmap = GetWindowBitmap(); 
    // CBitmap will delete this memory automatically.  Otherwise, call RawBitmap_t::Delete() to make sure the memory is deleted
    //
    CBitmap CreateBitmap(SIZE szBitmapSize);

    bool ShowConsole(bool bShow = true);
    bool HideConsole(bool bPermanent = false);
    bool ValidateControl(HWND hWnd);
    bool ValidateWindow(CWindow * cWin);

    // Determine if we're console app or a windows app.  
    //
    // This returns true of the app started as a Windows app (i.e. _WINDOWS is defined), even if it has created a console window.
    // This returns false if both _CONSOLE mode was set and there is no console window.  Therefore, if a console mode program has deleted
    // its console window, this will return true to indicate to the program that this isn't a console-capable application, since it has no console window
    //
    // This is meant to be called at any time, but particularly before or right after SageBox is created so that a prorgram may determine 
    // a course of action.
    // 
    // This is a static function and can be called without SageBox having been created, i.e. CSageBox::isConsoleApp()
    //
    static bool isConsoleApp() { 
            #ifdef _WINDOWS 
                return false;        // if _WINDOWS is defined, it's a good bet, so we'll trust it, even if it has defined a console app on its own.
            #else           
                // When _CONSOLE is defined, lets also make sure we have a console window
                return GetConsoleWindow() != nullptr;
            #endif          
    }

    // Returns true of the main thread is stopped.  
    // Use StartThread() to resume the main thread. 
    //
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    bool ThreadStopped();
    
    // Returns the status of the thread.  ThreadStatus::Running or ThreadStatus::Suspended (if the thread is stopped).
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    ThreadStatus GetThreadStatus();

    // Stop the main thread.  This is usually used when ending the main thread and transferring to full event-driven control. 
    //
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    // the value iReturnValue is returned from StopThread() when it wakes up.
    //
    //
    int StopThread();

    // Resume the main thread if it is suspended.
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    bool ResumeThread(int iValue = 0);

    // End the program when in the main message thread.  This is used when StopThread() has been used to stop the main thread but events are
    // still being handled through the Main Windows Message Thread.
    //
    // EndProgram() sets the window closing status and resumes the main thread.  Typically, the main thread will exit and SageBox will end.
    // However, StopThread() can be used anywhere and does not need to exit immediately.  It can take care of cleanup, memory deallocations, etc. 
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    bool EndProgram(int iReturnValue = 0);

   // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    RGBColor_t  GetColor(const char * sColor,bool * pColorFound = nullptr);

    // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    bool GetColor(const char * sColor,DWORD & rgbColor);
   // MakeColor() -- Make a named system color useable throughout SageBox functions.
    //
    // Make Color can create a color for use with other function, by namin the color and returning an RGBColor_t value that can be used with all Sagebox
    // functions.
    //
    // Example:
    //
    // MakeColor("MyColor",{ 255,128,128} );    -- Make a light red and call it MyColor
    // LightRed = MakeColor("MyColor",{ 255,128,128} );    -- Make "MyColor" and also store it in the RGBColor_t value LightRed
    //
    // cWin.SetFgColor("MyColor");      -- Set the foreground color to the color "MyColor";
    // cWin.SetFgColor(LightRed);       -- Set the foreground color to the RGBColor_t color LightRed
    // cWin.Write("{MyColor}This is light red{/}")   -- Set the color "MyColor" in an output string.
    //
    RGBColor_t MakeColor(const char * sColor,DWORD rgbColor);

    // MakeColor() -- Make a named system color useable throughout SageBox functions.
    //
    // Make Color can create a color for use with other function, by namin the color and returning an RGBColor_t value that can be used with all Sagebox
    // functions.
    //
    // Example:
    //
    // MakeColor("MyColor",{ 255,128,128} );    -- Make a light red and call it MyColor
    // LightRed = MakeColor("MyColor",{ 255,128,128} );    -- Make "MyColor" and also store it in the RGBColor_t value LightRed
    //
    // cWin.SetFgColor("MyColor");      -- Set the foreground color to the color "MyColor";
    // cWin.SetFgColor(LightRed);       -- Set the foreground color to the RGBColor_t color LightRed
    // cWin.Write("{MyColor}This is light red{/}")   -- Set the color "MyColor" in an output string.
    //
    RGBColor_t MakeColor(const char * sColor,RGBColor_t rgbColor);

   // ImportClipboardText() -- Returns Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CString object will be returned with its contents.
    // Otherwise, an empty CString will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CString returned is empty);
    //
    CString ImportClipboardText(bool * bSuccess = nullptr);

    // ImportClipboardTextW() -- Returns a Unicode Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CStringW object will be returned with its contents.
    // Otherwise, an empty CStringW will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CStringW returned is empty);
    //
    CStringW ImportClipboardTextW(bool * bSuccess = nullptr);

    // ImportClipboardBitmap() -- Returns a CBitmap with a copy of the Bitmap in the Clipboard buffer. 
    //
    // If there is no bitmap in the clipboard buffer, an empty CBitmap will be returned. 
    //
    // if a bSuccess pointer is provided, this is fille with the bitmap status (TRUE if the bitmap was filled, FALSE if there is no bitmap
    // in the Windows Clipboard, and the returned CBitmap is empty)
    //
    // This will copy an 8-bit, 24-bit, or 32-bit bitmap.  In the case of an 8-bit bitmap, the grayscale bitmap is returned in the 
    // CBitmap with the Red, Green, and Blue values filled with the gray value
    //
    // With 32-bit bitmaps, a mask element is created in the CBitmap, to which the Alpha channel for the bitmap is copied.
    //
    CBitmap ImportClipboardBitmap(bool * bSuccess = nullptr);

    /// <summary>
    /// Disables ^C from exiting the program when pressed in input boxes. 
     /// <para></para>&#160;&#160;&#160;
   /// By default, ^C will exit the program when the user presses it when in an input loop.
    /// For SageBox Quick C++ applications, pressing ^C any time will exit the program by default.  In Sagebox,
    /// calling EnableControlCExit() must be called. 
    /// <para></para>&#160;&#160;&#160;
    /// When this is disabled, the ^C will no longer have any effect, but the program can still be terminated via the Process Window (when active)
    /// or the System Menu. 
    /// </summary>
    /// <param name="bDisable">Disables Control^C exit when true.  Enables Control ^C exit when false</param>
    /// <returns></returns>
    bool DisableControlCExit(bool bDisable = true);

     /// <summary>
    /// Enables pressing ^C to exit from the program  when pressed in input boxes or when the program is running.
    /// <para></para>&#160;&#160;&#160;
    /// By default, ^C will exit the program when the user presses it when in an input loop.
    /// For SageBox Quick C++ applications, pressing ^C any time will exit the program by default.  In Sagebox,
    /// calling EnableControlCExit() must be called. 
    /// <para></para>&#160;&#160;&#160;
    /// When this is disabled, the ^C will no longer have any effect, but the program can still be terminated via the Process Window (when active)
    /// or the System Menu. 
    /// </summary>
    /// <param name="bEnable">Enables Exit by ^C when true; Disables when false</param>
    /// <returns></returns>
    bool EnableControlCExit(bool bEnable = true);

   /// <summary>
    /// Returns true when ^C will exit the program when pressed
    /// </summary>
    /// <returns></returns>
    bool isControlCExitDisabled(); 


    /// <summary>
    /// Returns a string with version information for the Sagebox library, as well as current
    /// platform (i.e. 32 bits or 64 bits). 
    /// </summary>
    /// <returns>String with current library version and run-time platform</returns>
    static CString GetVersionInfo();

    /// <summary>
    /// returns TRUE of running 64-bit windows, false if 32-it windows. 
    /// </summary>
    /// <returns></returns>
    static bool is64BitWindows();

    static bool CreateProcessWindow(ProcessWinStartMode eStartMode = ProcessWinStartMode::Default);
    static bool WriteProcessWindow(const char * sText); 
    static bool ShowProcessWindow(bool bShow,bool bShowDebug);
    static bool ProcessWindowVisible();
    static bool ProcessWindowKillTimer(bool bKillTimer);
    static bool ProcessWindowShowLineNumbers(bool bShowLineNumbers);

    // Debug class -- directly replicated from CWindow::WinDebug with a couple exceptions, such as starting the 
    // ^C accelerator.   Otherwise, it passes through the main hidden window in Sagebox



    // ---------------
    // Debug Functions
    // ---------------
    //
    // Provides debug output functions. 
    //
    // Note: This section is currently still under construction and is expected to grow over time.,
    //       Currently, it offers debug functions mostly through the Process Window, a widow that is disconected from
    //       Sagebox and any windows in Sagebox for the purpose of more low-level debugging.
    //
    // In the future, a more robust debug window more integral to Sagebox will also be provided.
    //
    // Debugging is meant to be as low-level as possible. 
    //
    // Important note:  Do not change this independently.  Except for unique items, CWindow::WinDebug must remain in parallel here. 
    //                  (they will probably be merged at some point)
    //
    // See items unique to CSagebox version (this one) at the bottom. 
    //
    struct WinDebug
    {
        friend CSagebox;
    private:
        CWindow         * m_cWin = nullptr;     // Main CSagebox Window (which is always hidden)
    public:
        WinDebug & operator << (const char * x)       ;
        WinDebug & operator << (char x)               ;
        WinDebug & operator << (std::string & cs)     ;
        WinDebug & operator << (CString & cs)         ;
        WinDebug & operator << (int x)                ;
        WinDebug & operator << (unsigned int x)       ;
        WinDebug & operator << (float x)              ;
        WinDebug & operator << (double x)             ;
                                                    ;
        // Process Control Window initialization types. 
        // Determines how to open Process Control Window when it is initialized with Init()
        //
        enum class InitType
        {
            Hidden,                 // Initially Hidden.  Use the System Menu on any main window or move move to upper-right desktop
            Visible,                // Shows as Title Bar only in upper-right of desktop.  Mouse mouse to title bar to enlarge
            Debug,                  // Brings up the Process Control Window as visible with the Debug Window.
            DebugLineNumbers,       // Brings up the Process Control Window as visible with the Debug Window with Line Numbers showing
        };

        // Init() -- Initialize the process control window.
        //
        // Init() initializes the process control window and initially brings it up as hidden (by default), unless overridden. 
        //
        // Note: when the Process Control Window is hidden, you can use the System Menu in any main window to bring it up, or 
        // move the mouse to the uper-right-hand corner of the window for more than .25 seconds (which toggles the Process Control Window display)
        //
        // Init() may be called with one of the following options
        //
        //      Init() or Init(false)                   -- Bring up Process Control Window as hidden.
        //      Init(true)                              -- Brings up the Process Control Window as a title bar in the 
        //                                                 upper-right of the window.  Move the mouse over the window to enlarge.
        //      Init(WinDebug::InitType::Debug)         -- Brings up the Process Control Window with the Debug Window showing. This will
        //                                                 not auto-minimize as it does when the "Show Debug" switch is off. 
        //      Init(WinDebug::InitType::DebugLineNumbers)         -- Same as WinDebug::InitType::Debug, but with line numbers turn on
        //                                                            (see WinDebug:ShowLineNumbers())
        //
        // Init() is not necessary for debug output -- the first call to any debug output routine will automatically initalize
        // and display the Process Control Window and its debug window. 
        //
        // returns TRUE if Process Window is active. 
        //
        bool Init(bool bVisible = false);
        
        // Init() -- Initialize the process control window. 
        //
        // Init() initializes the process control window and initially brings it up as hidden (by default), unless overridden. 
        //
        // Note: when the Process Control Window is hidden, you can use the System Menu in any main window to bring it up, or 
        // move the mouse to the uper-right-hand corner of the window for more than .25 seconds (which toggles the Process Control Window display)
        //
        // Init() may be called with one of the following options
        //
        //      Init() or Init(false)                   -- Bring up Process Control Window as hidden.
        //      Init(true)                              -- Brings up the Process Control Window as a title bar in the 
        //                                                 upper-right of the window.  Move the mouse over the window to enlarge.
        //      Init(WinDebug::InitType::Debug)         -- Brings up the Process Control Window with the Debug Window showing. This will
        //                                                 not auto-minimize as it does when the "Show Debug" switch is off. 
        //      Init(WinDebug::InitType::DebugLineNumbers)         -- Same as WinDebug::InitType::Debug, but with line numbers turn on
        //                                                            (see WinDebug:ShowLineNumbers())
        //
        // Init() is not necessary for debug output -- the first call to any debug output routine will automatically initalize
        // and display the Process Control Window and its debug window. 
        //
        // returns TRUE if Process Window is active. 
        //
        bool Init(InitType eInitType);
        
        
        // Show() -- Show or Hide the Process Control Window
        //
        bool Show(bool bShow = true); 

        // Hide() -- Hide or Show the Process Control Window
        //
        bool Hide(bool bHide = true);

        // printf() -- print out to the debug window, just as in std::printf()
        //
        bool printf(const char * Format,...);
// __Sandbox__Except__ deprecated (for now)
//#ifdef __Sandbox__Except__
        // printf() -- print out to the debug window, just as in std::printf()
        //
        bool __printf(const char * Format,...);
//#endif
        // Write() -- Write out to the debug window.  Can be quicker than using printf()
        //
        bool Write(const char * sText);

        // KillTimer() -- Kills the 250ms timer that looks at mouse movement.  
        //
        // KillTimer() removes the timer in the Process Window.  It is activated every 250ms to look for mousemovements to 
        // show or hide the Process Control Window when the mouse is moved to the upper-right-hand corner of the desktop. 
        //
        // The timer doesn't impact program execution time, but for those programs that do not want a timer for one simple purpose
        // can turn it off. 
        //
        // Use KillTimer(false) to turn the timer back on.
        //
        bool KillTimer(bool bKillTimer = true);

        // ShowLineNumbers() -- Shows line numbers in the debug display.
        //
        // The Debug window defaults to ShowLineNumbers off.  You can use 
        // Init(CWindow::WinDebug::InitType::DebugLineNumbers) to initialize the window with line numbers on.
        //
        // Use ShowLineNumbers(true) turn turn line numbers on
        // Use ShowLineNumbers(false) to turn line numbers off.
        //
        bool ShowLineNumbers(bool bShowLineNumbers = true);

        // --------------------------------------------------
        // CSagebox::WinDebug unique items (may be called by CWindow)
        // --------------------------------------------------

        // SetControlCBreak() -- Allow ^C to terminate the process. 
        //
        // When set to TRUE, using Control-C will bring up a message box and allow the user to terminate the process. 
        // This is a useful debugging tool to allow terminating the process if the process caught in a loop or may be otherwise
        // unresponsive.  Also use debug.Init() to activate the Process Window for more control, even if the window is not visible. 
        //
        // For Console Applications, you can just press control-C in the console-window. 
        //
        // Note: debug.Init() activates ^C, but debug.Write(), when it instantiates the Proess Control Window will not turn it on. 
        // use debug.Init() or SetControlCBreak() to ensure use of ^C to break the program execution.
        //
       bool SetControlCBreak(bool bActive); 

     };

    WinDebug debug; 
};

}; // namespace Sage;



#endif // _CSageBox_H_