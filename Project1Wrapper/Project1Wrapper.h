#include <vector>
#include <string>
#include "../Message/Message.h"

class Project1Wrapper {
public:
	void main(std::vector<std::string> paths);
private:
	void sendMessages(std::vector<std::string> paths, MsgPassingCommunication::EndPoint serverEP);
};