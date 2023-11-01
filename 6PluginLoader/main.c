// Benjamin Belandres, Plugin Loader
// This program takes plugins from shared object libraries and uses them
	// All exports from a .so file must fit the structure of the export 
	// struct in the code

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include "vector.h"

struct export {
	char name[65];
    int  (*init)(void);
    void (*fini)(void);
    int  (*cmd)(char *str);
};

int main() {
	
	Vector *handles = vector_new();	// Opaque locations where the plugins are stored
	struct export *currentExport;	// Loads the current relevant plugin's exports
	void *currentHandle;
	char cmd[65];

	// Waiting for a new command
	while (true) {
		printf("> ");

		scanf("%64s", cmd);

		if (strcmp(cmd, "load") == 0) {
			// Scanning for the path
			scanf("%64s", cmd);	// Reusing the cmd string to store the path

			currentHandle = dlopen(cmd, RTLD_LAZY);

			if (!currentHandle) {	// Checking if the plugin was successfully loaded
				puts(dlerror());

				// Closing all of the handles like in the quit command
				for (int i = 0; i < vector_size(handles); i++) {
					if (vector_get(handles, i, (int64_t*)&currentHandle)) {


						currentExport = dlsym(currentHandle, "export");
						currentExport->fini();
						dlclose(currentHandle);

					}

				}
				vector_free(handles);
				return -1;
			}

			vector_push(handles, (int64_t)currentHandle); 
			currentExport = dlsym(currentHandle, "export");
			currentExport->init();

		}

		else if (strcmp(cmd, "list") == 0 || strcmp(cmd, "plugins") == 0) {
			// Listing all of the loaded plugins 
			int i;
			for (i = 0; i < vector_size(handles); i++) {
				if (vector_get(handles, i, (int64_t*)&currentHandle)) {

					currentExport = dlsym(currentHandle, "export");
					printf("%s\n", currentExport->name);

				}

			}
			printf("%d plugins loaded.\n", i);
		}

		else if (strcmp(cmd, "unload") == 0) {
			// Scanning for the plugin name and then removing that plugin
			scanf("%64s", cmd);	// Reusing the cmd string to store the name

			for (int i = 0; i < vector_size(handles); i++) {
				if (vector_get(handles, i, (int64_t*)&currentHandle)) {

					currentExport = dlsym(currentHandle, "export");

					if (strcmp(cmd, currentExport->name) == 0) {	// Searching for the plugin by name
						currentExport->fini();
						dlclose(currentHandle);
						// printf("Removing index %d\n", i);
						vector_remove(handles, i);

						break;	// Breaking to make sure that we only remove one instance of the plugin
					}


				}

			}
		}

		else if (strcmp(cmd, "quit") == 0) {
			// Closing all of the handles
			for (int i = 0; i < vector_size(handles); i++) {
				if (vector_get(handles, i, (int64_t*)&currentHandle)) {


					currentExport = dlsym(currentHandle, "export");
					currentExport->fini();
					dlclose(currentHandle);

				}

			}
			vector_free(handles);
			
			return 0;

		}

		else {	// If none of the default commands work, then we start passing the command to the plugins
			for (int i = 0; i < vector_size(handles); i++) {
				if (vector_get(handles, i, (int64_t*)&currentHandle)) {

					currentExport = dlsym(currentHandle, "export");
					if (currentExport->cmd(cmd) != 0) {	// Implies a successful command
						break;
					}
				}
			}
		}

	}


	// myExport = dlsym(handle, "export");	// Getting the exports from the handle

	// printf("%s\n", myExport->name);
	
	return 0;
}
