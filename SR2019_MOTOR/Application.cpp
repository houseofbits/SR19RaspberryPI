#include "Core.h"



Application::Application() :
	motorState(false),
	motorDirection(false),
	motorSpeed(150),
	threadExitSignal(false),
	animationPos(0),
	animationMult(1),
	animation(),
	animationActiveBuffer(0),
	animationFilename(),
	animationPlaying(true),
	animationFiles(),
	softStepDelay(50000)
{
	commands[CommandDef::EXIT] = "exit";
	commands[CommandDef::QUIT] = "q";
	commands[CommandDef::MOTOR_UP] = "up";
	commands[CommandDef::MOTOR_DOWN] = "down";
	commands[CommandDef::MOTOR_OFF] = "off";
	commands[CommandDef::MOTOR_SPEED] = "speed";
	commands[CommandDef::SOFT_MOTOR_UP] = "sup";
	commands[CommandDef::SOFT_MOTOR_DOWN] = "sdown";
	commands[CommandDef::SOFT_MOTOR_SPEED] = "sspeed";
	commands[CommandDef::SOFT_MOTOR_OFF] = "soff";
}

Application::~Application()
{	}

void Application::init()
{
	open();

	animationFilename = "/home/pi/projects/data/simple.bin";
	animation[(int)animationActiveBuffer].ReadFromFile(animationFilename);
	animationFilename = "simple.bin";

	DIR *dp = 0;
	struct dirent *dirp;
	dp = opendir("/home/pi/projects/data/");

	if (dp) {
		while ((dirp = readdir(dp)) != NULL) {
			if (string(dirp->d_name) != "." && string(dirp->d_name) != "..") {
				animationFiles.push_back(string(dirp->d_name));
			}
		}
		closedir(dp);
	}

}

void Application::mainProcess()
{
	CommandDef paramsCmd = CommandDef::ENUM_SIZE;
	char	input[255];
	int		intInput;
	float	floatInput;
	string	filename;

	WingsKeyframe kf;

	while (true) {

		render(false);

		cin >> input;

		switch (matchInputCommand(input, 255)) {
		case CommandDef::MOTOR_UP:
			motorDirection = true;
			motorState = true;
			writeMotor();
			break;
		case CommandDef::MOTOR_DOWN:
			motorDirection = false;
			motorState = true;
			writeMotor();
			break;
		case CommandDef::MOTOR_OFF:
			motorDirection = false;
			motorState = false;
			writeMotor();
			break;
		case CommandDef::MOTOR_SPEED:
			paramsCmd = CommandDef::MOTOR_SPEED;
			continue;
			break;
		case CommandDef::SOFT_MOTOR_SPEED:
			paramsCmd = CommandDef::SOFT_MOTOR_SPEED;
			continue;
			break;
		case CommandDef::SOFT_MOTOR_UP:
			motorSoftUp();
			break;
		case CommandDef::SOFT_MOTOR_DOWN:
			motorSoftDown();
			break;
		case CommandDef::SOFT_MOTOR_OFF:
			motorSoftOff();
			break;
		case CommandDef::EXIT:
		case CommandDef::QUIT:
			threadExitSignal = true;
			render(true);
			return;
			break;
		case CommandDef::ENUM_SIZE:
			break;
		};

		switch (paramsCmd)
		{
		case CommandDef::MOTOR_SPEED:
			if (sscanf(input, "%d", &intInput) == 1){
				if (intInput > 10 && intInput <= 255) {
					motorSpeed = intInput;
					writeMotor();
				}
			}
			paramsCmd = CommandDef::ENUM_SIZE;
			break;
		case CommandDef::SOFT_MOTOR_SPEED:
			if (sscanf(input, "%d", &intInput) == 1) {
				if (intInput > 10 && intInput <= 255) {
					motorSoftSpeed(intInput);
				}
			}
			paramsCmd = CommandDef::ENUM_SIZE;
			break;
		default:
			break;
		}
	}
}

void Application::render(bool exit)
{
	cout << clearScreen << resetCursor;

	cout << SH_FG_YELLOW <<    "                      SR2019 - MOTOR TEST!" << colorWhite << endl;
	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;
	cout << SH_FG_DARK_GREY << "Status" << SH_FG_CYAN << endl;

	if (motorState && motorDirection)cout << SH_FG_GREEN << " Motor ON / UP";
	else if (motorState && !motorDirection)cout << SH_FG_GREEN << " Motor ON / DOWN";
	else if (!motorState)cout << SH_FG_RED << " Motor OFF";

	cout << " speed: " << motorSpeed << endl;

	cout << colorWhite;

	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;

	cout << SH_FG_YELLOW<< ": " << endl;

	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;

	cout << SH_FG_DARK_GREY << "Info" << SH_FG_CYAN << endl;

	cout << SH_FG_CYAN << " exit, q"<< SH_FG_DARK_GREY << " Exit" << endl;
	cout << SH_FG_CYAN << " up, down" << SH_FG_DARK_GREY << " Motor direction" << endl;
	cout << SH_FG_CYAN << " off" << SH_FG_DARK_GREY << " Motor off" << endl;
	cout << SH_FG_CYAN << " speed" << SH_FG_DARK_GREY << " Motor speed (100 - 255)" << endl;
	cout << SH_FG_CYAN << " sup, sdown, sspeed, soff" << SH_FG_DARK_GREY<< " Soft ctrl" << endl;

	if (exit) {
		cout << SH_FG_YELLOW << endl << "Bye!" << colorWhite << endl;
	}
	else {
		cout << editorLineCursor << SH_FG_YELLOW;
	}
}

