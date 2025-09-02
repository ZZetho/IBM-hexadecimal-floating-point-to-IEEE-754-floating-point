Converts between IBM hexidecimal floating point numbers and IEEE 754 floating point numbers

Instructions:
	1. Compile the program with: gcc FloatingPoint.c -o FloatingPoint
	2. Run the program with: FloatingPoint
	3. the program will ask for an input filename, its precision, and output filename and its precision, precision can either be single (S) or double (D)
	
Test cases (IBM singles => IEEE singles):
11000010011101101010000000000000 (-118.625)
01000010011011001010110100010101 (108.6761)
01100000111111111111111111111111 (largest positive IEEE single)
01100001111111111111111111111111 (largest positive IEEE single plus 1, (infinity))
11100000111111111111111111111111 (largest negative IEEE single)
00100001010000000000000000000000 (smallest normal IEEE single)
00100001001111111111111111111111 (largest denormalised IEEE single (technically not quite but its as close as i can get))
00011011100000000000000000000000 (smallest denormalised IEEE single)

License:
	CC BY-NC-SA 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
