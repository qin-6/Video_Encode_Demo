#ifndef _CLOUDXR_INPUT_EVENTS_
#define _CLOUDXR_INPUT_EVENTS_

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{ // extern "C"
#endif

typedef enum
{
    scxrKEY_NONE = 0,

    scxrKEY_APOSTROPHE = 0x0027,
    scxrKEY_COMMA = 0x002c,
    scxrKEY_MINUS = 0x002d,
    scxrKEY_PERIOD = 0x002e,
    scxrKEY_SLASH = 0x002f,
    scxrKEY_SEMICOLON = 0x003b,
    scxrKEY_EQUAL = 0x003d,
    scxrKEY_BRACKETLEFT = 0x005b,
    scxrKEY_BACKSLASH = 0x005c,
    scxrKEY_BRACKETRIGHT = 0x005d,
    scxrKEY_NONUS_BACKSLASH = 0x005e,
    scxrKEY_YEN = 0x005f, // This key is present on Japanese Keyboard [Windows ScanCode:7D]
    scxrKEY_HANGUL = 0x0060, // This key is present on Korean Keyboard [Windows ScanCode:72]
    scxrKEY_HANJA = 0x0061, // This key is present on Korean Keyboard [Windows ScanCode:71]

    scxrKEY_SPACE = 0x0020,

    scxrKEY_0 = 0x0030,
    scxrKEY_1 = 0x0031,
    scxrKEY_2 = 0x0032,
    scxrKEY_3 = 0x0033,
    scxrKEY_4 = 0x0034,
    scxrKEY_5 = 0x0035,
    scxrKEY_6 = 0x0036,
    scxrKEY_7 = 0x0037,
    scxrKEY_8 = 0x0038,
    scxrKEY_9 = 0x0039,

    scxrKEY_A = 0x0041,
    scxrKEY_B = 0x0042,
    scxrKEY_C = 0x0043,
    scxrKEY_D = 0x0044,
    scxrKEY_E = 0x0045,
    scxrKEY_F = 0x0046,
    scxrKEY_G = 0x0047,
    scxrKEY_H = 0x0048,
    scxrKEY_I = 0x0049,
    scxrKEY_J = 0x004a,
    scxrKEY_K = 0x004b,
    scxrKEY_L = 0x004c,
    scxrKEY_M = 0x004d,
    scxrKEY_N = 0x004e,
    scxrKEY_O = 0x004f,
    scxrKEY_P = 0x0050,
    scxrKEY_Q = 0x0051,
    scxrKEY_R = 0x0052,
    scxrKEY_S = 0x0053,
    scxrKEY_T = 0x0054,
    scxrKEY_U = 0x0055,
    scxrKEY_V = 0x0056,
    scxrKEY_W = 0x0057,
    scxrKEY_X = 0x0058,
    scxrKEY_Y = 0x0059,
    scxrKEY_Z = 0x005a,

    scxrKEY_AGRAVE = 0x00c0,
    scxrKEY_MULTIPLY = 0x00d7,
    scxrKEY_DIVISION = 0x00f7,

    scxrKEY_ESCAPE = 0x0100,
    scxrKEY_TAB = 0x0101,
    scxrKEY_BACKTAB = 0x0102,
    scxrKEY_BACKSPACE = 0x0103,
    scxrKEY_RETURN = 0x0104,
    scxrKEY_ENTER = 0x0105,
    scxrKEY_INSERT = 0x0106,
    scxrKEY_DELETE = 0x0107,
    scxrKEY_PAUSE = 0x0108,
    scxrKEY_PRINT = 0x0109,
    scxrKEY_CLEAR = 0x010a,

    scxrKEY_HOME = 0x0200,
    scxrKEY_END = 0x0201,
    scxrKEY_LEFT = 0x0202,
    scxrKEY_UP = 0x0203,
    scxrKEY_RIGHT = 0x0204,
    scxrKEY_DOWN = 0x0205,
    scxrKEY_PAGE_UP = 0x0206,
    scxrKEY_PAGE_DOWN = 0x0207,

    // Modifier keys
    scxrKEY_SHIFT = 0x0301,
    scxrKEY_LSHIFT = 0x0302,
    scxrKEY_RSHIFT = 0x0303,

    scxrKEY_CONTROL = 0x0304,
    scxrKEY_LCONTROL = 0x0305,
    scxrKEY_RCONTROL = 0x0306,

    scxrKEY_ALT = 0x0307,
    scxrKEY_LALT = 0x0308,
    scxrKEY_RALT = 0x0309,

    scxrKEY_META = 0x0310,
    scxrKEY_LMETA = 0x0311,
    scxrKEY_RMETA = 0x0312,

    scxrKEY_F1 = 0x0400,
    scxrKEY_F2 = 0x0401,
    scxrKEY_F3 = 0x0402,
    scxrKEY_F4 = 0x0403,
    scxrKEY_F5 = 0x0404,
    scxrKEY_F6 = 0x0405,
    scxrKEY_F7 = 0x0406,
    scxrKEY_F8 = 0x0407,
    scxrKEY_F9 = 0x0408,
    scxrKEY_F10 = 0x0409,
    scxrKEY_F11 = 0x040a,
    scxrKEY_F12 = 0x040b,
    scxrKEY_F13 = 0x040c,
    scxrKEY_F14 = 0x040d,
    scxrKEY_F15 = 0x040e,
    scxrKEY_F16 = 0x040f,
    scxrKEY_F17 = 0x0410,
    scxrKEY_F18 = 0x0411,
    scxrKEY_F19 = 0x0412,
    scxrKEY_F20 = 0x0413,
    scxrKEY_F21 = 0x0414,
    scxrKEY_F22 = 0x0415,
    scxrKEY_F23 = 0x0416,
    scxrKEY_F24 = 0x0417,

    // Lock keys
    scxrKEY_CAPS_LOCK = 0x0501,
    scxrKEY_NUM_LOCK = 0x0502,
    scxrKEY_SCROLL_LOCK = 0x0503,

    // Numpad keys
    scxrKEY_KP_0 = 0x0600,
    scxrKEY_KP_1 = 0x0601,
    scxrKEY_KP_2 = 0x0602,
    scxrKEY_KP_3 = 0x0603,
    scxrKEY_KP_4 = 0x0604,
    scxrKEY_KP_5 = 0x0605,
    scxrKEY_KP_6 = 0x0606,
    scxrKEY_KP_7 = 0x0607,
    scxrKEY_KP_8 = 0x0608,
    scxrKEY_KP_9 = 0x0609,

    scxrKEY_ADD = 0x060a,
    scxrKEY_SUBTRACT = 0x060b,
    scxrKEY_DECIMAL = 0x060c,

    scxrKEY_KP_INSERT = 0x060d,
    scxrKEY_KP_END = 0x060e,
    scxrKEY_KP_DOWN = 0x060f,
    scxrKEY_KP_PAGE_DOWN = 0x0610,
    scxrKEY_KP_LEFT = 0x0611,
    scxrKEY_KP_CLEAR = 0x0612,
    scxrKEY_KP_RIGHT = 0x0613,
    scxrKEY_KP_HOME = 0x0614,
    scxrKEY_KP_UP = 0x0615,
    scxrKEY_KP_PAGE_UP = 0x0616,
    scxrKEY_KP_DELETE = 0x0617,

    scxrKEY_MAX = 0x0618,

    scxrKEY_ALL = 0xffff,
} scxrKeyboardInput;

typedef enum
{
    scxrKeyEventType_NONE = 0,
    scxrKeyEventType_UP,
    scxrKeyEventType_DOWN
} scxrKeyEventType;

typedef enum
{
    scxrMF_NONE = 0x0000,
    scxrMF_SHIFT = 0x0001,
    scxrMF_CONTROL = 0x0002,
    scxrMF_ALT = 0x0004,
    scxrMF_META = 0x0008,
    scxrMF_SHIFTRIGHT = 0x0010,
    scxrMF_CONTROLRIGHT = 0x0020,
    scxrMF_ALTRIGHT = 0x0040,
    scxrMF_METARIGHT = 0x0080,
} scxrKeyboardModifierFlags;

typedef struct scxrKeyboardEvent
{
    scxrKeyEventType type;
    scxrKeyboardInput keyboardCode;
    scxrKeyboardModifierFlags flags;
} scxrKeyboardEvent;

typedef enum
{
    scxrMouseEventType_NONE = 0,
    scxrMouseEventType_MOVE,
    scxrMouseEventType_WHEEL,
    scxrMouseEventType_BUTTONUP,
    scxrMouseEventType_BUTTONDOWN
} scxrMouseEventType;

typedef enum
{
    scxrMouseButton_NONE = 0,
    scxrMouseButton_LEFT,
    scxrMouseButton_MIDDLE,
    scxrMouseButton_RIGHT,
    scxrMouseButton_THUMB01,
    scxrMouseButton_THUMB02
} scxrMouseButton;

typedef struct scxrMouseMotion
{
    int16_t x;
    int16_t y;
} scxrMouseMotion;

typedef struct scxrMouseWheel
{
    int16_t x;
    int16_t y;
} scxrMouseWheel;

typedef struct scxrMouseEvent
{
    scxrMouseEventType type;
    scxrMouseButton button;
    union
    {
        scxrMouseMotion motion;
        scxrMouseWheel wheel;
    };
    scxrKeyboardModifierFlags keyboardModifierFlags;
} scxrMouseEvent;

typedef enum
{
    scxrInputEventType_Invalid,
    scxrInputEventType_Keyboard,
    scxrInputEventType_Mouse,
    scxrInputEventType_Touch,
    scxrInputEventType_Generic,
} scxrTouchEventType;

typedef struct scxrTouchEvent
{
    scxrTouchEventType type;
    float x;
    float y;
} scxrTouchEvent;

typedef struct scxrGenericUserInputEvent
{
    uint8_t* data;
    uint32_t sizeInBytes;
} scxrGenericUserInputEvent;

typedef union scxrInputEventUnion
{
    scxrKeyboardEvent keyboardEvent;
    scxrMouseEvent mouseEvent;
    scxrTouchEvent touchEvent;
    scxrGenericUserInputEvent genericInputEvent;
} scxrInputEventUnion;

typedef struct scxrInputEvent
{
    scxrTouchEventType type;
    scxrInputEventUnion event;
} scxrInputEvent;

typedef struct scxrInputEvent2
{
    scxrTouchEventType type;
    union{
	scxrKeyboardEvent keyboardEvent;
    scxrMouseEvent mouseEvent;
    scxrTouchEvent touchEvent;
    scxrGenericUserInputEvent genericInputEvent;
	};
} scxrInputEvent2;

#ifdef __cplusplus
} // extern "C"
#endif

#endif
