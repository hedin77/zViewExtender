
namespace NAMESPACE {
  static Array<zCTexAniHandler*>& GetTexAniHandlers() {
    static Array<zCTexAniHandler*> TexAniHandlers;
    return TexAniHandlers;
  }



  zCTexAniHandler::zCTexAniHandler() {
    BaseTexture    = Null;
    TargetTexture  = Null;
    CurrentFrame   = 0;
    AniFps         = 0;
    AniSpeed       = 0;
    AniChannel     = 0;
    Looped         = True;
    Reversed       = False;
    Enabled        = False;
    EnabledOnPause = False;
    Paused         = False;
    GetTexAniHandlers() += this;
  }



  zCTexAniHandler::~zCTexAniHandler() {
    ResetAnimation();
    RemoveTexture();
    GetTexAniHandlers() -= this;
  }



  zCTexture* zCTexAniHandler::GetTexture() {
    return BaseTexture;
  }



  zCTexture* zCTexAniHandler::GetFrame() {
    return BaseTexture->GetAniTexture();
  }



  int zCTexAniHandler::GetCurrentFrame() {
    return CurrentFrame;
  }



  int zCTexAniHandler::GetNumFrames() {
    return BaseTexture->numAniFrames[AniChannel];
  }



  int zCTexAniHandler::GetAniFps() {
    return AniFps;
  }



  int zCTexAniHandler::GetAniSpeed() {
    return AniSpeed;
  }



  int zCTexAniHandler::GetAniChannel() {
    return AniChannel;
  }



  bool32 zCTexAniHandler::IsLooped() {
    return Looped;
  }



  bool32 zCTexAniHandler::IsReversed() {
    return Reversed;
  }



  bool32 zCTexAniHandler::IsEnabled() {
    return BaseTexture && Enabled;
  }



  bool32 zCTexAniHandler::IsEnabledOnPause() {
    return EnabledOnPause;
  }



  bool32 zCTexAniHandler::IsPaused() {
    return Paused;
  }



  bool32 zCTexAniHandler::IsAnimated() {
    return BaseTexture && BaseTexture->isAnimated;
  }



  bool32 zCTexAniHandler::HasTexture() {
    return BaseTexture != Null;
  }



  void zCTexAniHandler::RemoveTexture() {
    if( TargetTexture && BaseTexture ) {
      BaseTexture->Release();
      TargetTexture = Null;
      BaseTexture = Null;
    }
  }



  void zCTexAniHandler::SetTexture( zCTexture** texture ) {
    RemoveTexture();

    TargetTexture = texture;
    if( texture && *texture ) {
      BaseTexture = *texture;
      BaseTexture->AddRef();
    }
    else
      BaseTexture = Null;
  }



  void zCTexAniHandler::SetCurrentFrame( int frame, bool considerReverse ) {
    if( GetNumFrames() == 0 )
      return;

    if( Reversed && !considerReverse )
      frame = GetNumFrames() - frame;

    if( frame < 0 ) {
      frame = Looped ?
        GetNumFrames() - 1 :
        0;
    }
    else if( frame >= GetNumFrames() ) {
      frame = Looped ?
        0 :
        GetNumFrames() - 1;
    }

    CurrentFrame = frame;
    if( HasTexture() ) {
      BaseTexture->actAniFrame[AniChannel] = frame;

      // FIXME: Первый кадр (то бишь статический A0) может
      // неправильно отображаться на любом zCView объекте.
      *TargetTexture = BaseTexture->GetAniTexture();
    }
  }



  void zCTexAniHandler::SetAniFps( int fps ) {
    AniFps = fps;
    AniSpeed = !fps ? 0 : 1000 / fps;
  }



  void zCTexAniHandler::SetAniSpeed( int speed ) {
    AniSpeed = speed;
    AniFps = 1000 / speed;
  }



  int zCTexAniHandler::GetAniTime() {
    return GetNumFrames() * AniSpeed;
  }



  void zCTexAniHandler::SetAniChannel( int channel ) {
    AniChannel = channel;
    if( HasTexture() ) {
      BaseTexture->actAniFrame[channel] = CurrentFrame;
      *TargetTexture = BaseTexture->GetAniTexture();
    }
  }



  void zCTexAniHandler::SetLooped( bool32 looped ) {
    Looped = looped;
  }



  void zCTexAniHandler::SetReversed( bool32 reversed ) {
    Reversed = reversed;
  }



  void zCTexAniHandler::SetEnabled( bool32 enabled ) {
    Enabled = enabled;
  }



  void zCTexAniHandler::SetEnabledOnPause( bool32 enabled ) {
    EnabledOnPause = enabled;
  }



  void zCTexAniHandler::SetPaused( bool32 paused ) {
    Paused = paused;
  }



  void zCTexAniHandler::ResetAnimation() {
    FrameTimer[this].Delete();
    SetCurrentFrame( Reversed ? GetNumFrames() - 1 : 0 );
  }



  void zCTexAniHandler::NextFrame() {
    int frame = CurrentFrame;

    if( !Reversed ) {
      if( ++frame >= GetNumFrames() ) {
        if( !Looped ) {
          Enabled = False;
          return;
        }

        frame = 0;
      }

      SetCurrentFrame( frame );
    }
    else {
      if( --frame < 0 ) {
        if( !Looped ) {
          Enabled = False;
          return;
        }

        frame = GetNumFrames() - 1;
      }

      SetCurrentFrame( frame );
    }
  }



  void zCTexAniHandler::DoFrame() {
    FrameTimer.ClearUnused();

    if( IsAnimated() && Enabled ) {
      FrameTimer[0U].Suspend( Paused || ogame->IsOnPause() );

      if( !Paused && !ogame->IsOnPause() && FrameTimer[0U].Await( AniSpeed ) )
        NextFrame();
    }
  }
}