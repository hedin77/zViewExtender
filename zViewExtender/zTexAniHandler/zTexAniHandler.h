
namespace NAMESPACE {
  class VIEWAPI zCTexAniHandler {
    Timer FrameTimer;
    zCTexture* BaseTexture;
    zCTexture** TargetTexture;
    int CurrentFrame;
    int AniFps;
    int AniSpeed;
    int AniChannel;
    bool_t Looped;
    bool_t Reversed;
    bool_t Enabled;
    bool_t EnabledOnPause;
    bool_t Paused;
  public:

    zCTexAniHandler();
    virtual ~zCTexAniHandler();

    zCTexture* GetTexture();
    zCTexture* GetFrame();
    bool_t IsLooped();
    bool_t IsReversed();
    bool_t IsEnabled();
    bool_t IsEnabledOnPause();
    bool_t IsPaused();
    bool_t IsAnimated();
    bool_t HasTexture();
    int GetCurrentFrame();
    int GetNumFrames();
    int GetAniFps();
    int GetAniSpeed();
    int GetAniTime();
    int GetAniChannel();

    void RemoveTexture();
    void SetTexture( zCTexture** texture );
    void SetCurrentFrame( int frame, bool considerReverse = true );
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
}