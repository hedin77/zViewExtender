
namespace NAMESPACE {
  class VIEWAPI zCViewAnimated : public zCView {
    zCTexAniHandler* AniHandler;
  protected:

    friend class zCView;
    virtual bool_t IsOnBounds( int x, int y );
  public:

    zCViewAnimated();
    zCViewAnimated( int a0, int a1, int a2, int a3, zTviewID a4 = zTviewID::VIEW_ITEM );
    virtual ~zCViewAnimated();
    void Init();

    bool_t TextureIsAnimated();
    bool_t AnimationIsEnabled();
    bool_t AnimationIsEnabledOnPause();
    bool_t IsLooped();
    bool_t IsOnPause();
    bool_t IsPlayed();
    bool_t IsReversed();
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
    void SetFrame( int frame, bool considerReverse = true );
    void SetFrameByTime( int time, bool considerReverse = true );
    void Play();
    void Pause();
    void Stop();

    void InsertBack( zCTexture* texture );
    void InsertBack( const zSTRING& texname );
    virtual void Blit();
  };
}