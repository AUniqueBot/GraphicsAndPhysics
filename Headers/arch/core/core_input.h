#pragma once
#include <pch.h>



class InputSystem {
public:    
    enum INPUT_KEY {

        // - keyboard keys ------------

        KEYBOARD_A = 0,
        KEYBOARD_B,
        KEYBOARD_C,
        KEYBOARD_D,
        KEYBOARD_E,
        KEYBOARD_F,
        KEYBOARD_G,
        KEYBOARD_H,
        KEYBOARD_I,
        KEYBOARD_J,
        KEYBOARD_K,
        KEYBOARD_L,
        KEYBOARD_M,
        KEYBOARD_N,
        KEYBOARD_O,
        KEYBOARD_P,
        KEYBOARD_Q,
        KEYBOARD_R,
        KEYBOARD_S,
        KEYBOARD_T,
        KEYBOARD_U,
        KEYBOARD_V,
        KEYBOARD_W,
        KEYBOARD_X,
        KEYBOARD_Y,
        KEYBOARD_Z,

        KEYBOARD_1,
        KEYBOARD_2,
        KEYBOARD_3,
        KEYBOARD_4,
        KEYBOARD_5,
        KEYBOARD_6,
        KEYBOARD_7,
        KEYBOARD_8,
        KEYBOARD_9,
        KEYBOARD_0,

        KEYBOARD_ENTER,
        KEYBOARD_BACKSPACE,
        KEYBOARD_SPACE,
        KEYBOARD_COMMA,
        KEYBOARD_PERIOD,
        KEYBOARD_FORWARDSLASH,
        KEYBOARD_SEMICOLON,
        KEYBOARD_APOSTROPHE,
        KEYBOARD_LSQUAREBRACKET,
        KEYBOARD_RSQUAREBRACKET,
        KEYBOARD_BACKSLASH,
        KEYBOARD_MINUS,
        KEYBOARD_EQUALS,
        KEYBOARD_TICK,

        KEYBOARD_TAB,
        KEYBOARD_CAPSLOCK,
        KEYBOARD_LSHIFT,
        KEYBOARD_RSHIFT,
        KEYBOARD_LCONTROL,
        KEYBOARD_RCONTROL,
        KEYBOARD_LALT,
        KEYBOARD_RALT,



        // - function keys ------------

        KEYBOARD_F1,
        KEYBOARD_F2,
        KEYBOARD_F3,
        KEYBOARD_F4,
        KEYBOARD_F5,
        KEYBOARD_F6,
        KEYBOARD_F7,
        KEYBOARD_F8,
        KEYBOARD_F9,
        KEYBOARD_F10,
        KEYBOARD_F11,
        KEYBOARD_F12,

        KEYBOARD_INSERT,
        KEYBOARD_HOME,
        KEYBOARD_DELETE,
        KEYBOARD_END,
        KEYBOARD_PAGEUP,
        KEYBOARD_PAGEDOWN,

        KEYBOARD_UP,
        KEYBOARD_DOWN,
        KEYBOARD_LEFT,
        KEYBOARD_RIGHT,



        // - keypad -------------------

        KEYPAD_NUMLOCK,
        KEYPAD_FORWARDSLASH,
        KEYPAD_MULTIPLY,
        KEYPAD_MINUS,
        KEYPAD_PLUS,
        KEYPAD_ENTER,
        KEYPAD_PERIOD,
        KEYPAD_1,
        KEYPAD_2,
        KEYPAD_3,
        KEYPAD_4,
        KEYPAD_5,
        KEYPAD_6,
        KEYPAD_7,
        KEYPAD_8,
        KEYPAD_9,
        KEYPAD_0,


        // - ignore this ---------------
        _KEY_COUNT,
        _KEY_UNKNOWN = -1
    };

    enum INPUT_MOUSE_BUTTON {
        MOUSE_1 = 0,
        MOUSE_2,
        MOUSE_3,
        MOUSE_4,
        MOUSE_5,
        MOUSE_6,
        MOUSE_7,
        MOUSE_8,

        MOUSE_LEFT = MOUSE_1,
        MOUSE_RIGHT = MOUSE_2,
        MOUSE_MIDDLE = MOUSE_3,