Application::CommandDef Application::matchInputCommand(const char* input, unsigned int inputSize)
{
	char inputLower[inputSize];

	for (int i = 0; i < (int)inputSize; i++)
	{
		inputLower[i] = (char)::tolower((int)input[i]);
	}

	for (int i = 0; i < CommandDef::ENUM_SIZE; i++)
	{
		CommandDef current = (CommandDef)i;

		string str = commands[current];

		if (memsearch(inputLower, inputSize, str.c_str(), str.length()) == 0)
		{
			return current;
		}
	}
	return CommandDef::ENUM_SIZE;
}

int Application::memsearch(const char *hay, int haysize, const char *needle, int needlesize) {
	int haypos, needlepos;
	haysize -= needlesize;
	for (haypos = 0; haypos <= haysize; haypos++) {
		for (needlepos = 0; needlepos < needlesize; needlepos++) {
			if (hay[haypos + needlepos] != needle[needlepos]) {
				break;
			}
		}
		if (needlepos == needlesize) {
			return haypos;
		}
	}
	return -1;
}

bool	Application::dataOutputProcess()
{
	/*
	bool motorStateCurrent = motorState;
	bool motorDirectionCurrent = motorDirection;
	int motorSpeedCurrent = motorSpeed;
	WingsKeyframe kf;
	clock_t start, end;
	int dt;

	while (true) {
		
		if (threadExitSignal)return false;

		if (motorStateCurrent != motorState 
			|| motorDirectionCurrent != motorDirection
			|| motorSpeedCurrent != motorSpeed) {

			writeMotorSettings(motorState, motorDirection, motorSpeed);

			motorDirectionCurrent = motorDirection;
			motorStateCurrent = motorState;
			motorSpeedCurrent = motorSpeed;
		}
	}
	*/
	return true;
}

void Application::writeMotor() {
	writeMotorSettings(motorState, motorDirection, motorSpeed);
}

void Application::motorSoftUp() {
	if (motorState && !motorDirection) {
		for (int i = motorSpeed; i < 255; i++) {
			writeMotorSettings(true, false, i);
			usleep(softStepDelay);
		}
		writeMotorSettings(false, false, 255);
		usleep(softStepDelay * 10);
		for (int i = 255; i > motorSpeed; i--) {
			writeMotorSettings(true, true, i);
			usleep(softStepDelay);
		}
		motorDirection = true;
		motorState = true;
	}
	else if (!motorState) {

		motorDirection = true;
		motorState = true;

		int targetSpeed = motorSpeed;
		motorSpeed = 255;
		motorSoftSpeed(targetSpeed);
	}
}

void Application::motorSoftDown() {
	if (motorState && motorDirection) {
		for (int i = motorSpeed; i < 255; i++) {
			writeMotorSettings(true, true, i);
			usleep(softStepDelay);
		}
		writeMotorSettings(false, true, 255);
		usleep(softStepDelay);
		for (int i = 255; i > motorSpeed; i--) {
			writeMotorSettings(true, false, i);
			usleep(softStepDelay);
		}
		motorDirection = false;
		motorState = true;
	}
	else if (!motorState) {

		motorDirection = false;
		motorState = true;

		int targetSpeed = motorSpeed;
		motorSpeed = 255;
		motorSoftSpeed(targetSpeed);
	}
}

void Application::motorSoftSpeed(int targetSpeed) {
	if (motorState) {
		if (targetSpeed > motorSpeed) {
			for (int i = motorSpeed; i < targetSpeed; i++) {
				motorSpeed = i;
				writeMotorSettings(motorState, motorDirection, motorSpeed);
				usleep(softStepDelay);
			}
		}
		else {
			for (int i = motorSpeed; i > targetSpeed; i--) {
				motorSpeed = i;
				writeMotorSettings(motorState, motorDirection, motorSpeed);
				usleep(softStepDelay);
			}
		}
	}
	else {
		motorSpeed = targetSpeed;
	}
}

void Application::motorSoftOff() {
	if (motorState) {
		motorState = true;
		motorSoftSpeed(255);
		motorDirection = false;
		motorState = false;
		writeMotorSettings(motorState, motorDirection, motorSpeed);
	}
}