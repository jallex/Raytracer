#ifndef BVH_HPP_
#define BVH_HPP_
#include "./geometry.hpp"
#include "./rtCommon.hpp"
#include "./logeometry.hpp"

class BVHNode : public Geometry {
    public:
        BVHNode();

        BVHNode(LoGeometry& list, float time0, float time1)
            : BVHNode(list.objects, 0, list.objects.size(), time0, time1){}

        BVHNode(
            std::vector<shared_ptr<Geometry>>& objects,
            size_t start, size_t end, double time0, double time1);

        virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;
        virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override;

        public:
            shared_ptr<Geometry> left;
            shared_ptr<Geometry> right;
            AABB box; 
};

bool BVHNode::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const{
    if(!box.hit(ray, tMin, tMax)){
        return false;
    }

    bool hitLeft = left->hit(ray, tMin, tMax, rec);
    bool hitRight = right->hit(ray, tMin, hitLeft ? rec.t : tMax, rec);

    return hitLeft || hitRight;
}

bool BVHNode::boundingBox(float t0, float t1, AABB& outputBox) const {
    outputBox = box;
    return true;
}




#endif /* BVH_HPP_*/