#ifndef PERLINNOISE_HPP_
#define PERLINNOISE_HPP_
 
#include "./logeometry.hpp"
#include "./rtCommon.hpp"

class PerlinNoise{
    public:
    PerlinNoise(){
        randomFloat = new float[pointCount];
        for (int i = 0; i < pointCount; i++){
            randomFloat[i] = randomNum();
        } 

        permX = perlinGeneratePerm();
        permY = perlinGeneratePerm();
        permZ = perlinGeneratePerm();
    }

    ~PerlinNoise() {
        delete[] randomFloat;
        delete[] permX;
        delete[] permY;
        delete[] permZ;
    }

    float noise(const Vector3& p) const {
        auto u = p.getX() - floor(p.getX());
        auto v = p.getY() - floor(p.getY());
        auto w = p.getZ() - floor(p.getZ());

        auto i = static_cast<int>(4*p.getX()) & 255;
        auto j = static_cast<int>(4*p.getY()) & 255;
        auto k = static_cast<int>(4*p.getZ()) & 255;

        return randomFloat[permX[i] ^ permY[j] ^ permZ[k]];
    }

    private:
        static const int pointCount = 256;
        float* randomFloat;
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