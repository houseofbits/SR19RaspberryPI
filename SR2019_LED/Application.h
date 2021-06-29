#pragma once
#include "Serial.h"

class Application : public Serial
{
public:
	Application();
	~Application();

	enum CommandDef {
		EXIT = 0,
		QUIT,
		COLOR,
		POSITION,
		CLEAR,
		ENUM_SIZE
	};

	void init();

	void mainProcess(std::thread*);

	void render(bool);
	
	int		memsearch(const char *hay, int haysize, const char *needle, int needlesize);
	
	CommandDef matchInputCommand(const char*, unsigned int);
	
	bool	dataOutputProcess();

	string commands[CommandDef::ENUM_SIZE];
	atomic_bool threadExitSignal;
	
	Vector3		setColor;
	int			wingId;
	int			segmentId;

	bool		clear;

};

