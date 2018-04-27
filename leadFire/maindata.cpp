#include "maindata.h"


//std::vector<ScoutCar> ex_ScoutCar;

MainData::MainData()
{

}

void MainData::writeXml()
{

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("2.0", "", "");
    doc.LinkEndChild(decl);

    time_t tt=std::time(NULL);
    std::tm *t=std::localtime(&tt);

    auto sDay=std::to_string(t->tm_mday);
    auto sMonth = std::to_string(t->tm_mon+1);
    auto sHour = std::to_string(t->tm_hour);
    std::string st_time = sMonth + "." + sDay +" hour:" +sHour;
    const char * c_time = st_time.c_str();

    TiXmlElement * time = new TiXmlElement("time");
    doc.LinkEndChild(time);

    TiXmlComment * commentTime = new TiXmlComment();
    commentTime->SetValue(c_time);
    time->LinkEndChild(commentTime);



    //save the attacked Pnt
    {
        TiXmlElement *Aroot  = new TiXmlElement ("attackedPnt");
        doc.LinkEndChild(Aroot);

        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue("the attcaked pnt");
        Aroot->LinkEndChild(comment);

        TiXmlElement * vert = new TiXmlElement("Vert");
        Aroot->LinkEndChild(vert);

        std::string str_x  = std::to_string(_attackedPnt.x());
        std::string str_y  = std::to_string(_attackedPnt.y());
        //vert->SetAttribute("x",str_x);
        //vert->SetAttribute("y",str_y);
        vert->SetDoubleAttribute("x",_attackedPnt.x());
        vert->SetDoubleAttribute("y",_attackedPnt.y());
    }
    //save the workSpace
    {
        TiXmlElement *Wroot  = new TiXmlElement ("workSpace");
        doc.LinkEndChild(Wroot);

        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue("the work space ");
        Wroot->LinkEndChild(comment);

        for(auto &it:_wspace)
        {
            TiXmlElement * vert = new TiXmlElement("Vert");
            Wroot->LinkEndChild(vert);

            vert->SetDoubleAttribute("x",it.x());
            vert->SetDoubleAttribute("y",it.y());
        }

    }

    //save the targetPnt
    {
        TiXmlElement *Troot  = new TiXmlElement ("TargetPnt");
        doc.LinkEndChild(Troot);

        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue("the vector of the target pnt");
        Troot->LinkEndChild(comment);


        for(auto &it :_vTarget)
        {
            TiXmlElement * vert = new TiXmlElement("Vert");
            Troot->LinkEndChild(vert);
            std::string str_x  = std::to_string(it.x());
            std::string str_y  = std::to_string(it.y());

            vert->SetDoubleAttribute("x",it.x());
            vert->SetDoubleAttribute("y",it.x());
        }
    }
    // save the ScoutCar
    {
        TiXmlElement *Sroot  = new TiXmlElement ("ScoutCar");
        doc.LinkEndChild(Sroot);

        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue("the vector of the Scout Car");
        Sroot->LinkEndChild(comment);


        for(auto &it :_vScoutCar)
        {
            TiXmlElement * vert = new TiXmlElement("Vert");
            Sroot->LinkEndChild(vert);
            double d_x,d_y;
            it.getPos(d_x,d_y);

            std::string str_x  = std::to_string(d_x);
            std::string str_y  = std::to_string(d_y);
            std::string str_edge = std::to_string(it.getEdge());

            std::string vert_x = "x";
            std::string vert_y = "y";
            std::string vert_edge = "edge";

            vert->SetDoubleAttribute("x",d_x);
            vert->SetDoubleAttribute("y",d_y);
            vert->SetDoubleAttribute("edge",it.getEdge());
            vert->SetDoubleAttribute("ori",it.getOrient());
        }
    }

    {
        TiXmlElement *Rroot  = new TiXmlElement ("Ring");
        doc.LinkEndChild(Rroot);

        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue("the vector of the ob ring");
        Rroot->LinkEndChild(comment);


        size_t  i = 0;
        for(auto &it: *_vRingPtr)
        {
            //i++;

            for(size_t j = 0; j<it.size();j++)
            {
                TiXmlElement * vert = new TiXmlElement("Vert");
                Rroot->LinkEndChild(vert);
                vert->SetAttribute("id",i);
                vert->SetDoubleAttribute("x",it.at(j).x());
                vert->SetDoubleAttribute("y",it.at(j).y());
                //                vert->SetDoubleAttribute("x",it.getEdge());
            }
            i++;
        }
    }


    //    // save the map

    //    {

    //    //        TiXmlElement *Oroot  = new TiXmlElement ("TargetPnt");
    ////        doc.LinkEndChild(Troot);

    ////        TiXmlComment * comment = new TiXmlComment();
    ////        comment->SetValue("the vector of the target pnt");
    ////        Sroot->LinkEndChild(Troot);

    //    }


    doc.SaveFile("data.xml");
}

