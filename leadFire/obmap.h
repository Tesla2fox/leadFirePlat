#ifndef OBMAP_H
#define OBMAP_H
#include <vector>
#include "bGeo.hpp"

class obmap
{
public:
    obmap();
    void SetVRing(std::shared_ptr<std::vector<bgeo::DRing>> ptr) {_vRingPtr = ptr;}
    void pushRing(bgeo::DRing const &ring ){_vRingPtr->push_back(ring);}
   // void setVRing(std::vector<bgeo::DPoint> const &vring){_vRing.clear();
     //                                                    _vRing.assign(vring.begin(),vring.end());}

    std::vector<bgeo::DRing> getVRing() const {return *_vRingPtr;}

private:
    //std::vector<bgeo::DRing> _vRing;
    std::shared_ptr<std::vector<bgeo::DRing>> _vRingPtr = nullptr;
};

#endif // OBMAP_H
