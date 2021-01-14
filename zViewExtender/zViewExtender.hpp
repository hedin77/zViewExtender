
namespace NAMESPACE {
#pragma region zCTexAniHandler  
  static Array<zCTexAniHandler*> arrTexAniHandlers;

  zCTexAniHandler::zCTexAniHandler() {
    pBaseTexture    = Null;
    ppTargetTexture = Null;
    nCurrentFrame   = 0;
    nAniFps         = 0;
    nAniSpeed       = 0;
    nAniChannel     = 0;
    bLooped         = True;
    bReversed       = False;
    bEnabled        = False;
    bEnabledOnPause = False;
    bPaused         = False;
    arrTexAniHandlers += this;
  }

  zCTexAniHandler::~zCTexAniHandler() {
    ResetAnimation();
    RemoveTexture();
    arrTexAniHandlers -= this;
  }

  zCTexture* zCTexAniHandler::GetTexture() {
    return pBaseTexture;
  }

  zCTexture* zCTexAniHandler::GetFrame() {
    return pBaseTexture->GetAniTexture();
  }

  int zCTexAniHandler::GetCurrentFrame() {
    return nCurrentFrame;
  }

  int zCTexAniHandler::GetNumFrames() {
    return pBaseTexture->numAniFrames[nAniChannel];
  }

  int zCTexAniHandler::GetAniFps() {
    return nAniFps;
  }

  int zCTexAniHandler::GetAniSpeed() {
    return nAniSpeed;
  }

  int zCTexAniHandler::GetAniChannel() {
    return nAniChannel;
  }

  bool32 zCTexAniHandler::IsLooped() {
    return bLooped;
  }

  bool32 zCTexAniHandler::IsReversed() {
    return bReversed;
  }

  bool32 zCTexAniHandler::IsEnabled() {
    return pBaseTexture && bEnabled;
  }

  bool32 zCTexAniHandler::IsEnabledOnPause() {
    return bEnabledOnPause;
  }

  bool32 zCTexAniHandler::IsPaused() {
    return bPaused;
  }

  bool32 zCTexAniHandler::IsAnimated() {
    return pBaseTexture && pBaseTexture->isAnimated;
  }

  bool32 zCTexAniHandler::HasTexture() {
    return pBaseTexture != Null;
  }

  void zCTexAniHandler::RemoveTexture() {
    if( pBaseTexture )
      pBaseTexture->Release();
  }

  void zCTexAniHandler::SetTexture( zCTexture** texture ) {
    RemoveTexture();

    ppTargetTexture = texture;
    if( texture ) {
      pBaseTexture = *texture;
      pBaseTexture->AddRef();
    }
    else
      pBaseTexture = Null;
  }

  void zCTexAniHandler::SetCurrentFrame( int frame ) {
    nCurrentFrame = frame;
    if( HasTexture() ) {
      pBaseTexture->actAniFrame[nAniChannel] = frame;
      *ppTargetTexture = pBaseTexture->GetAniTexture();
    }
  }

  void zCTexAniHandler::SetAniFps( int fps ) {
    nAniFps = fps;
    nAniSpeed = !fps ? 0 : 1000 / fps;
  }

  void zCTexAniHandler::SetAniSpeed( int speed ) {
    nAniSpeed = speed;
    nAniFps = 1000 / speed;
  }

  int zCTexAniHandler::GetAniTime() {
    return GetNumFrames() * nAniSpeed;
  }

  void zCTexAniHandler::SetAniChannel( int channel ) {
    nAniChannel = channel;
    if( HasTexture() ) {
      pBaseTexture->actAniFrame[channel] = nCurrentFrame;
      *ppTargetTexture = pBaseTexture->GetAniTexture();
    }
  }

  void zCTexAniHandler::SetLooped( bool32 looped ) {
    bLooped = looped;
  }

  void zCTexAniHandler::SetReversed( bool32 reversed ) {
    bReversed = reversed;
  }

  void zCTexAniHandler::SetEnabled( bool32 enabled ) {
    bEnabled = enabled;
  }

