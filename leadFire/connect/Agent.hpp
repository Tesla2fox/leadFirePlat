#pragma once
#pragma warning( disable : 4996)
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <map>
#include <memory>
#include <functional>
#include <set>
#include <atomic>
///#define ZMQ_CPP11
#include "zmq.hpp"
#include <cstdio>


#define __STDC_WANT_LIB_EXT1__ 1


// #define snprintf _snprintf


namespace netAgent{
struct client_data;
class Agent;
class ReplyInterface;

using repHandler_t = std::function<void(int ID, int type, void* data, size_t size, ReplyInterface reply)>;


using subHandler_t = std::function<void(int ID, int type, void* data, size_t size)>;



/*
 * ����
*/
//����λ��
enum thread_type{
	rep_thread = 1,
	sub_thread = 2,
	all_thread = rep_thread | sub_thread
};

/*
 * �����ṹ
*/
struct client_data{
    char ip[32];
	int  ID;
	int  port_pub;
	int  port_rep;
	client_data& operator = (const client_data& b);
};

//client_data::client_data(char * ptrc,int m_id,int m_port_pub,int m_port_rep )
//{
//    ID = m_id;
//    port_pub = m_port_pub;
//    port_rep = m_port_rep;
//}

inline client_data& client_data::operator = (const client_data& b){
	// window
	// strcpy_s(ip, 32, b.ip);	
	// strcmp(ip,b.ip,32);
	strncpy(ip,b.ip,32);
	ID = b.ID;
	port_pub = b.port_pub;
	port_rep = b.port_rep;
	return *this;
}
inline std::ostream& operator << (std::ostream& os, const client_data& dat){
	os << "ID=" << dat.ID << ", ip=" << dat.ip << ", pub=" << dat.port_pub << ", rep=" << dat.port_rep;
	return os;
}

// ��һЩʵ��ʱ�õĶ�����װ��
// �����һ�������ռ���
// �Ա���netAgent������
namespace _tool{
using zmq::socket_t;
using zmq::context_t;
using zmq::message_t;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::thread;
using std::map;
using std::move;
using std::unique_ptr;

struct thread_control{
	enum {
		stop
	};
};

// ��������thread����Ϣ�ṹ
struct thread_ctl_msg{
	int thread;
	int cmd;
};

// req->rep��head_msg
struct req_head_msg{
	int ID;
	int type;
};

// rep->req��head_msg
struct rep_head_msg{
	int msg;
};

// pub->sub��head_msg
struct pub_head_msg{
	int type;
};
	

struct client_socket{
	client_socket(client_socket&& s) :
		sub(move(s.sub)), req(move(s.req)), info(s.info), valid(s.valid){}
	client_socket(zmq::context_t& ctx, client_data& data) :
		sub(ctx, ZMQ_SUB), req(ctx, ZMQ_REQ), info(data), valid(true){}

	zmq::socket_t sub;
	zmq::socket_t req;
	client_data info;
	bool valid;
};

// printf -> string
template<typename ... Args>
string format(const char* format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format, args ...) + 1; // Extra space for '\0'
    //string ret;
    //ret.resize(size);
	unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format, args ...);
    //snprintf((char*)ret.data(), size-1, format, args...);
    //	return ret;
    return string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

// return "tcp://ip:port"
inline string get_ipstr(const char* ip, int port, const char* protocal = "tcp"){
	if (!ip) return "";
	return format("%s://%s:%04d", protocal, ip, port);
}

// return "ID=01" etc...
inline string get_idstr(int id){
	return format("ID=%02d", id);
}
}

class ReplyInterface{
public:
	ReplyInterface(zmq::socket_t& socket)
		: rep(socket){}

	int send(int msg){
		return rep.send(&msg, sizeof(int));
	}
	int send(const std::string& msg){
		rep.send(nullptr, 0, ZMQ_SNDMORE);
		return rep.send((void*)msg.data(), msg.size());
	}
	int send(void* data, size_t size){
		rep.send(nullptr, 0, ZMQ_SNDMORE);
		return rep.send(data, size);
	}
private:
	zmq::socket_t& rep;
};

class Agent{
public:
	typedef int		ID_t;
	typedef size_t  typeIndex;

	Agent();
	Agent(const client_data& my_dat);
	~Agent();

	zmq::context_t& context() { return ctx; }

	// 1. ������������
	int req_timeout() const    { return rcv_tout; }
	void config_req(int tout, bool auto_reconnect = true) { 
		rcv_tout = tout; 
		auto_recon = auto_reconnect;
	}

	// 2. ����Server (��������Agent.me)
	int server_start();

	// 3. ����������
	int connect_to(const client_data& client);
	// ����client_list�г���ID��except�е�����client
	int connect_to(const std::vector<client_data>& client_list, const std::set<int>& except = {});

	// Broadcast(pub)
	// ���ص���zmq::send�ķ���ֵ
	int broadcast(int msg_type);
	int broadcast(int msg_type, const std::string& str);
	int broadcast(int msg_type, void* data, size_t size);

