/////////////////////////////////////////////////////////////////////
// Comm.h - message-passing communication facility                 //
// ver 1.0                                                         //
// Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017    //
/////////////////////////////////////////////////////////////////////

#include "Comm.h"
#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <iostream>
#include <functional>
#include <conio.h>

using namespace MsgPassingCommunication;
using namespace Sockets;
//using SUtils = Utilities::StringHelper;

//----< constructor sets port >--------------------------------------

Receiver::Receiver(EndPoint ep, const std::string& name) : listener(ep.port), rcvrName(name)
{
}
//----< returns reference to receive queue >-------------------------

BlockingQueue<Message>* Receiver::queue()
{
  return &rcvQ;
}
//----< starts listener thread running callable object >-------------

template<typename CallableObject>
void Receiver::start(CallableObject& co)
{
  listener.start(co);
}
//----< stops listener thread >--------------------------------------

void Receiver::stop()
{
  listener.stop();
}
//----< retrieves received message >---------------------------------

Message Receiver::getMessage()
{
  return rcvQ.deQ();
}
//----< constructor initializes endpoint object >--------------------

Sender::Sender(const std::string& name) : sndrName(name)
{
  lastEP = EndPoint();  // used to detect change in destination
}
//----< destructor waits for send thread to terminate >--------------

Sender::~Sender()
{
  if (sendThread.joinable())
    sendThread.join();
}
//----< starts send thread deQ, inspect, and send loop >-------------

void Sender::start()
{
  std::function <void()> threadProc = [&]() {
    while (true)
    {
      Message msg = sndQ.deQ();

      if (msg.command() == "quit")
      {
        return;
      }
      std::string msgStr = msg.toString();

      if (msg.to().address != lastEP.address || msg.to().port != lastEP.port)
      {
        connecter.shutDown();
        //connecter.close();
        if (!connect(msg.to()))
        {
          continue;
        }
      }
      bool sendRslt = connecter.send(msgStr.length(), (Socket::byte*)msgStr.c_str());
    }
  };
  std::thread t(threadProc);
  sendThread = std::move(t);
}
//----< stops send thread by posting quit message >------------------

void Sender::stop()
{
  Message msg;
  msg.name("quit");
  msg.command("quit");
  postMessage(msg);
  connecter.shutDown();
}
//----< attempts to connect to endpoint ep >-------------------------

bool Sender::connect(EndPoint ep)
{
  lastEP = ep;
  return connecter.connect(ep.address, ep.port);
}
//----< posts message to send queue >--------------------------------

void Sender::postMessage(Message msg)
{
  sndQ.enQ(msg);
}
//----< sends binary file >------------------------------------------
/*
*  - not implemented yet
*/
bool Sender::sendFile(const std::string& fileName)
{
  return false;
}
//----< callable object posts incoming message to rcvQ >-------------
/*
*  This is ClientHandler for receiving messages and posting
*  to the receive queue.
*/
class ClientHandler
{
public:
  //----< acquire reference to shared rcvQ >-------------------------

  ClientHandler(BlockingQueue<Message>* pQ, const std::string& name = "clientHandler") : pQ_(pQ), clientHandlerName(name)
  {
  }
  //----< shutdown message >-----------------------------------------

  ~ClientHandler() 
  { 
  }
  //----< set BlockingQueue >----------------------------------------

  void setQueue(BlockingQueue<Message>* pQ)
  {
    pQ_ = pQ;
  }
  //----< frame message string by reading bytes from socket >--------

  std::string readMsg(Socket& socket)
  {
    std::string temp, msgString;
    while (socket.validState())
    {
      temp = socket.recvString('\n');  // read attribute
      msgString += temp;
      if (temp.length() < 2)           // if empty line we are done
        break;
    }
    return msgString;
  }
  //----< reads messages from socket and enQs in rcvQ >--------------

  void operator()(Socket socket)
  {
    while (socket.validState())
    {
      std::string msgString = readMsg(socket);
      if (msgString.length() == 0)
      {
        // invalid message
        break;
      }
      Message msg = Message::fromString(msgString);
      //std::cout << "\n  -- " + clientHandlerName + " RecvThread read message: " + msg.name();
      pQ_->enQ(msg);
      //std::cout << "\n  -- message enqueued in rcvQ";
      if (msg.command() == "quit")
        break;
    }
  }
private:
  BlockingQueue<Message>* pQ_;
  std::string clientHandlerName;
};