  void zCTexAniHandler::SetEnabledOnPause( bool32 enabled ) {
    bEnabledOnPause = enabled;
  }

  void zCTexAniHandler::SetPaused( bool32 paused ) {
    bPaused = paused;
  }

  void zCTexAniHandler::ResetAnimation() {
    s_mFrameTimer.Delete( (int)this );
    SetCurrentFrame( bReversed ? GetNumFrames() - 1 : 0 );
  }

  void zCTexAniHandler::NextFrame() {
    int frame = nCurrentFrame;

    if( !bReversed ) {
      if( ++frame >= GetNumFrames() ) {
        if( !bLooped ) {
          bEnabled = False;
          return;
        }
        frame = 0;
      }
      SetCurrentFrame( frame );
    }
    else {
      if( --frame < 0 ) {
        if( !bLooped ) {
          bEnabled = False;
          return;
        }
        frame = GetNumFrames() - 1;
      }
      SetCurrentFrame( frame );
    }
  }

  void zCTexAniHandler::DoFrame() {
    s_mFrameTimer.Attach();

    if( IsAnimated() && bEnabled ) {
      s_mFrameTimer.Suspend( 0, bPaused || ogame->IsOnPause() );

      if( !bPaused && !ogame->IsOnPause() && s_mFrameTimer( 0, nAniSpeed ) )
        NextFrame();
    }
  }
#pragma endregion


#pragma region zCViewAnimated
  zCViewAnimated::zCViewAnimated() : zCView() {
    Init();
  }

  zCViewAnimated::zCViewAnimated( int a0, int a1, int a2, int a3, zTviewID a4 ) : zCView( a0, a1, a2, a3, a4 ) {
    Init();
  }

  zCViewAnimated::~zCViewAnimated() {
    delete pAniHandler;
  }

  void zCViewAnimated::Init() {
    pAniHandler = new zCTexAniHandler();
    pAniHandler->SetAniFps( 25 );
    pAniHandler->SetAniChannel( 0 );
    pAniHandler->SetEnabled( True );
    pAniHandler->SetLooped( True );
    pAniHandler->SetEnabledOnPause( False );
  }

  bool32 zCViewAnimated::TextureIsAnimated() {
    return pAniHandler->IsAnimated();
  }

  bool32 zCViewAnimated::AnimationIsEnabled() {
    return pAniHandler->IsEnabled();
  }

  bool32 zCViewAnimated::AnimationIsEnabledOnPause() {
    return pAniHandler->IsEnabledOnPause();
  }

  bool32 zCViewAnimated::IsLooped() {
    return pAniHandler->IsLooped();
  }

  bool32 zCViewAnimated::IsOnPause() {
    return !pAniHandler->IsEnabled();
  }

  bool32 zCViewAnimated::IsReversed() {
    return pAniHandler->IsReversed();
  }

  int zCViewAnimated::GetFps() {
    return pAniHandler->GetAniFps();
  }

  int zCViewAnimated::GetSpeed() {
    return pAniHandler->GetAniSpeed();
  }

  int zCViewAnimated::GetFrame() {
    return pAniHandler->GetCurrentFrame();
  }

  int zCViewAnimated::GetFrameByTime( int time ) {
    int frame = time / GetSpeed();
    if( pAniHandler->IsReversed() )
      frame = pAniHandler->GetNumFrames() - frame;
    return frame;
  }

  int zCViewAnimated::GetFramesNum() {
    return pAniHandler->GetNumFrames();
  }

  int zCViewAnimated::GetAnimationTime() {
    return pAniHandler->GetAniTime();
  }

  void zCViewAnimated::SetAnimationEnabled( bool32 enabled ) {
    pAniHandler->SetEnabled( enabled );
  }

  void zCViewAnimated::SetAnimationEnabledOnPause( bool32 enabled ) {
    pAniHandler->SetEnabledOnPause( enabled );
  }

  void zCViewAnimated::SetFps( int fps ) {
    pAniHandler->SetAniFps( fps );
  }

  void zCViewAnimated::SetSpeed( int speed ) {
    pAniHandler->SetAniSpeed( speed );
  }

