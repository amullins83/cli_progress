#include <unistd.h>
#include "cli_progress.h"

#define NUM_SECS 10

int
main()
{
	size_t i = 0;
	struct cli_progress progress;
	cli_progress_init(
		&progress,
		NUM_SECS,
		0,
		"Progress: [",
		"]",
		NULL,
		NULL);

	cli_progress_print(&progress, NULL);
	for(i = 1; i <= NUM_SECS; ++i)
	{
		cli_progress_update(&progress, i);
		sleep(1);
		cli_progress_print(&progress, NULL);
	}

	cli_progress_release(&progress);
	return 0;
}