        _MOUSE_COUNT = MOUSE_8 + 1,
        _MOUSE_UNKNOWN = -1
    };


public:
    void Init(GLFWwindow* _window);
    void Update();
    void PostUpdate();

    // - query --------------------------
    
    bool IsKeyPressed(INPUT_KEY) const;
    bool IsKeyHeld(INPUT_KEY) const;
    bool IsKeyReleased(INPUT_KEY) const;

    bool IsMouseButtonClicked(INPUT_MOUSE_BUTTON _button) const;
    bool IsMouseButtonHeld(INPUT_MOUSE_BUTTON _button) const;
    bool IsMouseButtonReleased(INPUT_MOUSE_BUTTON _button) const;

    glm::vec2 GetMousePosition() const;
    glm::vec2 GetMouseDelta() const;


public:


private:
    void SetupCallbacks(GLFWwindow* _window);
    // - callback functions -------------
    void _onMouseMove(double _xpos, double _ypos);
    void _onMouseButton(int _button, int _action, int _mods);
    void _onScroll(double _xoffset, double _yoffset);
    void _onKey(int _key, int _scancode, int _action, int _mods);

private:

    double m_mouseX                                         { 0 };
    double m_mouseY                                         { 0 };
    double m_prevMouseX                                     { 0 };
    double m_prevMouseY                                     { 0 };


    double m_scrollOffsetX                                  { 0 };
    double m_scrollOffsetY                                  { 0 };


    std::bitset<_MOUSE_COUNT> m_activatedMouseButtons       { false };
    std::bitset<_MOUSE_COUNT> m_activatedMouseButtonsPrev   { false }; //  history

    std::bitset<_KEY_COUNT> m_activatedKeyboardButtons      { false };
    std::bitset<_KEY_COUNT> m_activatedKeyboardButtonsPrev  { false }; // history


private:
    // - mappings -------------------------------

