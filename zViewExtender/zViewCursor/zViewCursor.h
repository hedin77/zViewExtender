
namespace NAMESPACE {
  enum zEMouseButton {
    zBUTTON_LEFT,
    zBUTTON_MIDDLE,
    zBUTTON_RIGHT
  };
  
  class zCViewInteractive;
  class zCViewNote;

  class VIEWAPI zCViewCursorVisual : public zCViewAnimated {
    friend class zCViewCursor;
  protected:
    int ActivePointX;
    int ActivePointY;
  public:

    zCViewCursorVisual();
    void SetActivePoint( const int& x, const int& y );
    void GetActivePoint( int& x, int& y );
  };

  class VIEWAPI zCViewCursor : public zCView {
    friend class zCViewInteractive;
    friend void Game_PreLoop();
    friend void Game_PostLoop();
    friend void Game_MenuLoop();

  protected:
    Array<zCViewCursorVisual*> Visuals;
    Array<zCView*> SelectedCollection;
    zCView* TopSelectedView;
    zCViewInteractive* TopSelectedViewInteractive;
    zCViewNote* TopSelectedViewNote;
    int PosX;
    int PosY;
    uint References;
    bool_t LeftPressed,   LeftToggled;
    bool_t MiddlePressed, MiddleToggled;
    bool_t RightPressed,  RightToggled;
    zCView** HandledView;
    zCViewInteractive* DragObject;

    void OnEnter();
    void OnLeave();
    void OnDown( zEMouseButton button );
    void OnDrag( int oldx, int oldy );
    void OnUp( zEMouseButton button );
    void Collect( zCView* view );
    void FrameBegin();
    void FrameEnd();
    void UpdateVisual();
    void UpdateRect();
    void Render( bool blit = true );
  public:

    uint DefaultVisualIndex;
    uint VisualIndex;

  public:
    zCViewCursor();
    virtual ~zCViewCursor();

    void UpdateKeyStates();
    void UpdateInput();
    void ClearInput();
    void ClearSelection();
    void GetActivePoint( int& px, int& py );
    void GetActivePointPosition( int& px, int& py );
    void GetCursorPosition( int& px, int& py );
    void Show();
    void ShowAt( int x, int y );
    void ShowAtCenter();
    void Hide();
    uint CreateVisual( const zSTRING& texName, const zVEC2& activePoint, const int& aniFps = 0 );
    zCViewCursorVisual* GetActiveVisual();
    zCViewCursorVisual* GetDefaultVisual();

    //
    void     SetHandledView( zCView*& view );
    zCView*& GetHandledView();
    void     ClearHandledView();


    zCView* GetTopSelectedView();
    Array<zCView*>& GetTopSelectedCollection();
    zCViewInteractive* GetDragObject();

    // left mouse button
    bool_t IsLeftPressed();
    bool_t IsLeftToggled();

    // middle mouse button
    bool_t IsMiddlePressed();
    bool_t IsMiddleToggled();

    // right mouse button
    bool_t IsRightPressed();
    bool_t IsRightToggled();

    static zCViewCursor* GetCursor();
  };
}