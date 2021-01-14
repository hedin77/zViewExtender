
namespace NAMESPACE {
  // <FIX ME> if texture is not used in World
  // material list, it resource can cause a
  // crash in Cache out functions. 
  static Array<zCTexture*>& GetAniTextureCache() {
    static Array<zCTexture*> cache;
    return cache;
  }

  static zCTexture* CreateAniTexture( const zSTRING& name ) {
    auto& cache = GetAniTextureCache();
    for each( zCTexture * tex in cache )
      if( name == tex->objectName )
        return tex;

    zCTexture* texture = zCTexture::Load( Z name, 0 );
    if( texture ) {
      texture->AddRef();
      cache += texture;
    }

    return texture;
  }
  // </FIX ME>








  zCTexture* LoadAnimatedTexture( const string& texName ) {
      return NULL;
  }



  zCViewAnimated::zCViewAnimated() : zCView() {
    Init();
  }



  zCViewAnimated::zCViewAnimated( int a0, int a1, int a2, int a3, zTviewID a4 ) : zCView( a0, a1, a2, a3, a4 ) {
    Init();
  }



  zCViewAnimated::~zCViewAnimated() {
    delete AniHandler;
  }



  void zCViewAnimated::Init() {
    AniHandler = new zCTexAniHandler();
    AniHandler->SetAniFps( 25 );
    AniHandler->SetAniChannel( 0 );
    AniHandler->SetEnabled( True );
    AniHandler->SetLooped( True );
    AniHandler->SetEnabledOnPause( False );
  }



  bool32 zCViewAnimated::TextureIsAnimated() {
    return AniHandler->IsAnimated();
  }



  bool32 zCViewAnimated::AnimationIsEnabled() {
    return AniHandler->IsEnabled();
  }



  bool32 zCViewAnimated::AnimationIsEnabledOnPause() {
    return AniHandler->IsEnabledOnPause();
  }



  bool32 zCViewAnimated::IsLooped() {
    return AniHandler->IsLooped();
  }



  bool32 zCViewAnimated::IsOnPause() {
    return AniHandler->IsPaused();
  }



  bool32 zCViewAnimated::IsPlayed() {
    return !IsOnPause() && AnimationIsEnabled();
  }



  bool32 zCViewAnimated::IsReversed() {
    return AniHandler->IsReversed();
  }



  int zCViewAnimated::GetFps() {
    return AniHandler->GetAniFps();
  }



  int zCViewAnimated::GetSpeed() {
    return AniHandler->GetAniSpeed();
  }



  int zCViewAnimated::GetFrame() {
    return AniHandler->GetCurrentFrame();
  }



  int zCViewAnimated::GetFrameByTime( int time ) {
    int frame = time / GetSpeed();
    if( AniHandler->IsReversed() )
      frame = AniHandler->GetNumFrames() - frame;
    return frame;
  }



  int zCViewAnimated::GetFramesNum() {
    return AniHandler->GetNumFrames();
  }



  int zCViewAnimated::GetAnimationTime() {
    return AniHandler->GetAniTime();
  }



  void zCViewAnimated::SetAnimationEnabled( bool32 enabled ) {
    AniHandler->SetEnabled( enabled );
  }



  void zCViewAnimated::SetAnimationEnabledOnPause( bool32 enabled ) {
    AniHandler->SetEnabledOnPause( enabled );
  }



  void zCViewAnimated::SetFps( int fps ) {
    AniHandler->SetAniFps( fps );
  }



  void zCViewAnimated::SetSpeed( int speed ) {
    AniHandler->SetAniSpeed( speed );
  }



  void zCViewAnimated::SetLooped( bool32 looped ) {
    AniHandler->SetLooped( looped );
  }



  void zCViewAnimated::SetReversed( bool32 reversed ) {
    AniHandler->SetReversed( reversed );
  }



  void zCViewAnimated::SetFrame( int frame, bool considerReverse ) {
    AniHandler->SetCurrentFrame( frame, considerReverse );
  }



  void zCViewAnimated::SetFrameByTime( int time, bool considerReverse ) {
    AniHandler->SetCurrentFrame( GetFrameByTime( time ), considerReverse );
  }



  void zCViewAnimated::Play() {
    AniHandler->SetEnabled( True );
    AniHandler->SetPaused( False );
  }



  void zCViewAnimated::Pause() {
    AniHandler->SetPaused( True );
  }



  void zCViewAnimated::Stop() {
    AniHandler->SetPaused( True );
    AniHandler->ResetAnimation();
  }


  void zCViewAnimated::InsertBack( zCTexture* texture ) {
    if( !texture )
      return;

    if( backTex ) {
      backTex->Release();
      backTex = Null;
    }

    backTex = texture;
    backTex->AddRef();

    AniHandler->SetTexture( &backTex );
    AniHandler->ResetAnimation();
  }



  void zCViewAnimated::InsertBack( const zSTRING& texname ) {
    zCTexture* texture = CreateAniTexture( texname );
    if( !texture )
      return;

    InsertBack( texture );
    texture->Release();
  }



  bool32 zCViewAnimated::IsOnBounds( int x, int y ) {
    return
      x > pposx && x < pposx + psizex &&
      y > pposy && y < pposy + psizey;
  }



  void zCViewAnimated::Blit() {
    if( AniHandler )
      AniHandler->DoFrame();

    zCView::Blit();
  }
}