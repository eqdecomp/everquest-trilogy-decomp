// unit_100044F0.cpp - eqmain.dll, the address run 0x100044F0..0x100044F0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <windows.h>

// eqmain.dll's own addresses (image base 0x10000000) go through eqaddr.h's EQ_CDECL / EQ_GLOBAL, so the match
// build (EQD_MATCH) gets direct `call rel32` and relocated absolutes, like every other eqmain file.
#include "trilogy/eqaddr.h"

// The original's calls through an import slot (`CALL dword ptr [va]`): __stdcall.
#define EQMAIN_IMPORT(ret, va, params)  ((ret (__stdcall *) params)(*(void **)(va)))

struct Widget_guess;

typedef int (__cdecl *WidgetHook_guess)(int event, Widget_guess *widget, int x, int y);

// A node of a window's widget list (and of a list box's item list): the widget at +4, the next node at +8.
struct WidgetNode_guess
{
    int unknown00;
    void *data;
    WidgetNode_guess *next;
};

// The window the front end shows: its widget list at +0x1C.
struct Window_guess
{
    int unknown00[7];
    WidgetNode_guess *widgets;
};

// A front-end widget; type 0 button, 1/8 edit, 2 list box, 3 scroll bar, 5 check box, 6 radio button.
// The fields from +0x1C on depend on the type.
struct Widget_guess
{
    int type;
    int unknown04[2];
    RECT rect;
    union
    {
        int state;
        WidgetNode_guess *items;
    };
    int unknown20;
    int itemCount;
    unsigned int flags;
    int topIndex;
    union
    {
        int visibleRows;
        int *thumbSize;
    };
    union
    {
        struct
        {
            int selected;
            WidgetHook_guess hook;
        };
        RECT downArrowRect;
    };
    RECT upArrowRect;
    int thumbLeft;
    int unknown58[3];
    int maxPos;
    int pos;
    int thumbTop;
    char dragging;
};

#define WIDGET_BUTTON     0
#define WIDGET_EDIT       1
#define WIDGET_LISTBOX    2
#define WIDGET_SCROLLBAR  3
#define WIDGET_CHECKBOX   5
#define WIDGET_RADIO      6
#define WIDGET_EDIT_ALT   8

#define LISTBOX_SMALL_ROWS  0x1
#define LISTBOX_READONLY    0x8

#define g_mainWindow       EQ_GLOBAL(HWND, 0x100D5B2C)
#define g_cursor           EQ_GLOBAL(POINT, 0x1004CE50)
#define g_draggedSlider    EQ_GLOBAL(Widget_guess *, 0x1004CE80)
#define g_currentWindow    EQ_GLOBAL(WidgetNode_guess *, 0x1004CE94)
#define g_hovered          EQ_GLOBAL(Widget_guess *, 0x1004CD7C)
#define g_pressed          EQ_GLOBAL(Widget_guess *, 0x1004CE6C)
#define g_repeatDelay      EQ_GLOBAL(DWORD, 0x100463A8)
#define g_repeatInterval   EQ_GLOBAL(DWORD, 0x100463AC)
#define g_repeatStart      EQ_GLOBAL(DWORD, 0x1004CE4C)
#define g_repeatNext       EQ_GLOBAL(DWORD, 0x1004CE48)

#define SetForegroundWindow_ EQMAIN_IMPORT(BOOL, 0x100401E0, (HWND))
#define BeginPaint_          EQMAIN_IMPORT(HDC, 0x100401E8, (HWND, PAINTSTRUCT *))
#define EndPaint_            EQMAIN_IMPORT(BOOL, 0x100401E4, (HWND, PAINTSTRUCT *))
#define ClientToScreen_      EQMAIN_IMPORT(BOOL, 0x100401D4, (HWND, POINT *))
#define PtInRect_            EQMAIN_IMPORT(BOOL, 0x100401DC, (const RECT *, POINT))
#define DefWindowProcA_      EQMAIN_IMPORT(LRESULT, 0x100401D8, (HWND, UINT, WPARAM, LPARAM))
#define timeGetTime_         EQMAIN_IMPORT(DWORD, 0x10040244, (void))

#define WidgetEvent_      EQ_CDECL(void, 0x100044B0, (int, Widget_guess *, int, int))
#define DrawWidget_       EQ_CDECL(void, 0x10004EC0, (Widget_guess *))
#define UpdateScrollbar_  EQ_CDECL(void, 0x100067E0, (Widget_guess *))
#define FocusEdit_        EQ_CDECL(void, 0x10008910, (Widget_guess *))

