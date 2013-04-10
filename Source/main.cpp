/*
 *  SDL powered Tetris clone
 *            - for really bored people
 */

// define some platform-specific macros
#include "platform.h"
SDL_Window *window;

// set up logging
#include "log/log.h"


#include "texture/texture.h"
#include "texture/texture_manager.h"
TextureManager texmanager;

#include "texture/font.h"
Font g_Font;

#include "gfx/skybox.h"
Skybox skybox;

#include "mesh/panel_mesh.h"
PanelMesh panel;

#include "system/display.h"
DisplayLayout displayLayout;

#include "game/tetris.h"
Tetris tetris(texmanager, g_Font);


#include "menu/menu.h"
Menu menu(3, 2);

struct GameConfig {

    GameConfig() :
        fullscreen(true),
        enabled_keyrepeat(true),
        disabled_textures(false),
        disabled_mystery(true),
        done(false)
        {}

    bool fullscreen;
    bool enabled_keyrepeat;
    bool disabled_textures;
    bool disabled_mystery;
    bool done;
} gConf;


float g_fYrot = 0;

// timers
GLint  g_Timer           = 0;
GLint  g_Timer2          = 0;
GLint  g_Timer3          = 0;

int frame=0;
int FPS = 0;

// prevent unwanted movement on game start and in case of a new tetro
bool first_move = true;


static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s", name, v);
}

void init_control_buttons()
{
    menu.setSize(displayLayout.controlWidth, displayLayout.controlHeight);
}


void toggle_fullscreen()
{
    menu.addButton( MenuType::OPTS,
    texmanager.get((gConf.fullscreen)?"menu/window.bmp":"menu/fullscreen.bmp"),
    ButtonValue::OPTS_FULLSCREEN, 0, 0, 0, 1);

    if (gConf.fullscreen){
        displayLayout.fullscreenMode();
        SDL_SetWindowSize(window, displayLayout.width, displayLayout.height);
        SDL_SetWindowFullscreen(window, SDL_TRUE);
    } else {
        displayLayout.windowMode();
        SDL_SetWindowFullscreen(window, SDL_FALSE);
        SDL_SetWindowSize(window, displayLayout.width, displayLayout.height);
    }

    init_control_buttons();
}


ButtonValue menu_click(Uint16 mouse_x, Uint16 mouse_y)
{
    if (displayLayout.isLandscape()) {
        if (mouse_x<displayLayout.mainWidth) return ButtonValue::NONE;
        mouse_x-=displayLayout.mainWidth;
    } else {
        if (mouse_y<displayLayout.mainHeight) return ButtonValue::NONE;
        mouse_y-=displayLayout.mainWidth;
    }

    return menu.getButtonValue(mouse_x, mouse_y);
}

void menu_update(ButtonValue menu_code)
{
    switch (menu.getActive())
    {
    case MenuType::MAIN:
        switch (menu_code)
        {
        case ButtonValue::NEWGAME:
            menu.setActive(MenuType::GAME);
            first_move = true;
            break;
        case ButtonValue::OPTIONS:
            menu.setActive(MenuType::OPTS);
            break;
        case ButtonValue::EXIT:
            gConf.done = true;
            break;
        default:
            break;
        }
        break;

    case MenuType::GAME:
        if (menu_code == ButtonValue::BACK) menu.setActive(MenuType::SAVE);
        break;

    case MenuType::OPTS:
        switch (menu_code)
        {
        case ButtonValue::OPTS_TEXTURES:
            tetris.setTextureDrawing(gConf.disabled_textures);
            gConf.disabled_textures = !gConf.disabled_textures;
            break;
        case ButtonValue::OPTS_MYSTERY:
           tetris.setMysteryBoxes(gConf.disabled_mystery);
            gConf.disabled_mystery = !gConf.disabled_mystery;
            break;
        case ButtonValue::OPTS_KEYS:
            gConf.enabled_keyrepeat = !gConf.enabled_keyrepeat;
            tetris.setKeyRepeat(gConf.enabled_keyrepeat);
            break;
        case ButtonValue::OPTS_FULLSCREEN:
            gConf.fullscreen = !gConf.fullscreen;
            toggle_fullscreen();
            break;
        case ButtonValue::BACK:
            menu.setActive(MenuType::MAIN);
            break;
        default:
            break;
        }
        break;

    case MenuType::SAVE:
        switch (menu_code)
        {
        case ButtonValue::SAVE_YES:
            menu.setActive(MenuType::MAIN);
            break;
        case ButtonValue::SAVE_NO:
            tetris.reset();
            menu.setActive(MenuType::MAIN);
            break;
        case ButtonValue::BACK:
            menu.setActive(MenuType::GAME);
            break;
        default:
            break;
        }
        break;
    case MenuType::OVER:
        if (menu_code == ButtonValue::BACK) {
            menu.setActive(MenuType::MAIN);
            tetris.reset();
        }
        break;
    default:
        break;
    }

}