    const std::unordered_map<INPUT_KEY, int> KEYBOARD_INPUT_MAP{
        { KEYBOARD_A,                                                       GLFW_KEY_A },
        { KEYBOARD_B,                                                       GLFW_KEY_B },
        { KEYBOARD_C,                                                       GLFW_KEY_C },
        { KEYBOARD_D,                                                       GLFW_KEY_D },
        { KEYBOARD_E,                                                       GLFW_KEY_E },
        { KEYBOARD_F,                                                       GLFW_KEY_F },
        { KEYBOARD_G,                                                       GLFW_KEY_G },
        { KEYBOARD_H,                                                       GLFW_KEY_H },
        { KEYBOARD_I,                                                       GLFW_KEY_I },
        { KEYBOARD_J,                                                       GLFW_KEY_J },
        { KEYBOARD_K,                                                       GLFW_KEY_K },
        { KEYBOARD_L,                                                       GLFW_KEY_L },
        { KEYBOARD_M,                                                       GLFW_KEY_M },
        { KEYBOARD_N,                                                       GLFW_KEY_N },
        { KEYBOARD_O,                                                       GLFW_KEY_O },
        { KEYBOARD_P,                                                       GLFW_KEY_P },
        { KEYBOARD_Q,                                                       GLFW_KEY_Q },
        { KEYBOARD_R,                                                       GLFW_KEY_R },
        { KEYBOARD_S,                                                       GLFW_KEY_S },
        { KEYBOARD_T,                                                       GLFW_KEY_T },
        { KEYBOARD_U,                                                       GLFW_KEY_U },
        { KEYBOARD_V,                                                       GLFW_KEY_V },
        { KEYBOARD_W,                                                       GLFW_KEY_W },
        { KEYBOARD_X,                                                       GLFW_KEY_X },
        { KEYBOARD_Y,                                                       GLFW_KEY_Y },
        { KEYBOARD_Z,                                                       GLFW_KEY_Z },
        { KEYBOARD_1,                                                       GLFW_KEY_1 },
        { KEYBOARD_2,                                                       GLFW_KEY_2 },
        { KEYBOARD_3,                                                       GLFW_KEY_3 },
        { KEYBOARD_4,                                                       GLFW_KEY_4 },
        { KEYBOARD_5,                                                       GLFW_KEY_5 },
        { KEYBOARD_6,                                                       GLFW_KEY_6 },
        { KEYBOARD_7,                                                       GLFW_KEY_7 },
        { KEYBOARD_8,                                                       GLFW_KEY_8 },
        { KEYBOARD_9,                                                       GLFW_KEY_9 },
        { KEYBOARD_0,                                                       GLFW_KEY_0 },
        { KEYBOARD_ENTER,                                                   GLFW_KEY_ENTER },
        { KEYBOARD_BACKSPACE,                                               GLFW_KEY_BACKSPACE },
        { KEYBOARD_SPACE,                                                   GLFW_KEY_SPACE },
        { KEYBOARD_COMMA,                                                   GLFW_KEY_COMMA },
        { KEYBOARD_PERIOD,                                                  GLFW_KEY_PERIOD },
        { KEYBOARD_FORWARDSLASH,                                            GLFW_KEY_SEMICOLON },
        { KEYBOARD_SEMICOLON,                                               GLFW_KEY_A },
        { KEYBOARD_APOSTROPHE,                                              GLFW_KEY_A },
        { KEYBOARD_LSQUAREBRACKET,                                          GLFW_KEY_A },
        { KEYBOARD_RSQUAREBRACKET,                                          GLFW_KEY_A },
        { KEYBOARD_BACKSLASH,                                               GLFW_KEY_A },
        { KEYBOARD_MINUS,                                                   GLFW_KEY_A },
        { KEYBOARD_EQUALS,                                                  GLFW_KEY_EQUAL },
        { KEYBOARD_TICK,                                                    GLFW_KEY_GRAVE_ACCENT },
        { KEYBOARD_TAB,                                                     GLFW_KEY_TAB },
        { KEYBOARD_CAPSLOCK,                                                GLFW_KEY_CAPS_LOCK },
        { KEYBOARD_LSHIFT,                                                  GLFW_KEY_LEFT_SHIFT },
        { KEYBOARD_RSHIFT,                                                  GLFW_KEY_RIGHT_SHIFT },
        { KEYBOARD_LCONTROL,                                                GLFW_KEY_LEFT_CONTROL },
        { KEYBOARD_RCONTROL,                                                GLFW_KEY_RIGHT_CONTROL },
        { KEYBOARD_LALT,                                                    GLFW_KEY_LEFT_ALT },
        { KEYBOARD_RALT,                                                    GLFW_KEY_RIGHT_ALT },
        { KEYBOARD_F1,                                                      GLFW_KEY_F1 },
        { KEYBOARD_F2,                                                      GLFW_KEY_F2 },
        { KEYBOARD_F3,                                                      GLFW_KEY_F3 },
        { KEYBOARD_F4,                                                      GLFW_KEY_F4 },
        { KEYBOARD_F5,                                                      GLFW_KEY_F5 },
        { KEYBOARD_F6,                                                      GLFW_KEY_F6 },
        { KEYBOARD_F7,                                                      GLFW_KEY_F7 },
        { KEYBOARD_F8,                                                      GLFW_KEY_F8 },
        { KEYBOARD_F9,                                                      GLFW_KEY_F9 },
        { KEYBOARD_F10,                                                     GLFW_KEY_F10 },
        { KEYBOARD_F11,                                                     GLFW_KEY_F11 },
        { KEYBOARD_F12,                                                     GLFW_KEY_F12 },
        { KEYBOARD_INSERT,                                                  GLFW_KEY_INSERT },
        { KEYBOARD_HOME,                                                    GLFW_KEY_HOME },
        { KEYBOARD_DELETE,                                                  GLFW_KEY_DELETE },
        { KEYBOARD_END,                                                     GLFW_KEY_END },
        { KEYBOARD_PAGEUP,                                                  GLFW_KEY_PAGE_UP },
        { KEYBOARD_PAGEDOWN,                                                GLFW_KEY_PAGE_DOWN },
        { KEYBOARD_UP,                                                      GLFW_KEY_UP },
        { KEYBOARD_DOWN,                                                    GLFW_KEY_DOWN },
        { KEYBOARD_LEFT,                                                    GLFW_KEY_LEFT },
        { KEYBOARD_RIGHT,                                                   GLFW_KEY_RIGHT },
        { KEYPAD_NUMLOCK,                                                   GLFW_KEY_NUM_LOCK },
        { KEYPAD_FORWARDSLASH,                                              GLFW_KEY_SLASH },
        { KEYPAD_MULTIPLY,                                                  GLFW_KEY_KP_MULTIPLY },
        { KEYPAD_MINUS,                                                     GLFW_KEY_KP_SUBTRACT },
        { KEYPAD_PLUS,                                                      GLFW_KEY_KP_ADD },
        { KEYPAD_ENTER,                                                     GLFW_KEY_KP_ENTER },
        { KEYPAD_PERIOD,                                                    GLFW_KEY_KP_DECIMAL },
        { KEYPAD_1,                                                         GLFW_KEY_KP_1 },
        { KEYPAD_2,                                                         GLFW_KEY_KP_2 },
        { KEYPAD_3,                                                         GLFW_KEY_KP_3 },
        { KEYPAD_4,                                                         GLFW_KEY_KP_4 },
        { KEYPAD_5,                                                         GLFW_KEY_KP_5 },
        { KEYPAD_6,                                                         GLFW_KEY_KP_6 },
        { KEYPAD_7,                                                         GLFW_KEY_KP_7 },
        { KEYPAD_8,                                                         GLFW_KEY_KP_8 },
        { KEYPAD_9,                                                         GLFW_KEY_KP_9 },
        { KEYPAD_0,                                                         GLFW_KEY_KP_0 },
        { _KEY_COUNT,                                                       GLFW_KEY_LAST + 1 },
        { _KEY_UNKNOWN,                                                     GLFW_KEY_UNKNOWN  }
    };