	// Reqest(req)
	// �յ����ǶԷ����ص�reply�ĵ�һ����(һ���Ǹ���)
	int request(int targetID, int req_type);
	int request(int targetID, int req_type, const std::string& msg);
	int request(int targetID, int req_type, void* data, size_t size);

	// ����еڶ����ֵĻ�   ���صڶ����ֵ�message
	// ���û�еڶ����ֵĻ� ���ؿյ�message
	zmq::message_t&& get_more_reply();
	// ����reply message�Ĵ�С
	size_t get_reply_size() const;
	// ���T�Ĵ�С��reply messageһ���򷵻�dataָ�� ���򷵻�nullptr
	template<class T> T* get_more_reply();

	// Threads Control
	void start(int option = all_thread);	// ��ʼĳ���߳�			 (������)
	void stop(int option  = all_thread);	// ����ֹͣ���߳̽���	 (������)
	void join(int option  = all_thread);	// �ȴ����߳̽���		 (����)

	// �����иı�rep/sub��Functor
	void change_requst_handler(repHandler_t new_request_handler);
	void change_broadcast_handler(subHandler_t new_broadcast_handler);

protected:
	int on_broadcast();	// sub�߳�
	int on_request();	// rep�߳�

public:
	repHandler_t handle_request   = nullptr;	// rep��Ӧ��Functor
	subHandler_t handle_broadcast = nullptr;	// sub��Ӧ��Functor
	
	client_data me;

protected:
	using client_socket = _tool::client_socket;

	std::atomic_bool valid_request_hnd;		// handle_request�ǲ�����Ч(����nullʱ)
	std::atomic_bool valid_broad_hnd;		// handle_broadcast�ǲ�����Ч(����nullʱ)

	std::map<ID_t, typeIndex>   client_map;		// index of client_list
	std::vector<client_socket>  socket_list;
	int  rcv_tout = -1;
	bool auto_recon = true;

	zmq::context_t ctx  = zmq::context_t(1);
	zmq::socket_t  pub  = zmq::socket_t(ctx, ZMQ_PUB);	// publish data
	zmq::socket_t  rep  = zmq::socket_t(ctx, ZMQ_REP);	// answer specific requests
	zmq::message_t msg_replied;	// ��ʱ

	// �����ڲ��߳�
	zmq::socket_t ctl = zmq::socket_t(ctx, ZMQ_PUB);	// for internal control
	std::thread rcv_rep;		// used for reply requests
	std::thread rcv_sub;		// used for deal with subscriptions
};

inline Agent::~Agent(){ 
	stop();
	join();

	// ֻҪ��һ��socketû��close�ͻ�HANG!!!
	ctl.close();
	pub.close();
	rep.close();
	for (auto& s : socket_list){
		s.req.close();
		s.sub.close();
	}
}


// Broadcast(pub)
inline int Agent::broadcast(int msg_type){
	return broadcast(msg_type, nullptr, 0);
}
inline int Agent::broadcast(int msg_type, const std::string& str){
	return broadcast(msg_type, (void*)str.data(), str.size());
}

// Request (req)
inline int Agent::request(int targetID, int req_type){
	return request(targetID, req_type, nullptr, 0);
}
inline int Agent::request(int targetID, int req_type, const std::string& msg){
	return request(targetID, req_type, (void*)msg.data(), msg.size());
}
inline zmq::message_t&& Agent::get_more_reply(){
	return std::move(msg_replied);
}

inline size_t Agent::get_reply_size()const{
	return msg_replied.size();
}

template<class T> 
inline T* Agent::get_more_reply(){
	return sizeof(T) == msg_replied.size() ? static_cast<T*>(msg_replied.data())
										   : nullptr;
}

// �߳̿���
inline void Agent::start(int option){
	if ((option & rep_thread) && !rcv_rep.joinable()) rcv_rep = std::thread(&Agent::on_request, this);
	if ((option & sub_thread) && !rcv_sub.joinable()) rcv_sub = std::thread(&Agent::on_broadcast, this);
}
inline void Agent::stop(int option){
	using _tool::thread_control;
	using _tool::thread_ctl_msg;

	thread_ctl_msg msg;
	msg.cmd = thread_control::stop;
	if (option & rep_thread){
		msg.thread = rep_thread;
		ctl.send(&msg, sizeof(thread_ctl_msg));
	}
	if (option & sub_thread){
		msg.thread = sub_thread;
		ctl.send(&msg, sizeof(thread_ctl_msg));
	}
}
inline void Agent::join(int option){
	if ((option & rep_thread) && rcv_rep.joinable()) rcv_rep.join();
	if ((option & sub_thread) && rcv_sub.joinable()) rcv_sub.join();
}

// �����иı�rep/sub��Functor
inline void Agent::change_requst_handler(repHandler_t new_request_handler){
	valid_request_hnd = false;
	handle_request = new_request_handler;
	valid_request_hnd = true;
}
inline void Agent::change_broadcast_handler(subHandler_t new_broadcast_handler){
	valid_broad_hnd = false;
	handle_broadcast = new_broadcast_handler;
	valid_broad_hnd = true;
}

}
