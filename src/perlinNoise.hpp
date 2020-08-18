#ifndef PERLINNOISE_HPP_
#define PERLINNOISE_HPP_
 
#include "./logeometry.hpp"
#include "./rtCommon.hpp"

#define IX(grid3d, i, j, k, u, v, w) (k*u + (1-k)*(1-u)) * (j*v + (1-j)*(1-v)) * (i*w + (1-i)*(1-w)) * grid3d[k][j][i]

//trilinear interpolation for perlin noise smoothing:
//the linear interpolation of two bilinear interpolations (one for the front face of the cell and one for the back face)
inline float trilinearInterpolation(float grid3d[2][2][2], float u, float v, float w ) {
    //accumulator
    auto acc = 0.0;
    int numVertices = 2; 

    for (int k = 0; k < numVertices; k++) {
        for (int j = 0; j < numVertices; j++) {
             for (int i = 0; i < numVertices; i++) {
                acc += IX(grid3d, i, j, k, u, v, w);
             }
    }
}
    return acc;
}

class PerlinNoise{
    public:
    PerlinNoise(){
        randomVector = new Vector3[pointCount];
        for (int i = 0; i < pointCount; i++){
            randomVector[i] = unitVector(randomVec(-1, 1));
        } 

        permX = perlinGeneratePerm();
        permY = perlinGeneratePerm();
        permZ = perlinGeneratePerm();
    }

    ~PerlinNoise() {
        delete[] randomVector;
        delete[] permX;
        delete[] permY;
        delete[] permZ;
    }

    float noise(const Vector3& p) const {
        auto u = p.getX() - floor(p.getX());
        auto v = p.getY() - floor(p.getY());
        auto w = p.getZ() - floor(p.getZ());

        int i = floor(p.getX());
        int j = floor(p.getY());
        int k = floor(p.getZ());

        Vector3 grid3d[2][2][2];

        for (int di = 0; di < 2; di ++){
            for (int dj = 0; dj < 2; dj++){
                for(int dk=0; dk < 2; dk++){
                    grid3d[di][dj][dk] = randomVector[
                        permX[(i + di) & 255] ^
                        permY[(j+dj) & 255] ^
                        permZ[(k+dk) & 255] 
                    ];
                }
            }
        }
        return perlinInterpolation(grid3d, u, v, w);
    }

    //a noise created from multiple frequencies added together
    float turbulence(const Vector3& p, int depth=7) const{
        float acc = 0.0;
        auto tempP = p;
        float weight = 1.0;

        for(int i = 0; i < depth; i++) {
            acc += weight*noise(tempP);
            weight *= 0.5;
            tempP *= 2;
        }
        return fabs(acc);
    }

    private:
    inline float perlinInterpolation(Vector3 grid3d[2][2][2], float u, float v, float w ) const{
    auto cubicu = u*u*(3-2*u);
    auto cubicv = v*v*(3-2*v);
    auto cubicw = w*w*(3-2*w);

    auto acc = 0.0;

    for (int k = 0; k < 2; k++) {
        for (int j = 0; j < 2; j++) {
             for (int i = 0; i < 2; i++) {
                Vector3 weightV(u-k, v-j, w-i);
                acc += weightV.dotProduct(grid3d[k][j][i]) * (k*cubicu + (1-k)*(1-cubicu)) * (j*cubicv + (1-j)*(1-cubicv)) * (i*cubicw + (1-i)*(1-cubicw));
             }
        }
    }
        return acc;
    }

    private:
        static const int pointCount = 256;
        Vector3* randomVector;
        int* permX;
        int* permY;
        int* permZ;

        static int* perlinGeneratePerm(){
            auto p = new int[pointCount];

            for(int i = 0; i < PerlinNoise:: pointCount; i++){
                p[i] = i;
            }

            permute(p, pointCount);

            return p;
        }

        static void permute(int* p, int n){
            for(int i = n - 1; i > 0; i--){
                int target = static_cast<int>(randomNum(0, i+1));
                int temp = p[i];
                p[i] = p[target];
                p[target] = temp;
            }
        }
};


#endif /* PERLINNOISE_HPP_ */