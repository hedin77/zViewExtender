// Supported with union (c) 2020 Union team

// User API for zCCamera
// Add your methods here

static float GetNearClipZ() {
  float farZ;
  if( zCCamera::activeCam ) farZ = zCCamera::activeCam->nearClipZ + 1;
  else                      farZ = 1;
  return farZ;
}