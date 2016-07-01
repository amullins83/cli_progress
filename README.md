#CLI Progress

This is a simple library to create and display command-line progress bars in C.

##Important parts

The main idea is to overwrite the output file with new information every time a particular step is reached. I accomplished this using the VT100 emulator hack from [this Stack Overflow answer][1]. In a nutshell, printing `^[[2K` to `stdout` erases the current line, but it doesn't necessarily move the cursor to the start. Printing `\r` does that. Also, I decided I wanted to print a newline character after the progress indicator, but I need to get rid of that newline on the next print. That's what `\b` does: it inserts a backspace, deleting the last character printed.

Also important is the call to `fflush`, which will guarantee that the print operation completes and is visible before the program moves on to its next task (see [this Stack Overflow answer][2]).

##Less important parts

I decided to let the progress function manage its own internal buffers, which are dynamically allocated. Therefore, it is necessary to call `cli_progress_release` to prevent memory leaks.

I also decided to go ahead and print an extra newline character in the first `cli_progress_print` call. Otherwise the first `\b` printed would delete the previous line from the output, which is allowed but not usually expected.

##TODO

Add an interface that allows the user to allocate their own buffers, which would make the library more useful for embedded systems.

[1]: http://stackoverflow.com/a/1508589/1896761 "use VT100 emulation to clear a line"
[2]: http://stackoverflow.com/a/338295/1896761 "use fflush to ensure printf completes before the next task"
