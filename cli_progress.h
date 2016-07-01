#ifndef CLI_PROGRESS
#define CLI_PROGRESS

#include <stddef.h>
#include <stdio.h>

struct cli_progress_internal;
struct cli_progress
{
	size_t size;
	size_t max_count;
	size_t done_count;
	struct cli_progress_internal *internals;
};

void
cli_progress_init(
	struct cli_progress *progress,
	size_t max_count,
	size_t done_count,
	const char *prefix,
	const char *suffix,
	const char *done_string,
	const char *incomplete_string
	);

void
cli_progress_release(
	struct cli_progress *progress);

void
cli_progress_print(
	struct cli_progress *progress,
	FILE *output_file);

void
cli_progress_update(
	struct cli_progress *progress,
	size_t done_count);

void
cli_progress_set_max(
	struct cli_progress *progress,
	size_t max_count);

void
cli_progress_get_buffer(
	struct cli_progress *progress,
	char *buffer,
	size_t buffer_size);

void
cli_progress_set_prefix(
	struct cli_progress *progress,
	const char *prefix);

void
cli_progress_set_suffix(
	struct cli_progress *progress,
	const char *suffix);

void
cli_progress_set_done_string(
	struct cli_progress *progress,
	const char *done_string);

void
cli_progress_set_incomplete_string(
	struct cli_progress *progress,
	const char *incomplete_string);

#endif
