#include "MyMath.h"
#include <sstream>
void TransposeMatrix( XMMATRIX &mat)
{
	for(int x = 0; x < 4;x++) {
		for(int y=x; y < 4; y++) {
			float m = mat.m[x][y];
			mat.m[x][y] = mat.m[y][x];
			mat.m[y][x] = m;
		}
	}
}


std::string toString(const int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}