
namespace NAMESPACE {
  zCViewInteractive::zCViewInteractive() : zCViewAnimated() {
    EventEnter = Null;
    EventLeave = Null;
    EventDown = Null;
    EventDrag = Null;
    EventUp = Null;
    EventLoop = Null;
    CursorVisualIndex = Invalid;
  }


  zCViewInteractive::zCViewInteractive( int a0, int a1, int a2, int a3, zTviewID a4 ) : zCViewAnimated( a0, a1, a2, a3, a4 ) {
    EventEnter = Null;
    EventLeave = Null;
    EventDown = Null;
    EventDrag = Null;
    EventUp = Null;
    EventLoop = Null;
    CursorVisualIndex = Invalid;
  }


  void zCViewInteractive::InitFunctions(
    zTViewInteractiveEventMovement enter,
    zTViewInteractiveEventMovement leave,
    zTViewInteractiveEventToggled  down,
    zTViewInteractiveEventMovement drag,
    zTViewInteractiveEventToggled  up,
    zTViewInteractiveEventMovement loop
    ) {
    EventEnter = enter;
    EventLeave = leave;
    EventDown = down;
    EventDrag = drag;
    EventUp = up;
    EventLoop = loop;
  }


  void zCViewInteractive::SetColor( const zCOLOR& color ) {
    if( dynamic_cast<zCViewShaped*>(this) )
      ((zCViewShaped*)this)->SetColor( color );
    else
      zCViewAnimated::SetColor( color );
  }


  bool_t zCViewInteractive::IsSelected() {
    zCViewCursor* cursor = zCViewCursor::GetCursor();
    return cursor && cursor->GetTopSelectedView() == this;
  }


  void zCViewInteractive::DoLoopedEvent() {
    if( EventLoop ) {
      zCViewCursor* cursor = zCViewCursor::GetCursor();
      EventLoop( this, cursor->GetTopSelectedView() == this ? cursor : Null );
    }
  }


  void zCViewInteractive::Blit() {
    DoLoopedEvent();
    zCViewAnimated::Blit();
  }


  zCViewInteractive::~zCViewInteractive() {
    zCViewCursor* cur = zCViewCursor::GetCursor();
    if( cur->SelectedCollection.HasEqual( this ) ) {
      cur->SelectedCollection.Clear();
      cur->SetHandledView( screen );
    }
  }
}