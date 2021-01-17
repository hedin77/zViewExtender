
namespace NAMESPACE {
  //CInvoke<void( __thiscall* )( zCViewDialogChoice* )> Ivk_zCViewDialogChoice_BlitText( ZCVIEWDIALOGCHOICE_BLITTEXT, &zCViewDialogChoice::BlitText_Union, HOOK_ENABLED );
  //CInvoke<void( __fastcall* )( zCViewDialogChoice* )> Ivk_zCViewDialog_StartSelection( ZCVIEWDIALOG_STARTSELECTION, &zCViewDialogChoice::StartSelection_Union, HOOK_ENABLED );
  //CInvoke<void( __fastcall* )( zCViewDialogChoice* )> Ivk_zCViewDialog_StopSelection( ZCVIEWDIALOG_STOPSELECTION, &zCViewDialogChoice::StopSelection_Union, HOOK_ENABLED );
  //CInvoke<bool_t( __thiscall* )( zCViewDialogChoice*, int )> Ivk_zCViewDialogChoice_HandleEvent ( ZCVIEWDIALOGCHOICE_HANDLEEVENT, &zCViewDialogChoice::HandleEvent_Union, HOOK_ENABLED );

#if 1

  static BOOL bUseCursorInDialogs = NULL;

  bool needUseCursorInDialogs() {
       if (bUseCursorInDialogs == NULL) {
           Union.GetSysPackOption().Read(bUseCursorInDialogs, "zViewExtender", "UseCursorInDialogs", true);
       }
       return bUseCursorInDialogs;
  }

  HOOK Ivk_zCViewDialogChoice_BlitText    PATCH_IF( &zCViewDialogChoice::BlitText,       &zCViewDialogChoice::BlitText_Union, needUseCursorInDialogs());
  HOOK Ivk_zCViewDialog_StartSelection    PATCH_IF( &zCViewDialogChoice::StartSelection, &zCViewDialogChoice::StartSelection_Union, needUseCursorInDialogs());
  HOOK Ivk_zCViewDialog_StopSelection     PATCH_IF( &zCViewDialogChoice::StopSelection,  &zCViewDialogChoice::StopSelection_Union, needUseCursorInDialogs());
  HOOK Ivk_zCViewDialogChoice_HandleEvent PATCH_IF( &zCViewDialogChoice::HandleEvent,    &zCViewDialogChoice::HandleEvent_Union, needUseCursorInDialogs());


  void zCViewDialogChoice::BlitText_Union() {
    int linesNum = ListTextLines.GetNum();
    if( linesNum && IsActivated ) {

      zCViewText2* viewText    = ListTextLines[0];
      zCViewFont&  viewFont    = viewText->ViewFont;
      zCFont*      font        = viewFont.Font;
      int          fontHeight  = zPixelY( font->GetFontY() );
      int          linesNumMax = VirtualSize.Y / fontHeight;

      int x, y;
      zCViewCursor::GetCursor()->GetActivePointPosition( x, y );
      int top = VirtualPosition.Y;

      if( zKeyToggled( MOUSE_WHEELUP ) ) {
        if( LineStart > 0 )
          ScrollUp();
      }
      else if( zKeyToggled( MOUSE_WHEELDOWN ) ) {
        if( LineStart + 1 < linesNum )
          ScrollDown();
      }

      for( int_t i = 0; i < linesNumMax; i++ ) {
        if( x >= VirtualPosition.X && x < VirtualPosition.X + VirtualSize.X )
          if( ( y - zPixelY( 5 ) ) >= ( top + fontHeight * i ) && ( y - zPixelY( 5 ) ) < ( top + fontHeight * i + fontHeight ) ) {
            Select( LineStart + i );
            break;
          }
      }
    }

    THISCALL( Ivk_zCViewDialogChoice_BlitText )();
  }


  void zCViewDialogChoice::StartSelection_Union() {
    if( !IsDone )
      zCViewCursor::GetCursor()->ShowAtCenter();

    THISCALL( Ivk_zCViewDialog_StartSelection )();
  }


  void zCViewDialogChoice::StopSelection_Union() {
    if( IsDone )
      zCViewCursor::GetCursor()->Hide();

    THISCALL( Ivk_zCViewDialog_StopSelection )();
  }

  int zCViewDialogChoice::HandleEvent_Union( int key ) {
    if( key == MOUSE_BUTTONLEFT ) {
      zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SCROLL.WAV" );
      if( sfx ) {
        zsound->PlaySound( sfx, 1 );
        sfx->Release();
      }
    }

    if( key == MOUSE_WHEELUP || key == MOUSE_WHEELDOWN )
      return True;

    return THISCALL( Ivk_zCViewDialogChoice_HandleEvent)( key );
  }
#endif





  /*HOOK Ivk_zCViewDialogChoice_HandleEvent AS( &zCViewDialogChoice::HandleEvent, &zCViewDialogChoice::HandleEvent_Union );

  int zCViewDialogChoice::HandleEvent_Union( int key ) {
    if( key == KEY_ESCAPE ) {
      cmd << "close" << endl;
      RemoveAllChoices();

      return True;
    }

    return THISCALL( Ivk_zCViewDialogChoice_HandleEvent )( key );
  }*/
}