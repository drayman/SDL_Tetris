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

#include "mesh/button_mesh.h"
ButtonMesh controlButton;

#include "system/display.h"
DisplayLayout displayLayout;

#include "game/tetris.h"
Tetris tetris(texmanager, g_Font);


#include "menu/menu.h"
Menu menu(3, 2);

struct GameConfig {

    GameConfig() :
        menu_mode(0),
#       ifdef DESKTOP_DETECTED
        fullscreen(false),
#       else
        fullscreen(true),
#       endif
        textures(true),
        mystery(false),
        done(false)
        {}

    unsigned char menu_mode;
    bool fullscreen;
    bool textures;
    bool mystery;
    bool done;
} gConf;


float g_fYrot = 0;

GLint  g_Timer           = 0;
GLint  g_Timer2          = 0;
GLint  g_Timer3          = 0;

int frame=0;
int FPS = 0;


static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s", name, v);
}

void init_control_buttons()
{
    if ( displayLayout.isLandscape() )
        if ( displayLayout.controlWidth > displayLayout.controlHeight*0.667f )
            controlButton.scale((float)displayLayout.controlHeight/displayLayout.controlWidth*0.667f, 1.0f);
        else
            controlButton.scale(1.0f, (float)displayLayout.controlWidth/displayLayout.controlHeight*1.5f);
    else
        if ( displayLayout.controlWidth > displayLayout.controlHeight*1.5f )
            controlButton.scale((float)displayLayout.controlHeight/displayLayout.controlWidth*1.5f, 1.0f);
        else
            controlButton.scale(1.0f, (float)displayLayout.controlWidth/displayLayout.controlHeight*0.667f);
}


void toggle_fullscreen()
{

    SDL_SetWindowFullscreen(window, gConf.fullscreen);

/*
    if (gConf.fullscreen)
        displayLayout.fullscreenMode();
    else
        displayLayout.windowMode();
*/
    displayLayout.toggleMode();

    SDL_SetWindowSize(window, displayLayout.width, displayLayout.height);

    init_control_buttons();
}


void menu_select(unsigned char menu_x, unsigned char menu_y)
{
    if (gConf.menu_mode==0) {

        if (displayLayout.isLandscape()) {      // Main menu
            if (menu_x == 1 && menu_y==0) gConf.menu_mode = 1;
            else
            if (menu_x == 1 && menu_y==1) gConf.menu_mode = 2;
            else
            if (menu_x == 1 && menu_y==2) gConf.done = true;
        } else {
            if (menu_x == 0 && menu_y==1) gConf.menu_mode = 1;
            else
            if (menu_x == 1 && menu_y==1) gConf.menu_mode = 2;
            else
            if (menu_x == 2 && menu_y==1) gConf.done = true;
        }
    } else if (gConf.menu_mode==1) {            // Game
        if (displayLayout.isLandscape()) {
            if (menu_x == 1 && menu_y==2) gConf.menu_mode=3;
        } else {
            if (menu_x == 2 && menu_y==1) gConf.menu_mode=3;
        }
    } else if (gConf.menu_mode==2) {            // Settings
        if (menu_x == 0 && menu_y==0) gConf.textures = !gConf.textures;
        else
        if (menu_x == 1 && menu_y==0) gConf.mystery = !gConf.mystery;
        else
        if (menu_x == 0 && menu_y==1) {
            gConf.fullscreen = !gConf.fullscreen;
            toggle_fullscreen();
        } else {
            if (displayLayout.isLandscape()) {
                if (menu_x == 1 && menu_y==2) gConf.menu_mode=0;
            } else {
                if (menu_x == 2 && menu_y==1) gConf.menu_mode=0;
            }
        }
    } else if (gConf.menu_mode==3) {            // Save or leave
        if (displayLayout.isLandscape()) {
            if (menu_x==1) {
                if (menu_y==0) gConf.menu_mode=0;
                else
                if (menu_y==1) {
                    tetris.reset();
                    gConf.menu_mode=0; }
                else
                if (menu_y==2) gConf.menu_mode=1;
            }
        } else {
            if (menu_y==1) {
                if (menu_x==0) gConf.menu_mode=0;
                else
                if (menu_x==1) {
                    tetris.reset();
                    gConf.menu_mode=0; }
                else
                if (menu_x==2) gConf.menu_mode=1;
            }
        }
    } else if (gConf.menu_mode==4) {            // GAME OVER
        if (displayLayout.isLandscape()) {
            if (menu_x == 1 && menu_y==2) {
                gConf.menu_mode=0;
                tetris.reset();
            }
        } else {
            if (menu_x == 2 && menu_y==1)
            {
                gConf.menu_mode=0;
                tetris.reset();
            }
        }
    }
}


