// Supported with union (c) 2020 Union team

// User API for zTBBox2D
// Add your methods here

bool IsOnBounds( const zVEC2& point ) const {
  return
    point[VX] >= mins[VX] &&
    point[VY] >= mins[VY] &&
    point[VX] <= maxs[VX] &&
    point[VY] <= maxs[VY];
}

void Reset( const zVEC2& center ) {
  mins = maxs = center;
}

void AddPoint( const zVEC2& point ) {
  if( point[VX] < mins[VX] ) mins[VX] = point[VX];
  if( point[VY] < mins[VY] ) mins[VY] = point[VY];
  if( point[VX] > maxs[VX] ) maxs[VX] = point[VX];
  if( point[VY] > maxs[VY] ) maxs[VY] = point[VY];
}