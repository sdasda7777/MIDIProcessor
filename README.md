# MIDIProcessor
Processes live MIDI input based on passed script. Library used for interaction with MIDI is rtmidi.

Due to the fact I took lot of code from my school project and I might want to use it again in other school projects, please consider filing issue before making forks or pull requests. I am very open to implementing new stuff if I find your use case reasonable.

## Building
### Building on Linux
Run `make compile_linux` with the clone repo being in the same folder as the rtmidi library.

This builds for ALSA by default. You can change this to Jack by replacing `__LINUX_ALSA__` in the makefile for `__UNIX_JACK__` and then changing `-lasound -lpthread` to `-ljack`

### Building for Windows (on Linux)
Run `make compile_win` with the clone repo being in the same folder as the rtmidi library.

More details to be specified.

### Building on Windows
?????

## Programming
This section explains programming of the scripts. The formatting is terrible to keep it brief, but you can insert newlines and spaces as you desire, they get ignored by the processor.

### Operators
Operators are fairly standard for C-like language. Only notable differences from C++ are that there is only one assignment operator (no +=, -=, *=, etc.), and symbol used for concatenation is #, not +.

Following is the table of operators, operators on top line have highest precedence. Letters i and j represent numbers, letters e and f represent either number or string.

| Notation | Name |
|---|---|
| i++, i-- | Postfix incrementation, Postfix decrementation |
| ++i, --i, +i, -i, !e, ~i | Prefix incrementation, prefix decrementation, Plus i\*, Negative i\*, Logical negation\**, Bitwise negation\*** |
| i**j | Power (i to the power of j) |
| i*j, i/j, i%j | Multiplication, Division, Modulo |
| i+j, i-j, e#f | Addition, Subtraction, Concatenation |
| i<j, i<=j, i>j, i>=j | Less than, Less than or equal, Greater than, Greater than or equal |
| e==f, e!=f | Equal, Not equal |
| i&j | Bitwise and |
| i^j | Bitwise xor |
| i\|j | Bitwise or |
| e&&f | Logical and |
| e\|\|f | Logical or |
| e=f | Assignment |

\* - +i returns i * 1, -i returns i * -1

\** - Returns 1 for 0 or empty string (""), 0 otherwise

\*** - This operator is honestly kinda useless because of the stupid way it is implemented


### Functions
Following is the table of functions.

| Name and parameters | Usage |
|---|---|
| string nStr(number n, number decimalPlaces) | Returns number n as string with decimal places as specified by second parameter. |
| number sNum(string s, variable v) | Attempts to parse number from string s and place it to variable v. Returns 0 if that is not possible. |
| number strLen(string s) | Returns length of string s. |
| string strSub(string s, number starti[, number endi]) | Returns substring of string s, starting at starti and ending at endi or end if endi is not specified. |
| number strLike(string s, string r) | Returns 1 if string s matches regular expression in string r. |
| number isSet(variable v) | Returns 1 if variable v exists. |
| number unset(variable v) | Attempts to unset (erase) variable v. Returns 0 if variable v did not exist, 1 otherwise. |
| number random() | Returns random value between 0 and 1. |
| number print(string s) | Prints string s to console. Returns 1. |
| number printLn(string s) | Prints string s to console, then prints new line. Returns 1. |
| number noteNameFromNumber(number n) | Returns name and octave from its number in the MIDI standard. |
| number sendToDefault(number message_type, number channel, number byte1, number byte2) | Attempts to send specified message to default output port. |

### Control structures
#### if/else if/else
This structure allows you to define conditions for something to happen. There can be as many `else if` blocks as you'd like, including none, and the `else` block is optional too.

For example `if(a == 5){printLn("five");}else if(a == 7){printLn("seven");}else if(a == 11){printLn("eleven");}else{printLn("some other number");}` will print name of the number if a is 5, 7 or 11, or "some other number" if none of those.

#### while
This structure allows you to repeat a block of code while some condition is true.

For example `a = 5; while(a > 0){ printLn("a is " # a); a = a - 1;}` will print 5 times line starting with "a is ", followed by current value of a.

### Variables
You can define your own variables simply by assigning value to them like so: `PI = 3.14159; project_name = "MIDIProcessor";`

Remember that the complete script file is executed once for every received message, so if you want to define initial value, use `if(!isSet(variable)){variable = 5;}`

Following is list of variables that get filled by processor before every execution of the script

| Name | Meaning |
|---|---|
| DELTA | Amount of time that passed since last message |
| MESSAGE_TYPE | Type of the MIDI message |
| CHANNEL | Channel the MIDI message got send on |
| BYTE_1 | First data byte of the MIDI message |
| BYTE_2 | Second data byte of the MIDI message (or -1 if there wasn't one) |

### Comments
When you write `//`, rest of the line gets ignored, so you can add comments to your scripts explaining code that might be harder to understand.

## FAQ
#### Can I use this program commercially for free? Can I share it with my friends? Can I pack it with my own project and sell it as a whole? Can I fork it and implement class structure?
Yes, yes, yes and yes. Though I don't know why you would do the last thing.

#### What editor/IDE did you use for development?
I used Gedit for development of the old project that I took most of the code from and Mousepad for refactoring it into MIDIProcessor. I liked Gedit better, but I didn't like GNOME all that much.