// FUNCTION: EQMAIN 0x100044f0
// The front end's window procedure: keeps the game window in front and drives the widgets of the current window.
LRESULT __stdcall FrontEndWndProc_guess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    WidgetNode_guess *node;
    Widget_guess *widget;
    Widget_guess *pressed;
    WidgetNode_guess *item;
    int row;
    int count;
    RECT thumbRect;
    PAINTSTRUCT paint;

    switch (msg)
    {
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
        {
            SetForegroundWindow_(g_mainWindow);
            return 0;
        }
        break;
    case WM_PAINT:
        BeginPaint_(hwnd, &paint);
        EndPaint_(hwnd, &paint);
        return 0;
    case WM_ACTIVATEAPP:
    case WM_NCACTIVATE:
        if (wParam == 0)
        {
            SetForegroundWindow_(g_mainWindow);
            return 0;
        }
        break;
    case WM_KILLFOCUS:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_SYSCHAR:
    case WM_SYSCOMMAND:
        SetForegroundWindow_(g_mainWindow);
        return 0;
    case WM_KEYDOWN:
    case WM_KEYUP:
        if (wParam >= VK_LWIN && wParam <= VK_APPS)
        {
            SetForegroundWindow_(g_mainWindow);
            return 0;
        }
        break;
    case WM_MOUSEMOVE:
        g_cursor.y = HIWORD(lParam);
        g_cursor.x = LOWORD(lParam);
        ClientToScreen_(hwnd, &g_cursor);
        if (g_draggedSlider != 0 && g_draggedSlider->dragging != 0)
        {
            g_draggedSlider->thumbTop = g_cursor.y - g_draggedSlider->thumbSize[1] / 2;
            UpdateScrollbar_(g_draggedSlider);
            WidgetEvent_(1, g_draggedSlider, g_cursor.x, g_cursor.y);
        }
        for (node = ((Window_guess *)g_currentWindow->data)->widgets; node != 0; node = node->next)
        {
            widget = (Widget_guess *)node->data;
            if (widget != g_hovered && PtInRect_(&widget->rect, g_cursor) && widget->type == WIDGET_BUTTON &&
                g_pressed == 0 && widget->state == 0)
            {
                if (g_hovered != 0)
                {
                    g_hovered->state = 0;
                    DrawWidget_(g_hovered);
                }
                widget->state = 2;
                DrawWidget_(widget);
                g_hovered = widget;
            }
        }
        if (g_hovered != 0 && !PtInRect_(&g_hovered->rect, g_cursor))
        {
            g_hovered->state = 0;
            DrawWidget_(g_hovered);
            g_hovered = 0;
            return 0;
        }
        return 0;
    default:
        break;
    case WM_RBUTTONDOWN:
        g_cursor.x = LOWORD(lParam);
        g_cursor.y = HIWORD(lParam);
        ClientToScreen_(hwnd, &g_cursor);
        for (node = ((Window_guess *)g_currentWindow->data)->widgets; node != 0; node = node->next)
        {
            widget = (Widget_guess *)node->data;
            if (PtInRect_(&widget->rect, g_cursor) && widget->type == WIDGET_LISTBOX &&
                (row = (g_cursor.y - widget->rect.top) / 13) < widget->visibleRows &&
                widget->topIndex + row < widget->itemCount &&
                (widget->hook == 0 || widget->hook(3, widget, g_cursor.x, g_cursor.y) != 0))
            {
                WidgetEvent_(3, widget, g_cursor.x, g_cursor.y);
            }
        }
        return 0;
    case WM_LBUTTONUP:
        g_cursor.x = LOWORD(lParam);
        g_cursor.y = HIWORD(lParam);
        ClientToScreen_(hwnd, &g_cursor);
        if (g_draggedSlider != 0)
        {
            g_draggedSlider->dragging = 0;
        }
        pressed = g_pressed;
        g_draggedSlider = 0;
        if (pressed != 0)
        {
            switch (pressed->type)
            {
            case WIDGET_BUTTON:
                if (PtInRect_(&pressed->rect, g_cursor))
                {
                    if (g_hovered != 0 && g_hovered != pressed)
                    {
                        g_hovered->state = 0;
                        DrawWidget_(g_hovered);
                    }
                    pressed->state = 2;
                    g_hovered = pressed;
                    WidgetEvent_(0, pressed, g_cursor.x, g_cursor.y);
                }
                else
                {
                    pressed->state = 0;
                }
                DrawWidget_(g_pressed);
                g_pressed = 0;
                return 0;
            case WIDGET_EDIT:
            case WIDGET_EDIT_ALT:
                WidgetEvent_(1, pressed, g_cursor.x, g_cursor.y);
                return 0;
            }
        }
        return 0;
    case WM_LBUTTONDOWN:
        g_cursor.x = LOWORD(lParam);
        g_cursor.y = HIWORD(lParam);
        ClientToScreen_(hwnd, &g_cursor);
        for (node = ((Window_guess *)g_currentWindow->data)->widgets; node != 0; node = node->next)
        {
            widget = (Widget_guess *)node->data;
            if (PtInRect_(&widget->rect, g_cursor))
            {
                switch (widget->type)
                {
                case WIDGET_BUTTON:
                    widget->state = 1;
                    DrawWidget_(widget);
                    if (g_hovered == widget)
                    {
                        g_hovered = 0;
                    }
                    g_pressed = widget;
                    WidgetEvent_(1, widget, g_cursor.x, g_cursor.y);
                    return 0;
                case WIDGET_CHECKBOX:
                    widget->state = (widget->state == 0);
                    DrawWidget_(widget);
                    WidgetEvent_(1, widget, g_cursor.x, g_cursor.y);
                    return 0;
                case WIDGET_RADIO:
                    if (widget->state == 0)
                    {
                        widget->state = 1;
                    }
                    DrawWidget_(widget);
                    WidgetEvent_(1, widget, g_cursor.x, g_cursor.y);
                    return 0;
                case WIDGET_EDIT:
                case WIDGET_EDIT_ALT:
                    FocusEdit_(widget);
                    WidgetEvent_(1, widget, g_cursor.x, g_cursor.y);
                    return 0;
                case WIDGET_LISTBOX:
                    row = (g_cursor.y - widget->rect.top) / ((widget->flags & LISTBOX_SMALL_ROWS) ? 11 : 13);
                    if (widget->hook != 0 && widget->hook(1, widget, g_cursor.x, g_cursor.y) == 0)
                    {
                        return 0;
                    }
                    if (widget->flags & LISTBOX_READONLY)
                    {
                        return 0;
                    }
                    count = 0;
                    for (item = widget->items; item != 0; item = item->next)
                    {
                        count++;
                    }
                    row = widget->topIndex + row;
                    if (row < count)
                    {
                        widget->selected = row;
                    }
                    DrawWidget_(widget);
                    WidgetEvent_(1, widget, g_cursor.x, g_cursor.y);
                    return 0;
                case WIDGET_SCROLLBAR:
                    g_draggedSlider = widget;
                    if (PtInRect_(&widget->upArrowRect, g_cursor) && widget->pos < widget->maxPos - 1)
                    {
                        widget->pos++;
                        DrawWidget_(widget);
                    }
                    else if (PtInRect_(&widget->downArrowRect, g_cursor) && widget->pos > 0)
                    {
                        widget->pos--;
                        DrawWidget_(widget);
                    }
                    else
                    {
                        if (widget->dragging != 0)
                        {
                            return 0;
                        }
                        thumbRect.top = widget->thumbTop;
                        thumbRect.bottom = widget->thumbSize[1] + thumbRect.top;
                        thumbRect.left = widget->thumbLeft;
                        thumbRect.right = widget->thumbSize[0] + thumbRect.left;
                        if (PtInRect_(&thumbRect, g_cursor))
                        {
                            widget->dragging = 1;
                            return 0;
                        }
                        widget->dragging = 1;
                        widget->thumbTop = g_cursor.y - widget->thumbSize[1] / 2;
                        DrawWidget_(widget);
                        widget->dragging = 0;
                        WidgetEvent_(1, widget, g_cursor.x, g_cursor.y);
                        return 0;
                    }
                    WidgetEvent_(1, widget, g_cursor.x, g_cursor.y);
                    g_repeatStart = timeGetTime_() + g_repeatDelay;
                    g_repeatNext = timeGetTime_() + g_repeatInterval;
                    return 0;
                }
            }
        }
        return 0;
    case WM_DESTROY:
    case WM_CLOSE:
    case WM_CONTEXTMENU:
        return 0;
    }
    return DefWindowProcA_(hwnd, msg, wParam, lParam);
}
