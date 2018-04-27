#include "ScoutCar.h"

//ScoutCar::ScoutCar()
//{
//    _camRegion.setEdge(5);
//}

using std::endl;

Encode::Encode()
{

}

std::default_random_engine enge(std::time(nullptr));

ScoutCar::ScoutCar(const double &c_x, const double &c_y, const double &c_edge, const double &c_ori, const double &rad)
{
    setOrient(c_ori);
    bgeo::DPoint pnt(c_x,c_y);
    setTurnRad(rad);
    //setPos(pnt);
    _camRegion.setEdge(c_edge);
    _camRegion.setPos(pnt, this->getOrient());
    this->_pos = pnt;
    _rsSpacePtr = std::make_shared<ReedsSheppStateSpace>(getTurnRad());
}


ScoutCar::~ScoutCar()
{
}

bool ScoutCar::leadFire()
{
    return false;
}

bool ScoutCar::scoutRegion()
{
    return false;
}

bool ScoutCar::scoutTarget()
{
    return false;
}

bool ScoutCar::initEncode(const size_t &engType)
{
    switch (engType) {
    case generatorType::randType:
    {
        _generator = [this](){ return this->randInitEncode();};
        break;
    }
    case generatorType::greedType:
    {
        _generator = [this](){ return this->greedInitEncode();};
        break;
    }
    default:
        break;
    }
    _generator();
    return false;
}



bool ScoutCar::generateAllPath()
{
    _vRSPath.clear();
    if(_vEncode.empty())
    {
        return false;
    }

    for(size_t i = 0; i<_vEncode.size();i++)
    {
        generatePath(i);
    }
    return true;
}

bool ScoutCar::generatePath(const size_t &encodeIndex)
{
    auto size = this->_vEncode.size();
    if(encodeIndex>=size)
    {
        return false;
    }
    else
    {
        auto dirpnt = encode2DirPoint(_vEncode.at(encodeIndex));
        //ReedsSheppStateSpace _rsStateSpace(14.5);
        double q0[3];
        q0[0] = getPos().x();
        q0[1] = getPos().y();
        q0[2] = getOrient();
        DirPnt startPnt(getPos(),getOrient());
        double q1[3];
        q1[0]  = std::get<0>(dirpnt).x();
        q1[1]  = std::get<0>(dirpnt).y();
        q1[2]  = std::get<1>(dirpnt);
        RSPathWithSE pathSE;
        auto pathUnit = _rsSpacePtr->reedsShepp(q0,q1);
        pathSE._path = pathUnit;
        pathSE._start = startPnt;
        pathSE._end = dirpnt;
        this->_vRSPath.push_back(pathSE);

        double total_len = 0;
        for(size_t i = 0; i<5;i++)
        {
            total_len += std::abs(pathUnit.length_[i]);
        }
        total_len = total_len* this->getTurnRad();


        std::cout <<" the total_len is "<< total_len<<std::endl;
        //update  ScoutCar position
        this->setOrient(q1[2]);
        this->setPos(std::get<0>(dirpnt));
        return true;
    }
}

double ScoutCar::getArriveTime(const Encode &en_code)
{
    double len  = this->getEncodeLength(en_code);
    return (len/this->getVel());
}

double ScoutCar::getEncodeLength(const Encode &en_code)
{
    auto dirpnt = encode2DirPoint(en_code);
    double q0[3];
    q0[0] = getPos().x();
    q0[1] = getPos().y();
    q0[2] = getOrient();
    double q1[3];
    q1[0]  = std::get<0>(dirpnt).x();
    q1[1]  = std::get<0>(dirpnt).y();
    q1[2]  = std::get<1>(dirpnt);
    auto pathUnit = _rsSpacePtr->reedsShepp(q0,q1);
    double total_len = 0;
    for(size_t i = 0; i<5;i++)
    {
        total_len += std::abs(pathUnit.length_[i]);
    }
    total_len = total_len* this->getTurnRad();

    std::cout <<" the total_len is "<< total_len<<std::endl;

    return total_len;
}

