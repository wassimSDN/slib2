#pragma once

#include <array>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "slibpos.h"
#include "slibmisc.h"

namespace slib
{
	struct State
	{
		bool previous = false;
		bool current = false;

        bool nowFalse() const;
		bool nowJustFalse() const;
		bool nowTrue() const;
		bool nowJustTrue() const;
	};

	struct Window
	{
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
        TTF_TextEngine* textEngine = nullptr;
		std::array<State, SDL_SCANCODE_COUNT> keyStates;
        std::array<State, 5> buttonStates;
        std::pair<Vector2, bool> mouse = { {0, 0}, false };
        State wheelu;
        State wheeld;
        State wheelr;
        State wheell;
        bool closeRequested = false;
        State hasFocus;
        State hasMouse;
		uint32_t ID = 0;

        bool createWindow(const char* title, int width, int height, int flags);
        void clear();
        void present();
        Color getRenderColor();
        void setRenderColor(Color color);
        Dimensions size();
        void destroyWindow();
	};
    
    struct Mixer
    {
        MIX_Mixer* mixer = nullptr;

        bool init();
        void destroyMixer();
    };

    struct TimeManager
    {
        long double counterFrequency = 0;
        long double deltaTime = 0;
        long double lastTime = 0;
        long double timeStep = 0;
        long double accumulatedTime = 0;

        void update();
        void setTicks(int ticks);
        bool ticking();
    };

    enum class Keys
    {
        unknown = 0,
        a = 4,
        b = 5,
        c = 6,
        d = 7,
        e = 8,
        f = 9,
        g = 10,
        h = 11,
        i = 12,
        j = 13,
        k = 14,
        l = 15,
        m = 16,
        n = 17,
        o = 18,
        p = 19,
        q = 20,
        r = 21,
        s = 22,
        t = 23,
        u = 24,
        v = 25,
        w = 26,
        x = 27,
        y = 28,
        z = 29,
        key_1 = 30,
        key_2 = 31,
        key_3 = 32,
        key_4 = 33,
        key_5 = 34,
        key_6 = 35,
        key_7 = 36,
        key_8 = 37,
        key_9 = 38,
        key_0 = 39,
        key_return = 40,
        escape = 41,
        backspace = 42,
        tab = 43,
        space = 44,
        minus = 45,
        equals = 46,
        leftbracket = 47,
        rightbracket = 48,
        backslash = 49,
        nonushash = 50,
        semicolon = 51,
        apostrophe = 52,
        grave = 53,
        comma = 54,
        period = 55,
        slash = 56,
        capslock = 57,
        f1 = 58,
        f2 = 59,
        f3 = 60,
        f4 = 61,
        f5 = 62,
        f6 = 63,
        f7 = 64,
        f8 = 65,
        f9 = 66,
        f10 = 67,
        f11 = 68,
        f12 = 69,
        printscreen = 70,
        scrolllock = 71,
        pause = 72,
        insert = 73,
        home = 74,
        pageup = 75,
        key_delete = 76,
        end = 77,
        pagedown = 78,
        right = 79,
        left = 80,
        down = 81,
        up = 82,
        numlockclear = 83,
        kp_divide = 84,
        kp_multiply = 85,
        kp_minus = 86,
        kp_plus = 87,
        kp_enter = 88,
        kp_1 = 89,
        kp_2 = 90,
        kp_3 = 91,
        kp_4 = 92,
        kp_5 = 93,
        kp_6 = 94,
        kp_7 = 95,
        kp_8 = 96,
        kp_9 = 97,
        kp_0 = 98,
        kp_period = 99,
        nonusbackslash = 100,
        application = 101,
        power = 102,
        kp_equals = 103,
        f13 = 104,
        f14 = 105,
        f15 = 106,
        f16 = 107,
        f17 = 108,
        f18 = 109,
        f19 = 110,
        f20 = 111,
        f21 = 112,
        f22 = 113,
        f23 = 114,
        f24 = 115,
        execute = 116,
        help = 117,
        menu = 118,
        select = 119,
        stop = 120,
        again = 121,
        undo = 122,
        cut = 123,
        copy = 124,
        paste = 125,
        find = 126,
        mute = 127,
        volumeup = 128,
        volumedown = 129,
        lockingcapslock = 130,
        lockingnumlock = 131,
        lockingscrolllock = 132,
        kp_comma = 133,
        kp_equalsas400 = 134,
        international1 = 135,
        international2 = 136,
        international3 = 137,
        international4 = 138,
        international5 = 139,
        international6 = 140,
        international7 = 141,
        international8 = 142,
        international9 = 143,
        lang1 = 144,
        lang2 = 145,
        lang3 = 146,
        lang4 = 147,
        lang5 = 148,
        lang6 = 149,
        lang7 = 150,
        lang8 = 151,
        lang9 = 152,
        alterase = 153,
        sysreq = 154,
        cancel = 155,
        clear = 156,
        prior = 157,
        return2 = 158,
        separator = 159,
        out = 160,
        oper = 161,
        clearagain = 162,
        crsel = 163,
        exsel = 164,
        kp_00 = 176,
        kp_000 = 177,
        thousandsseparator = 178,
        decimalseparator = 179,
        currencyunit = 180,
        currencysubunit = 181,
        kp_leftparen = 182,
        kp_rightparen = 183,
        kp_leftbrace = 184,
        kp_rightbrace = 185,
        kp_tab = 186,
        kp_backspace = 187,
        kp_a = 188,
        kp_b = 189,
        kp_c = 190,
        kp_d = 191,
        kp_e = 192,
        kp_f = 193,
        kp_xor = 194,
        kp_power = 195,
        kp_percent = 196,
        kp_less = 197,
        kp_greater = 198,
        kp_ampersand = 199,
        kp_dblampersand = 200,
        kp_verticalbar = 201,
        kp_dblverticalbar = 202,
        kp_colon = 203,
        kp_hash = 204,
        kp_space = 205,
        kp_at = 206,
        kp_exclam = 207,
        kp_memstore = 208,
        kp_memrecall = 209,
        kp_memclear = 210,
        kp_memadd = 211,
        kp_memsubtract = 212,
        kp_memmultiply = 213,
        kp_memdivide = 214,
        kp_plusminus = 215,
        kp_clear = 216,
        kp_clearentry = 217,
        kp_binary = 218,
        kp_octal = 219,
        kp_decimal = 220,
        kp_hexadecimal = 221,
        lctrl = 224,
        lshift = 225,
        lalt = 226,
        lgui = 227,
        rctrl = 228,
        rshift = 229,
        ralt = 230,
        rgui = 231,
        mode = 257,
        sleep = 258,
        wake = 259,
        channel_increment = 260,
        channel_decrement = 261,
        media_play = 262,
        media_pause = 263,
        media_record = 264,
        media_fast_forward = 265,
        media_rewind = 266,
        media_next_track = 267,
        media_previous_track = 268,
        media_stop = 269,
        media_eject = 270,
        media_play_pause = 271,
        media_select = 272,
        ac_new = 273,
        ac_open = 274,
        ac_close = 275,
        ac_exit = 276,
        ac_save = 277,
        ac_print = 278,
        ac_properties = 279,
        ac_search = 280,
        ac_home = 281,
        ac_back = 282,
        ac_forward = 283,
        ac_stop = 284,
        ac_refresh = 285,
        ac_bookmarks = 286,
        softleft = 287,
        softright = 288,
        call = 289,
        endcall = 290,
        reserved = 400,
        count = 512

    };
    enum class Buttons
    {
        left = SDL_BUTTON_LEFT,    
        middle = SDL_BUTTON_MIDDLE,  
        right = SDL_BUTTON_RIGHT,
        back = SDL_BUTTON_X1,      
        forward = SDL_BUTTON_X2,
    };
     