void draw_menu()
{
    displayLayout.selectControls();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glEnable (GL_BLEND);

    menu.draw();

    if (menu.getActive()==MenuType::OPTS) {
        menu.setActive(MenuType::MISC);
        menu.draw();
        menu.setActive(MenuType::OPTS);
    }

    glEnable(GL_DEPTH_TEST);
    glDisable (GL_BLEND);

}

void calculate()
{
    if ( menu.getActive() == MenuType::GAME )
        if (!tetris.update()) menu.setActive(MenuType::OVER); // GAME OVER
}

void render() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    glDisable (GL_BLEND);
    if (!gConf.disabled_textures) {

        if (displayLayout.isLandscape())
            displayLayout.selectFull();
        else
            displayLayout.selectMain();

        glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);

        if ((g_fYrot+=0.1f) > 360) g_fYrot-=360.0f;
        glRotatef(-g_fYrot, 0.0f, 1.0f, 0.0f );

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        skybox.draw();

    }



    if (menu.getActive()==MenuType::GAME ||
        menu.getActive()==MenuType::SAVE ||
        menu.getActive()==MenuType::OVER )
    {

        displayLayout.selectPanel();

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        glEnable (GL_BLEND);
        glColor4f(1.0f,1.0f,1.0f,0.2f);
        panel.draw();
        glDisable (GL_BLEND);


        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        g_Font.set_upper_font(true);
        g_Font.set_align(Font::FontAlign::LEFT);
        g_Font.set_plain(Font::FontPlain::XY_2D);

        g_Font.set_size(12.0f,15.6f);
        g_Font.draw_text( 22.0f, 227.0f, 0.0f, "Next",
                    0.0f, 0.0f, 90.0f );
        g_Font.set_upper_font(false);

        g_Font.set_size(10.0f,13.0f);
        g_Font.draw_text ( 10.0f, 175.0f, 0.0f, "Level:");
        g_Font.draw_text ( 10.0f, 120.0f, 0.0f, "Lines:");
        g_Font.draw_text ( 10.0f,  65.0f, 0.0f, "Score:");


        char buff[10];
        sprintf( buff, "FPS:%d", FPS );
        g_Font.set_align( Font::FontAlign::CENTER );
        g_Font.set_size(8.0f,10.4f);
        g_Font.draw_text ( 50.0f, 19.0f, 0.0f, buff);

        g_Font.set_align( Font::FontAlign::RIGHT );
        g_Font.set_size(10.0f,13.0f);
        sprintf( buff, "%d", tetris.level );
        g_Font.draw_text ( 90.0f, 155.0f, 0.0f, buff);
        sprintf( buff, "%d", tetris.lines );
        g_Font.draw_text ( 90.0f, 100.0f, 0.0f, buff);
        sprintf( buff, "%d", tetris.score );
        g_Font.draw_text ( 90.0f,  45.0f, 0.0f, buff);
        g_Font.set_align( Font::FontAlign::LEFT );



        displayLayout.selectGame();
        tetris.draw();

        displayLayout.selectNext();
        tetris.drawNext();

    }


    draw_menu();

    //if (g_bPaused) g_Font.draw_text ( g_nScrWidth-70.0f, g_nScrHeight-30.0f, 0.0f, "PAUSED");



    SDL_GL_SwapWindow(window);