void menu_click(Uint16 mouse_x, Uint16 mouse_y)
{
    unsigned char menu_x, menu_y;

    if (displayLayout.isLandscape()) {
        if (mouse_x<displayLayout.mainWidth) return;
        mouse_x-=displayLayout.mainWidth;
        menu_x = mouse_x*2/displayLayout.controlWidth;
        menu_y = mouse_y*3/displayLayout.controlHeight;
    } else {
        if (mouse_y<displayLayout.mainHeight) return;
        mouse_y-=displayLayout.mainWidth;
        menu_x = mouse_x*3/displayLayout.controlWidth;
        menu_y = mouse_y*2/displayLayout.controlHeight;
    }

    menu_select(menu_x, menu_y);
}


void draw_menu()
{
    displayLayout.selectControls();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);


    if (gConf.menu_mode == 0) {
/*
        if (displayLayout.isLandscape()) glTranslatef( 1.5f,  2.5f,  0.0f);
                                    else glTranslatef( 0.5f,  0.5f,  0.0f);
        texmanager.bind("menu/start.bmp");
        controlButton.draw();

        if (displayLayout.isLandscape()) glTranslatef( 0.0f,  -1.0f,  0.0f);
                                    else glTranslatef( 1.0f,  0.0f,  0.0f);
        texmanager.bind("menu/settings.bmp");
        controlButton.draw();

        if (displayLayout.isLandscape()) glTranslatef( 0.0f,  -1.0f,  0.0f);
                                    else glTranslatef( 1.0f,  0.0f,  0.0f);
        texmanager.bind("menu/exit.bmp");
        controlButton.draw();
*/
        menu.draw();

    } else if (gConf.menu_mode == 1) {

        if (displayLayout.isLandscape()) glTranslatef(0.5f, 1.5f, 0.0f);
                                    else glTranslatef( 0.5f,  0.5f,  0.0f);
        texmanager.bind("menu/rotate.bmp");
        controlButton.draw();

        glTranslatef(0.0f, 1.0f, 0.0f);
        texmanager.bind("menu/arrow_left.bmp");
        controlButton.draw();

        glTranslatef(1.0f, 0.0f,  0.0f);
        texmanager.bind("menu/arrow_right.bmp");
        controlButton.draw();

        glTranslatef( 0.0f, -1.0f,  0.0f);
        texmanager.bind("menu/arrow_down.bmp");
        controlButton.draw();

        if (displayLayout.isLandscape()) glTranslatef( 0.0f, -1.0f,  0.0f);
                                    else glTranslatef( 1.0f,  0.0f,  0.0f);
        texmanager.bind("menu/return.bmp");
        controlButton.draw();

    } else  if (gConf.menu_mode == 2) {

        if (displayLayout.isLandscape()) glTranslatef( 0.5f, 1.5f, 0.0f);
                                    else glTranslatef( 0.5f, 0.5f, 0.0f);
        if (!gConf.fullscreen) texmanager.bind("menu/fullscreen.bmp");
                          else texmanager.bind("menu/window.bmp");
        controlButton.draw();

        glTranslatef(0.0f, 1.0f, 0.0f);
        texmanager.bind("menu/texture.bmp");
        controlButton.draw();
        if (!gConf.textures) {
            texmanager.bind("menu/no.bmp");
            controlButton.draw();
        }

        glTranslatef(1.0f, 0.0f,  0.0f);
        texmanager.bind("menu/mystery.bmp");
        controlButton.draw();
        if (!gConf.mystery) {
            texmanager.bind("menu/no.bmp");
            controlButton.draw();
        }

        if (displayLayout.isLandscape()) glTranslatef( 0.0f, -2.0f,  0.0f);
                                    else glTranslatef( 1.0f, -1.0f,  0.0f);
        texmanager.bind("menu/return.bmp");
        controlButton.draw();

    } else if (gConf.menu_mode == 3) {

        if (displayLayout.isLandscape()) glTranslatef( 0.5f, 2.5f, 0.0f);
                                    else glTranslatef( 0.5f, 1.5f, 0.0f);
        texmanager.bind("menu/save.bmp");
        controlButton.draw();

        if (displayLayout.isLandscape()) glTranslatef( 1.0f, 0.0f, 0.0f);
                                    else glTranslatef( 0.0f, -1.0f, 0.0f);
        texmanager.bind("menu/yes.bmp");
        controlButton.draw();

        if (displayLayout.isLandscape()) glTranslatef( 0.0f, -1.0f, 0.0f);
                                    else glTranslatef( 1.0f, 0.0f, 0.0f);
        texmanager.bind("menu/no.bmp");
        controlButton.draw();

        if (displayLayout.isLandscape()) glTranslatef( 0.0f, -1.0f,  0.0f);
                                    else glTranslatef( 1.0f, 0.0f,  0.0f);
        texmanager.bind("menu/return.bmp");
        controlButton.draw();

    } else if (gConf.menu_mode == 4) {
        if (displayLayout.isLandscape()) glTranslatef( 1.5f, 0.5f,  0.0f);
                                    else glTranslatef( 2.5f, 0.5f,  0.0f);
        texmanager.bind("menu/return.bmp");
        controlButton.draw();
    }

    glEnable(GL_DEPTH_TEST);
    glDisable (GL_BLEND);

}

