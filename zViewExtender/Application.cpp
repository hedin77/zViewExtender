#include "UnionAfx.h"

//#ifdef __G1
//#define NAMESPACE Gothic_I_Classic
//#include "zViewExtender.h"
//#endif
//#ifdef __G1A
//#define NAMESPACE Gothic_I_Addon
//#include "zViewExtender.h"
//#endif
//#ifdef __G2
//#define NAMESPACE Gothic_II_Classic
//#include "zViewExtender.h"
//#endif
#ifdef __G2A
#define NAMESPACE Gothic_II_Addon
#include "zViewExtender.h"
#endif

void Game_Entry() {
}

#include <math.h>

using namespace Gothic_II_Addon;

#if 0
// 008D1D84
bool_t& joystickEnabled = *(bool_t*)0x008D1D84;

zBOOL InitDInput_Joystick( HWND hWnd, HINSTANCE hInstance );

// .text:004D3930 InitDInput_Joystick proc near
HOOK Ivk_InitDInput_Joystick AS( 0x004D3930, InitDInput_Joystick );

zBOOL InitDInput_Joystick( HWND hWnd, HINSTANCE hInstance ) {
  zBOOL v = Ivk_InitDInput_Joystick( hWnd, hInstance );
  joystickEnabled = v;
  Message::Box( "Joy: " + A joystickEnabled );
  return v;
}



#include <dinput.h>
// 0x008D1D60 struct IDirectInput7A * lpDInput
IDirectInput7A*& lpDInput = *(IDirectInput7A**)0x008D1D60;
LPDIRECTINPUTDEVICE7& lpDIJoystick = *(LPDIRECTINPUTDEVICE7*)0x008D1D88;

// 0x004D32E0 int __stdcall _EnumJoysticksCallback(struct DIDEVICEINSTANCEA const *,void *)
// 0x004D3310 int __stdcall _EnumJoystickObjectsCallback(struct DIDEVICEOBJECTINSTANCEA const *,void *)
//BOOL CALLBACK _EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
//BOOL CALLBACK _EnumJoystickObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
//
//HOOK Ivk_EnumJoysticksCallback       AS( 0x004D32E0, _EnumJoysticksCallback );
//HOOK Ivk_EnumJoystickObjectsCallback AS( 0x004D3310, _EnumJoystickObjectsCallback );
//
//BOOL CALLBACK _EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext ) {
//  HRESULT hr;
//
//  // Obtain an interface to the enumerated joystick.
//  hr = lpDInput->CreateDeviceEx( pdidInstance->guidInstance,
//    IID_IDirectInputDevice7A,
//    (void**)&lpDIJoystick,
//    NULL );
//  if( FAILED( hr ) )
//    return DIENUM_CONTINUE;
//
//  return DIENUM_STOP;
//}
//
//BOOL CALLBACK _EnumJoystickObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext ) {
//  LPDIRECTINPUTDEVICE7 pdi = (LPDIRECTINPUTDEVICE7)( pContext );
//
//  // For axes that are returned, set the DIPROP_RANGE property for the
//  // enumerated axis in order to scale min/max values.
//  if( ( pdi ) && ( pdidoi->dwType & DIDFT_AXIS ) ) {
//    DIPROPRANGE diprg;
//    diprg.diph.dwSize = sizeof( DIPROPRANGE );
//    diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
//    diprg.diph.dwHow = DIPH_BYID;
//    diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
//    diprg.lMin = -1000;
//    diprg.lMax = +1000;
//
//    // Set the range for the axis
//    if( FAILED( pdi->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
//      return DIENUM_STOP;
//  }
//  return DIENUM_CONTINUE;
//}



struct zTMouseResult {
  int    xpos;
  int    ypos;
  int    zpos;
  bool_t buttonPressedLeft;
  bool_t buttonPressedMid;
  bool_t buttonPressedRight;
};