#   ifdef DEBUG_MODE    // Check for errors
    for (GLint error = glGetError(); error; error = glGetError())
        LOGW("glError (0x%x) at rendering", error);

    const char* sdl_error = SDL_GetError();
    if (strlen(sdl_error) > 0)
        LOGW("%s", sdl_error);
#   endif
}


//-----------------------------------------------------------------------------
// Define the attributes of the light source
//-----------------------------------------------------------------------------
void setup_light() {

    glShadeModel (GL_SMOOTH);

    GLfloat light_position[] = { -1.0f, 5.0f, 10.0f, 1.0f };
    //GLfloat diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
    //GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    //GLfloat shininess[] = {70};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
    //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0);
    //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

    //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //glLightfv(GL_LIGHT0, GL_SPECULAR, specular );

}


/* A general OpenGL initialization function.    Sets all of the initial parameters. */
void InitGL()                  // We call this right after our OpenGL window is created.
{

    glClearColor(0.09f, 0.09f, 0.09f, 1.0f);

    // Fill the front with the texture.
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //glEnable(GL_POLYGON_SMOOTH);

    // Enables Depth Testing
    glEnable( GL_DEPTH_TEST );

    // The Type Of Depth Test To Do
    glDepthFunc( GL_LEQUAL );

    // The only blending type we use
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Colors affects textures
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Really Nice Perspective Calculations
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    //glLightfv(GL_LIGHT0, GL_SPECULAR, specular );
}


void createWindow()
{
    SDL_DisplayMode current;

    /* Initialize SDL for video output */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        LOGE("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    // Get current display mode of all displays.
    int display;
    for(display=0; display < SDL_GetNumVideoDisplays(); ++display) {

    	int should_be_zero = SDL_GetCurrentDisplayMode(display, &current);

    	if(should_be_zero != 0)
            // In case of error...
            LOGW("Could not get display mode for video display #%d: %s", display, SDL_GetError());

        else
            // On success, log the current display mode.
            LOGI("Display #%d: current display mode is %dx%dpx @%dhz",
            	display, current.w, current.h, current.refresh_rate);
    }


    displayLayout.setFullscreenSize(current.w, current.h);
#	ifdef DESKTOP_DETECTED
    displayLayout.setWindowSize(360,600);
    displayLayout.windowMode();
    gConf.fullscreen = false;
#   else
    displayLayout.setWindowSize(current.w, current.h);
    displayLayout.fullscreenMode();
    gConf.fullscreen = true;
#   endif


    /** TODO Create OpenGL screen from given display not from the last */
    if ((window = SDL_CreateWindow("Tetris SDL",
                              SDL_WINDOWPOS_UNDEFINED_MASK | display,
                              SDL_WINDOWPOS_UNDEFINED_MASK | display,
                              displayLayout.width, displayLayout.height,
#	ifdef DESKTOP_DETECTED
                              SDL_WINDOW_OPENGL ))
#   else
                              SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN ))
#   endif
        == NULL )
    {
        LOGE("Unable to create OpenGL screen: %s", SDL_GetError());
        SDL_Quit();
        exit(2);
    }

}

