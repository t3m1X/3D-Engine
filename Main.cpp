#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#include "Brofiler\Brofiler.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment(lib, "Brofiler/ProfilerCore32.lib")

enum main_states
{
	MAIN_CREATION,
	MAIN_AWAKE,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	LOG_OUT("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		BROFILER_FRAME("YourThreadName")
		switch (state)
		{
		case MAIN_CREATION:

			LOG_OUT("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_AWAKE;
			break;

		case MAIN_AWAKE:

			LOG_OUT("-------------- Application Awake --------------");
			if (App->Awake() == false)
			{
				LOG_OUT("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_START;
				LOG_OUT("-------------- Application Starts --------------");
			}

			break;

		case MAIN_START:

			LOG_OUT("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG_OUT("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG_OUT("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG_OUT("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
		break;

		case MAIN_FINISH:

			LOG_OUT("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG_OUT("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}
	delete App;
	App = nullptr;
	LOG_OUT("Exiting application '%s'...\n", TITLE);
	return main_return;
}