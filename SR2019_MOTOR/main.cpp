#include "Core.h"

int main()
{	
	Application app;

	app.init();
	
	//std::thread outputThread(&Application::dataOutputProcess, &app);

	app.mainProcess();
	
	return 0;


	/*
	serial.open();

	animation.ReadFromFile("/home/pi/projects/data/simple.bin");

	WingsKeyframe kf;

	if (animation.getInterpolatedKeyframe(99, kf)) {

		cout << kf.colors[0][0].toString() << endl;

	}

	return 1;

	//cout << "Waiting for response" << endl;

	//serialWrite("Hello from raspberry pi", serialHandle);

	int number;

	while (true) {
		cout << "Send number: ";
		cin >> number;
		serial.write((unsigned char)number);
	}

	return 0;
	*/
}