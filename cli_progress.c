#include "cli_progress.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct cli_progress_internal
{
	size_t prefix_length;
	size_t suffix_length;
	size_t buffer_size;
	size_t done_length;
	size_t incomplete_length;
	const char *prefix;
	const char *suffix;
	const char *done_string;
	const char *incomplete_string;
	char *buffer;
} cli_progress_internal;

size_t
make_copy(
	const char **destination,
	const char *original)
{
	if (!original)
		return 0;
	size_t length = strlen(original);
	char *copy = calloc(length + 1, 1);
	strcpy(copy, original);
	*destination = copy;
	return length;
}

size_t
size_needed(
	struct cli_progress *progress)
{
	cli_progress_internal *internals = progress->internals;
	return 
		internals->prefix_length +
		internals->suffix_length +
		MAX(internals->done_length, internals->incomplete_length) * progress->max_count
		+ 1; // \0
}

void
cli_progress_internals_release(
	cli_progress_internal *internals)
{
	if (!internals)
		return;
	free((char *)internals->prefix);
	free((char *)internals->suffix);
	free((char *)internals->done_string);
	free((char *)internals->incomplete_string);
	free(internals->buffer);
	free(internals);
}

void
cli_progress_init(
	struct cli_progress *progress,
	size_t max_count,
	size_t done_count,
	const char *prefix,
	const char *suffix,
	const char *done_string,
	const char *incomplete_string
	)
{
	cli_progress_internal *internals = calloc(1, sizeof(*internals));
	internals->prefix_length = make_copy(&internals->prefix, prefix);
	internals->suffix_length = make_copy(&internals->suffix, suffix);
	done_string = done_string ? done_string : "#";
	internals->done_length = make_copy(&internals->done_string, done_string);
	incomplete_string = incomplete_string ? incomplete_string : " ";
	internals->incomplete_length = make_copy(&internals->incomplete_string, incomplete_string);
	
	progress->internals = internals;
	progress->max_count = max_count;
	internals->buffer_size = size_needed(progress);
	internals->buffer = calloc(internals->buffer_size, 1);
	cli_progress_update(progress, done_count);
}

void
cli_progress_release(
	struct cli_progress *progress)
{
	cli_progress_internals_release(progress->internals);
}

void
cli_progress_print(
	struct cli_progress *progress,
	FILE *output_file)
{
	static char is_first_print = 1;
	if (!output_file)
		output_file = stdout;

	if (is_first_print)
	{
		fprintf(output_file, "%s\n", ""); // Prevents clearing command line
		is_first_print = 0;
	}

	fprintf(output_file, "\b%c[2K\r%s\n", 27, progress->internals->buffer);
	fflush(output_file);
}

void
cli_progress_update(
	struct cli_progress *progress,
	size_t done_count)
{
	size_t i = 0;
	size_t buffer_position = 0;
	cli_progress_internal *internals = progress->internals;

	progress->done_count = MIN(done_count, progress->max_count);
	strcpy(internals->buffer, internals->prefix);
	buffer_position = internals->prefix_length;
	for (; i < progress->done_count; ++i)
	{
		strcpy(&internals->buffer[buffer_position], internals->done_string);
		buffer_position += internals->done_length;
	}

	for (i = 0; i < progress->max_count - progress->done_count; ++i)
	{
		strcpy(&internals->buffer[buffer_position], internals->incomplete_string);
		buffer_position += internals->incomplete_length;
	}

	strcpy(&internals->buffer[buffer_position], internals->suffix);
}

void
cli_progress_internals_realloc_buffer(
	struct cli_progress *progress)
{
	progress->internals->buffer = realloc(progress->internals->buffer, size_needed(progress));
}

void
cli_progress_set_max(
	struct cli_progress *progress,
	size_t max_count)
{
	progress->max_count = max_count;
	cli_progress_internals_realloc_buffer(progress);
}

void
cli_progress_get_buffer(
	struct cli_progress *progress,
	char *buffer,
	size_t buffer_size)
{
	strncpy(buffer, progress->internals->buffer, buffer_size);
}

void
cli_progress_internals_set_string(
	struct cli_progress *progress,
	const char *new_string,
	const char **progress_string,
	size_t *progress_string_size)
{
	free((char *)*progress_string);
	*progress_string_size = make_copy(progress_string, new_string);
	cli_progress_internals_realloc_buffer(progress);
}

void
cli_progress_set_prefix(
	struct cli_progress *progress,
	const char *prefix)
{
	cli_progress_internals_set_string(
		progress,
		prefix,
		&progress->internals->prefix,
		&progress->internals->prefix_length);
}

void
cli_progress_set_suffix(
	struct cli_progress *progress,
	const char *suffix)
{
	cli_progress_internals_set_string(
		progress,
		suffix,
		&progress->internals->suffix,
		&progress->internals->suffix_length);
}

void
cli_progress_set_done_string(
	struct cli_progress *progress,
	const char *done_string)
{
	cli_progress_internals_set_string(
		progress,
		done_string,
		&progress->internals->done_string,
		&progress->internals->done_length);
}

void
cli_progress_set_incomplete_string(
	struct cli_progress *progress,
	const char *incomplete_string)
{
	cli_progress_internals_set_string(
		progress,
		incomplete_string,
		&progress->internals->incomplete_string,
		&progress->internals->incomplete_length);
}
