
namespace NAMESPACE {

  class zCTexAniHandler {
    CTimer s_mFrameTimer;
    zCTexture* pBaseTexture;
    zCTexture** ppTargetTexture;
    int nCurrentFrame;
    int nAniFps;
    int nAniSpeed;
    int nAniChannel;
    bool32 bLooped;
    bool32 bReversed;
    bool32 bEnabled;
    bool32 bEnabledOnPause;
    bool32 bPaused;
  public:

    zCTexAniHandler();
    virtual ~zCTexAniHandler();

    zCTexture* GetTexture();
    zCTexture* GetFrame();
    bool32 IsLooped();
    bool32 IsReversed();
    bool32 IsEnabled();
    bool32 IsEnabledOnPause();
    bool32 IsPaused();
    bool32 IsAnimated();
    bool32 HasTexture();
    int GetCurrentFrame();
    int GetNumFrames();
    int GetAniFps();
    int GetAniSpeed();
    int GetAniTime();
    int GetAniChannel();

    void RemoveTexture();
    void SetTexture( zCTexture** texture );
    void SetCurrentFrame( int frame );
    void SetAniFps( int fps );
    void SetAniSpeed( int speed );
    void SetAniChannel( int channel );
    void SetLooped( bool32 looped );
    void SetReversed( bool32 reversed );
    void SetEnabled( bool32 enabled );
    void SetEnabledOnPause( bool32 enabled );
    void SetPaused( bool32 paused );
    void ResetAnimation();
    void NextFrame();
    void DoFrame();
  };





  class zCViewAnimated : public zCView {
    zCTexAniHandler* pAniHandler;
  protected:

    friend class zCView;
    virtual bool32 IsOnBounds( int x, int y );
  public:

    zCViewAnimated();
    zCViewAnimated( int a0, int a1, int a2, int a3, zTviewID a4 = zTviewID::VIEW_ITEM );
    virtual ~zCViewAnimated();
    void Init();

    bool32 TextureIsAnimated();
    bool32 AnimationIsEnabled();
    bool32 AnimationIsEnabledOnPause();
    bool32 IsLooped();
    bool32 IsOnPause();
    bool32 IsReversed();
    int GetFps();
    int GetSpeed();
    int GetFrame();
    int GetFrameByTime( int time );
    int GetFramesNum();
    int GetAnimationTime();

    void SetAnimationEnabled( bool32 enabled );
    void SetAnimationEnabledOnPause( bool32 enabled );
    void SetFps( int fps );
    void SetSpeed( int speed );
    void SetLooped( bool32 looped );
    void SetReversed( bool32 reversed );
    void SetFrame( int frame );
    void SetFrameByTime( int time );
    void Play();
    void Pause();
    void Stop();

    void InsertBack( zCTexture* texture );
    void InsertBack( const zSTRING& texname );
    virtual void Blit();
  };




  
  enum zEMouseButton {
    zBUTTON_LEFT,
    zBUTTON_MIDDLE,
    zBUTTON_RIGHT
  };
  
  class zCViewInteractive;

  class zCViewCursorVisual : public zCViewAnimated {
    friend class zCViewCursor;
  protected:
    int nActivePointX;
    int nActivePointY;
  public:

    zCViewCursorVisual();
    void SetActivePoint( const int& x, const int& y );
    void GetActivePoint( int& x, int& y );
  };

  class zCViewCursor : public zCView {
    Array<zCViewCursorVisual*> arrBodies;
    Array<zCView*> arrSelectedCollection;
    zCView* pTopSelectedView;
    zCViewInteractive* pTopSelectedViewInteractive;
    uint nVisualIndex;
    int nPosX;
    int nPosY;
    uint nReferences;
    bool_t bLeftPressed,   bLeftToggled;
    bool_t bMiddlePressed, bMiddleToggled;
    bool_t bRightPressed,  bRightToggled;
    zCView** pTopHandleView;

    void OnEnter();
    void OnLeave();
    void OnDown( zEMouseButton button );
    void OnDrag( int oldx, int oldy );
    void OnUp( zEMouseButton button );
  public:

    zCViewCursor();
    virtual ~zCViewCursor();

    void UpdateKeyStates();
    void UpdateInput();
    void ClearInput();
    void ClearSelection();
    void Collect( zCView* view );
    void GetActivePoint( int& px, int& py );
    void GetActivePointPosition( int& px, int& py );
    void GetCursorPosition( int& px, int& py );
    void FrameBegin();
    void FrameEnd();
    void Render();
    void Show();
    void ShowAt( int x, int y );
    void ShowAtCenter();
    void Hide();
    zCViewCursorVisual* GetActiveVisual();
    zCViewCursorVisual* GetDefaultVisual();

    //
    void     SetTopHandleView( zCView*& view );
    zCView*& GetTopHandleView();
    void     ClearTopHandleView();


    zCView* GetTopSelectedView();
    Array<zCView*>& GetTopSelectedCollection();

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





  typedef bool( __cdecl* zTViewInteractiveEventMovement )( zCViewInteractive*, zCViewCursor* );
  typedef bool( __cdecl* zTViewInteractiveEventToggled )( zCViewInteractive*, zCViewCursor*, zEMouseButton );

  class zCViewInteractive : public zCViewAnimated {
    friend class zCViewCursor;
  protected:
    bool_t bActive;

    zTViewInteractiveEventMovement pEventEnter;
    zTViewInteractiveEventMovement pEventLeave;
    zTViewInteractiveEventToggled  pEventDown;
    zTViewInteractiveEventMovement pEventDrag;
    zTViewInteractiveEventToggled  pEventUp;
    zTViewInteractiveEventMovement pEventLoop;
  public:

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

    bool_t IsSelected();
    bool_t IsActive();

    virtual ~zCViewInteractive();
  };
  




  struct zTVec2_Feature {
    zVEC2 coord;
    zVEC2 uv;
  };
  
  class zCViewPoly : public zCViewAnimated {
    Array<zTVec2_Feature> arrFeature;
  public:

  };
}