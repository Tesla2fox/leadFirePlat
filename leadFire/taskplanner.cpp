#include "taskplanner.h"



void TaskPlanner::onBroadcast(int ID, int type, void* data, size_t size){
    string msg((char*)data, size);
    std::cout <<"data = "<< data <<std::endl;
    std::cout <<"msg = "<<msg <<std::endl;
    std::cout << netAgent::_tool::format("Get broadcast from %d, type %d, content = %s\n", ID, type, msg.c_str());
    conf_debug <<" msg = " << msg << endl;
}

void TaskPlanner::onRequest(int ID, int type, void* data, size_t size, netAgent::ReplyInterface reply){
    string msg((char*)data, size);
    //std::cout <<" wtf !!!!!!!"<<std::endl;
    //std::cout <<
    netAgent::_tool::format("Get request from %d, type %d, content = %s\n", ID, type, msg.c_str());
    if (type == 0){
        //test_data data;
        //reply.send(&data, sizeof(test_data));
        reply.send(2333);
    }
    else
        reply.send(2333);
}

double TaskPlanner::decode(const vector<Chrom> &mpop)
{
    // something init


    //  the scout car can move or not
    std::vector<bool> vboolean;
    vboolean.assign(_vScoutCar.size(),false);


    // current encode is idnex;
    std::vector<size_t> vEncodeIndex;
    vEncodeIndex.assign(_vScoutCar.size(),0);

    // the  task pnt has been completed or not
    vector<bool> vboolTaskPnt;
    vboolTaskPnt.assign(this->_vTarget.size(),false);

    // the scoutcar leave time
    vector<double> vleaveTime;
    vleaveTime.assign(_vScoutCar.size(),0);

    // the scoutcar arrive time
    vector<double> varriveTime;
    varriveTime.assign(_vScoutCar.size(),0);


    // the scoutcar act during time at task pnt
    // 0 ~1
    vector<double> vdur;
    vdur.assign(_vScoutCar.size(),0);


    // the scoutcar  act during
    vector<double> vactTime;
    vactTime.assign(_vScoutCar.size(),0);


    // the scoutcar time during the path
    vector<double> vpathTime;
    vpathTime.assign(_vScoutCar.size(),0);

    // the scoutcar stay the task pnt until end

    vector<bool> vboolEnd;
    vboolEnd.assign(_vScoutCar.size(),false);


    auto vScoutCar = _vScoutCar;

    // the scoutcar in which task pnt
    vector<bool> vboolUnit;
    vboolUnit.assign(vScoutCar.size(),false);
    vector<vector<bool>> m_mat;
    m_mat.assign(_vTarget.size(),vboolUnit);


    // the init state
    vector<double> vState;
    for(auto &it :_vTarget)
    {
        vState.push_back(it.initState);
    }


    // the boolean leaveOrArrive
    vector<bool> vLeaveOrArrive;
    vLeaveOrArrive.assign(_vScoutCar.size(),false);


    // the arrive time mat

    vector<double> arriveTimeUnit;
    arriveTimeUnit.assign(_vTarget.size(),0);
    vector<vector<double>> arriveMat;
    arriveMat.assign(_vScoutCar.size(),arriveTimeUnit);


    //
    vector<double> actMatUnit;
    actMatUnit.assign(_vTarget.size(),0);
    vector<vector<double>> actMat;
    actMat.assign(_vScoutCar.size(),actMatUnit);



    // the encode is end ?
    vector<bool> vEncodeEndBoolean;
    vEncodeEndBoolean.assign(_vScoutCar.size(),false);



    // the arrive is vaild
    vector<bool> vAvaildBool;
    vAvaildBool.assign(_vScoutCar.size(),true);

    //
    //

    // some function

    // the all the task pnts have been completed
    auto complete  = [=] (vector<bool> const &vb)
    {
        auto it = std::find(vb.begin(),vb.end(),false);
        if(it == vb.end())
        {
            return false;
        }
        else
        {
            return true;
        }
    };


    // find the min leave  time  car index
    auto findMinIndex = [=](vector<double> const &vd)
    {
        auto it  = std::min_element(vd.begin(),vd.end());
        size_t p = 0;
        conf_debug<<endl;
        for(auto &wtf : vd)
        {
            conf_debug<< p++ << " leave time "<< wtf<<endl;
        }
        size_t indexCar = it - vd.begin();
        //for(auto )
        cout << "indexCar is " <<indexCar<<endl;
        return indexCar;
    };

    // find the taskid task pnt has which scout car
    auto findInTask = [=](size_t const & taskid,     vector<vector<bool>>  const mmat)
    {
        auto &row = mmat[taskid];
        vector<size_t> vCarIndex;
        for(size_t i = 0; i < row.size(); i++)
        {
            if(row[i])
            {
                vCarIndex.push_back(i);
            }
        }
        return vCarIndex;
    };

    //leave time sort the list
    auto sortAddList = [](list<size_t> &m_list,size_t const &m_index)
    {
        //        if(m_list.empty())
        //        {
        //           m_list.push_back(m_index);
        //           return;
        //        }
        //        if(varriveTime[m_index]>=varriveTime[m_list.back()])
        //        {
        //            m_list.push_back(m_index);
        //            return;
        //        }

        //        for(auto it = m_list.begin(); it != m_list.end(); it++)
        //        {
        //           if(varriveTime[m_index]>=varriveTime[*it])
        //           {
        //               m_list.insert(it,m_index);
        //               return;
        //           }
        //        }
    };



    // update the car  before arrive
    auto updateBeforeCarId  =[=](vector<size_t> & m_before, size_t const & m_index , double const & m_workedState)
    {
        //        auto coordSize = m_before.size() + 1;
        //        double updateLeaveTime = (vState[m_index] -m_workedState)/coordSize + varriveTime[m_index];
        //        doubele mleaveTime =   (vState[it] -m_workedState)/coordSize *dur + arriveTime;
        //        for (auto it = m_before.begin(); it != m_before.end(); it++)
        //        {

        //            double updateDur = updateLeaveTime - varriveTime[it];
        //            double coLeaveTime = updateDur*vdur[it] + varriveTime[it];
        //            conf_debug <<"id = " << it <<"  coordination leave time is "<< coLeaveTime<<endl;
        //            vleaveTime[it] = coLeaveTime;
        //        }
    };




    // find the motionid and the leave id
    auto findMotionOrLeaveId = [=](vector<double> const &va , vector<double> const &vl , vector<bool> const & vb , vector<bool> const &ve)
    {


        conf_debug <<endl;
        //        auto ita  = std::min_element(va.begin(),va.end());
        //        size_t p = 0;
        //        conf_debug<<endl;
        //        for(auto &wtf : va)
        //        {
        //            conf_debug<< p++ << " arrive time  "<< wtf<<endl;
        //        }

        //        size_t indexCar = ita - va.begin();

        double vamin = 999999;
        size_t indexCar;
        for(size_t i = 0 ; i <va.size() ; i++)
        {
            conf_debug<<i <<"  arrive time is " << va[i] ;
            if(ve[i]==false)
            {
                conf_debug<<" no use " <<endl;
                continue;

            }
            //means there is no encode at the end

            if(va[i]<vamin)
            {
                vamin = va[i];
                indexCar = i;
            }
//            conf_debug<<"<<<<<<<<<<";
            conf_debug<<endl;
        }


        //for(auto )
        cout << "indexCar is " <<indexCar<<endl;

        double vlmin = 999999;
        size_t index;
        for(size_t i = 0; i<vl.size(); i++)
        {
            conf_debug<<i <<"  leave time is " << vl[i];
            if(vb[i]==false)
            {
                conf_debug <<"  no use " <<endl;
                continue;
            }
            //fasle means on the road;

            if(vl[i]<vlmin)
            {
                vlmin = vl[i];
                index = i;
            }
            //conf_debug<<"######";
            conf_debug<<endl;
        }

        tuple<bool,size_t> res;

        conf_debug<<endl;
        if(vlmin<vamin)
        {
            std::get<0>(res) = false;
            std::get<1>(res) = index;
            if(index > 10)
            {
                conf_debug<<"wtf"<<endl;
            }
            conf_debug << "cal leave  id  = "<<index <<endl;
        }
        else
        {
            std::get<0>(res) = true;
            std::get<1>(res) = indexCar;
            if(indexCar > 10)
            {
                conf_debug<<"wtf"<<endl;
            }
            conf_debug << "cal arrive  id  = "<< indexCar <<endl;
        }

        return res;
    };

    for(size_t i = 0; i <vScoutCar.size(); i++)
    {
        double  initArriveTime = vScoutCar[i].getArriveTime(mpop.at(i)[vEncodeIndex[i]]);
        varriveTime[i] = initArriveTime;
        vpathTime[i] = initArriveTime;
        arriveMat[i][mpop.at(i)[vEncodeIndex[i]]._id] = initArriveTime;
    }

    //
    vector<CarTime> cartimeUnit;
    vector<vector<CarTime>> carTimeMat;
    carTimeMat.assign(_vScoutCar.size(),cartimeUnit);

    for(size_t i = 0 ; i < vScoutCar.size(); i++)
    {
        CarTime timeUnit;
        //timeUnit._leaveForwardTime = 0;

    }


    //carTimeMat.push_back();





    double fitness;
    bool invaildFitness = false;
    // cal function

    while(complete(vboolTaskPnt))
    {

        if(!complete(vEncodeEndBoolean))
        {
            invaildFitness = true;
            break;
            //break;
            //goto invaildFitness;
        }
        //auto motionId = findMinIndex(varriveTime);
        //vboolean[motionId]  = true;


        tuple<bool,size_t> markType = findMotionOrLeaveId(varriveTime,vleaveTime,vLeaveOrArrive,vAvaildBool);

        if(std::get<0>(markType))
        {
            size_t motionId = std::get<1>(markType);
            vboolean[motionId]  = true;

            size_t i = motionId;
            //vboolean]
            //            for(size_t i = 0; i < vScoutCar.size(); i++)
            //            {
            //                // can update
            //                if(vboolean.at(i))
            //                {
            auto &car = vScoutCar[i];
            auto &carChrom =  mpop.at(i);
            auto &eEncode = carChrom[vEncodeIndex[i]];
            auto &taskid = eEncode._id;
            auto &dur  = eEncode._dur;
            auto &carid = i;
            vdur[i] = dur;

            cout<<"carid = " <<carid <<endl;
            cout<< "taskid = " << taskid<<endl;

            conf_debug<<endl;
            conf_debug<<"carid = " <<carid <<endl;
            conf_debug<< "taskid = " << taskid<<endl;

            //cout<<carid
            //carChrom[vEncodeIndex[i]

            if(abs(1-dur)<0.001)
            {
                // stay the task pnt until end;
                vboolEnd[carid] = true;
                cout <<"stay the task pnt until end"<< endl;
                conf_debug<<"stay the task pnt until end"<< endl;

            }
            else
            {
                vboolEnd[carid] = false;
                cout <<"not stay the task pnt until end"<< endl;
                conf_debug<<" not stay the task pnt until end"<< endl;
            }

            vboolean[carid] = false;

            //double arriveTime = car.getArriveTime(eEncode);

            double arriveTime = varriveTime.at(carid);

            //arriveTime += vleaveTime.at(i);

            cout<<"the arrive time is "<< arriveTime<<endl;
            conf_debug<<"the arrive time is "<< arriveTime<<endl;

            double leaveTime;

            if(vboolTaskPnt.at(eEncode._id))
            {
                //complete
                leaveTime = arriveTime;
                vleaveTime[carid] = arriveTime;
                vactTime[carid] = 0;

                if(vactTime[carid] ==0)
                {
                    conf_debug<<"wtf"<<endl;
                }


            }
            else
            {
                auto vIn   = findInTask(eEncode._id,m_mat);
                //no coordination
                if(vIn.empty())
                {
                    conf_debug <<" the current state is " <<vState[taskid] <<endl;
                    //update
                    double actDur  = vState.at(taskid)*dur;
                    leaveTime  = arriveTime + actDur;
                    cout<<" no coordination "<< endl;

                    vleaveTime[carid]  = leaveTime;
                    vactTime[carid] = actDur;

                    if(vactTime[carid] ==0)
                    {
                        conf_debug<<"wtf"<<endl;
                    }

                    conf_debug<<" no coordination "<< endl;
                }
                else
                {
                    // coordination
                    double workedState = 0;
                    auto coordSize = vIn.size() + 1;

                    conf_debug <<" task id " << taskid <<" has coordinated "<<endl;
                    conf_debug <<" the current state is " <<vState[taskid] <<endl;

                    vector<size_t> vbcarId;
                    //                            list<size_t> vacarId;
                    //vacarId.push_back(taskid);
                    for(auto &it : vIn)
                    {
                        conf_debug<<"  coordination id  =  "<<  it << endl;
                        cout<<"  coordination id  =  "<<  it << endl;
                        double stateUnit = arriveTime - arriveMat[it][taskid];

                        conf_debug << "arriveTime " << arriveTime;
                        conf_debug << " it id " <<it <<endl;
                        if(stateUnit<0)
                        {
                            conf_debug<<"wtf"<<endl;
                        }
                        conf_debug <<" co arrive mat " <<arriveMat[it][taskid]<<endl;
                        workedState += stateUnit;
                        conf_debug <<"before car id " <<it <<endl;
                        vbcarId.push_back(it);
                    }

                    conf_debug << " task pnt has workedState is " << workedState << endl;
                    if(workedState > 20)
                    {
                        conf_debug<<"&&&"<<endl;
                    }
                    if(workedState>vState[taskid])
                    {
                        leaveTime = arriveTime;
                        //

                        conf_debug<<"debug" <<endl;
                        conf_debug <<"arrive this task pnt has been completed "<<endl;
                        //goto assLeaveTime;
                    }


                    double updateLeaveTime = (vState[taskid] - workedState)/coordSize +varriveTime[carid];

                    conf_debug<<"predict completed time is " << updateLeaveTime<<endl;
                    vactTime[carid] = (vState[taskid] -workedState)/coordSize *dur ;

                    if(vactTime[carid] == 0)
                    {
                        conf_debug<<"wtf"<<endl;
                    }

                    leaveTime = vactTime[carid] + varriveTime[carid];
                    vleaveTime[carid] =  leaveTime;
                    for (auto &it :vbcarId)
                    {
                        //double updateLeaveTime =leaveTime - varriveTime[it];
                        double updateDur = updateLeaveTime - arriveMat[it][taskid];

                        conf_debug<<"updateDur = " << updateDur <<endl;
                        double actDur  = updateDur*vdur[it];

                        if(vactTime[it] <0)
                        {
                            conf_debug<<"wtf"<<endl;
                        }
                        conf_debug<<"arrive it time is "<<arriveMat[it][taskid]<<endl;
                        double coLeaveTime =  actDur + arriveMat[it][taskid];

                        if(coLeaveTime<arriveTime)
                        {
                            actDur = arriveTime - arriveMat[it][taskid];
                            conf_debug<< "actDur = " <<actDur<<endl;
                            vactTime[it] = actDur;
                            actMat[it][taskid] = actDur;
                            conf_debug <<"id = " << it <<"  coordination leave time is "<< coLeaveTime<<endl;
                            vleaveTime[it] = coLeaveTime;
                            varriveTime[it] = coLeaveTime + vpathTime[it];
                        }
                        else{
                            conf_debug<< "actDur = " <<actDur<<endl;
                            vactTime[it] = actDur;
                            actMat[it][taskid] = actDur;

                            //varriveTime[it];
                            conf_debug <<"id = " << it <<"  coordination leave time is "<< coLeaveTime<<endl;
                            vleaveTime[it] = coLeaveTime;
                            varriveTime[it] = coLeaveTime + vpathTime[it];
                        }
                    }

                }
            }
assLeaveTime:
            DirPnt dirpnt  = car.encode2DirPoint(eEncode);
            bgeo::DPoint pnt = std::get<0>(dirpnt);
            car.setPos(pnt);
            m_mat[taskid][carid] = true;

            conf_debug <<" the leave time is "<< leaveTime <<endl;
            cout<<"the leave time is "<< leaveTime <<endl;
            // _vScoutCar[i].getTime();
            //if(vEncodeIndex[i])
            //double dur = _vScoutCar[i].getTime(vEncodeIndex[i]);
            vLeaveOrArrive[carid] = true;

            vAvaildBool[carid] = false;
        }
        else
        {
            //auto motionId = findMinIndex(vleaveTime);
            size_t leaveId = std::get<1>(markType);
            vboolean[leaveId] =  true;
            auto &carChrom =  mpop.at(leaveId);
            auto &eEncode = carChrom[vEncodeIndex[leaveId]];
            auto &taskid = eEncode._id;
            m_mat[taskid][leaveId] = false;

            for(size_t i = 0;i<_vTarget.size();i++)
            {
                conf_debug<<"task id" <<i;
                for(size_t j = 0; j<_vScoutCar.size();j++)
                {
                    if(m_mat[i][j]==true)
                    {
                        conf_debug<< " car "<<j <<" ";
                    }
                }
                conf_debug<<endl;
            }

            conf_debug <<endl;
            conf_debug << " leave condition " <<endl;

            conf_debug << "the car id is " << leaveId <<endl;
            if(vboolEnd[leaveId])
            {
                cout <<" task id " << taskid <<" has been done "<<endl;
                conf_debug <<" task id " << taskid <<" has bindexCareen done "<<endl;
                vboolTaskPnt[taskid] = true;

                double tes     = vState[taskid] - vactTime[leaveId];
                vState[taskid]   = tes;
            }
            else
            {
                double tes     = vState[taskid] - vactTime[leaveId];
                if(tes>20)
                {
                    conf_debug<<"wtf"<<endl;
                }
                else
                {
                    vState[taskid]   = tes;
                }

                if(vState[taskid]<0)
                {
                    conf_debug<<"wtef"<<endl;
                }
                conf_debug <<" task id " << taskid <<" has not been done "<<endl;
                conf_debug <<" the cleaveTime state is " <<vState[taskid] <<endl;
            }
            vLeaveOrArrive[leaveId] = false;

            vEncodeIndex[leaveId]++;
            actMat[leaveId][taskid]  = vactTime[leaveId];

            conf_debug<< "carid = " <<leaveId <<"the actTime is " <<vactTime[leaveId]<<endl;

            conf_debug<<"EncodeIndex =  " << vEncodeIndex[leaveId] <<endl;
            if(vEncodeIndex[leaveId] == _vTarget.size())
            {
                vEncodeEndBoolean[leaveId] = true;
                conf_debug<<"stop"<<endl;
                for (size_t i = 0; i<vState.size(); i++)
                {
                    conf_debug<<" the task pnt vState id "<<i <<" val = "<<vState[i] << endl;
                }

                for(size_t i = 0 ; i< actMat.size(); i++)
                {
                    auto &arow = actMat[i];
                    conf_debug<<" carid "<<i;
                    for(size_t j = 0; j< arow.size(); j++)
                    {
                        conf_debug<<" pntId "<<j << "  actTime  " <<actMat[i][j];
                    }
                    conf_debug<<endl;
                }

                conf_debug<<"ttsdkahs"<<endl;
                continue;
                // break;
            }
            auto & tencode = carChrom[vEncodeIndex[leaveId]];
            double pathDur = vScoutCar[leaveId].getArriveTime(tencode);
            conf_debug <<" the path dur is " << pathDur <<endl;
            vpathTime[leaveId] = pathDur;
            varriveTime[leaveId] = vleaveTime[leaveId] + pathDur;
            arriveMat[leaveId][carChrom[vEncodeIndex[leaveId]]._id] = varriveTime[leaveId];
            vAvaildBool[leaveId] = true;

        }
    }
    //auto fitnessInter;
    if(invaildFitness)
    {
        conf_debug<<"fitness is invaild"<<endl;
        return 99999999;
    }
    auto fitnessInter = std::max_element(vleaveTime.begin(),vleaveTime.end());
    conf_debug << "fitness is "<<*fitnessInter <<endl;
    return *fitnessInter;
    //    invaildFitness :
    //    //
    //    fitness = 9999999;
    //    return fitness;

}

