#include "Core.h"



Application::Application() :
	threadExitSignal(false),
	setColor(0,0,0),
	wingId(-1),
	segmentId(-1),
	clear(false)
{
	commands[CommandDef::EXIT] = "exit";
	commands[CommandDef::QUIT] = "q";
	commands[CommandDef::COLOR] = "col";
	commands[CommandDef::POSITION] = "pos";
	commands[CommandDef::CLEAR] = "clear";
}

Application::~Application()
{	}

void Application::init()
{
	open();
	/*
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
	*/
}

void Application::mainProcess(std::thread* outputThread)
{
	CommandDef paramsCmd = CommandDef::ENUM_SIZE;
	char	input[255];
	//int		intInput;
	//float	floatInput;
	//string	filename;

	int		int1Input;
	int		int2Input;
	int		int3Input;

	//WingsKeyframe kf;

	while (true) {

		render(false);

		cin >> input;

		switch (matchInputCommand(input, 255)) {
		case CommandDef::COLOR:
			paramsCmd = CommandDef::COLOR;
			continue;
			break;
		case CommandDef::POSITION:
			paramsCmd = CommandDef::POSITION;
			continue;
			break;
		case CommandDef::CLEAR:
			clear = true;
			wingId = -1;
			segmentId = -1;
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
		case CommandDef::COLOR:
			if (sscanf(input, "%i,%i,%i", &int1Input, &int2Input, &int3Input) == 3) {
				if (int1Input > 255)int1Input = 255;
				if (int2Input > 255)int2Input = 255;
				if (int3Input > 255)int3Input = 255;

				if (int1Input < 0)int1Input = 0;
				if (int2Input < 0)int2Input = 0;
				if (int3Input < 0)int3Input = 0;

				setColor.x = (float)int1Input;
				setColor.y = (float)int2Input;
				setColor.z = (float)int3Input;
			}
			paramsCmd = CommandDef::ENUM_SIZE;
			break;
		case CommandDef::POSITION:
			if (sscanf(input, "%i,%i", &int1Input, &int2Input) == 2) {
				if (int1Input > 11)int1Input = 11;
				if (int2Input > 6)int2Input = 6;

				if (int1Input < 0)int1Input = 0;
				if (int2Input < 0)int2Input = 0;

				wingId = int1Input;
				segmentId = int2Input;

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

	cout << SH_FG_YELLOW <<    "                      LED TEST!" << colorWhite << endl;

	cout << SH_FG_YELLOW << "col: "<<(int)setColor.x <<","<< (int)setColor.y<<","<< (int)setColor.z<< colorWhite;

	cout << SH_FG_YELLOW << " wing: " << wingId << colorWhite;

	cout << SH_FG_YELLOW << " segment: " << segmentId << colorWhite;

	cout << endl;

	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;

	cout << SH_FG_YELLOW<< ": " << endl;

	cout << SH_FG_DARK_GREY << "=========================================================" << colorWhite << endl;

	cout << SH_FG_DARK_GREY << "Info" << SH_FG_CYAN << endl;

	cout << SH_FG_CYAN << " exit, q"<< SH_FG_DARK_GREY << " Exit" << endl;
	cout << SH_FG_CYAN << " col %i,%i,%i" << SH_FG_DARK_GREY << " Set RGB color (decimal)" << endl;
	cout << SH_FG_CYAN << " pos %i,%i" << SH_FG_DARK_GREY << " Set pos (wing[0,11], segment[0,7])" << endl;
	cout << SH_FG_CYAN << " clear" << SH_FG_DARK_GREY << " Clear all with color" << endl;

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
	WingsKeyframe kf;
	clock_t start, end;
	int dt;

	while (true) {
		
		if (threadExitSignal)return false;

		start = clock();

		for (int w = 0; w < 12; w++) {
			for (int c = 0; c < 7; c++) {
				if (clear || (wingId == w && segmentId == c)) {
					kf.colors[w][c].x = setColor.y / 255.0f;
					kf.colors[w][c].y = setColor.x / 255.0f;
					kf.colors[w][c].z = setColor.z / 255.0f;
				}
			}
		}

		clear = false;

		writeKeyFrame(kf);
		wait();

		end = clock();
		dt = int(((double)(end - start)) / CLOCKS_PER_SEC * 1000000);

		//~60hz
		if (dt > 0 && dt < 16000) {
			usleep(16000 - dt);
		}
		usleep(1000);
	}
	return true;
}