void build_menu()
{

    menu.addButton( MenuType::MAIN,
        texmanager.get("menu/start.bmp"),
        ButtonValue::NEWGAME, 0, 0, 1, 2);

    menu.addButton( MenuType::MAIN,
        texmanager.get("menu/settings.bmp"),
        ButtonValue::OPTIONS, 1, 0, 1, 1);

    menu.addButton( MenuType::MAIN,
        texmanager.get("menu/exit.bmp"),
        ButtonValue::EXIT, 2, 0, 1, 0);


    menu.addButton( MenuType::GAME,
        texmanager.get("menu/rotate.bmp"),
        ButtonValue::GAME_ROTATE, 0, 0, 0, 1);

    menu.addButton( MenuType::GAME,
        texmanager.get("menu/arrow_down.bmp"),
        ButtonValue::GAME_DOWN, 1, 0, 1, 1);

    menu.addButton( MenuType::GAME,
        texmanager.get("menu/arrow_left.bmp"),
        ButtonValue::GAME_LEFT, 0, 1, 0, 2);

    menu.addButton( MenuType::GAME,
        texmanager.get("menu/arrow_right.bmp"),
        ButtonValue::GAME_RIGHT, 1, 1, 1, 2);

    menu.addButton( MenuType::GAME,
        texmanager.get("menu/return.bmp"),
        ButtonValue::BACK, 2, 0, 1, 0);


    menu.addButton( MenuType::OPTS,
        texmanager.get((gConf.fullscreen)?"menu/window.bmp"
                                         :"menu/fullscreen.bmp"),
        ButtonValue::OPTS_FULLSCREEN, 0, 0, 0, 1);

    menu.addButton( MenuType::OPTS,
        texmanager.get("menu/keys.bmp"),
        ButtonValue::OPTS_KEYS, 1, 0, 1, 1);

    menu.addButton( MenuType::OPTS,
        texmanager.get("menu/texture.bmp"),
        ButtonValue::OPTS_TEXTURES, 0, 1, 0, 2);

    menu.addButton( MenuType::OPTS,
        texmanager.get("menu/mystery.bmp"),
        ButtonValue::OPTS_MYSTERY, 1, 1, 1, 2);

    menu.addButton( MenuType::OPTS,
        texmanager.get("menu/return.bmp"),
        ButtonValue::BACK, 2, 0, 1, 0);

    menu.addButton( MenuType::MISC,
        texmanager.get("menu/no.bmp"),
        ButtonValue::NONE, 0, 1, 0, 2, &gConf.disabled_textures);

    menu.addButton( MenuType::MISC,
        texmanager.get("menu/no.bmp"),
        ButtonValue::NONE, 1, 1, 1, 2, &gConf.disabled_mystery);

    menu.addButton( MenuType::MISC,
        texmanager.get("menu/rotate.bmp"),
        ButtonValue::NONE, 1, 0, 1, 1, &gConf.enabled_keyrepeat);


    menu.addButton( MenuType::SAVE,
        texmanager.get("menu/save.bmp"),
        ButtonValue::NONE, 0, 1, 0, 2);

    menu.addButton( MenuType::SAVE,
        texmanager.get("menu/yes.bmp"),
        ButtonValue::SAVE_YES, 0, 0, 1, 2);

    menu.addButton( MenuType::SAVE,
        texmanager.get("menu/no.bmp"),
        ButtonValue::SAVE_NO, 1, 0, 1, 1);

    menu.addButton( MenuType::SAVE,
        texmanager.get("menu/return.bmp"),
        ButtonValue::BACK, 2, 0, 1, 0);


    menu.addButton( MenuType::OVER,
        texmanager.get("menu/return.bmp"),
        ButtonValue::BACK, 2, 0, 1, 0);

    menu.setActive(MenuType::MAIN);
}


void load_textures_draw(int ratio)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glColor4f(0.8f, 0.8f, 0.8f, (float)(100-ratio)/100);
    menu.draw();
    SDL_GL_SwapWindow(window);
}