void MainData::loadXml(const char *pFileName)
{
    TiXmlDocument doc(pFileName);
    bool loadOkay = doc.LoadFile();

    if (loadOkay)
    {
        TiXmlElement *RootElement = doc.RootElement();
        ///
        RootElement = RootElement->NextSiblingElement();
        //load attacked pnt
        {
            TiXmlElement *vert = RootElement->FirstChildElement();
            auto e_x  = std::atof(vert->Attribute("x"));
            auto e_y  = std::atof(vert->Attribute("y"));
            _attackedPnt.x(e_x);
            _attackedPnt.y(e_y);
        }

        //load workSpace

        {
            RootElement = RootElement->NextSiblingElement();
            TiXmlElement *vert = RootElement->FirstChildElement();
            for(vert;vert; vert =vert->NextSiblingElement())
            {
                auto e_x  = std::atof(vert->Attribute("x"));
                auto e_y  = std::atof(vert->Attribute("y"));
                bgeo::DPoint pnt;
                pnt.x(e_x);
                pnt.y(e_y);
                this->_wspace.push_back(pnt);
            }

        }
        //load target pnt
        _vTarget.clear();
        {
            RootElement = RootElement->NextSiblingElement();
            TiXmlElement *vert = RootElement->FirstChildElement();

            for(vert;vert; vert =vert->NextSiblingElement())
            {
                auto e_x  = std::atof(vert->Attribute("x"));
                auto e_y  = std::atof(vert->Attribute("y"));
                bgeo::DPoint pnt;
                pnt.x(e_x);
                pnt.y(e_y);
                ex_TargetPnt.push_back(pnt);
                _vTarget.push_back(pnt);
            }
        }


        //load ScoutCar

        {
            RootElement = RootElement->NextSiblingElement();

            TiXmlElement *vert = RootElement->FirstChildElement();

            _vScoutCar.clear();
            size_t i = 0;
            for(vert;vert; vert =vert->NextSiblingElement())
            {
                auto e_x  = std::atof(vert->Attribute("x"));
                auto e_y  = std::atof(vert->Attribute("y"));
                auto e_edge = std::atof(vert->Attribute("edge"));
                auto e_ori = std::atof(vert->Attribute("ori"));
                bgeo::DPoint pnt;
                pnt.x(e_x);
                pnt.y(e_y);

                double rad = 10;
                ScoutCar car (e_x,e_y,e_edge,e_ori,rad);
                car.setRobotId(i);
                i++;
                car.setVel(5);
                ex_ScoutCar.push_back(car);
                this->_vScoutCar.push_back(car);
            }

        }
        //load ring
        {
            size_t  _times = 0;
            RootElement = RootElement->NextSiblingElement();
            TiXmlElement *vert = RootElement->FirstChildElement();

            _vRingPtr->clear();

            size_t   m_id  = 0 ;
            do{
                bgeo::DRing ring;
                for(;vert; vert =vert->NextSiblingElement())
                {
                    auto e_id = std::atoi(vert->Attribute("id"));
                    if(e_id == m_id)
                    {
                        auto e_x  = std::atof(vert->Attribute("x"));
                        auto e_y  = std::atof(vert->Attribute("y"));
                        bgeo::DPoint pnt;
                        pnt.x(e_x);
                        pnt.y(e_y);
                        ring.push_back(pnt);
                        std::cout <<"times = "<<_times++ <<std::endl;
                    }
                    else
                    {
                        break;
                    }
                }
                m_id++;
                _vRingPtr->push_back(ring);
            }
            while(vert);

            std::cout <<"wtf" <<std::endl;
        }



    }
}

void MainData::initScoutCarEncode()
{
    for(auto &it:this->_vScoutCar)
    {
        it.setVTaget(this->_vTarget);
        it.initEncode(generatorType::randType);
        // greed type has big bug
        //it.initEncode(generatorType::greedType);
    }
}
