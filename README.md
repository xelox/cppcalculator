# Calculator in c++
If you have g++ installed, you can complie with build.sh script

## Usage
Example: 
./cppcompute "1 + 1";
Will output
1 + 1 = 2

Supported operations are +, -, *, /, ^ and x is alias for *
You can use () brakets to define expressions:
2 x (1 + 1) = 4
You can nest as many brakets as you want, but care to close them!

white space is ignored and you other "useless" characters are also ignored... 6_000 is interpreted as 6000, and so is 6,000 6a000 etc
A - sign after another operation assumes that you intend to negate the following expression:
5 x -2 = -10

PS. may crash on edgecases and unexpected input :D