  void zCViewAnimated::SetLooped( bool32 looped ) {
    pAniHandler->SetLooped( looped );
  }

  void zCViewAnimated::SetReversed( bool32 reversed ) {
    pAniHandler->SetReversed( reversed );
  }

  void zCViewAnimated::SetFrame( int frame ) {
    pAniHandler->SetCurrentFrame( frame );
  }

  void zCViewAnimated::SetFrameByTime( int time ) {
    pAniHandler->SetCurrentFrame( GetFrameByTime( time ) );
  }

  void zCViewAnimated::Play() {
    pAniHandler->SetEnabled( True );
    pAniHandler->SetPaused( False );
  }

  void zCViewAnimated::Pause() {
    pAniHandler->SetPaused( True );
  }

  void zCViewAnimated::Stop() {
    pAniHandler->SetPaused( True );
    pAniHandler->ResetAnimation();
  }

  void zCViewAnimated::InsertBack( zCTexture* texture ) {
    zCView::InsertBack( texture );
    pAniHandler->SetTexture( &texture );
    pAniHandler->ResetAnimation();
  }

  void zCViewAnimated::InsertBack( const zSTRING& texname ) {
    zCView::InsertBack( texname );
    pAniHandler->SetTexture( &backTex );
    pAniHandler->ResetAnimation();
  }

  bool32 zCViewAnimated::IsOnBounds( int x, int y ) {
    return
      x > pposx && x < pposx + psizex &&
      y > pposy && y < pposy + psizey;
  }

  void zCViewAnimated::Blit() {
    if( pAniHandler )
      pAniHandler->DoFrame();

    zCView::Blit();
  }
#pragma endregion


#pragma region zCViewCursor

  HOOK Ivk_zCRnd_D3D_BeginFrame AS( ZCRND_D3D_BEGINFRAME, &zCRnd_D3D::BeginFrame_Union, HOOK_ENABLED );
  void zCRnd_D3D::BeginFrame_Union() {
    THISCALL( Ivk_zCRnd_D3D_BeginFrame )();
    if( screen )
      zCViewCursor::GetCursor()->FrameBegin();
  }
  
  HOOK Ivk_zCRnd_D3D_EndFrame AS( ZCRND_D3D_ENDFRAME, &zCRnd_D3D::EndFrame_Union, HOOK_ENABLED );
  void zCRnd_D3D::EndFrame_Union() {
    if( screen ) {
      zCViewCursor::GetCursor()->FrameEnd();
      zCViewCursor::GetCursor()->Render();
    }
    THISCALL( Ivk_zCRnd_D3D_BeginFrame )();
  }



  bool32 zCView::IsOnBounds( int x, int y ) {
    zCViewAnimated* animated = dynamic_cast<zCViewAnimated*>( this );
    if( animated )
      return animated->IsOnBounds( x, y );

    return
      x > pposx && x < pposx + psizex &&
      y > pposy && y < pposy + psizey;
  }



#if 0
  HOOK Ivk_zCWorld_Render AS( ZCWORLD_RENDER, &zCWorld::Render_Union, IvkEnabled( ENGINE ) );

  void zCWorld::Render_Union( zCCamera& camera ) {
    zCViewCursor::GetCursor()->FrameBegin();
    (this->*Ivk_zCWorld_Render)( camera );
    zCViewCursor::GetCursor()->FrameEnd();
  }



  HOOK Ivk_oCItemContainer_Container_Draw AS( OCITEMCONTAINER_CONTAINER_DRAW, &oCItemContainer::Container_Draw_Union, IvkEnabled( ENGINE ) );

  void oCItemContainer::Container_Draw_Union() {
    Ivk_oCItemContainer_Container_Draw();
    zCViewCursor::GetCursor()->Render();
  }
#endif






  zCViewCursorVisual::zCViewCursorVisual() : zCViewAnimated( 0, 0, 8192, 8192 ) {
    nActivePointX = 0;
    nActivePointY = 0;
  }

