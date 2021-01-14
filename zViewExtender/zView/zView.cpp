
namespace NAMESPACE {
  bool32 zCView::IsOnBounds( const int& x, const int& y ) {
    zCViewAnimated* animated = dynamic_cast<zCViewAnimated*>( this );
    if( animated )
      return animated->IsOnBounds( x, y );

    return
      x > pposx && x < pposx + psizex &&
      y > pposy && y < pposy + psizey;
  }



  zCView* zCView::GetTopView( const int& x, const int& y ) {
    zCView* child = childs.root;
    zCView* frontView = IsOnBounds( x, y ) ? this : Null;
    while( child ) {

      zCView* view = child->GetTopView( x, y );
      if( view )
        frontView = view;

      child = child->next;
    }

    return frontView;
  }



  Array<zCView*> zCView::GetTopViewList( const int& x, const int& y ) {
    zCView* child = childs.root;

    Array<zCView*> topList;
    if( IsOnBounds( x, y ) )
      topList += this;

    while( child ) {
      topList += child->GetTopView( x, y );
      child = child->next;
    }

    return topList;
  }



  void zCView::ShowBounds( const zCOLOR& color ) {
    screen->Line(
      zPixelX( pposx ),
      zPixelY( pposy ),
      zPixelX( pposx + psizex ),
      zPixelY( pposy ),
      color
    );

    screen->Line(
      zPixelX( pposx + psizex ),
      zPixelY( pposy ),
      zPixelX( pposx + psizex ),
      zPixelY( pposy + psizey ),
      color
    );

    screen->Line(
      zPixelX( pposx + psizex ),
      zPixelY( pposy + psizey ),
      zPixelX( pposx ),
      zPixelY( pposy + psizey ),
      color
    );

    screen->Line(
      zPixelX( pposx ),
      zPixelY( pposy + psizey ),
      zPixelX( pposx ),
      zPixelY( pposy ),
      color
    );
  }



  zVEC2 zCView::PixelToVirtual( const zVEC2& point ) {
    float pixelPosX   = (float)pposx;
    float pixelPosY   = (float)pposy;
    float pixelSizeX  = (float)psizex;
    float pixelSizeY  = (float)psizey;
    float offsetX     = point[VX] - pixelPosX;
    float offsetY     = point[VY] - pixelPosY;
    float virtualPosX = 8192.0f / pixelSizeX * offsetX;
    float virtualPosY = 8192.0f / pixelSizeY * offsetY;

    return zVEC2(
      virtualPosX,
      virtualPosY
      );
  }



  zVEC2 zCView::VirtualToPixel( const zVEC2& point ) {
    float pixelPosX   = (float)pposx;
    float pixelPosY   = (float)pposy;
    float pixelSizeX  = (float)psizex;
    float pixelSizeY  = (float)psizey;
    float pointMultiX = 1.0f / 8192.0f * point[VX];
    float pointMultiY = 1.0f / 8192.0f * point[VY];
    float pointPosX   = pixelSizeX * pointMultiX;
    float pointPosY   = pixelSizeY * pointMultiY;

    return zVEC2(
      pixelPosX + pointPosX,
      pixelPosY + pointPosY
      );
  }



  zVEC2 zCView::GetVirtualScale() {
    float xdim       = (float)zrenderer->vid_xdim;
    float ydim       = (float)zrenderer->vid_ydim;
    float pixelSizeX = (float)psizex;
    float pixelSizeY = (float)psizey;

    return zVEC2(
      1.0f / xdim * pixelSizeX,
      1.0f / ydim * pixelSizeY
      );
  }
}