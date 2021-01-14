
// This file added in headers queue
// File: "Sources.h"

namespace NAMESPACE {

  // TO DO
  // Your code ...

    bool bUseCursorInMenu; 
    bool bUseCursorInDialogs;
    bool bUseCursorInInventory;


    void checkIniParams() {
        Union.GetSysPackOption().Read(bUseCursorInMenu, "zViewExtender", "UseCursorInMenu", true);
        Union.GetSysPackOption().Read(bUseCursorInDialogs, "zViewExtender", "UseCursorInDialogs", true);
        Union.GetSysPackOption().Read(bUseCursorInInventory, "zViewExtender", "UseCursorInInventory", true);
    }


  LRESULT __stdcall KeyboardProc( int code, WPARAM wParam, LPARAM lParam ) {
    if( code == HC_ACTION ) {
      GetInputCharacter() = 0;

      switch( wParam ) {
      case WM_KEYDOWN:
        LPKBDLLHOOKSTRUCT kbHookData = (LPKBDLLHOOKSTRUCT)lParam;

        WORD symbol = 0;
        byte kbState[256];
        GetKeyboardState( kbState );
        if( ToAscii( kbHookData->vkCode, MapVirtualKey( kbHookData->vkCode, 0 ), kbState, &symbol, 0 ) != Invalid )
          GetInputCharacter() = (char)symbol;

        break;
      }
    }

    return CallNextHookEx( 0, code, wParam, lParam );
  }


  void Game_Entry() {
    SetWindowsHookEx( WH_KEYBOARD_LL, KeyboardProc, 0, 0 );
    PATCH_EXECUTE(
      "MemSet(0x0064F1C7, '90', 0x0064F1F0 - 0x0064F1C7)"
    );


    checkIniParams();

    Ivk_Menu_Render; 
    Ivk_Menu_HandleEvent;

    bool enableInDialogs = true;
    Union.GetSysPackOption().Read( enableInDialogs, "zViewExtender", "UseCursorInDialogs", enableInDialogs );
    if( !enableInDialogs ) {
      Ivk_zCViewDialogChoice_BlitText.Detach();
      Ivk_zCViewDialog_StartSelection.Detach();
      Ivk_zCViewDialog_StopSelection.Detach();
      Ivk_zCViewDialogChoice_HandleEvent.Detach();
    }


    bool enableInInventory = true;
    Union.GetSysPackOption().Read( enableInInventory, "zViewExtender", "UseCursorInInventory", enableInInventory );
    if( !enableInInventory ) {
      Ivk_oCItemContainer_HandleEvent.Detach();
      Ivk_oCStealContainer_HandleEvent.Detach();
      Ivk_oCNpcContainer_HandleEvent.Detach();
      Ivk_oCNpcInventory_HandleEvent.Detach();
      Ivk_oCNpcInventory_OpenPassive.Detach();
      Ivk_oCNpcInventory_Close.Detach();
      Ivk_oCItemContainer_Draw.Detach();
      Ivk_oCItem_RenderItem.Detach();
    }
  }

  static zCViewNote* note = Null;
  void Game_Init() {
    RECT rect;
    GetWindowRect( hWndApp, &rect );
    ClipCursor( &rect );
  }

  void Game_Exit() {
  }

  void Game_PreLoop() {
    zCViewCursor::GetCursor()->FrameBegin();
  }

  void Game_Loop() {
    return;
    static zCViewRenderer* rnd = Null;
    if( !rnd ) {
      rnd = new zCViewRenderer();
      rnd->InsertBack( "Black" );
      rnd->InsertVisual( player );
      screen->InsertItem( rnd );
      rnd->SetSize( 1024, 4096 );
      rnd->Properties.Camera.Rotation[VX] = 15.0f;
    }

    rnd->Properties.Camera.Rotation[VY] += 0.05f * ztimer->frameTimeFloat;

    static zCView* grd = Null;
    if( !grd ) {
      grd = new zCView( 0, 0, 0, 0 );
      grd->InsertBack( "Black_2_Alpha" );
      rnd->InsertItem( grd );
      grd->SetPos( 0, 4096 );
      grd->SetSize( 8192, 4096 );
    }


    return;
    static zCView* Slot = Null;
    static zCViewInteractive* Cell1 = Null;
    static zCViewAnimated* Cell2 = Null;
    if( !Slot ) {
      Slot = new zCView();
      screen->InsertItem( Slot );
      Slot->InsertBack( "Black" );
      Slot->SetSize( 8192, 8192 );// zPixelX( 600 ), zPixelY( 600 ) );
      Slot->SetPos( 0, 0 );

      Cell1 = new zCViewInteractive( 0, 0, 8192, 8192 );
      Slot->InsertItem( Cell1 );

      Cell2 = new zCViewAnimated( 0, 0, 8192, 3072 );
      //Cell2->InsertBack( "INVENTORYFOG_A0.TGA" );// "Black_2_Alpha" );
      //Cell2->SetFps( 20 );
      Cell2->SetAlphaBlendFunc( zRND_ALPHA_FUNC_BLEND );
      Cell2->SetPos( 0, 8192 - 3072 );
      Slot->InsertItem( Cell2 );
    }
  }

  void Game_PostLoop() {
    zCViewCursor::GetCursor()->FrameEnd();
    zCViewCursor::GetCursor()->Render();
  }

  void Game_MenuLoop() {

    /*if( !note ) {
      note = new zCViewNote();
      screen->InsertItem( note );
      note->SetPos( 4196, 0 );
      note->SetFont( "FONT_OLD_10_WHITE.TGA" );
      note->EditBegin();
      note->InsertBack( "black" );
      note->SetSize( 3000, 3000 );
    }*/

    // note->SetHandleEventTop();

#if 0
    // Test program
    InitTest();
#endif
    if( !player ) {
      screen->RemoveItem( zCViewCursor::GetCursor() );
      zCViewCursor::GetCursor()->FrameBegin();
      zCViewCursor::GetCursor()->FrameEnd();

      screen->InsertItem( zCViewCursor::GetCursor() );
      zCViewCursor::GetCursor()->Render( false );
    }
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

  // A multiplatform application reference.
  // please disable unused functions.
  // write 'false' in 'Enabled' macro.
#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_PreLoop,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_PostLoop,
    Enabled( AppDefault ) Game_MenuLoop,
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
}