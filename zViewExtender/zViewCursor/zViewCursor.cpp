
namespace NAMESPACE {



  zCViewCursorVisual::zCViewCursorVisual() : zCViewAnimated( 0, 0, 8192, 8192 ) {
    ActivePointX = 0;
    ActivePointY = 0;
  }



  void zCViewCursorVisual::SetActivePoint( const int& x, const int& y ) {
    ActivePointX = x;
    ActivePointY = y;
  }



  void zCViewCursorVisual::GetActivePoint( int& x, int& y ) {
    x = ActivePointX;
    y = ActivePointY;
  }












  static int psize = 36;
  


  void zCViewCursor::OnEnter() {
    if( TopSelectedViewInteractive && TopSelectedViewInteractive->EventEnter )
      TopSelectedViewInteractive->EventEnter( TopSelectedViewInteractive, this );
  }
    


  void zCViewCursor::OnLeave() {
    if( TopSelectedViewInteractive && TopSelectedViewInteractive->EventLeave )
      TopSelectedViewInteractive->EventLeave( TopSelectedViewInteractive, this );
  }



   void zCViewCursor::OnDown( zEMouseButton button ) {
    if( ActiveViewNote && ActiveViewNote != TopSelectedViewNote )
      ActiveViewNote->EditEnd();

    if( TopSelectedViewInteractive ) {
      if( TopSelectedViewInteractive->EventDown )
        TopSelectedViewInteractive->EventDown( TopSelectedViewInteractive, this, button );
    }
    else if( TopSelectedViewNote ) {

      TopSelectedViewNote->EditBegin();

      if( button == zBUTTON_LEFT ) {
        int x, y;
        GetCursorPosition( x, y );
        TopSelectedViewNote->SetCaretPosition( x, y );
      }
    }
  }



  void zCViewCursor::OnDrag( int oldx, int oldy ) {
    if( !IsLeftPressed() && !IsMiddlePressed() && !IsRightPressed() ) {
      DragObject = Null;
      return;
    }

    if( PosX == oldx && PosY == oldy )
      return;

    zCViewInteractive* topInter = TopSelectedViewInteractive;
    if( topInter ) {
      if( topInter->EventDrag ) {
        DragObject = topInter;

        // move before calling for test
        int movx = zAnx( topInter->owner, zNax( screen, PosX - oldx ) );
        int movy = zAny( topInter->owner, zNay( screen, PosY - oldy ) );
        topInter->Move( movx, movy );

        // restore position after cancel
        if( !topInter->EventDrag( topInter, this ) )
          topInter->Move( -movx, -movy );
      }

      return;
    }

    if( IsLeftPressed() ) {
      if( TopSelectedViewNote ) {
        int x, y;
        GetCursorPosition( x, y );
        TopSelectedViewNote->SetCaretPosition( x, y, true );
      }
    }
  }



  void zCViewCursor::OnUp( zEMouseButton button ) {
    zCViewInteractive* topInter = TopSelectedViewInteractive;
    if( topInter && topInter->EventUp )
      topInter->EventUp( topInter, this, button );
  }



  zCViewCursor::zCViewCursor() : zCView() {
    TopSelectedView = Null;
    TopSelectedViewInteractive = Null;
    TopSelectedViewNote = Null;
    DragObject = Null;
    References = 0;
    HandledView = Null;
    DefaultVisualIndex = 0;
    VisualIndex = 0;
  }



  zCViewCursor::~zCViewCursor() { }



  zCView* zCViewCursor::GetTopSelectedView() {
    return TopSelectedView;
  }



  Array<zCView*>& zCViewCursor::GetTopSelectedCollection() {
    return SelectedCollection;
  }



  zCViewInteractive* zCViewCursor::GetDragObject() {
    return DragObject;
  }


  void zCViewCursor::UpdateKeyStates() {

    if(zMouseKeyPressed(Left)) {
        if( !LeftPressed ) {
        LeftPressed = True;
        LeftToggled = True;
        OnDown( zBUTTON_LEFT );
      }
      else if( LeftToggled )
        LeftToggled = False;
    }
    else {
      if( LeftPressed ) {
        LeftPressed = False;
        LeftToggled = False;
        OnUp( zBUTTON_LEFT );
      }
    }

    if( zMouseKeyPressed( Mid )) {
      if( !MiddlePressed ) {
        MiddlePressed = True;
        MiddleToggled = True;
        OnDown( zBUTTON_MIDDLE );
      }
      else if( MiddleToggled )
        MiddleToggled = False;
    }
    else {
      if( MiddlePressed ) {
        MiddlePressed = False;
        MiddleToggled = False;
        OnUp( zBUTTON_MIDDLE );
      }
    }

    if( zMouseKeyPressed( Right )) {
      if( !RightPressed ) {
        RightPressed = True;
        RightToggled = True;
        OnDown( zBUTTON_RIGHT );
      }
      else if( RightToggled )
        RightToggled = False;
    }
    else {
      if( RightPressed ) {
        RightPressed = False;
        RightToggled = False;
        OnUp( zBUTTON_RIGHT );
      }
    }
  }
  


