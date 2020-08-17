#ifndef BVH_HPP_
#define BVH_HPP_

#include <algorithm>

#include "./geometry.hpp"
#include "./rtCommon.hpp"
#include "./logeometry.hpp"

//Bounding Volume Hierarchy (BVH)
//https://www.scratchapixel.com/lessons/advanced-rendering/introduction-acceleration-structure/bounding-volume-hierarchy-BVH-part1
class BVHNode : public Geometry {
    public:
        BVHNode();

        BVHNode(LoGeometry& list, float time0, float time1)
            : BVHNode(list.objects, 0, list.objects.size(), time0, time1){}

        BVHNode(
            std::vector<shared_ptr<Geometry>>& objects,
            size_t start, size_t end, float time0, float time1);

        virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;
        virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override;

        public:
            shared_ptr<Geometry> left;
            shared_ptr<Geometry> right;
            AABB box; 
};

//constructor to build BVH by randomly choosing an axis, sorting the objects, and splitting them into subtrees
BVHNode::BVHNode(std::vector<shared_ptr<Geometry>>& objects,
    size_t start, size_t end, float time0, float time1){
    //get a random int representing x, y, or z axis
    int axis = static_cast<int>(randomNum(0, 3));
    auto comparator = (axis == 0) ? xBoxCompare
                    : (axis == 1)? yBoxCompare
                    : zBoxCompare;
    
    size_t objectSpan = end - start;
    if(objectSpan == 1){
        left = objects[start];
        right = objects[start];
    }
    else if (objectSpan == 2) {
        if(comparator(objects[start], objects[start+1])){
            left = objects[start];
            right = objects[start+1];
        }
        else {
            right = objects[start];
            left=objects[start+1];
        }
    }
    else{
        //Sort the primitives
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
   
        auto mid = start + objectSpan/2;
        left = make_shared<BVHNode>(objects, start, mid, time0, time1);
        right = make_shared<BVHNode>(objects, mid, end, time0, time1);
   }

   AABB boxLeft, boxRight;

   if(!left->boundingBox(time0, time1, boxLeft) || !right->boundingBox(time0, time1, boxRight))

   std::cerr << "Must create a bounding box in BVHNode constructor\n";

   box = surroundingBox(boxLeft, boxRight);
    }

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

bool xBoxCompare(const shared_ptr<Geometry> a, const shared_ptr<Geometry> b){
    AABB boxA;
    AABB boxB;

    if (!a->boundingBox(0,0,boxA) || !b->boundingBox(0,0, boxB)){
        std::cerr<<"Must create a bounding box in BVHNode constructor\n";
    }
    return boxA.min().getX() < boxB.min().getX();
}

bool yBoxCompare(const shared_ptr<Geometry> a, const shared_ptr<Geometry> b){
        AABB boxA;
    AABB boxB;

    if (!a->boundingBox(0,0,boxA) || !b->boundingBox(0,0, boxB)){
        std::cerr<<"Must create a bounding box in BVHNode constructor\n";
    }
    return boxA.min().getY() < boxB.min().getY();
}

bool zBoxCompare(const shared_ptr<Geometry> a, const shared_ptr<Geometry> b){
    AABB boxA;
    AABB boxB;

    if (!a->boundingBox(0,0,boxA) || !b->boundingBox(0,0, boxB)){
        std::cerr<<"Must create a bounding box in BVHNode constructor\n";
    }
    return boxA.min().getZ() < boxB.min().getZ();
}




#endif /* BVH_HPP_*/