void load_textures()
{
#	ifdef DESKTOP_DETECTED
    texmanager.respath="../../Common/";
#   else
    texmanager.respath="";
#   endif

    g_Font.set_texture( texmanager.get("font.bmp"), 256, 256, 20, 26, ' ' );

    if ( displayLayout.fullscreenWidth  < 500 || displayLayout.fullscreenHeight  < 500)
        texmanager.prefix="s_";

    menu.addButton( MenuType::OVER,
        texmanager.get("menu/loading.bmp"),
        ButtonValue::NONE, 2, 0, 1, 0);

    menu.setActive( MenuType::OVER);
    menu.useColors(false);

    displayLayout.selectControls();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    SDL_GL_SwapWindow(window);

    glEnable(GL_BLEND);

    load_textures_draw(0);

    tetris.initTextures();

    texmanager.add("menu/start.bmp");
    texmanager.add("menu/settings.bmp");
    texmanager.add("menu/exit.bmp");
    texmanager.add("menu/arrow_down.bmp");
    texmanager.add("menu/arrow_left.bmp");
    texmanager.add("menu/arrow_right.bmp");
    texmanager.add("menu/rotate.bmp");
    load_textures_draw(15);
    texmanager.add("menu/return.bmp");
    texmanager.add("menu/fullscreen.bmp");
    texmanager.add("menu/window.bmp");
    texmanager.add("menu/texture.bmp");
    texmanager.add("menu/mystery.bmp");
    texmanager.add("menu/yes.bmp");
    texmanager.add("menu/no.bmp");
    texmanager.add("menu/save.bmp");
    texmanager.add("menu/bye.bmp");

    load_textures_draw(30);
    texmanager.add("skybox/south.bmp");

    load_textures_draw(45);
    texmanager.add("skybox/north.bmp");

    load_textures_draw(60);
    texmanager.add("skybox/west.bmp");

    load_textures_draw(75);
    texmanager.add("skybox/east.bmp");

    load_textures_draw(90);
    texmanager.add("skybox/top.bmp");

    skybox.setProperties(false, true, true);

    skybox.setTextures
    (
        texmanager.get("skybox/south.bmp"),
        texmanager.get("skybox/north.bmp"),
        texmanager.get("skybox/west.bmp"),
        texmanager.get("skybox/east.bmp"),
        texmanager.get("skybox/top.bmp")
    );

    texmanager.prefix="";

    menu.setActive( MenuType::MAIN);
    menu.useColors(true);
}

void fade_quit()
{
    menu.addButton( MenuType::OVER,
        texmanager.get("menu/bye.bmp"),
        ButtonValue::NONE, 2, 0, 1, 0);

    menu.setActive( MenuType::OVER);
    menu.useColors(false);

    displayLayout.selectControls();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);

    for(int i = 100; i>=0; i-=5) {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glColor4f(0.8f, 0.8f, 0.8f, (float)i/100);
        menu.draw();
        SDL_Delay(50);
        SDL_GL_SwapWindow(window);
    }
}


