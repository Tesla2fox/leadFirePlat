#include "Agent.hpp"

namespace netAgent{

using namespace _tool;

void configREQ(socket_t& req, int rcv_tout = -1){
    req.setsockopt(ZMQ_SNDHWM, 2);
    req.setsockopt(ZMQ_RCVHWM, 10);
    req.setsockopt(ZMQ_LINGER, 0);		// ��ֹʱ������ֹ
    req.setsockopt(ZMQ_SNDTIMEO, -1);	// ����ʱ����
    req.setsockopt(ZMQ_RCVTIMEO, rcv_tout);	// ����ʱ����
}
void configREP(socket_t& req){
    req.setsockopt(ZMQ_SNDHWM, 20);
    req.setsockopt(ZMQ_RCVHWM, 20);
    req.setsockopt(ZMQ_LINGER, 0);		// ��ֹʱ������ֹ
    req.setsockopt(ZMQ_RCVTIMEO, -1);	// ����ʱ����
    req.setsockopt(ZMQ_SNDTIMEO, -1);	// ����ʱ����
}
void configPUB(socket_t& sub){
    sub.setsockopt(ZMQ_RCVHWM, 20);
    sub.setsockopt(ZMQ_LINGER, 0);		// ��ֹʱ������ֹ
    sub.setsockopt(ZMQ_SNDTIMEO, -1);	// ����ʱ����
}
void configSUB(socket_t& sub, int myID){
    sub.setsockopt(ZMQ_RCVHWM, 20);
    sub.setsockopt(ZMQ_LINGER, 0);		// ��ֹʱ������ֹ
    sub.setsockopt(ZMQ_SNDTIMEO, -1);	// ����ʱ����
    sub.setsockopt(ZMQ_SUBSCRIBE, nullptr, 0);	// ���������ź�

    string idstr = get_idstr(myID);
    if (idstr.length() != 5)
        throw std::logic_error("Invalid ID");
    sub.setsockopt(ZMQ_SUBSCRIBE, idstr.c_str(), 5);
}

Agent::Agent(){
    valid_request_hnd = true;
    valid_broad_hnd = true;

    ctl.setsockopt(ZMQ_LINGER, 0);
    ctl.bind("inproc://ctl");
}
Agent::Agent(const client_data& my_dat) : me(my_dat){
    ctl.setsockopt(ZMQ_LINGER, 0);
    ctl.bind("inproc://ctl");
    valid_request_hnd = true;
    valid_broad_hnd = true;
}

int Agent::server_start(){
    configREP(this->rep);
    configPUB(this->pub);
    rep.bind(get_ipstr(me.ip, me.port_rep));
    pub.bind(get_ipstr(me.ip, me.port_pub));
    return 0;
}

int Agent::connect_to(const client_data& _client){
    auto client = _client;
    int tarID = client.ID;
    auto it = client_map.find(tarID);
    if (it != client_map.end()){
        cout << "Client Already Connected" << endl;
        return -1;
    }
    client_map[tarID] = socket_list.size();
    socket_list.push_back(move(client_socket{ ctx, client }));	// Note, zmq sockets are not copy constructable
    // Must use move (and define rvalue constructor for client_socket)

    socket_t& req = socket_list.back().req;
    configREQ(req, rcv_tout);
    req.connect(get_ipstr(client.ip, client.port_rep));

    socket_t& sub = socket_list.back().sub;
    configSUB(sub, me.ID);
    sub.connect(get_ipstr(client.ip, client.port_pub));
    return 0;
}

int Agent::connect_to(const vector<client_data>& clients, const std::set<int>& except){
    int n = 0;
    for (auto& client : clients){
        if (except.find(client.ID) != except.end())
            continue;
        cout << "Connecting to " << client << endl;
        if (connect_to(client) != 0){
            cout << "Fail to connect to a client : " << client << endl;
        }
        else ++n;
    }
    return n;
}

void reconnect(zmq::context_t& ctx, client_socket& client, int rcv_tout = -1){
    client.req.close();
    client.req = std::move(zmq::socket_t(ctx, ZMQ_REQ));
    configREQ(client.req, rcv_tout);
    client.req.connect(get_ipstr(client.info.ip, client.info.port_rep));
}

int Agent::request(int targetID, int req_type, void* data, size_t size){
    // ����
    // �ж϶Է��Ƿ������ǵ�������

    //lunix
    auto it = client_map.find(targetID);

    //windows
    //auto& it = client_map.find(targetID);

    if (it == client_map.end())
        return -1;
    typeIndex index = client_map[targetID];
    socket_t& req = socket_list[index].req;

    // ����Ѿ�ʧЧ �Ͳ��ٷ���
    if (!socket_list[index].valid){
        if (auto_recon){
            if (rcv_tout != -1)
                reconnect(ctx, socket_list[index], rcv_tout * 2);
            else
                reconnect(ctx, socket_list[index], -1);
            socket_list[index].valid = true;
        }
        return -2;
    }

    // ������һ֡
    req_head_msg head;
    head.ID = me.ID;
    head.type = req_type;

    // ��������
    try{
        if (data == nullptr || size == 0)
            req.send(&head, sizeof(req_head_msg));
        else{
            req.send(&head, sizeof(req_head_msg), ZMQ_SNDMORE);
            req.send(data, size);
        }
    }
    catch (zmq::error_t e){
        cout << "Error zmq::" << e.what() << endl;
        return -1;
    }
    catch (std::exception e){
        cout << "Error std::" << e.what() << endl;
        return -1;
    }

    // ���Reply
    // ��һ֡
    message_t rep_head;
    if (!req.recv(&rep_head)){
        // ��ȡreq��ʱ...
        // ����socketΪ��Ч
        socket_list[index].valid = false;
        return -2;
    }

    // ��ú���֡ ֻ�洢���еĵ�һ֡ ��������֡
    vector<message_t> msg_list;
    while (req.getsockopt<int>(ZMQ_RCVMORE)){
        message_t msg;
        req.recv(&msg);
        msg_list.push_back(move(msg));
    }
    msg_replied = msg_list.empty() ? message_t()
                                   : move(msg_list[0]);

    // ȷ������ֵ
    if (rep_head.size() == sizeof(rep_head_msg)){
        rep_head_msg& head = *(rep_head_msg*)rep_head.data();
        return head.msg;
    }
    else if (rep_head.size() == 0){
        if (msg_list.empty()){
            // ֻ���ڶԷ��޷������repʱ
            // �Ż����ֻ��һ����֡�����
            return -1;
        }
        return 0;
    }
    else
        return -1;
}

int Agent::on_request(){
    socket_t ctl_sub(ctx, ZMQ_SUB);
    ctl_sub.setsockopt(ZMQ_LINGER, 0);			// ��ֹʱ������ֹ
    ctl_sub.setsockopt(ZMQ_SUBSCRIBE, "", 0);	// ����������Ϣ
    ctl_sub.connect("inproc://ctl");

    vector<zmq_pollitem_t> items(2);
    items[0].socket = ctl_sub;
    items[0].events = ZMQ_POLLIN;
    items[1].socket = rep;
    items[1].events = ZMQ_POLLIN;

    std::cout <<"on_request is open "<<std::endl;
    message_t msg;
    while (1){
        zmq::poll(items.data(), items.size(), -1);
        if (items[0].revents & ZMQ_POLLIN){
            message_t msg;
            ctl_sub.recv(&msg);
            if (msg.size() == sizeof(thread_ctl_msg)){
                thread_ctl_msg& cmd = *(thread_ctl_msg*)msg.data();
                if (cmd.thread == rep_thread &&
                        cmd.cmd    == thread_control::stop)
                    break;
            }
        }
        if (items[1].revents & ZMQ_POLLIN){
            // ��ȡ��һ֡
            message_t type_msg;
            rep.recv(&type_msg);

            // ��ȡ����֡
            vector<message_t> msg_list;
            while (rep.getsockopt<int>(ZMQ_RCVMORE)){
                message_t msg;
                rep.recv(&msg);
                msg_list.push_back(move(msg));
            }

            // �����һ֡
            if (type_msg.size() != sizeof(req_head_msg)){
                cout << "Invalid request header";
                rep.send(message_t());
            }
            req_head_msg& head = *(req_head_msg*)type_msg.data();
            int type = head.type;
            int ID	 = head.ID;

            // �������֡(�����û�)
            //if(valid_request_hnd)
            //{
            //                std::cout<<"debug"<<std::endl;
            //}
            //valid_request_hnd = true;
            if (valid_request_hnd && handle_request != nullptr){
                if (msg_list.empty())
                    handle_request(ID, type, nullptr, 0, ReplyInterface(rep));
                else{
                    message_t& tail = msg_list[0];
                    handle_request(ID, type, tail.data(), tail.size(), ReplyInterface(rep));
                }
            }
            else
                rep.send(message_t());
        }
    }
    ctl_sub.close();
    return 0;
}

int Agent::broadcast(int msg_type, void* data, size_t size){
    pub_head_msg head;
    head.type = msg_type;
    if (data == nullptr || size == 0)
        return pub.send(&head, sizeof(pub_head_msg));
    else{
        pub.send(&head, sizeof(pub_head_msg), ZMQ_SNDMORE);
        return pub.send(data, size);
    }
}

int Agent::on_broadcast(){
    socket_t ctl_sub(ctx, ZMQ_SUB);
    ctl_sub.setsockopt(ZMQ_LINGER, 0);		// ��ֹʱ������ֹ
    ctl_sub.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    ctl_sub.connect("inproc://ctl");

    vector<zmq_pollitem_t> items(socket_list.size() + 1);
    items[0] = { ctl_sub, 0, ZMQ_POLLIN, 0 };
    for (size_t i = 0; i < socket_list.size(); ++i)
        items[i + 1] = { socket_list[i].sub, 0, ZMQ_POLLIN, 0};

    while (1){
        zmq::poll(items.data(), items.size(), 500);
        if (items[0].revents & ZMQ_POLLIN){
            message_t msg;
            ctl_sub.recv(&msg);
            if (msg.size() == sizeof(thread_ctl_msg)){
                thread_ctl_msg& cmd = *(thread_ctl_msg*)msg.data();
                if (cmd.thread == sub_thread &&
                        cmd.cmd == thread_control::stop)
                    break;
            }
        }
        for (size_t i = 0; i < socket_list.size(); ++i){
            if (items[i + 1].revents & ZMQ_POLLIN){
                socket_t& skt = socket_list[i].sub;

                // ��ȡ��һ֡
                message_t type_msg;
                skt.recv(&type_msg);

                // ��ȡ����֡
                vector<message_t> msg_list;
                while (skt.getsockopt<int>(ZMQ_RCVMORE)){
                    message_t msg;
                    skt.recv(&msg);
                    msg_list.push_back(move(msg));
                }

                // �����һ֡
                if (type_msg.size() != sizeof(pub_head_msg)){
                    cout << "Invalid request header";
                }
                pub_head_msg& head = *(pub_head_msg*)type_msg.data();
                int type = head.type;
                int ID = socket_list[i].info.ID;

                // ����ڶ�֡(�����û�) ��������֡
                if (valid_broad_hnd && handle_broadcast != nullptr){
                    if (msg_list.empty())
                        handle_broadcast(ID, type, nullptr, 0);
                    else{
                        message_t& tail = msg_list[0];
                        handle_broadcast(ID, type, tail.data(), tail.size());
                    }
                }
            }
        }
    }
    ctl_sub.close();
    return 0;
}



}