double TaskPlanner::rationalize( vector<Chrom> &mpop)
{
    vector<bool> m_list;
    m_list.assign(_vTarget.size(),false);

    auto terminateCondition  = [](vector<bool>  const &vl)
    {
        auto ite = std::find(vl.begin(),vl.end(),false);
        if(ite == vl.end())
        {
            return false;
        }
        else
        {
            return true;
        }
    };

    while(terminateCondition(m_list))
    {
        std::uniform_int_distribution<int> disCar(0,_vScoutCar.size()-1);
        std::uniform_int_distribution<int> disTarget(0,_vTarget.size()-1);
        //i])

        size_t carid = disCar(enge);
        size_t targetid = disTarget(enge);
        auto &en_code = mpop[carid][targetid];
        if(!m_list[en_code._id])
        {
            en_code._dur = 1;
            m_list[en_code._id] = true;
        }
    };
}


void TaskPlanner::generatePop(const size_t &mpop_num)
{

    vector<double> vfitness;
    for(size_t p = 0 ; p<500 ;p++)
    {
        conf_debug <<"/////////////////////////"<<p<<endl;
        _pop.clear();
        for(size_t i = 0; i < _vScoutCar.size() ; i++)
        {
            conf_debug<<" index encode is " << i <<std::endl;
            _pop.push_back(generateChrom());
        }
        rationalize(_pop);
        //
         double fitness = decode(_pop);
         vfitness.push_back(fitness);
    }

    auto ite  = std::min_element(vfitness.begin(),vfitness.end());
    conf_debug<<"^^^^^^^^^^^^^^^^^^^^^^^"<<endl;
    conf_debug<<"min fitness is "<<*ite<<endl;
    std::cout<<"####################"<<endl;
    std::cout<<"min fitness is "<<*ite<<endl;
}