    const std::unordered_map<INPUT_MOUSE_BUTTON, int> MOUSE_INPUT_MAP{
        { MOUSE_1,                                                          GLFW_MOUSE_BUTTON_LEFT   },
        { MOUSE_2,                                                          GLFW_MOUSE_BUTTON_RIGHT  },
        { MOUSE_3,                                                          GLFW_MOUSE_BUTTON_MIDDLE },
        { MOUSE_4,                                                          GLFW_MOUSE_BUTTON_4      },
        { MOUSE_5,                                                          GLFW_MOUSE_BUTTON_5      },
        { MOUSE_6,                                                          GLFW_MOUSE_BUTTON_6      },
        { MOUSE_7,                                                          GLFW_MOUSE_BUTTON_7      },
        { MOUSE_8,                                                          GLFW_MOUSE_BUTTON_8      },
        { MOUSE_LEFT,                                                       GLFW_MOUSE_BUTTON_LEFT   },
        { MOUSE_RIGHT,                                                      GLFW_MOUSE_BUTTON_RIGHT  },
        { MOUSE_MIDDLE,                                                     GLFW_MOUSE_BUTTON_MIDDLE },
        { _MOUSE_COUNT,                                                     GLFW_MOUSE_BUTTON_LAST   }
    };

    const std::unordered_map<int, INPUT_KEY> GLFW_TO_KEY_MAP{
        { GLFW_KEY_A,                                                       KEYBOARD_A },
        { GLFW_KEY_B,                                                       KEYBOARD_B },
        { GLFW_KEY_C,                                                       KEYBOARD_C },
        { GLFW_KEY_D,                                                       KEYBOARD_D },
        { GLFW_KEY_E,                                                       KEYBOARD_E },
        { GLFW_KEY_F,                                                       KEYBOARD_F },
        { GLFW_KEY_G,                                                       KEYBOARD_G },
        { GLFW_KEY_H,                                                       KEYBOARD_H },
        { GLFW_KEY_I,                                                       KEYBOARD_I },
        { GLFW_KEY_J,                                                       KEYBOARD_J },
        { GLFW_KEY_K,                                                       KEYBOARD_K },
        { GLFW_KEY_L,                                                       KEYBOARD_L },
        { GLFW_KEY_M,                                                       KEYBOARD_M },
        { GLFW_KEY_N,                                                       KEYBOARD_N },
        { GLFW_KEY_O,                                                       KEYBOARD_O },
        { GLFW_KEY_P,                                                       KEYBOARD_P },
        { GLFW_KEY_Q,                                                       KEYBOARD_Q },
        { GLFW_KEY_R,                                                       KEYBOARD_R },
        { GLFW_KEY_S,                                                       KEYBOARD_S },
        { GLFW_KEY_T,                                                       KEYBOARD_T },
        { GLFW_KEY_U,                                                       KEYBOARD_U },
        { GLFW_KEY_V,                                                       KEYBOARD_V },
        { GLFW_KEY_W,                                                       KEYBOARD_W },
        { GLFW_KEY_X,                                                       KEYBOARD_X },
        { GLFW_KEY_Y,                                                       KEYBOARD_Y },
        { GLFW_KEY_Z,                                                       KEYBOARD_Z },
        { GLFW_KEY_1,                                                       KEYBOARD_1 },
        { GLFW_KEY_2,                                                       KEYBOARD_2 },
        { GLFW_KEY_3,                                                       KEYBOARD_3 },
        { GLFW_KEY_4,                                                       KEYBOARD_4 },
        { GLFW_KEY_5,                                                       KEYBOARD_5 },
        { GLFW_KEY_6,                                                       KEYBOARD_6 },
        { GLFW_KEY_7,                                                       KEYBOARD_7 },
        { GLFW_KEY_8,                                                       KEYBOARD_8 },
        { GLFW_KEY_9,                                                       KEYBOARD_9 },
        { GLFW_KEY_0,                                                       KEYBOARD_0 },
        { GLFW_KEY_ENTER,                                                   KEYBOARD_ENTER },
        { GLFW_KEY_BACKSPACE,                                               KEYBOARD_BACKSPACE },
        { GLFW_KEY_SPACE,                                                   KEYBOARD_SPACE },
        { GLFW_KEY_COMMA,                                                   KEYBOARD_COMMA },
        { GLFW_KEY_PERIOD,                                                  KEYBOARD_PERIOD },
        { GLFW_KEY_SEMICOLON,                                               KEYBOARD_FORWARDSLASH },
        { GLFW_KEY_A,                                                       KEYBOARD_SEMICOLON },
        { GLFW_KEY_A,                                                       KEYBOARD_APOSTROPHE },
        { GLFW_KEY_A,                                                       KEYBOARD_LSQUAREBRACKET },
        { GLFW_KEY_A,                                                       KEYBOARD_RSQUAREBRACKET },
        { GLFW_KEY_A,                                                       KEYBOARD_BACKSLASH },
        { GLFW_KEY_A,                                                       KEYBOARD_MINUS },
        { GLFW_KEY_EQUAL,                                                   KEYBOARD_EQUALS },
        { GLFW_KEY_GRAVE_ACCENT,                                            KEYBOARD_TICK },
        { GLFW_KEY_TAB,                                                     KEYBOARD_TAB },
        { GLFW_KEY_CAPS_LOCK,                                               KEYBOARD_CAPSLOCK },
        { GLFW_KEY_LEFT_SHIFT,                                              KEYBOARD_LSHIFT },
        { GLFW_KEY_RIGHT_SHIFT,                                             KEYBOARD_RSHIFT },
        { GLFW_KEY_LEFT_CONTROL,                                            KEYBOARD_LCONTROL },
        { GLFW_KEY_RIGHT_CONTROL,                                           KEYBOARD_RCONTROL },
        { GLFW_KEY_LEFT_ALT,                                                KEYBOARD_LALT },
        { GLFW_KEY_RIGHT_ALT,                                               KEYBOARD_RALT },
        { GLFW_KEY_F1,                                                      KEYBOARD_F1 },
        { GLFW_KEY_F2,                                                      KEYBOARD_F2 },
        { GLFW_KEY_F3,                                                      KEYBOARD_F3 },
        { GLFW_KEY_F4,                                                      KEYBOARD_F4 },
        { GLFW_KEY_F5,                                                      KEYBOARD_F5 },
        { GLFW_KEY_F6,                                                      KEYBOARD_F6 },
        { GLFW_KEY_F7,                                                      KEYBOARD_F7 },
        { GLFW_KEY_F8,                                                      KEYBOARD_F8 },
        { GLFW_KEY_F9,                                                      KEYBOARD_F9 },
        { GLFW_KEY_F10,                                                     KEYBOARD_F10 },
        { GLFW_KEY_F11,                                                     KEYBOARD_F11 },
        { GLFW_KEY_F12,                                                     KEYBOARD_F12 },
        { GLFW_KEY_INSERT,                                                  KEYBOARD_INSERT },
        { GLFW_KEY_HOME,                                                    KEYBOARD_HOME },
        { GLFW_KEY_DELETE,                                                  KEYBOARD_DELETE },
        { GLFW_KEY_END,                                                     KEYBOARD_END },
        { GLFW_KEY_PAGE_UP,                                                 KEYBOARD_PAGEUP },
        { GLFW_KEY_PAGE_DOWN,                                               KEYBOARD_PAGEDOWN },
        { GLFW_KEY_UP,                                                      KEYBOARD_UP },
        { GLFW_KEY_DOWN,                                                    KEYBOARD_DOWN },
        { GLFW_KEY_LEFT,                                                    KEYBOARD_LEFT },
        { GLFW_KEY_RIGHT,                                                   KEYBOARD_RIGHT },
        { GLFW_KEY_NUM_LOCK,                                                KEYPAD_NUMLOCK },
        { GLFW_KEY_SLASH,                                                   KEYPAD_FORWARDSLASH },
        { GLFW_KEY_KP_MULTIPLY,                                             KEYPAD_MULTIPLY },
        { GLFW_KEY_KP_SUBTRACT,                                             KEYPAD_MINUS },
        { GLFW_KEY_KP_ADD,                                                  KEYPAD_PLUS },
        { GLFW_KEY_KP_ENTER,                                                KEYPAD_ENTER },
        { GLFW_KEY_KP_DECIMAL,                                              KEYPAD_PERIOD },
        { GLFW_KEY_KP_1,                                                    KEYPAD_1 },
        { GLFW_KEY_KP_2,                                                    KEYPAD_2 },
        { GLFW_KEY_KP_3,                                                    KEYPAD_3 },
        { GLFW_KEY_KP_4,                                                    KEYPAD_4 },
        { GLFW_KEY_KP_5,                                                    KEYPAD_5 },
        { GLFW_KEY_KP_6,                                                    KEYPAD_6 },
        { GLFW_KEY_KP_7,                                                    KEYPAD_7 },
        { GLFW_KEY_KP_8,                                                    KEYPAD_8 },
        { GLFW_KEY_KP_9,                                                    KEYPAD_9 },
        { GLFW_KEY_KP_0,                                                    KEYPAD_0 }
    };

    const std::unordered_map<int, INPUT_MOUSE_BUTTON> GLFW_TO_MOUSE_BUTTON_MAP{
        { GLFW_MOUSE_BUTTON_LEFT,                                           MOUSE_1 },
        { GLFW_MOUSE_BUTTON_RIGHT,                                          MOUSE_2 },
        { GLFW_MOUSE_BUTTON_MIDDLE,                                         MOUSE_3 },
        { GLFW_MOUSE_BUTTON_4,                                              MOUSE_4 },
        { GLFW_MOUSE_BUTTON_5,                                              MOUSE_5 },
        { GLFW_MOUSE_BUTTON_6,                                              MOUSE_6 },
        { GLFW_MOUSE_BUTTON_7,                                              MOUSE_7 },
        { GLFW_MOUSE_BUTTON_8,                                              MOUSE_8 },
        { GLFW_MOUSE_BUTTON_LEFT,                                           MOUSE_LEFT },
        { GLFW_MOUSE_BUTTON_RIGHT,                                          MOUSE_RIGHT },
        { GLFW_MOUSE_BUTTON_MIDDLE,                                         MOUSE_MIDDLE },
        { GLFW_MOUSE_BUTTON_LAST,                                           _MOUSE_COUNT }

    };


};