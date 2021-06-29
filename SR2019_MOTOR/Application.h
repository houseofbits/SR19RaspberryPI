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
		SOFT_MOTOR_UP,
		SOFT_MOTOR_DOWN,
		SOFT_MOTOR_SPEED,
		SOFT_MOTOR_OFF,
		ENUM_SIZE
	};

	void init();

	void mainProcess();

	void render(bool);
	
	int		memsearch(const char *hay, int haysize, const char *needle, int needlesize);
	
	CommandDef matchInputCommand(const char*, unsigned int);
	
	bool	dataOutputProcess();

	string commands[CommandDef::ENUM_SIZE];

	bool motorState;
	bool motorDirection;
	int	motorSpeed;

	atomic_bool threadExitSignal;

	float animationPos;
	float animationMult;

	WingKeyframesResource animation[2];
	
	atomic_bool	animationActiveBuffer;

	string animationFilename;

	atomic_bool animationPlaying;

	vector<string> animationFiles;

	int softStepDelay;

	void writeMotor();

	void motorSoftUp();
	void motorSoftDown();
	void motorSoftOff();
	void motorSoftSpeed(int speed);

};