bool TaskPlanner::initConnect()
{
    vector<netAgent::client_data>  data_list;
    //data_list.push_back;
    auto  size = _vScoutCar.size();
    for(size_t i = 0 ; i< this->_vScoutCar.size(); i++)
    {
        netAgent::client_data _data;
        _data.ID = i;
        _data.port_pub  = portPubBase + i;
        _data.port_rep = portRepBase +i;
        string str_ip = "127.0.0.1";
        for(size_t j= 0; j<str_ip.size();j++)
        {
            _data.ip[j] =str_ip.at(j);
        }
        _data.ip[str_ip.size()] = '\0';
        //_data.ip = str_ip.data();
        data_list.push_back(_data);
        //data_list.push_back({"127.0.0.1", i, portPubBase + i, portRepBase + i });
    }
    auto myID = _agent.getRobotId();
    netAgent::client_data &myData = data_list.at(myID);

    _cntPtr = std::make_shared<netAgent::Agent>(myData);
    _cntPtr->server_start();
    _cntPtr->connect_to(data_list,{myID});

    _cntPtr->handle_broadcast = [=](int ID, int type, void* data, size_t size){return this->onBroadcast(ID, type, data, size);};
    _cntPtr->handle_request = [=](int ID, int type, void* data, size_t size, netAgent::ReplyInterface reply){
        return this->onRequest(ID,type,data,size,reply);};

    _cntPtr->start(netAgent::all_thread);
    conf_debug <<" succces in id  " << myID << endl;

    std::cout <<" success in id "<< myID <<std::endl;

    char wtf[20];
    string str_ip = "127.0.0.0";
    for(size_t j =0 ;j<str_ip.size();j++)
    {
        wtf[j] = str_ip.at(j);
    }

    _cntPtr->handle_broadcast(1,1,wtf,str_ip.size());
    for(size_t i = 0 ;i <size; i++)
    {
        if(i == myID) continue;
        auto res = _cntPtr->request(i,0,"ggq test");
        std::cout <<" myID  = "<< myID <<" res  = "<<res <<std::endl;
        conf_debug <<" myID  " << myID <<  "res = " <<res <<std::endl;
        sleep(1);
    }

    for(size_t i = 0; i<2;i++)
    {
        std::cout<<"sended"<< myID <<std::endl;
        std::string str =  "test GGQ";
        //auto  msg  = str.c_str();
        auto res = _cntPtr->broadcast(110,str);
        std::cout <<" broadCast " <<" myID  = "<< myID <<" res  = "<<str <<std::endl;
        conf_debug <<" broadCast " <<" myID  = "<< myID <<" str  = "<<str <<std::endl;
        sleep(1);
    }

    this->generateChrom();
    return true;
}

bool TaskPlanner::sendMessage(const size_t myid)
{
    if(_vScoutCar.empty())
    {
        return false;
    }
    return true;
}

Chrom TaskPlanner::generateChrom()
{
    auto size = this->_vTarget.size();
    auto aRand = randN(size);
    std::uniform_real_distribution<double> dis(0.0,1.0);
    Chrom chrom;
    for(size_t i = 0;i<size;i++)
    {
        Encode encode;
        auto angle = dis(enge)*M_PI*2;
        encode._angle = angle;
        encode._dur = dis(enge);
        // the encode is 1
        //encode._dur = 1;

        encode._id = aRand.at(i);
        std::cout<<"Encode:"<<encode;
        //this->_vEncode.push_back(encode);
        chrom.push_back(encode);
        conf_debug << "Encode index is "<<i << encode<<std::endl;
    }

    return chrom;

}
