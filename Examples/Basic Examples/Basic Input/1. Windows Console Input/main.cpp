
// --------------------------------------
// Sagebox  Basic Input Examples
// --------------------------------------
//
// This is a set of 5 examples showing basic input, from basic stdio input/output to using console-based
// Sagebox functions, to using one-line Sagebox dialogs and quick dialogs with multiple input controls. 
//
// ** Note: The examples in this solution can be either Console Mode or Pure Windows programs. 
// **       Select Build->Configuration to Set Console Mode vs. Windows, and other options.
//
// The program simply asks for two numbers, Radius1 and Radius 2, and puts an ellipse with using the two 
// radius values in a Sagebox window.
//
// The examples slowly start adding validation and filled vs. non-filled ellipse selections.
//
//      Examples Description
//      --------------------
//
//      1. --> Windows-Based Console Input                  -- uses the Sagebox Window's console-like input/output 
//
//             This uses the Sagebox version of console input.  It is very similar to console input and output, using the 
//             same form as cin and cout, but with the window name.
//
//             We can also use "console." functions -- try typing "console." or "cWin.console." to see the list of functions.
//
//             The console window isn't needed, so we could have used the Windows version of the Console project, but it was left
//             showing to show that we're still technically in a Console mode program, though it might as well be a pure Windows program, which 
//             is adjustable as a project setting in the regular console project (it was removed here to show the console project as an example_
//
//
//             Note there is no validation here: this is done in the next example
//
//      2.     Windows-Based Console Input
//             With Validation                              -- Adds Sagebox Options to add quick-and-easy validation to the previous version
//      3.     One-Item Dialog Entry                        -- Uses a Sagebox GetInteger() dialog to get a single Radius value.
//      4.     Two-Item Dialog Entry                        -- Uses Sagebox Quick Dialog to get both radius values (with validation)
//      5.     Sagebox-Based Two Items and a Checkbox       -- Adds a checkbox to the previous example to select filled/non-filled ellipse. 
//

#include "CSagebox.h"

int main()
{   
    auto &cWin = CSageBox::AutoWindow();    // It's a simple app so we can just create Sagebox and the Window together.

    RgbColor rgbColor = { 255,0,0 };        // Or we could use win.GetColor("Red'); 
                                            // We can also use a standard Windows COLORREF RGB(255,0,0); 

    int iRadius1 = 0,iRadius2 = 0;          // We don't need to set them to 0, since we get a guaranteed numeric result. 
                                            // (which is 0 when the number is invalid, such as non-numeric entry)

    // Use the window's console i/o versions for input/output.  If the user closes the window the entry falls through with 
    // a return of 0

    cWin << "Enter Radius 1: ";          // Simple i/o, as in C++, but without the "cin >> integer" issue. 
    cWin >> iRadius1;

    cWin << "Enter Radius 2: "; 
    cWin >> iRadius2; 

    cWin.DrawEllipse(400,400,iRadius1,iRadius2,rgbColor); 

    cWin.ExitButton();              // Get input (via the Exit Button) from the user so the program doesn't close down.
                                    // We can also use cWin.WaitforClose() to wait for the window to close, without the button.
  
}