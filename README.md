# vshell

vshell is a simple shell interface written in the C programming
language. It supports input and output redirection, as well
as pipelining. A history feature is also implemented.

The shell emulates a standard Unix terminal, with notations for
redirections as < or > (input and output, respectively). Pipelines
are indicated with a | between commands. 

The user may use UP/DOWN arrows to move arond the history of 
commands typed by the user in the current session. 
Typing !(n) executes the n-last command executed previously, and
!! the last command.


