// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  enum zTMatType {
    MAT_SUMMARY,
    MAT_TRANSLATION,
    MAT_ROTATION,
    MAT_SCALE,
    MAT_MAX
  };

  class VIEWAPI zCViewShaped : public zCViewInteractive {
  protected:
    zTBBox2D BBox;
    Array<zTRndSimpleVertex> Modified;
  public:

    Array<zTRndSimpleVertex> Vertexes;
    zMAT3 TrafoShape[MAT_MAX];
    zMAT3 TrafoUV[MAT_MAX];
    bool_t UseParentTrafo; // Reserved

    virtual bool_t IsOnBounds( int x, int y );
  public:

    zCViewShaped();
    zCViewShaped( int a0, int a1, int a2, int a3, zTviewID a4 = zTviewID::VIEW_ITEM );
    Array<zTRndSimpleVertex>& GetVertexes();
    void GetShapeBounds( zVEC2& min, zVEC2& max );
    void CreateFlatUV();
    void CreateBoxShape( const zVEC2& min, const zVEC2& max );
    void CreateRoundShape( const zVEC2& center, const float& radius, const uint& edges );
    void SetColor( const zCOLOR& color );
    void UpdateTrafoShape();
    void UpdateTrafoUV();

    // Shape operations
    void MoveShape( const zVEC2& direction );
    void RotateShape( const float& angle, const zVEC2& pivot = 4096.0f );
    void ScaleShape( const zVEC2& direction, const zVEC2& pivot = 4096.0f, const float& angle = 0.0f );
    void ResetShapeTranslation( const zVEC2& trans = 0.0f );
    void ResetShapeRotation( const float& angle = 0.0f, const zVEC2& pivot = 4096.0f );
    void ResetShapeScaling( const zVEC2& direction = 0.0f, const zVEC2& pivot = 4096.0f, const float& angle = 0.0f );

    // UV operations
    void MoveUV( const zVEC2& direction );
    void RotateUV( const float& angle, const zVEC2& pivot = 4096.0f );
    void ScaleUV( const zVEC2& direction, const zVEC2& pivot = 4096.0f, const float& angle = 0.0f );
    void MoveUVLocal( const zVEC2& direction );
    void RotateUVLocal( const float& angle, const zVEC2& pivot = 4096.0f );
    void ScaleUVLocal( const zVEC2& direction, const zVEC2& pivot = 4096.0f, const float& angle = 0.0f );
    void ResetUVTranslation( const zVEC2& direction = 0.0f );
    void ResetUVRotation( const float& angle = 0.0f, const zVEC2& pivot = 4096.0f );
    void ResetUVScaling( const zVEC2& direction = 0.0f, const zVEC2& pivot = 0.0f, const float& angle = 0.0f );

    virtual void InsertVertex( const zVEC2& point );
    virtual void InsertVertex( const zVEC2& point, const zVEC2& uv );
    virtual void Blit();
    virtual ~zCViewShaped();
  };
}