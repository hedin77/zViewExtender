// Supported with union (c) 2020 Union team

// User API for zCTexture
// Add your methods here

//void AddRefAni() {
//  AddRef();
//
//  for( uint i = 0; i < zTEX_MAX_ANIS; i++ ) {
//    zCTexture* nextTex = nextAni[i];
//    while( nextTex ) {
//      nextTex->AddRef();
//      nextTex = nextTex->nextAni[i];
//    }
//  }
//}
//
//void ReleaseAni() {
//  AddRef();
//
//  for( uint i = 0; i < zTEX_MAX_ANIS; i++ ) {
//    zCTexture* nextTex = nextAni[i];
//    while( nextTex ) {
//      zCTexture* _nextTex = nextTex->nextAni[i];
//      nextTex->Release();
//      nextTex = _nextTex;
//    }
//  }
//}