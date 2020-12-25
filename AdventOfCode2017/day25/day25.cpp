// day25.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// data

class Transition
{
public:
	Transition() = default;

	int newValue = -1;
	int direction = 0;
	char nextState = 0;
};

class State
{
public:
	State() = default;

	char stateId = 0;
	std::array<Transition, 2> transitions;
};

std::map<char, State> states;

char initialState = 0;
int steps = 0;

// state

using Tape = std::list<int>;
Tape tape;

Tape::iterator position;

char currentState;

int main()
{
	states.clear();

	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );

	State readState;
	Transition* readTransition = nullptr;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				// "Begin in state A."
				// "Perform a diagnostic checksum after 6 steps."
				// 
				// "In state A:"
				// "  If the current value is 0:"
				// "    - Write the value 1."
				// "    - Move one slot to the right."
				// "    - Continue with state B."
				switch ( *thisLine )
				{
					case 'B':
					{
						int count = sscanf( thisLine, "Begin in state %c.", &initialState );
						assert( count == 1 );
						break;
					}
					case 'P':
					{
						int count = sscanf( thisLine, "Perform a diagnostic checksum after %d steps.", &steps );
						assert( count == 1 );
						break;
					}
					case 'I':
					{
						if ( readState.stateId != 0 )
						{
							states[readState.stateId] = readState;
						}
						int count = sscanf( thisLine, "In state %c:", &readState.stateId );
						assert( count == 1 );
						break;
					}
					case ' ':
					{
						switch ( thisLine[6] )
						{
							case 'h':
							{
								int index;
								int count = sscanf( thisLine, "  If the current value is %d:", &index );
								assert( count == 1 );
								readTransition = &readState.transitions[index];
								break;
							}
							case 'W':
							{
								assert( readTransition );
								int count = sscanf( thisLine, "    - Write the value %d.", &readTransition->newValue );
								assert( count == 1 );
								break;
							}
							case 'M':
							{
								char direction[10];
								assert( readTransition );
								int count = sscanf( thisLine, "    - Move one slot to the %[^.].", direction );
								assert( count == 1 );
								if ( strcmp( direction, "right" ) == 0 )
								{
									readTransition->direction = 1;
								}
								else
								{
									assert( strcmp( direction, "left" ) == 0 );
									readTransition->direction = -1;
								}
								break;
							}
							case 'C':
							{
								assert( readTransition );
								int count = sscanf( thisLine, "    - Continue with state %c.", &readTransition->nextState );
								assert( count == 1 );
								break;
							}
							default:
							{
								assert( false );
								break;
							}
						}
						break;
					}
					default:
					{
						assert( false );
						break;
					}
				}
			}
		}
	}
	if ( readState.stateId != 0 )
	{
		states[readState.stateId] = readState;
	}
	fclose( fp );
	delete[] buffer;

	tape.clear();
	tape.push_back( 0 );
	position = tape.begin();
	currentState = initialState;

	for ( int step = 0; step < steps; ++step )
	{
		const State& state = states[currentState];
		const Transition& transition = state.transitions[*position];
		*position = transition.newValue;
		if ( transition.direction > 0 )
		{
			++position;
			if ( position == tape.end() )
			{
				--position;
				tape.push_back( 0 );
				++position;
			}
		}
		else
		{
			if ( position == tape.begin() )
			{
				tape.push_front(0);
			}
			--position;
		}
		currentState = transition.nextState;
	}

	__int64 result = 0;
	for ( int v : tape )
	{
		result += v;
	}
	printf( "result: %lld", result );
	return 0;
}