  void zCViewCursor::UpdateInput() {
    UpdateKeyStates();
  }



  void zCViewCursor::ClearInput() {
    if( LeftPressed )   OnUp( zBUTTON_LEFT );
    if( MiddlePressed ) OnUp( zBUTTON_MIDDLE );
    if( RightPressed )  OnUp( zBUTTON_RIGHT );

    LeftPressed   = False;
    LeftToggled   = False;
    MiddlePressed = False;
    MiddleToggled = False;
    RightPressed  = False;
    RightToggled  = False;
  }


  
  void zCViewCursor::ClearSelection() {
    if( TopSelectedViewInteractive ) {
      OnLeave();
      TopSelectedViewInteractive = Null;
    }

    if( TopSelectedViewNote ) {
      TopSelectedViewNote->EditEnd();
      TopSelectedViewNote = Null;
    }

    TopSelectedView = Null;
    SelectedCollection.Clear();
  }



  void zCViewCursor::Collect( zCView* view ) {
    int x, y;
    GetActivePointPosition( x, y );
    if( view->IsOnBounds( x, y ) ) {
      SelectedCollection += view;
      TopSelectedView = view;
    }
  }



  void zCViewCursor::GetActivePoint( int& px, int& py ) {
    zCViewCursorVisual* visual = GetActiveVisual();
    px = visual->ActivePointX;
    py = visual->ActivePointY;
  }



  void zCViewCursor::GetActivePointPosition( int& px, int& py ) {
    zCViewCursorVisual* visual = GetActiveVisual();
    zVEC2 viewScale            = GetVirtualScale();

    px = PosX - (int)((float)(visual->ActivePointX) * viewScale[VX]);
    py = PosY - (int)((float)(visual->ActivePointY) * viewScale[VY]);
  }



  void zCViewCursor::GetCursorPosition( int& px, int& py ) {
    px = PosX;
    py = PosY;
  }

  void zCViewCursor::FrameBegin() {
    if( References == 0 )
      return;

    UpdateInput();

    RECT clipCursor;
    GetClipCursor( &clipCursor );
    float sx = float(clipCursor.right  - clipCursor.left);
    float sy = float(clipCursor.bottom - clipCursor.top);

    POINT realCursorPos;
    GetCursorPos( &realCursorPos );

    int nOldPosX = PosX;
    int nOldPosY = PosY;
    
    PosX = int( 8192.0f / sx * float( realCursorPos.x - clipCursor.left ) );
    PosY = int( 8192.0f / sy * float( realCursorPos.y - clipCursor.top  ) );

    OnDrag( nOldPosX, nOldPosY );

    // ...
  }



  void zCViewCursor::FrameEnd() {
    if( References == 0 )
      return;

    if (IsLeftPressed() || IsMiddlePressed() || IsRightPressed())
      return;
       
    // 
    zCView* oldTopView = TopSelectedView;
    
    // Pre-render cleaning
    SelectedCollection.Clear();
    TopSelectedView = Null;

    Array<zCView*> selectedViews = GetHandledView()->GetTopViewList(
      screen->nax( PosX ),
      screen->nay( PosY )
      );

    uint index      = selectedViews.GetNum() - 1;
    TopSelectedView = Null;

    index = selectedViews.GetNum() - 1;
    zCView* lastInteractive = NULL;
    
    while (index != Invalid) {

        if (selectedViews[index] != screen) {
            if (dynamic_cast<zCViewInteractive*>(selectedViews[index])) {
                TopSelectedView = dynamic_cast<zCViewInteractive*>(selectedViews[index]);
            }
        }
        index--;
    }
    
    if (!TopSelectedView) {
        index = selectedViews.GetNum() - 1;
        while (index != Invalid) {
            TopSelectedView = selectedViews[index] != screen ? selectedViews[index] : Null;
            if (selectedViews[index]) {
               // cmd << Z "FrameEnd case usual  " << endl;
                break;
            }
            index--;
        }
    }

    bool isNewTop = oldTopView != TopSelectedView;

    if( isNewTop )
      if( TopSelectedViewInteractive )
        OnLeave();

    TopSelectedViewInteractive = dynamic_cast<zCViewInteractive*>(TopSelectedView);
    TopSelectedViewNote = dynamic_cast<zCViewNote*>(TopSelectedView);
    if( isNewTop )
      if( TopSelectedViewInteractive )
        OnEnter();




#if 0
    TopSelectedView = GetHandledView()->GetTopView(
      screen->nax( PosX ),
      screen->nay( PosY )
      );

    if( TopSelectedView == screen )
      TopSelectedView = Null;

    bool_t isNewTop = oldTopView != TopSelectedView;

    if( isNewTop )
      if( TopSelectedViewInteractive )
        OnLeave();

    TopSelectedViewInteractive = dynamic_cast<zCViewInteractive*>( TopSelectedView );
    TopSelectedViewNote = dynamic_cast<zCViewNote*>( TopSelectedView );
    if( isNewTop )
      if( TopSelectedViewInteractive )
        OnEnter();
#endif
  }



