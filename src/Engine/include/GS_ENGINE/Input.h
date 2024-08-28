#pragma once
#ifndef GS_INPUT_H
#define GS_INPUT_H

#ifdef _WIN32
#define GS_LBUTTON 0x01  //	Left mouse button
#define GS_RBUTTON 0x02  //	Right mouse button
#define GS_CANCEL 0x03   //	Control-break processing
#define GS_MBUTTON 0x04  //	Middle mouse button
#define GS_XBUTTON1 0x05 //	X1 mouse button
#define GS_XBUTTON2 0x06 //	X2 mouse button
#define GS_BACK 0x08     //	BACKSPACE key
#define GS_TAB 0x09      //	TAB key
#define GS_CLEAR 0x0C    //	CLEAR key
#define GS_RETURN 0x0D   //	ENTER key
#define GS_SHIFT 0x10    //	SHIFT key
#define GS_CONTROL 0x11  //	CTRL key
#define GS_MENU 0x12     //	ALT key
#define GS_PAUSE 0x13    //	PAUSE key
#define GS_CAPITAL 0x14  //	CAPS LOCK key
#define GS_ESCAPE 0x1B   // ESC key
#define GS_SPACE 0x20    // SPACEBAR
#define GS_PRIOR 0x21    // PAGE UP key
#define GS_NEXT 0x22     // PAGE DOWN key
#define GS_END 0x23      // END key
#define GS_HOME 0x24     // HOME key
#define GS_LEFT 0x25     // LEFT ARROW key
#define GS_UP 0x26       // UP ARROW key
#define GS_RIGHT 0x27    // RIGHT ARROW key
#define GS_DOWN 0x28     //	DOWN ARROW key
#define GS_SELECT 0x29   // SELECT key
#define GS_PRINT 0x2A    // PRINT key
#define GS_EXECUTE 0x2B  //	EXECUTE key
#define GS_SNAPSHOT 0x2C //	PRINT SCREEN key
#define GS_INSERT 0x2D   // INS key
#define GS_DELETE 0x2E   // DEL key
#define GS_HELP 0x2F     //	HELP key
#define GS_0 0x30
#define GS_1 0x31
#define GS_2 0x32
#define GS_3 0x33
#define GS_4 0x34
#define GS_5 0x35
#define GS_6 0x36
#define GS_7 0x37
#define GS_8 0x38
#define GS_9 0x39
#define GS_A 0x41
#define GS_B 0x42
#define GS_C 0x43
#define GS_D 0x44
#define GS_E 0x45
#define GS_F 0x46
#define GS_G 0x47
#define GS_H 0x48
#define GS_I 0x49
#define GS_J 0x4A
#define GS_K 0x4B
#define GS_L 0x4C
#define GS_M 0x4D
#define GS_N 0x4E
#define GS_O 0x4F
#define GS_P 0x50
#define GS_Q 0x51
#define GS_R 0x52
#define GS_S 0x53
#define GS_T 0x54
#define GS_U 0x55
#define GS_V 0x56
#define GS_W 0x57
#define GS_X 0x58
#define GS_Y 0x59
#define GS_Z 0x5A
#define GS_LWIN 0x5B      // Left Windows key
#define GS_RWIN 0x5C      // Right Windows key
#define GS_APPS 0x5D      // Applications key
#define GS_NUMPAD0 0x60   // Numeric keypad 0 key
#define GS_NUMPAD1 0x61   // Numeric keypad 1 key
#define GS_NUMPAD2 0x62   // Numeric keypad 2 key
#define GS_NUMPAD3 0x63   // Numeric keypad 3 key
#define GS_NUMPAD4 0x64   // Numeric keypad 4 key
#define GS_NUMPAD5 0x65   // Numeric keypad 5 key
#define GS_NUMPAD6 0x66   // Numeric keypad 6 key
#define GS_NUMPAD7 0x67   // Numeric keypad 7 key
#define GS_NUMPAD8 0x68   // Numeric keypad 8 key
#define GS_NUMPAD9 0x69   // Numeric keypad 9 key
#define GS_MULTIPLY 0x6A  // Multiply key
#define GS_ADD 0x6B       // Add key
#define GS_SEPARATOR 0x6C // Separator key
#define GS_SUBTRACT 0x6D  // Subtract key
#define GS_DECIMAL 0x6E   // Decimal key
#define GS_DIVIDE 0x6F    // Divide key
#define GS_F1 0x70        // F1 key
#define GS_F2 0x71        // F2 key
#define GS_F3 0x72        // F3 key
#define GS_F4 0x73        // F4 key
#define GS_F5 0x74        // F5 key
#define GS_F6 0x75        // F6 key
#define GS_F7 0x76        // F7 key
#define GS_F8 0x77        // F8 key
#define GS_F9 0x78        // F9 key
#define GS_F10 0x79       // F10 key
#define GS_F11 0x7A       // F11 key
#define GS_F12 0x7B       // F12 key
#define GS_F13 0x7C       // F13 key
#define GS_F14 0x7D       // F14 key
#define GS_F15 0x7E       // F15 key
#define GS_F16 0x7F       // F16 key
#define GS_F17 0x80       // F17 key
#define GS_F18 0x81       // F18 key
#define GS_F19 0x82       // F19 key
#define GS_F20 0x83       // F20 key
#define GS_F21 0x84       // F21 key
#define GS_F22 0x85       // F22 key
#define GS_F23 0x86       // F23 key
#define GS_F24 0x87       // F24 key
#define GS_NUMLOCK 0x90   // NUM LOCK key
#define GS_SCROLL 0x91    // SCROLL LOCK key
#define GS_LSHIFT 0xA0    // Left SHIFT key
#define GS_RSHIFT 0xA1    // Right SHIFT key
#define GS_LCONTROL 0xA2  // Left CONTROL key
#define GS_RCONTROL 0xA3  // Right CONTROL key
#define GS_LMENU 0xA4     // Left ALT key
#define GS_RMENU 0xA5     // Right ALT key

#define VK_TO_GS_KEY(VK_KEY) (VK_KEY)

#elif __linux__
#error "Not Implemented Yet"
#endif
#endif
