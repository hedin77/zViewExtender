// Supported with union (c) 2020 Union team

// User API for zCView
// Add your methods here

bool32 IsOnBounds( const int& x, const int& y );
zCView* GetTopView( const int& x, const int& y );
Array<zCView*> GetTopViewList( const int& x, const int& y );
void ShowBounds( const zCOLOR& color );
void Blit_Union();
void GetPixelExtends_Union( float&, float&, float&, float& );
zVEC2 PixelToVirtual( const zVEC2& point );
zVEC2 VirtualToPixel( const zVEC2& point );
zVEC2 GetVirtualScale();
void PrintChars_Union( int, int, zSTRING const& );