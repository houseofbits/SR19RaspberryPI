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
		MOTOR_UP,
		MOTOR_DOWN,
		MOTOR_OFF,
		MOTOR_SPEED,
		SEEK,
		SPEED,
		PLAY,
		PAUSE,
		LOAD, 
		ENUM_SIZE
	};

	void init();

	void mainProcess(std::thread*);

	void render(bool);
	
	int		memsearch(const char *hay, int haysize, const char *needle, int needlesize);
	
	CommandDef matchInputCommand(const char*, unsigned int);
	
	bool	dataOutputProcess();

	string commands[CommandDef::ENUM_SIZE];
	atomic_bool motorState;
	atomic_bool motorDirection;
	atomic_int	motorSpeed;
	atomic_bool threadExitSignal;

	float animationPos;
	float animationMult;

	WingKeyframesResource animation[2];
	
	atomic_bool	animationActiveBuffer;

	string animationFilename;

	atomic_bool animationPlaying;

	vector<string> animationFiles;
};