Comm::Comm(EndPoint ep, const std::string& name) : rcvr(ep, name), sndr(name), commName(name) {}

void Comm::start()
{
  BlockingQueue<Message>* pQ = rcvr.queue();
  ClientHandler* pCh = new ClientHandler(pQ, commName);
  /*
    There is a trivial memory leak here.  
    This ClientHandler is a prototype used to make ClientHandler copies for each connection.
    Those are not created on the heap, and are destroyed when the connection closes.
    Only one Client handler prototype is created for each Comm object and will live until
    the end of the program.

    I will clean this up in the next version.
  */
  rcvr.start(*pCh);
  sndr.start();
}

void Comm::stop()
{
  rcvr.stop();
  sndr.stop();
}

void Comm::postMessage(Message msg)
{
  sndr.postMessage(msg);
}

Message Comm::getMessage()
{
  return rcvr.getMessage();
}

std::string Comm::name()
{
  return commName;
}

//----< handler for first concurrent client >------------------------

void ThreadProcClnt1()
{
  Comm comm(EndPoint("localhost", 9891), "client1Comm");
  comm.start();
  EndPoint serverEP("localhost", 9890);
  //EndPoint clientEP("localhost", 9891);
  //size_t IMax = 3;
  //for (size_t i = 0; i < IMax; ++i)
  //{
    Message msg;
	msg.to(serverEP);
	msg.command("fudeo geral");
    msg.name("client #1 : msg #");
    std::cout << "\n  " + comm.name() + " posting:  " << msg.name();
    comm.postMessage(msg);
    //Message rply = comm.getMessage();
    //std::cout << "\n  " + comm.name() + " received: " << rply.name();
    //::Sleep(100);
  //}
  ::Sleep(200);
  Message stop;
  stop.name("stop");
  stop.to(serverEP);
  stop.command("stop");
  comm.postMessage(stop);
}
//----< handler for 2nd concurrent client >--------------------------

void ThreadProcClnt2()
{
  Comm comm(EndPoint("localhost", 9892), "client2Comm");
  comm.start();
  EndPoint serverEP("localhost", 9890);
  EndPoint clientEP("localhost", 9892);
  size_t IMax = 3;
  for (size_t i = 0; i < IMax; ++i)
  {
    Message msg(serverEP, clientEP);
    msg.name("client #2 : msg #" + Utilities::Converter<size_t>::toString(i));
    std::cout << "\n  " + comm.name() + " posting:  " << msg.name();
    comm.postMessage(msg);
    Message rply = comm.getMessage();
    std::cout << "\n  " + comm.name() + " received: " << rply.name();
  }
}

void server() {
	SocketSystem ss;

	//starting the server
	EndPoint serverEP("localhost", 9890);
	//EndPoint clientEP("localhost", 9891);
	Comm comm(serverEP, "serverComm");
	comm.start();

	Message msg;
	//rply.name("reply");
	//size_t count = 0;
	while (true)
	{
		msg = comm.getMessage();
		std::cout << "\n  " + comm.name() + " received message: " << msg.command();

		//msg.show();
		//rply.to(msg.from());
		//rply.from(serverEP);
		//rply.name("server reply #" + Utilities::Converter<size_t>::toString(++count) + " to " + msg.from().toString());
		//rply.show();
		//comm.postMessage(rply);
		if (msg.command() == "stop")
		{
			break;
		}
	}
	comm.stop();
}
//----< server demonstrates two-way asynchronous communication >-----
/*
*  - One server receiving messages and sending replies to
*    two concurrent clients.
*/
void DemoClientServer()
{

  std::thread t1(server);
  t1.detach();

  ::Sleep(5000);

  ThreadProcClnt1();
  //std::thread t2(ThreadProcClnt2);
  //t2.detach();

 
  std::cout << "\n  press enter to quit DemoClientServer";
}

int main()
{
  DemoClientServer();
  return 0;
}

