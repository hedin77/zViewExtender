
namespace NAMESPACE {
  typedef bool( __cdecl* zTViewInteractiveEventMovement )( zCViewInteractive*, zCViewCursor* );
  typedef bool( __cdecl* zTViewInteractiveEventToggled )( zCViewInteractive*, zCViewCursor*, zEMouseButton );

  class VIEWAPI zCViewInteractive : public zCViewAnimated {
    friend class zCViewCursor;
  protected:
    void DoLoopedEvent();

  public:
    group {
      uint CursorVisualIndex : 16;
      bool_t Selectable : 16;
    };

    zTViewInteractiveEventMovement EventEnter;
    zTViewInteractiveEventMovement EventLeave;
    zTViewInteractiveEventToggled  EventDown;
    zTViewInteractiveEventMovement EventDrag;
    zTViewInteractiveEventToggled  EventUp;
    zTViewInteractiveEventMovement EventLoop;

    zCViewInteractive();
    zCViewInteractive( int a0, int a1, int a2, int a3, zTviewID a4 = zTviewID::VIEW_ITEM );
    void InitFunctions(
      zTViewInteractiveEventMovement enter,
      zTViewInteractiveEventMovement leave,
      zTViewInteractiveEventToggled  down,
      zTViewInteractiveEventMovement drag,
      zTViewInteractiveEventToggled  up,
      zTViewInteractiveEventMovement loop
      );

    void SetColor( const zCOLOR& color );
    bool_t IsSelected();
    virtual void Blit();
    virtual ~zCViewInteractive();
  };
}