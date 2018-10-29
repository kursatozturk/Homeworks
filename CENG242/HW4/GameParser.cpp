#include "GameParser.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
int getnum(std::string line){
	std::stringstream in;
	int num;			
	std::string::iterator it = line.begin();
	for(; it != line.end(); ++it)
		if (*it == ':')break;
		in.str(std::string());
		++it;
		while(it != line.end()){
			if(*it != ' ')in << *it;
			it++;
		}
		in >> num;
		in.str(std::string());
		return num;	
}

//PlayerID::PlayerType::PlayerxCoord::PlayeryCoord

Player *parsePlayer(std::string line){
	Player *parsedPlayer;
	std::stringstream s;
	char c;
	int *arr = new int [4];
	s << line;
	std::string type = "";
	s >> arr[1];
	s >> c;
	s >> c;
	do{
		s >> c;
		type += c;
	}while(c != ':');

	if( type == "Berserk:")arr[0] = 1;
	else if(type == "Ambusher:")arr[0] = 2;
	else if(type == "Dummy:") arr[0] = 3;
	else if(type == "Tracer:") arr[0] = 4;
	else if(type == "Pacifist:") arr[0] = 5;

	s >> c;
	s >> arr[2];
	s >> c;
	s >> c;
	s >> arr[3];

	switch(arr[0]){
		case 1:
		parsedPlayer = new Berserk(arr[1],arr[2],arr[3]);
		break;
		case 2:
		parsedPlayer = new Ambusher(arr[1],arr[2],arr[3]);
		break;
		case 3:
		parsedPlayer = new Dummy(arr[1],arr[2],arr[3]);
		break;
		case 4:
		parsedPlayer = new Tracer(arr[1],arr[2],arr[3]);
		break;
		case 5:
		parsedPlayer = new Pacifist(arr[1],arr[2],arr[3]);
		break;
		default:
		break;
	}
	delete[] arr;
	return parsedPlayer;
}
/*
std::pair<int, std::vector<Player *> *> GameParser::parseFileWithName(const std::string& filename){
	std::vector<Player *> *Players = new std::vector<Player *>();
	std::ifstream read(filename.c_str());
	std::string line;
	std::getline(read, line);
	int boardSize = getnum(line);
	line = "";
	std::getline(read, line);
	int playerCount = getnum(line);
	line = "";
	for(int i = 0; i < playerCount; ++i){
		line = "";
		std::getline(read, line);
		Players->push_back(parsePlayer(line));
	}
	return std::pair<int, std::vector<Player *> *>(boardSize, Players);
}*/

std::pair<int, std::vector<Player *> *> GameParser::parseFileWithName(const std::string& filename){
	std::vector<Player *> *Players = new std::vector<Player *>();
	std::ifstream read(filename.c_str());
	std::string line;
	std::vector<std::string> Lines;
	while(getline(read,line))Lines.push_back(line);
	int boardSize = getnum(Lines[0]);
	int playerCount = getnum(Lines[1]);
	for(int i = 0; i < playerCount; i++)Players->push_back(parsePlayer(Lines[i + 2]));
	read.close();
	return std::pair<int, std::vector<Player *> *>(boardSize, Players);
}