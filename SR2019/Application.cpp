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
	animationFiles()
{
	commands[CommandDef::EXIT] = "exit";
	commands[CommandDef::QUIT] = "q";
	commands[CommandDef::MOTOR_UP] = "up";
	commands[CommandDef::MOTOR_DOWN] = "down";
	commands[CommandDef::MOTOR_OFF] = "off";
	commands[CommandDef::MOTOR_SPEED] = "speed";
	commands[CommandDef::SEEK] = "seek";
	commands[CommandDef::SPEED] = "mult";
	commands[CommandDef::LOAD] = "load";
	commands[CommandDef::PLAY] = "play";
	commands[CommandDef::PAUSE] = "pause";
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

void Application::mainProcess(std::thread* outputThread)
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
			break;
		case CommandDef::MOTOR_DOWN:
			motorDirection = false;
			motorState = true;
			break;
		case CommandDef::MOTOR_OFF:
			motorDirection = false;
			motorState = false;
			break;
		case CommandDef::MOTOR_SPEED:
			paramsCmd = CommandDef::MOTOR_SPEED;
			continue;
			break;
		case CommandDef::SEEK:
			paramsCmd = CommandDef::SEEK;
			continue;
			break;
		case CommandDef::SPEED:
			paramsCmd = CommandDef::SPEED;
			continue;
			break;
		case CommandDef::PLAY:
			animationPlaying = true;
			break;
		case CommandDef::PAUSE:
			animationPlaying = false;
			break;
		case CommandDef::LOAD:
			paramsCmd = CommandDef::LOAD;
			continue;
			break;
		case CommandDef::EXIT:
		case CommandDef::QUIT:
			threadExitSignal = true;
			outputThread->join();
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
				}
			}
			paramsCmd = CommandDef::ENUM_SIZE;
			break;
		case CommandDef::SEEK:
			if (sscanf(input, "%f", &floatInput) == 1) {
				animationPos = floatInput;
			}
			paramsCmd = CommandDef::ENUM_SIZE;
			break;
		case CommandDef::SPEED:
			if (sscanf(input, "%f", &floatInput) == 1) {
				animationMult = floatInput;
			}
			paramsCmd = CommandDef::ENUM_SIZE;
			break;
		case CommandDef::LOAD:
			if (sscanf(input, "%d", &intInput) == 1) {
				if (intInput >= 0 && intInput < (int)animationFiles.size()) {
					filename = string("/home/pi/projects/data/") + animationFiles[intInput];
					if (animation[(int)(!animationActiveBuffer)].ReadFromFile(filename)) {
						animationActiveBuffer = !animationActiveBuffer;
						animationFilename = animationFiles[intInput];
						animationPos = animation[(int)(animationActiveBuffer)].tstart;
					}
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


	cout << SH_FG_YELLOW <<    "                      SR2019!" << colorWhite << endl;
	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;
	cout << SH_FG_DARK_GREY << "Status" << SH_FG_CYAN << endl;

	if (motorState && motorDirection)cout << SH_FG_GREEN << " Motor ON / UP";
	else if (motorState && !motorDirection)cout << SH_FG_GREEN << " Motor ON / DOWN";
	else if (!motorState)cout << SH_FG_RED << " Motor OFF";

	cout << " speed: " << motorSpeed << endl;

	if (animationPlaying) cout << SH_FG_GREEN;
	else cout << SH_FG_RED;

	cout<< " Animation: " << animationFilename << " speed:" << animationMult << " playing: " << (int)animationPlaying;

	cout << colorWhite << endl;

	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;

	cout << SH_FG_YELLOW<< ": " << endl;

	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;

	cout << SH_FG_DARK_GREY << "Animation files" << SH_FG_CYAN << endl;
	
	for (size_t i = 0; i < animationFiles.size(); i++)	{
		cout <<" "<< i<<" - "<<animationFiles[i]<< endl;
	}

	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;

	cout << SH_FG_DARK_GREY << "Info" << SH_FG_CYAN << endl;

	cout << SH_FG_CYAN << " exit, q"<< SH_FG_DARK_GREY << " Exit" << endl;
	cout << SH_FG_CYAN << " up, down" << SH_FG_DARK_GREY << "Motor direction" << endl;
	cout << SH_FG_CYAN << " off" << SH_FG_DARK_GREY << " Motor off" << endl;
	cout << SH_FG_CYAN << " speed" << SH_FG_DARK_GREY << " Motor speed (100 - 255)" << endl;
	cout << SH_FG_CYAN << " seek %f" << SH_FG_DARK_GREY << " Seek to animation position" << endl;
	cout << SH_FG_CYAN << " mult %f" << SH_FG_DARK_GREY << " Multiply keyframe interval (speed)" << endl;
	cout << SH_FG_CYAN << " load %d" << SH_FG_DARK_GREY << " Load animation file by Id" << endl;
	cout << SH_FG_CYAN << " play, pause" << SH_FG_DARK_GREY << " Play, pause animation" << endl;

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
	
		if (memsearch(inputLower, inputSize, str.c_str(), str.length()) >= 0)
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

			usleep(1000);
		}
		else{
			
			start = clock();
			if (animation[(int)animationActiveBuffer].getInterpolatedKeyframe(animationPos, kf)) {					
				writeKeyFrame(kf);
				wait();

			//	cout << animationPos<<" / "<<kf.colors[0][0].toString()<<endl;
			}
			end = clock();
			dt = int(((double)(end - start)) / CLOCKS_PER_SEC * 1000000);

			//~60hz
			if (dt > 0 && dt < 16000) {
				usleep(16000 - dt);
			}
			usleep(1000);

			if (animationPlaying) {
				animationPos += (0.016f * animationMult);
			}

			if (animationPos > animation[(int)animationActiveBuffer].tend) {
				animationPos = animation[(int)animationActiveBuffer].tstart;
			}
		}
	}
	return true;
}