  void zCViewCursor::UpdateVisual() {
    if( TopSelectedViewInteractive && TopSelectedViewInteractive->CursorVisualIndex != Invalid ) {
      if( VisualIndex != TopSelectedViewInteractive->CursorVisualIndex )
        VisualIndex = TopSelectedViewInteractive->CursorVisualIndex;
      return;
    }

    if( TopSelectedViewNote && TopSelectedViewNote->CursorVisualIndex != Invalid ) {
      if( VisualIndex != TopSelectedViewNote->CursorVisualIndex )
        VisualIndex = TopSelectedViewNote->CursorVisualIndex;
      return;
    }
    
    if( VisualIndex != DefaultVisualIndex )
      VisualIndex = DefaultVisualIndex;
  }



  void zCViewCursor::UpdateRect() {
    if( GetForegroundWindow() == hWndApp ) {
      RECT rect;
      GetWindowRect( hWndApp, &rect );
      ClipCursor( &rect );
    }
  }



  void zCViewCursor::Render( bool blit ) {
    if( References == 0 ) {
      screen->RemoveItem( this );
      return;
    }

    // Update cursor image and
    // cursor movement range
    UpdateVisual();
    UpdateRect();

    // Place cursor in
    // viewport before blit
    if( blit )
      ogame->viewport->InsertItem( this );

    // Remove old cursor picture and
    // place new cursor picture
    static zCViewCursorVisual* lastVisual = Null;
    if( lastVisual )
      RemoveItem( lastVisual );

    lastVisual = GetActiveVisual();
    InsertItem( lastVisual );

    // Update cursor size and
    // cursor picture position
    SetSize(zPixelX(psize), zPixelY(psize));

    int ox, oy;
    GetActivePointPosition( ox, oy );
    SetPos( ox, oy );

    // Blit picture
    if( blit ) {
      lastVisual->Blit();
      ogame->viewport->RemoveItem( this );
      RemoveItem( lastVisual );
    }

    // Post-render cleaning
    SelectedCollection.Clear();
  }



  void zCViewCursor::Show() {
    References++;
  }



  void zCViewCursor::ShowAt( int x, int y ) {
    if( References == 0 )
      SetCursorPos( x, y );

    Show();
  }



  void zCViewCursor::ShowAtCenter() {
    ShowAt( VID_MAX_XDIM / 2, VID_MAX_YDIM / 2 );
  }


  
  void zCViewCursor::Hide() {
    References--;
    if( References == Invalid )
      References = 0;

    if( References == 0 ) {
      ClearInput();
      ClearSelection();
    }
  }



  uint zCViewCursor::CreateVisual( const zSTRING& texName, const zVEC2& activePoint, const int& aniFps ) {
    GetDefaultVisual();

    zCViewCursorVisual* visual = new zCViewCursorVisual();
    visual->InsertBack( texName );
    visual->SetActivePoint( (int)activePoint[VX], (int)activePoint[VY] );

    if( aniFps ) {
      visual->SetAnimationEnabled( True );
      visual->SetFps( aniFps );
    }

    uint index = Visuals.GetNum();
    Visuals += visual;
    return index;
  }
  


  zCViewCursorVisual* zCViewCursor::GetActiveVisual() {
    if( VisualIndex >= Visuals.GetNum() )
      return GetDefaultVisual();

    return Visuals[VisualIndex];
  }
  


  zCViewCursorVisual* zCViewCursor::GetDefaultVisual() {
    static zCViewCursorVisual* s_visual = Null;
    if( !s_visual ) {
      s_visual = new zCViewCursorVisual();
      s_visual->InsertBack( "UnionCursorDefault" );
      s_visual->SetActivePoint( 100, 100 );
      Visuals += s_visual;
    }

    return s_visual;
  }



  bool_t zCViewCursor::IsLeftPressed() {
    return LeftPressed;
  }



  bool_t zCViewCursor::IsLeftToggled() {
    return LeftPressed && LeftToggled;
  }



  bool_t zCViewCursor::IsMiddlePressed() {
    return MiddlePressed;
  }



  bool_t zCViewCursor::IsMiddleToggled() {
    return MiddlePressed && MiddleToggled;
  }



  bool_t zCViewCursor::IsRightPressed() {
    return RightPressed;
  }



  bool_t zCViewCursor::IsRightToggled() {
    return RightPressed && RightToggled;
  }



  inline zCViewCursor* CreateCursor() {
    zCViewCursor* cursor = new zCViewCursor();
    float scale;
    Union.GetSysPackOption().Read(scale, "INTERFACE", "Scale");
    if (scale == 0.0)
        scale = 1; 
    else if (scale > 2)
        scale = 2;
    psize = static_cast<int>(36.0 * scale);
    return cursor;
  }



  zCViewCursor* zCViewCursor::GetCursor() {
    static zCViewCursor* cursor = CreateCursor();
    return cursor;
  }



  void zCViewCursor::SetHandledView( zCView*& view ) {
    HandledView = &view;
  }



  zCView*& zCViewCursor::GetHandledView() {
    if( HandledView == Null )
      return screen;

    return *HandledView;
  }



  void zCViewCursor::ClearHandledView() {
    HandledView = Null;
  }
}