double ScoutCar::getEncodeLength(const Encode &en_code1, const Encode &en_code2)
{
    auto dirpnt1 = encode2DirPoint(en_code1);
    double q0[3];
    q0[0]  = std::get<0>(dirpnt1).x();
    q0[1]  = std::get<0>(dirpnt1).y();
    q0[2]  = std::get<1>(dirpnt1);
    auto dirpnt2 = encode2DirPoint(en_code2);
    double q1[3];
    q1[0]  = std::get<0>(dirpnt2).x();
    q1[1]  = std::get<0>(dirpnt2).y();
    q1[2]  = std::get<1>(dirpnt2);
    auto pathUnit = _rsSpacePtr->reedsShepp(q0,q1);
    double total_len = 0;
    for(size_t i = 0; i<5;i++)
    {
        total_len += std::abs(pathUnit.length_[i]);
    }
    total_len = total_len* this->getTurnRad();
    std::cout <<" the total_len is "<< total_len<<std::endl;
    return total_len;

}

double ScoutCar::getListEncode(const std::list<Encode> &lencode)
{
    if(lencode.empty())
    {
        return 0;
    }
    double leng = getEncodeLength(lencode.front());




     std::list<int> ltest;
     for(size_t j = 0; j<10;j++)
    {
         ltest.push_back(j);
     }

     for(auto ll  = ltest.begin(); ll !=ltest.end(); ll++)
     {
         std::cout<<"number is "<< *ll <<std::endl;
     }

    size_t  i = 0;

    for(auto bb = lencode.begin(); bb != lencode.end(); /*bb++*/)
    {
        std::cout << "size is "<<lencode.size()<<std::endl;
        std::cout <<"index is "<< i++<<std::endl;
        std::cout<<*bb<<std::endl;
        bb =  std::next(bb);
    }

    auto eit = lencode.end();





    auto it =lencode.begin();
    while (it != eit)
    {
        auto nit = it;
        std::cout << "before is  " <<*nit<<std::endl;
        nit++;

        std::cout << "after is  " <<*nit<<std::endl;

        if(nit == eit)
        {
            std::cout<<"wtf"<<std::endl;
            break;
        }
        std::cout<<"i = " <<i++<<std::endl;
        std::cout<< "first" << *it<<std::endl;
        std::cout <<" second " << *nit <<std::endl;
        leng  += getEncodeLength(*it,*nit);
        it++;
        std::cout <<" first change is "<< *it <<std::endl;
    }
    return leng;
}