	class App
	{
	public:
		App() = default;
		App(const char* title, int width, int height, int flags);
		~App();

		void init(const char* title, int width, int height, int flags);
        void updateEvents();
        bool running() const;
        void clear();
        void present();

        Window getWindow() const;
        

        bool addWindow(const char* title, int width, int height, int flags);
        void popWindow();

        Color getRenderColor() const;
        void setRenderColor(Color color = {0, 0, 0, 0});

        Color getRenderColor(int windowIndex) const;
        void setRenderColor(int windowIndex, Color color = { 0, 0, 0, 0 });

        float mousex();
        float mousey();

        float mousex(int windowIndex);
        float mousey(int windowIndex);

        bool hasFocus() const;
        bool justHasFocus() const;

        bool hasFocus(int windowIndex) const;
        bool justHasFocus(int windowIndex) const;

        bool closeRequested() const;
        bool closeRequested(int windowIndex) const;
        bool oneCloseRequested() const;

		bool keyDown(Keys key) const;
        bool keyJustDown(Keys key) const;
        bool keyUp(Keys key) const;
        bool keyJustUp(Keys key) const;

        bool keyDown(Keys key, int windowIndex) const;
        bool keyJustDown(Keys key, int windowIndex) const;
        bool keyUp(Keys key, int windowIndex) const;
        bool keyJustUp(Keys key, int windowIndex) const;

        bool mouseDown(Buttons button) const;
        bool mouseJustDown(Buttons button) const;
        bool mouseUp(Buttons button) const;
        bool mouseJustUp(Buttons button) const;
        
        bool mouseDown(Buttons button, int windowIndex) const;
        bool mouseJustDown(Buttons button, int windowIndex) const;
        bool mouseUp(Buttons button, int windowIndex) const;
        bool mouseJustUp(Buttons button, int windowIndex) const;

        bool scrollu() const;
        bool scrolld() const;
        bool scrolll() const;
        bool scrollr() const;

        bool scrollJustu() const;
        bool scrollJustd() const;
        bool scrollJustl() const;
        bool scrollJustr() const;

        bool scrollu(int windowIndex) const;
        bool scrolld(int windowIndex) const;
        bool scrolll(int windowIndex) const;
        bool scrollr(int windowIndex) const;

        bool scrollJustu(int windowIndex) const;
        bool scrollJustd(int windowIndex) const;
        bool scrollJustl(int windowIndex) const;
        bool scrollJustr(int windowIndex) const;

        void setTicks(int ticks);
        bool ticking();
        long double getdt();


	public:
		static Window mainWindow;
		static std::vector<Window> secondaryWindows;
        static TimeManager time;
        static Mixer mixer;

        friend class Rect;
        friend class Texture;
        friend class Text;
        friend class Sound;
	};
}