  void zCViewCursorVisual::SetActivePoint( const int& x, const int& y ) {
    nActivePointX = x;
    nActivePointY = y;
  }

  void zCViewCursorVisual::GetActivePoint( int& x, int& y ) {
    x = nActivePointX;
    y = nActivePointY;
  }












  static int s_nInterfaceScaleMultiplier = 1;
  


  void zCViewCursor::OnEnter() {
    zCViewInteractive* topInter = pTopSelectedViewInteractive;
    if( topInter && topInter->pEventEnter )
      topInter->pEventEnter( topInter, this );
  }
    


  void zCViewCursor::OnLeave() {
    zCViewInteractive* topInter = pTopSelectedViewInteractive;
    if( topInter && topInter->pEventLeave )
      topInter->pEventLeave( topInter, this );
  }



  void zCViewCursor::OnDown( zEMouseButton button ) {
    zCViewInteractive* topInter = pTopSelectedViewInteractive;
    if( topInter && topInter->pEventDown )
      topInter->pEventDown( topInter, this, button );
  }



  void zCViewCursor::OnDrag( int oldx, int oldy ) {
    if( !IsLeftPressed() && !IsMiddlePressed() && !IsRightPressed() )
      return;

    zCViewInteractive* topInter = pTopSelectedViewInteractive;
    if( topInter && topInter->pEventDrag ) {

      // move before calling for test
      int movx = zAnx( topInter->owner, zNax( screen, nPosX - oldx ) );
      int movy = zAny( topInter->owner, zNay( screen, nPosY - oldy ) );
      topInter->Move( movx, movy );

      // restore position after cancel
      if( !topInter->pEventDrag( topInter, this ) )
        topInter->Move( -movx, -movy );
    }
  }



  void zCViewCursor::OnUp( zEMouseButton button ) {
    zCViewInteractive* topInter = pTopSelectedViewInteractive;
    if( topInter && topInter->pEventUp )
      topInter->pEventUp( topInter, this, button );
  }



  zCViewCursor::zCViewCursor() : zCView() {
    pTopSelectedView = Null;
    pTopSelectedViewInteractive = Null;
    nVisualIndex = 0;
    nReferences = 0;
    pTopHandleView = Null;
  }



  zCViewCursor::~zCViewCursor() { }



  zCView* zCViewCursor::GetTopSelectedView() {
    return pTopSelectedView;
  }



  Array<zCView*>& zCViewCursor::GetTopSelectedCollection() {
    return arrSelectedCollection;
  }


  
  void zCViewCursor::UpdateKeyStates() {
    if( bLeftPressed && !bLeftToggled ) {
      bLeftToggled = True;
      OnDown( zBUTTON_LEFT );
    }

    if( bMiddlePressed && !bMiddleToggled ) {
      bMiddleToggled = True;
      OnDown( zBUTTON_MIDDLE );
    }

    if( bRightPressed && !bRightToggled ) {
      bRightToggled = True;
      OnDown( zBUTTON_RIGHT );
    }

    if( !bLeftPressed && bLeftToggled ) {
      bLeftToggled = False;
      OnUp( zBUTTON_LEFT );
    }

    if( !bMiddlePressed && bMiddleToggled ) {
      bMiddleToggled = False;
      OnUp( zBUTTON_MIDDLE );
    }

    if( !bRightPressed && bRightToggled ) {
      bRightToggled = False;
      OnUp( zBUTTON_RIGHT );
    }
  }
  


  void zCViewCursor::UpdateInput() {
    bLeftPressed   = zinput->GetMouseButtonPressedLeft();
    bMiddlePressed = zinput->GetMouseButtonPressedMid();
    bRightPressed  = zinput->GetMouseButtonPressedRight();
    UpdateKeyStates();
  }


  void zCViewCursor::ClearInput() {
    bLeftPressed   = False;
    bMiddlePressed = False;
    bRightPressed  = False;
    UpdateKeyStates();
  }


  
  void zCViewCursor::ClearSelection() {
    if( pTopSelectedViewInteractive ) {
      OnLeave();
      pTopSelectedViewInteractive = Null;
    }
    arrSelectedCollection.Clear();
  }