int main(int argc, char **argv)
{
	SDL_version compiled;
	SDL_version linked;
	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	LOGI("Compiled against SDL version %d.%d.%d ...",
	            compiled.major, compiled.minor, compiled.patch);
	LOGI("Linked against SDL version %d.%d.%d.",
	      linked.major, linked.minor, linked.patch);

	createWindow();

#	ifdef MOBILE_DETECTED
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#	endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    srand(SDL_GetTicks());

    InitGL();

    setup_light();

    init_control_buttons();

    load_textures();

    build_menu();

    tetris.init();
    tetris.setTextureDrawing(!gConf.disabled_textures);
    tetris.setKeyRepeat(gConf.enabled_keyrepeat);

    /* Loop, drawing and checking events */
    while ( ! gConf.done ) {


        /* This could go in a separate function */
        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {

            switch ( event.type ) {

            case SDL_QUIT:
                gConf.done = true;
                break;

            case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
					gConf.done = true;
                    break;

                default:
                    break;
                }
                break;

            case SDL_KEYUP:
                first_move = false;
				switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    tetris.releaseLeft();
                    break;
                case SDLK_RIGHT:
                    tetris.releaseRight();
                    break;
                case SDLK_UP:
                    tetris.releaseRotate();
                    break;
                default:
                    break;
                }
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_SHOWN:
                    SDL_Log("Window %d shown", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_HIDDEN:
                    SDL_Log("Window %d hidden", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_EXPOSED:
                    SDL_Log("Window %d exposed", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_MOVED:
                    SDL_Log("Window %d moved to %d,%d",
                            event.window.windowID, event.window.data1,
                            event.window.data2);
                    break;
                case SDL_WINDOWEVENT_RESIZED:
#                   ifdef MOBILE_DETECTED
                    displayLayout.activeMode(event.window.data1, event.window.data2);
                    init_control_buttons();
#                   endif
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    SDL_Log("Window %d minimized", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_MAXIMIZED:
                    SDL_Log("Window %d maximized", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_RESTORED:
                    SDL_Log("Window %d restored", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_ENTER:
                    SDL_Log("Mouse entered window %d",
                            event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    SDL_Log("Mouse left window %d", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    SDL_Log("Window %d gained keyboard focus",
                            event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    SDL_Log("Window %d lost keyboard focus",
                            event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    SDL_Log("Window %d closed", event.window.windowID);
                    break;
                default:
                    SDL_Log("Window %d got unknown event %d",
                            event.window.windowID, event.window.event);
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button==SDL_BUTTON_LEFT) {
                    ButtonValue menu_code = menu_click(event.button.x, event.button.y);
                    if (menu_code!=ButtonValue::NONE) menu_update(menu_code);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                first_move = false;
                if (event.button.button==SDL_BUTTON_LEFT) {
                    if (menu.getActive()==MenuType::GAME) {
                        tetris.releaseLeft();
                        tetris.releaseRight();
                        tetris.releaseRotate();
                    }
                }
            default:
                break;
            }
        }

        if (menu.getActive()==MenuType::GAME && !first_move) {
            /* Fill the game's keymap */

            int m_x, m_y;
            if (SDL_GetMouseState(&m_x, &m_y) & SDL_BUTTON(1))
            {
                ButtonValue menu_code = menu_click(m_x, m_y);
                switch (menu_code)
                {
                case ButtonValue::GAME_LEFT:
                    if (tetris.getCurrentStage() != 0) tetris.moveLeft();
                    else first_move = true;
                    break;
                case ButtonValue::GAME_RIGHT:
                    if (tetris.getCurrentStage() != 0) tetris.moveRight();
                    else first_move = true;
                    break;
                case ButtonValue::GAME_ROTATE:
                    if (tetris.getCurrentStage() != 0) tetris.rotate();
                    else first_move = true;
                    break;
                case ButtonValue::GAME_DOWN:
                    if (tetris.getCurrentStage() != 0) tetris.moveDown();
                    else first_move = true;
                    break;
                default:
                    break;
                }
            }

            Uint8 *key_state = SDL_GetKeyboardState(NULL);

            if ( key_state[SDL_SCANCODE_LEFT] ) {
                if (tetris.getCurrentStage() != 0) tetris.moveLeft();
                else first_move = true;
            }

            if ( key_state[SDL_SCANCODE_RIGHT] ) {
                if (tetris.getCurrentStage() != 0) tetris.moveRight();
                else first_move = true;
            }

            if ( key_state[SDL_SCANCODE_UP] ) {
                if (tetris.getCurrentStage() != 0) tetris.rotate();
                else first_move = true;
            }

            if ( key_state[SDL_SCANCODE_DOWN] ) {
                if (tetris.getCurrentStage() != 0) tetris.moveDown();
                else first_move = true;
            }

        }

        // refresh timers
        GLint timer = SDL_GetTicks();
        if (timer - g_Timer < 34) {
            SDL_Delay(1);
            continue;
        }

        calculate();

        render();

        g_Timer = timer;
        frame++;
        if (g_Timer>g_Timer3+1000 ) {
            g_Timer3 = g_Timer;
            FPS = frame;
            frame=0;
        }

    }

    fade_quit();

    // Once finished with OpenGL functions, the SDL_GLContext can be deleted.
    SDL_GL_DeleteContext(glcontext);

    // Done! Close the window, clean-up and exit the program.
    SDL_DestroyWindow(window);

    SDL_Quit();

    exit(0);

    return 0;
}
