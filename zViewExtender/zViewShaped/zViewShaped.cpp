// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static const zVEC2 s_SingleUV = 1.0f / 8192.0;



  bool Alg_FindIntersection2D( const zVEC2& a1, const zVEC2& b1, const zVEC2& a2, const zVEC2& b2, zVEC2& intersection ) {
    float x1 = b1[0] - a1[0];
    float y1 = b1[1] - a1[1];
    float x2 = b2[0] - a2[0];
    float y2 = b2[1] - a2[1];

    float denom = x1 * y2 - x2 * y1;
    if( denom == 0.0f )
      return false;

    bool denomIsPositive = denom > 0.0f;
    float x3 = a1[0] - a2[0];
    float y3 = a1[1] - a2[1];

    float xy1 = x1 * y3 - y1 * x3;
    if( (xy1 < 0.0f) == denomIsPositive )
      return false;

    float sy2 = x2 * y3 - y2 * x3;
    if( (sy2 < 0.0f) == denomIsPositive )
      return false;

    if( (xy1 > denom) == denomIsPositive || (sy2 > denom) == denomIsPositive )
      return false;

    float multiplier = sy2 / denom;

    intersection[VX] = a1[VX] + (multiplier * x1);
    intersection[VY] = a1[VY] + (multiplier * y1);
    return true;
  }



  zCViewShaped::zCViewShaped() {
    for( uint i = 0; i < MAT_MAX; i++ ) {
      TrafoShape[i] = Alg_Identity2D();
      TrafoUV[i] = Alg_Identity2D();
    }

    UseParentTrafo = True;
  }



  zCViewShaped::zCViewShaped( int a0, int a1, int a2, int a3, zTviewID a4 ) : zCViewInteractive( a0, a1, a2, a3, a4 ) {
    for( uint i = 0; i < MAT_MAX; i++ ) {
      TrafoShape[i] = Alg_Identity2D();
      TrafoUV[i] = Alg_Identity2D();
    }

    UseParentTrafo = True;
  }



  bool_t zCViewShaped::IsOnBounds( int x, int y ) {
    if( Modified.GetNum() <= 2 )
      return False;

    if( !BBox.IsOnBounds( zVEC2( (float)x, (float)y ) ) )
      return False;

    static zVEC2 tracerVector = zVEC2( 1000000.0f, 10.0f );
    uint  intersections       = 0;
    zVEC2 intersection        = 0.0f;
    zVEC2 cursorPosStart      = zVEC2( (float)x, (float)y );
    zVEC2 cursorPosEnd        = cursorPosStart + tracerVector;
    zVEC2 lastVertex          = VirtualToPixel( Modified.GetLast().pos );

    for( uint i = 0; i < Modified.GetNum(); i++ ) {
      zVEC2& vertex = VirtualToPixel( Modified[i].pos );
      if( Alg_FindIntersection2D( cursorPosStart, cursorPosEnd, lastVertex, vertex, intersection ) )
        intersections++;

      lastVertex = vertex;
    }

    return (intersections % 2) != 0 ? True : False;
  }



  Array<zTRndSimpleVertex>& zCViewShaped::GetVertexes() {
    return Vertexes;
  }



  void zCViewShaped::GetShapeBounds( zVEC2& min, zVEC2& max ) {
    min = Vertexes.GetFirst().pos;
    max = Vertexes.GetLast().pos;
    for( uint i = 0; i < Vertexes.GetNum(); i++ ) {
      if( Vertexes[i].pos[VX] < min[VX] ) min[VX] = Vertexes[i].pos[VX];
      if( Vertexes[i].pos[VY] < min[VY] ) min[VY] = Vertexes[i].pos[VY];
      if( Vertexes[i].pos[VX] > max[VX] ) max[VX] = Vertexes[i].pos[VX];
      if( Vertexes[i].pos[VY] > max[VY] ) max[VY] = Vertexes[i].pos[VY];
    }
  }



  void zCViewShaped::CreateFlatUV() {
    if( Vertexes.GetNum() <= 2 )
      return;

    zVEC2 min, max;
    GetShapeBounds( min, max );

    static zVEC2 once = 1.0f;
    zVEC2 len = max - min;
    for( uint i = 0; i < Vertexes.GetNum(); i++ )
      Vertexes[i].uv = once / len * (Vertexes[i].pos - min);
  }



  void zCViewShaped::CreateBoxShape( const zVEC2& min, const zVEC2& max ) {
    Vertexes.Clear();
    InsertVertex( zVEC2( min[VX], min[VY] ) );
    InsertVertex( zVEC2( max[VX], min[VY] ) );
    InsertVertex( zVEC2( max[VX], max[VY] ) );
    InsertVertex( zVEC2( min[VX], max[VY] ) );
    CreateFlatUV();
  }



  void zCViewShaped::CreateRoundShape( const zVEC2& center, const float& radius, const uint& edges ) {
    Vertexes.Clear();
    if( edges <= 2 )
      return;

    float angleMax = PI * 2.0f;
    float angle    = angleMax / (float)edges;

    for( float alpha = 0.0f; alpha <= angleMax; alpha += angle ) {
      InsertVertex( zVEC2(
        center[VX] +  sin( alpha ) * radius,
        center[VY] + -cos( alpha ) * radius // The first edge on top
        ) );
    }

    CreateFlatUV();
  }



  void zCViewShaped::SetColor( const zCOLOR& color ) {
    for( uint i = 0; i < Vertexes.GetNum(); i++ )
      Vertexes[i].color = color;    
  }



  void zCViewShaped::UpdateTrafoShape() {
    TrafoShape[MAT_SUMMARY] = zMAT3::s_identity *
      TrafoShape[MAT_TRANSLATION] *
      TrafoShape[MAT_SCALE] *
      TrafoShape[MAT_ROTATION];
  }



  void zCViewShaped::UpdateTrafoUV() {
    TrafoUV[MAT_SUMMARY] = zMAT3::s_identity*
      TrafoUV[MAT_TRANSLATION] *
      TrafoUV[MAT_SCALE] *
      TrafoUV[MAT_ROTATION];
  }



  void zCViewShaped::MoveShape( const zVEC2& direction ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    zMAT3 trans = Alg_Translation2D( (zVEC2)direction );
    TrafoShape[MAT_TRANSLATION] *= TrafoShape[MAT_TRANSLATION].Inverse() * trans * TrafoShape[MAT_TRANSLATION];
    UpdateTrafoShape();
  }



  void zCViewShaped::RotateShape( const float& angle, const zVEC2& pivot ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    zMAT3 rot = Alg_Rotation2D( (zVEC2)pivot, angle );
    TrafoShape[MAT_ROTATION] = rot * TrafoShape[MAT_ROTATION];
    UpdateTrafoShape();
  }



  void zCViewShaped::ScaleShape( const zVEC2& direction, const zVEC2& pivot, const float& angle ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    zMAT3 preScale         = Alg_Scaling2D( (zVEC2)direction );
    zMAT3 trans            = Alg_Translation2D( (zVEC2)pivot );
    zMAT3	rot              = Alg_Rotation2D( (zVEC2)pivot, angle );
    zMAT3 postScale        = trans * preScale * trans.Inverse();
    TrafoShape[MAT_SCALE] *= TrafoShape[MAT_SCALE].Inverse() * rot * postScale * rot.Inverse() * TrafoShape[MAT_SCALE];
    UpdateTrafoShape();
  }



  void zCViewShaped::ResetShapeTranslation( const zVEC2& trans ) {
    TrafoShape[MAT_TRANSLATION].MakeIdentity();
    MoveShape( trans );
  }



  void zCViewShaped::ResetShapeRotation( const float& angle, const zVEC2& pivot ) {
    TrafoShape[MAT_ROTATION].MakeIdentity();
    RotateShape( angle, pivot );
  }



  void zCViewShaped::ResetShapeScaling( const zVEC2& direction, const zVEC2& pivot, const float& angle ) {
    TrafoShape[MAT_SCALE].MakeZero();
    ScaleShape( direction, pivot, angle );
  }



  void zCViewShaped::RotateUV( const float& angle, const zVEC2& pivot ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    zVEC2 uvPivot = TrafoShape[MAT_ROTATION].Inverse() * pivot * s_SingleUV;
    TrafoUV[MAT_ROTATION] *= Alg_Rotation2D( uvPivot, angle ).Inverse();
    UpdateTrafoUV();
  }



  void zCViewShaped::MoveUV( const zVEC2& direction ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    zMAT3 trafoInverse        = TrafoShape[MAT_ROTATION].Inverse();
    zVEC2 uvStart             = trafoInverse * zVEC2( 0.0f );
    zVEC2 uvEnd               = trafoInverse * direction;
    zVEC2 uvVector            = uvEnd - uvStart;
    zVEC2 uvDirection         = uvVector.Normalize() * direction.Length();
    TrafoUV[MAT_TRANSLATION] *= Alg_Translation2D( uvDirection * s_SingleUV ).Inverse();
    UpdateTrafoUV();
  }



  void zCViewShaped::ScaleUV( const zVEC2& direction, const zVEC2& pivot, const float& angle ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    float shapeRot      = TrafoShape[MAT_ROTATION].ExtractAngle() * DEGREE;
    zVEC2 uvPivot       = TrafoShape[MAT_ROTATION].Inverse() * pivot * s_SingleUV;
    zMAT3 preScale      = Alg_Scaling2D( (zVEC2)direction ).Inverse();
    zMAT3	trans         = Alg_Translation2D( uvPivot );
    zMAT3	rot           = Alg_Rotation2D( uvPivot, angle - shapeRot );
    zMAT3 postScale     = trans * preScale * trans.Inverse();
    TrafoUV[MAT_SCALE] *= rot * postScale * rot.Inverse();
    UpdateTrafoUV();
  }



  void zCViewShaped::RotateUVLocal( const float& angle, const zVEC2& pivot ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    zVEC2 uvPivot = pivot * s_SingleUV;
    TrafoUV[MAT_ROTATION] *= Alg_Rotation2D( uvPivot, angle ).Inverse();
    UpdateTrafoUV();
  }



  void zCViewShaped::MoveUVLocal( const zVEC2& direction ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    TrafoUV[MAT_TRANSLATION] *= Alg_Translation2D( direction * s_SingleUV ).Inverse();
    UpdateTrafoUV();
  }



  void zCViewShaped::ScaleUVLocal( const zVEC2& direction, const zVEC2& pivot, const float& angle ) {
    if( Vertexes.GetNum() <= 2 )
      return;

    zVEC2 uvPivot       = pivot * s_SingleUV;
    zMAT3 preScale      = Alg_Scaling2D( (zVEC2)direction ).Inverse();
    zMAT3	trans         = Alg_Translation2D( uvPivot );
    zMAT3	rot           = Alg_Rotation2D( uvPivot, angle );
    zMAT3 postScale     = trans * preScale * trans.Inverse();
    TrafoUV[MAT_SCALE] *= rot * postScale * rot.Inverse();
    UpdateTrafoUV();
  }



  void zCViewShaped::ResetUVTranslation( const zVEC2& trans ) {
    TrafoUV[MAT_TRANSLATION].MakeIdentity();
    MoveUV( trans );
  }



  void zCViewShaped::ResetUVRotation( const float& angle, const zVEC2& pivot ) {
    TrafoUV[MAT_ROTATION].MakeIdentity();
    RotateUV( angle, pivot );
  }



  void zCViewShaped::ResetUVScaling( const zVEC2& direction, const zVEC2& pivot, const float& angle ) {
    TrafoUV[MAT_SCALE].MakeIdentity();
    ScaleUV( direction, pivot, angle );
  }
  
  

  void zCViewShaped::InsertVertex( const zVEC2& point ) {
    InsertVertex( point, 0.0f );
  }



  void zCViewShaped::InsertVertex( const zVEC2& point, const zVEC2& uv ) {
    zTRndSimpleVertex& vertex = Vertexes.Create();
    vertex.pos   = point;
    vertex.uv    = uv;
    vertex.color = color;
    vertex.z     = zCCamera::activeCam ? zCCamera::activeCam->nearClipZ + 1 : 1;

    Modified += vertex;
  }
  


  void zCViewShaped::Blit() {
    DoLoopedEvent();
    if( Vertexes.GetNum() <= 2 )
      return;

    // Backup current render mode
    zTRnd_AlphaBlendFunc zBlendFunc = zrenderer->GetAlphaBlendFunc();
    bool_t zBuffer                  = zrenderer->GetZBufferWriteEnabled();
    zTRnd_ZBufferCmp	zCompare      = zrenderer->GetZBufferCompare();

    // Set a new render mode
    zrenderer->SetZBufferWriteEnabled( False );
    zrenderer->SetZBufferCompare( zRND_ZBUFFER_CMP_ALWAYS );
    zrenderer->SetAlphaBlendFunc( alphafunc );
    color.alpha = alpha;

    // Update vertices list
    if( Modified.GetNum() != Vertexes.GetNum() )
      Modified = Vertexes;
    
    // Apply matrix to vertices and UV
    for( uint i = 0; i < Modified.GetNum(); i++ ) {
      Modified[i].pos   = TrafoShape[MAT_SUMMARY] * Vertexes[i].pos;
      Modified[i].uv    = TrafoUV[MAT_SUMMARY] * Vertexes[i].uv;
      Modified[i].color = Vertexes[i].color;
    }

    // Transform virtual vertices to pixel
    Array<zTRndSimpleVertex> VertexesVirtual;
    VertexesVirtual = Modified;
    
    zTBBox2D pixelBBox;
    BBox.Reset( Modified.GetFirst().pos );
    for( uint i = 0; i < Modified.GetNum(); i++ ) {
      zVEC2& pos = VertexesVirtual[i].pos = VirtualToPixel( Modified[i].pos );
      pixelBBox.AddPoint( Modified[i].pos );
      BBox.AddPoint( pos );
    }

    // Render object
    zrenderer->DrawPolySimple( backTex, &VertexesVirtual[0], VertexesVirtual.GetNum() );
    BlitText();

    // Restore an previous render mode
    zrenderer->SetAlphaBlendFunc( zBlendFunc );
    zrenderer->SetZBufferWriteEnabled( zBuffer );
    zrenderer->SetZBufferCompare( zCompare );
  }



  zCViewShaped::~zCViewShaped() {
  }
}