void calculate()
{
    if ( gConf.menu_mode == 1 )
        if (!tetris.update()) gConf.menu_mode = 4; // GAME OVER
}

void render() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    glDisable (GL_BLEND);
    if (gConf.textures) {

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



    if (gConf.menu_mode == 1 || gConf.menu_mode == 3 || gConf.menu_mode == 4)
    {

        displayLayout.selectPanel();

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
        tetris.draw(gConf.textures);

        displayLayout.selectNext();
        tetris.drawNext(gConf.textures);

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
    displayLayout.setWindowSize(480,800);
    displayLayout.windowMode();
#   else
    displayLayout.setWindowSize(current.w, current.h);
    displayLayout.fullscreenMode();
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
/*
    texmanager.add("menu/start.bmp");
    texmanager.add("menu/settings.bmp");
    texmanager.add("menu/exit.bmp");
    texmanager.add("menu/arrow_down.bmp");
    texmanager.add("menu/arrow_left.bmp");
    texmanager.add("menu/arrow_right.bmp");
    texmanager.add("menu/rotate.bmp");
    texmanager.add("menu/return.bmp");
    texmanager.add("menu/fullscreen.bmp");
    texmanager.add("menu/window.bmp");
    texmanager.add("menu/texture.bmp");
    texmanager.add("menu/mystery.bmp");
    texmanager.add("menu/yes.bmp");
    texmanager.add("menu/no.bmp");
    texmanager.add("menu/save.bmp");
*/

    menu.setSize(displayLayout.controlWidth, displayLayout.controlHeight);

    menu.addButton( MenuType::MAIN,
        texmanager.get("menu/start.bmp"),
        ButtonValue::NEWGAME, 0, 0, 1, 2);

    menu.addButton( MenuType::MAIN,
        texmanager.get("menu/settings.bmp"),
        ButtonValue::OPTIONS, 1, 0, 1, 1);

    menu.addButton( MenuType::MAIN,
        texmanager.get("menu/exit.bmp"),
        ButtonValue::EXIT, 2, 0, 1, 0);
}


void load_textures_draw(int ratio)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glColor4f((float)(100-ratio)/100, (float)(100-ratio)/100, (float)(100-ratio)/100, 1.0f);
    g_Font.draw_text( (displayLayout.isLandscape())?(1.7f):(2.7f), 0.3f, 0.0f, "Loading");
    SDL_GL_SwapWindow(window);
}