bool ScoutCar::greedInitEncode()
{

    _vEncode.clear();
    auto size = this->_vTarget.size();
    // for(size_t i = 0; i< )
    size_t sampleTimes  = 5;
    std::vector<double> vSample;
    for(double i = 0 ; i<sampleTimes ;i++)
    {
        double unit  =  i/(sampleTimes-1);
        vSample.push_back(unit);
    }
    std::list<Encode> vencode;
    for(size_t i = 0 ;i < size; i++)
    {
        //Encode
        for(size_t j = 0; j<sampleTimes ;j++)
        {
            vencode.push_back(Encode(i,vSample[j],0.5));
        }
    }

    std::list<Encode> lencode;

    //    Encode xx;
    //lencode.push_back(xx);
    //
    //
    //
    double c_val = 0;
    size_t  addIndex;
    decltype(lencode.begin()) ait;
    decltype(vencode.end()) vait;
    //size


    while (!vencode.empty())
    {
        double min = 999999;
        for(auto vit =  vencode.begin(); vit!= vencode.end();vit++)
        {
            size_t i = 0;
            auto it  = lencode.begin();
            auto eit = lencode.end();
            for(; it != eit;it++)
            {
                 std::cout << "index = "<<i <<std::endl;
                 lencode.insert(it,*vit);

                size_t  j  = 0;
                auto ll = lencode.begin();
                auto ls = lencode.end();
                for(/*auto ll  = lencode.begin()*/; ll != ls /*lencode.end()*/; ll++)
                {
                    std::cout<<" index is "<<j++ <<std::endl;
                    std::cout <<*ll<<std::endl;
                    std::cout <<*lencode.end();
                }

//                double n_val = getListEncode(lencode);
//                n_val = n_val-c_val;
//                if(n_val<min)
//                {
//                    vait = vit;
//                    ait = lencode.begin();
//                    std::cout<<*ait<<std::endl;
//                }
//                auto mid_it = it;
//                mid_it--;
//                lencode.erase(mid_it);
            }
            if(it == eit)
            {
                lencode.push_back(*vit);
                double n_val = getListEncode(lencode);
                n_val = n_val-c_val;
                if(n_val<min)
                {
                    vait = vit;
                    ait = lencode.begin();
                    std::cout<<*ait<<std::endl;
                }
                lencode.pop_back();
            }
        }
        lencode.insert(ait,*vait);
        auto taskId = vait->_id;
        bool findBoolean  = false;
        decltype(vencode.begin())  bit;
        decltype(vencode.end()) eit = vencode.end();
        for(auto it= vencode.begin(); it!= vencode.end();it++)
        {
            if(!findBoolean)
            {if(it->_id == taskId)
                {
                    bit = it;
                    findBoolean = true;
                    std::cout<<"begin it = "<<*it<<std::endl;
                }
            }
            else{
                if(it->_id != taskId)
                {
                    eit = it;
                    std::cout<<"end it  is "<<*it<<endl;
                    std::cout<<*it<<std::endl;
                    break;
                }
            }
        }
        vencode.erase(bit,eit);
    }

    return false;
}

bool ScoutCar::randInitEncode()
{

    _vEncode.clear();
    auto size = this->_vTarget.size();
    auto aRand = randN(size);
    std::uniform_real_distribution<double> dis(0.0,1.0);
    for(size_t i = 0;i<size;i++)
    {
        Encode encode;
        auto angle = dis(enge)*M_PI*2;
        encode._angle = angle;
        encode._dur = dis(enge);
        encode._id =aRand.at(i);
        std::cout<<"Encode:"<<encode;
        this->_vEncode.push_back(encode);
    }
    return false;
}


DirPnt ScoutCar::encode2DirPoint(const Encode &encode)
{
    //encode._id
    auto pntCentre = this->_vTarget.at(encode._id);
    auto edge = _camRegion.getEdge();
    double x = pntCentre.x()+cos(encode._angle)*edge;
    double y = pntCentre.y()+sin(encode._angle)*edge;
    bgeo::DPoint pnt(x,y);
    DirPnt dirpnt(pnt,encode._angle+M_PI);
    return dirpnt;
}

void ScoutCar::setPos(bgeo::DPoint const & pnt)
{
    _camRegion.setPos(pnt, this->getOrient());
    this->_pos = pnt;
}

//
std::vector<size_t> randN(const size_t &n)
{
    //    std::default_random_engine eng(std::time(sNULL));
    std::uniform_int_distribution<int> dis(1,2000);

    std::vector<std::tuple<size_t,int>> vUnit;
    for(size_t i =0; i<n;i++)
    {
        std::tuple<size_t,int> Unit(i,dis(enge));
        vUnit.push_back(Unit);
    }

    using compUnit = std::tuple<size_t,int>;

    //    std::function<bool(std::tuple<size_t,int>,std::tuple<size_t,int>)>
    std::function<bool(compUnit,compUnit)> cmpFunc =
            [=](compUnit A,compUnit B)
    {
        auto a2 = std::get<1>(A);
        auto b2 = std::get<1>(B);
        return a2<b2?true:false;
    };

    std::sort(vUnit.begin(),vUnit.end(),cmpFunc);

    std::vector<size_t> res;

    for(auto &it:vUnit)
    {
        res.push_back(std::get<0>(it));
    }
    return res;
}