bool*          keyevent    = (bool*)          ZenDef( 0x0086CCC8, 0x008B27C0, 0x008C3020, 0x008D1678 );
bool*          keytoggle   = (bool*)          ZenDef( 0x0086CED0, 0x008B29D4, 0x008C3260, 0x008D18B8 );
zCArray<int>&  keybuffer   = *(zCArray<int>*) ZenDef( 0x0086D2DC, 0x008B2E00, 0x008C36F0, 0x008D1D50 );
zTMouseResult& zmouseState = *(zTMouseResult*)ZenDef( 0x0086CCAC, 0x008B27A8, 0x008C3004, 0x008D165C );

// 0x004D3D60 int __cdecl SetAcquireJoystick(void)
CInvoke<int( __cdecl* )( )> SetAcquireJoystick( 0x004D3D60 );

// .text:004D42A0 UpdateJoystick  proc near
void UpdateJoystick();

HOOK Ivk_UpdateJoystick AS( 0x004D42A0, &UpdateJoystick );

void UpdateJoystick_KeyState( DIDEVICEOBJECTDATA& od ) {
  switch( od.dwOfs ) {
  case DIJOFS_X:  // x-axis
  {
    zmouseState.xpos = od.dwData / 10;
    cmd << zmouseState.xpos << endl;

    //if( ( keyId != 0 ) /*&& !keyevent[keyId]*/ ) {
    //  bool keyEventBefore = keyevent[keyId];
    //  keyevent[keyId] = true;
    //  keytoggle[keyId] = !keyEventBefore;
    //  keybuffer.InsertEnd( keyId );
    //}
    //else {
    //  keyevent[JOY_LEFT] = false;
    //  keyevent[JOY_RIGHT] = false;
    //}
  }
  break;
  case DIJOFS_Y:  // y-axis
  {
    int keyId = 0;
    if( (int)( od.dwData ) < -500 )  keyId = JOY_UP;  else
      if( (int)( od.dwData ) > 500 )    keyId = JOY_DOWN;

    if( ( keyId != 0 ) /*&& !keyevent[keyId]*/ ) {
      bool keyEventBefore = keyevent[keyId];
      keyevent[keyId] = true;
      keytoggle[keyId] = !keyEventBefore;
      keybuffer.InsertEnd( keyId );
    }
    else {
      keyevent[JOY_UP] = false;
      keyevent[JOY_DOWN] = false;
    }
  }
  break;
  }
}

void UpdateJoystick() {
  //cmd << "UpdateJoystick: " << joystickEnabled << tab << AHEX32( lpDIJoystick ) << endl;
  //joystickEnabled = True;

  if( lpDIJoystick ) {
    SetAcquireJoystick();

    BOOL                bDone;
    DIDEVICEOBJECTDATA  od;
    DWORD               dwElements;
    HRESULT             hr;

    lpDIJoystick->Poll();
    bDone = FALSE;

    while( !bDone ) {
      dwElements = 1;
      hr = lpDIJoystick->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), &od, &dwElements, 0 );

      if( hr == DIERR_INPUTLOST ) {
        SetAcquireJoystick();
        break;
      }

      if( FAILED( hr ) || dwElements == 0 ) {
        break;
      }

      UpdateJoystick_KeyState( od );
    }
  }
}
#endif



bool viewInter_Enter( zCViewInteractive* view, zCViewCursor* cursor ) {
    cmd << "#1" << endl;
  view->SetFps( 16 );
  return true;
}

bool viewInter_Leave( zCViewInteractive* view, zCViewCursor* cursor ) {
    cmd << "#2" << endl;
  view->SetFps( 2 );
  return true;
}

bool viewInter_Down( zCViewInteractive* view, zCViewCursor* cursor, zEMouseButton button ) {
    cmd << "#DOWN" << endl;
  view->SetFps( 16 );
  return true;
}

bool viewInter_Up( zCViewInteractive* view, zCViewCursor* cursor, zEMouseButton button ) {
    cmd << "#UP" << endl;
  view->SetFps( 2 );
  return true;
}