void load_textures()
{
#	ifdef DESKTOP_DETECTED
    texmanager.respath="../../Common/";
#   else
    texmanager.respath="";
#   endif

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    displayLayout.selectControls();

    g_Font.set_texture( texmanager.get("font.bmp"), 256, 256, 20, 26, ' ' );
    g_Font.set_align( Font::FontAlign::RIGHT );

    if ( displayLayout.isLandscape() )
        if ( displayLayout.controlWidth > displayLayout.controlHeight*0.667f )
            g_Font.set_size((float)displayLayout.controlHeight/displayLayout.controlWidth*0.667f*0.2f, 0.26f);
        else
            g_Font.set_size(0.2f, (float)displayLayout.controlWidth/displayLayout.controlHeight*1.5f*0.26f);
    else
        if ( displayLayout.controlWidth > displayLayout.controlHeight*1.5f )
            g_Font.set_size((float)displayLayout.controlHeight/displayLayout.controlWidth*1.5f*0.2f, 0.26f);
        else
            g_Font.set_size(0.2f, (float)displayLayout.controlWidth/displayLayout.controlHeight*0.667f*0.26f);

    load_textures_draw(0);


    if ( displayLayout.fullscreenWidth  < 500 || displayLayout.fullscreenHeight  < 500)
        texmanager.prefix="s_";

    tetris.initTextures();

    texmanager.add("menu/start.bmp");
    texmanager.add("menu/settings.bmp");
    texmanager.add("menu/exit.bmp");
    texmanager.add("menu/arrow_down.bmp");
    texmanager.add("menu/arrow_left.bmp");
    load_textures_draw(15);
    texmanager.add("menu/arrow_right.bmp");
    texmanager.add("menu/rotate.bmp");
    texmanager.add("menu/return.bmp");
    texmanager.add("menu/fullscreen.bmp");
    texmanager.add("menu/window.bmp");
    texmanager.add("menu/texture.bmp");
    //texmanager.add("menu/x.bmp");
    texmanager.add("menu/mystery.bmp");
    texmanager.add("menu/yes.bmp");
    texmanager.add("menu/no.bmp");
    texmanager.add("menu/save.bmp");

    build_menu();

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
}

void fade_quit()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    displayLayout.selectControls();

    g_Font.set_align( Font::FontAlign::RIGHT );

    if ( displayLayout.isLandscape() )
        if ( displayLayout.controlWidth > displayLayout.controlHeight*0.667f )
            g_Font.set_size((float)displayLayout.controlHeight/displayLayout.controlWidth*0.667f*0.2f, 0.26f);
        else
            g_Font.set_size(0.2f, (float)displayLayout.controlWidth/displayLayout.controlHeight*1.5f*0.26f);
    else
        if ( displayLayout.controlWidth > displayLayout.controlHeight*1.5f )
            g_Font.set_size((float)displayLayout.controlHeight/displayLayout.controlWidth*1.5f*0.2f, 0.26f);
        else
            g_Font.set_size(0.2f, (float)displayLayout.controlWidth/displayLayout.controlHeight*0.667f*0.26f);

    for(int i = 100; i>=0; i-=5) {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glColor4f((float)i/100, (float)i/100, (float)i/100, 1.0f);
        g_Font.draw_text( (displayLayout.isLandscape())?(1.7f):(2.7f), 0.3f, 0.0f, "Bye!");
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

    tetris.init();

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
#                       ifdef MOBILE_DETECTED
                    displayLayout.activeMode(event.window.data1, event.window.data2);
                    init_control_buttons();
#                       endif
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
                if (event.button.button==SDL_BUTTON_LEFT)
                    menu_click(event.button.x, event.button.y);
                break;


            default:
                break;
            }
        }

        if (gConf.menu_mode == 1) {
            /* Fill the game's keymap */
            Uint8 *key_state = SDL_GetKeyboardState(NULL);

            int m_x, m_y;
            if (SDL_GetMouseState(&m_x, &m_y) & SDL_BUTTON(1))
            {
                if (displayLayout.isLandscape()) {
                    if (m_x>=(int)displayLayout.mainWidth)
                    {
                        m_x-=displayLayout.mainWidth;
                        m_x = m_x*2/displayLayout.controlWidth;
                        m_y = m_y*3/displayLayout.controlHeight;
                    }
                } else {
                    if (m_y>=(int)displayLayout.mainHeight)
                    {
                        m_y-=displayLayout.mainWidth;
                        m_x = m_x*3/displayLayout.controlWidth;
                        m_y = m_y*2/displayLayout.controlHeight;
                    }
                }

                if (m_y==0) {
                    if (m_x==0)
                        tetris.moveLeft();
                    else if (m_x==1)
                        tetris.moveRight();
                } else if (m_y==1) {
                    if (m_x==0)
                        tetris.rotate();
                    else if (m_x==1)
                        tetris.moveDown();
                }

            }

            if ( key_state[SDL_SCANCODE_LEFT] )
                tetris.moveLeft();

            if ( key_state[SDL_SCANCODE_RIGHT] )
                tetris.moveRight();

            if ( key_state[SDL_SCANCODE_UP] )
                tetris.rotate();

            if ( key_state[SDL_SCANCODE_DOWN] )
                tetris.moveDown();

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
