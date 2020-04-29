The executable `logicians-main` can be built by running `make`. C++11 and up are
supported, using C++17 will make some output nicer.
To specify a standard, use `make STD=c++XX`.

Running the executable will bring up a menu with several options, including
creating truth tables and transforming expressions.

Tables can be created from the command line by calling the executable like so:
`./logicians-main filename format 'expression'`
Supported formats are "TXT" and "HTML".
Surrounding the expression in quotes is HIGHLY recommended.
If the filename is `-`, then output will be directed to stdout.

LDSL snippets can be evaluated via the `--c` flag:
`./logicians-main --c '(transform demorgans (stm p^q))'`
The quotes around the snippet are REQUIRED.
Enter `(help)` at the REPL to see more examples of commands.


Class documentation can be found in the header files or at:
http://riddler.tamucc.edu/~bcolburn/logicians/