bool viewInter_Drag( zCViewInteractive* view, zCViewCursor* cursor ) {
  return true;
}

static zCViewInteractive* viewInter = Null;
static zCViewInteractive* viewInter2 = Null;
void Game_Init() {
  if( !viewInter ) {
    viewInter = new zCViewInteractive( 0, 0, 3000, 3000 );
    viewInter->InsertBack( "BPFX_GOLEM_A0" );
    viewInter->SetFps( 2 );
    viewInter->InitFunctions(
      viewInter_Enter,
      viewInter_Leave,
      viewInter_Down,
      viewInter_Drag,
      viewInter_Up,
      Null
    );
    screen->InsertItem( viewInter );

    viewInter2 = new zCViewInteractive( 0, 0, 3000, 3000 );
    viewInter2->InsertBack( "BPFX_GOLEM_A0" );
    viewInter2->SetFps( 2 );
    viewInter2->InitFunctions(
      viewInter_Enter,
      viewInter_Leave,
      Null,
      viewInter_Drag,
      Null,
      Null
    );
    viewInter->InsertItem( viewInter2 );
  }
}

void Game_Exit() {
}


void Game_Loop() {
  viewInter->Top();


  zCViewCursor* cursor = zCViewCursor::GetCursor();
  zCView* top = cursor->GetTopSelectedView();
  if( top ) {
    top->ShowBounds( GFX_RED );
  }


  /*static zCViewAnimated* view = Null;
  if( !view ) {
    view = new zCViewAnimated( 0, 0, 3000, 3000 );
    view->InsertBack( "BPFX_GOLEM_A0" );
    view->SetFps( 2 );
    screen->InsertItem( view );
  }*/
  
//#ifdef __G1
//  Gothic_I_Classic::zCTexAniHandler::DoFrame();
//#endif
//#ifdef __G1A
//  Gothic_I_Addon::zCTexAniHandler::DoFrame();
//#endif
//#ifdef __G2
//  Gothic_II_Classic::zCTexAniHandler::DoFrame();
//#endif
//#ifdef __G2A
//  Gothic_II_Addon::zCTexAniHandler::DoFrame();
//#endif
}

void Game_SaveBegin() {
}

void Game_SaveEnd() {
}

void LoadBegin() {
}

void LoadEnd() {
}

void Game_LoadBegin_NewGame() {
  LoadBegin();
}

void Game_LoadEnd_NewGame() {
 LoadEnd();
}

void Game_LoadBegin_SaveGame() {
 LoadBegin();
}

void Game_LoadEnd_SaveGame() {
 LoadEnd();
}

void Game_LoadBegin_ChangeLevel() {
 LoadBegin();
}

void Game_LoadEnd_ChangeLevel() {
 LoadEnd();
}

void Game_LoadBegin_Trigger() {
}

void Game_LoadEnd_Trigger() {
}

void Game_Pause() {
}

void Game_Unpause() {
}

void Game_DefineExternals() {
}

#define AppDefault False
CApplication* lpApplication = CApplication::CreateRefApplication (
  Enabled( AppDefault ) Game_Entry,
  Enabled( True )       Game_Init,
  Enabled( AppDefault ) Game_Exit,
  Enabled( True )       Game_Loop,
  Enabled( AppDefault ) Game_SaveBegin,
  Enabled( AppDefault ) Game_SaveEnd,
  Enabled( AppDefault ) Game_LoadBegin_NewGame,
  Enabled( AppDefault ) Game_LoadEnd_NewGame,
  Enabled( AppDefault ) Game_LoadBegin_SaveGame,
  Enabled( AppDefault ) Game_LoadEnd_SaveGame,
  Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
  Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
  Enabled( AppDefault ) Game_LoadBegin_Trigger,
  Enabled( AppDefault ) Game_LoadEnd_Trigger,
  Enabled( AppDefault ) Game_Pause,
  Enabled( AppDefault ) Game_Unpause,
  Enabled( AppDefault ) Game_DefineExternals
  );