  void zCViewCursor::Collect( zCView* view ) {
    int x, y;
    GetActivePointPosition( x, y );
    if( view->IsOnBounds( x, y ) ) {
      arrSelectedCollection += view;
      pTopSelectedView = view;
    }
  }

  void zCViewCursor::GetActivePoint( int& px, int& py ) {
    zCViewCursorVisual* visual = GetActiveVisual();
    int ix = /*zPixelX(*/ visual->nActivePointX /*)*/ * s_nInterfaceScaleMultiplier;
    int iy = /*zPixelY(*/ visual->nActivePointY /*)*/ * s_nInterfaceScaleMultiplier;
    px = ix;
    py = iy;
  }

  void zCViewCursor::GetActivePointPosition( int& px, int& py ) {
    zCViewCursorVisual* visual = GetActiveVisual();
    int ix = /*zPixelX(*/ visual->nActivePointX /*)*/ * s_nInterfaceScaleMultiplier;
    int iy = /*zPixelY(*/ visual->nActivePointY /*)*/ * s_nInterfaceScaleMultiplier;
    px = nPosX + ix;
    py = nPosY + iy;
  }

  void zCViewCursor::GetCursorPosition( int& px, int& py ) {
    px = nPosX;
    py = nPosY;
  }

  void zCViewCursor::FrameBegin() {
    if( nReferences == 0 )
      return;

    UpdateInput();

    float sx = VID_MAX_XDIM;
    float sy = VID_MAX_YDIM;

    POINT realCursorPos;
    GetCursorPos( &realCursorPos );

    int nOldPosX = nPosX;
    int nOldPosY = nPosY;
    
    nPosX = int( 8192.0f / sx * float( realCursorPos.x ) );
    nPosY = int( 8192.0f / sy * float( realCursorPos.y ) );

    if( nPosX != nOldPosX || nPosY != nOldPosY )
      OnDrag( nOldPosX, nOldPosY );

    // ...
  }

  void zCViewCursor::FrameEnd() {
    if( nReferences == 0 )
      return;
    
    if( IsLeftPressed() || IsMiddlePressed() || IsRightPressed() )
      return;
       
    // 
    zCView* oldTopView = pTopSelectedView;
    
    // Clear selected views
    arrSelectedCollection.Clear();
    pTopSelectedView = Null;

    pTopSelectedView = GetTopHandleView()->GetTopView(
      screen->nax( nPosX ),
      screen->nay( nPosY )
      );

    if( pTopSelectedView == screen )
      pTopSelectedView = Null;

    bool_t isNewTop = oldTopView != pTopSelectedView;
    if( isNewTop )
    cmd << isNewTop << tab << AHEX32( oldTopView ) << tab << AHEX32( pTopSelectedView ) << endl;

    if( isNewTop ) {
      if( pTopSelectedViewInteractive ) {
        OnLeave();
      }
    }

    pTopSelectedViewInteractive = dynamic_cast<zCViewInteractive*>( pTopSelectedView );
    if( isNewTop )
      if( pTopSelectedViewInteractive )
        OnEnter();

    
  }

  void zCViewCursor::Render() {
    if( nReferences == 0 )
      return;

    screen->InsertItem( this );
    this->InsertItem( GetActiveVisual() );

    SetSize(
      zPixelX( 24 * s_nInterfaceScaleMultiplier ),
      zPixelY( 24 * s_nInterfaceScaleMultiplier )
      );

    int ox, oy;
    GetActivePoint( ox, oy );
    SetPos( nPosX - ox, nPosY - oy );
    GetActiveVisual()->Blit();

    this->RemoveItem( GetActiveVisual() );
    screen->RemoveItem( this );
  }

  void zCViewCursor::Show() {
    nReferences++;
  }

  void zCViewCursor::ShowAt( int x, int y ) {
    if( nReferences == 0 )
      SetCursorPos( x, y );

    Show();
  }

  void zCViewCursor::ShowAtCenter() {
    ShowAt( VID_MAX_XDIM / 2, VID_MAX_YDIM / 2 );
  }
  
  void zCViewCursor::Hide() {
    nReferences--;
    if( nReferences == Invalid )
      nReferences = 0;

    if( nReferences == 0 ) {
      ClearInput();
      ClearSelection();
    }
  }
  
  zCViewCursorVisual* zCViewCursor::GetActiveVisual() {
    if( nVisualIndex >= arrBodies.GetNum() )
      return GetDefaultVisual();

    return arrBodies[nVisualIndex];
  }
  
  zCViewCursorVisual* zCViewCursor::GetDefaultVisual() {
    static zCViewCursorVisual* s_visual = Null;
    if( !s_visual ) {
      s_visual = new zCViewCursorVisual();
      s_visual->InsertBack( "UNIONDEFAULTCURSOR" );
      s_visual->SetActivePoint( 3, 3 );
    }

    return s_visual;
  }

  // left mouse button
  bool_t zCViewCursor::IsLeftPressed() {
    return bLeftPressed;
  }

  bool_t zCViewCursor::IsLeftToggled() {
    return bLeftPressed && !bLeftToggled;
  }


  // middle mouse button
  bool_t zCViewCursor::IsMiddlePressed() {
    return bMiddlePressed;
  }

  bool_t zCViewCursor::IsMiddleToggled() {
    return bMiddlePressed && !bMiddleToggled;
  }


  // right mouse button
  bool_t zCViewCursor::IsRightPressed() {
    return bRightPressed;
  }

  bool_t zCViewCursor::IsRightToggled() {
    return bRightPressed && !bRightToggled;
  }




  zCViewCursor* zCViewCursor::GetCursor() {
    static zCViewCursor* cursor = Null;
    if( !cursor ) {
      cursor = new zCViewCursor();

      bool s_bInterfaceScaleEnabled;
      Union.GetSysPackOption().Read( s_bInterfaceScaleEnabled, "INTERFACE", "Scale", false );
      if( s_bInterfaceScaleEnabled )
        s_nInterfaceScaleMultiplier = 2;
    }

    return cursor;
  }

  void zCViewCursor::SetTopHandleView( zCView*& view ) {
    pTopHandleView = &view;
  }

  zCView*& zCViewCursor::GetTopHandleView() {
    if( pTopHandleView == Null )
      return screen;

    return *pTopHandleView;
  }

  void zCViewCursor::ClearTopHandleView() {
    pTopHandleView = Null;
  }






  


  zCViewInteractive::zCViewInteractive() : zCViewAnimated() {
    pEventEnter = Null;
    pEventLeave = Null;
    pEventDown  = Null;
    pEventDrag  = Null;
    pEventUp    = Null;
    pEventLoop  = Null;
  }

  zCViewInteractive::zCViewInteractive( int a0, int a1, int a2, int a3, zTviewID a4 ) : zCViewAnimated( a0, a1, a2, a3, a4 ) {
    pEventEnter = Null;
    pEventLeave = Null;
    pEventDown  = Null;
    pEventDrag  = Null;
    pEventUp    = Null;
    pEventLoop  = Null;
  }

  void zCViewInteractive::InitFunctions(
    zTViewInteractiveEventMovement enter,
    zTViewInteractiveEventMovement leave,
    zTViewInteractiveEventToggled  down,
    zTViewInteractiveEventMovement drag,
    zTViewInteractiveEventToggled  up,
    zTViewInteractiveEventMovement loop
  ) {
    pEventEnter = enter;
    pEventLeave = leave;
    pEventDown  = down;
    pEventDrag  = drag;
    pEventUp    = up;
    pEventLoop  = loop;
  }

  bool_t zCViewInteractive::IsSelected() {
    zCViewCursor* cursor = zCViewCursor::GetCursor();
    return cursor && cursor->GetTopSelectedView() == this;
  }

  bool_t zCViewInteractive::IsActive() {
    return bActive;
  }

  zCViewInteractive::~zCViewInteractive() {
    // TO DO remove from cursor view list
  }

#pragma endregion
}