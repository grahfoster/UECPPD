/*
	A simple yet flavorful code-breaking game in which the user is tasked with
	guessing three integer values given their sum and product. With each of the
	twelve levels, the maximum potential size of the numbers increases, making
	for a greater arithmetic challenge. If the player solves all twelve puzzles,
	they win the game; if they fail, they are informed of their loss and shown a
	summary of their progress.
*/

#include <vector>
#include <iostream>
#include <ctime>

// Names for each riddle-protected gate
const std::vector<std::string> Gates{
	"the Nile", "Amon-Ra", "Mut", "Geb", "Osiris", "Isis",
	"Thoth", "Horus", "Anubis", "Seth", "Bastet", "Sekhmet"
};

// Ordinal numbers to describe gate position
const std::vector<std::string> Ordinals{
	"first", "second", "third", "fourth", "fifth", "sixth",
	"seventh", "eighth", "ninth", "tenth", "eleventh", "last"
};

// Introduce user to game
void PrintIntroduction()
{
	std::cout <<
		"            /\\ \n"
		"           /__\\ \n"
		"          /_| _\\ \n"
		"         /_|_ _|\\ \n"
		"        /__ _|_ _\\    `_,\n"
		"       /_|__  |_ |\\ - (_)-\n"
		"      /_|__|_ _ | _\\  ' `\n"
		"     / __| _|_ | _  \\ \n"
		"_ - / _ | __ | __ | _\\ _ - _ - _ - _ - _ TRIPLE X _ - _ - _ - _ -\n\n"
		"After months of arduous excavation, you have finally uncovered the\n"
		"entrance to the Lost Pyramid of Khaset. As you make your way through\n"
		"the winding crypt, a bright-eyed sphinx suddenly appears, blocking\n"
		"your path.\n\n";
}

// Pose riddle to user and indicate if answer is correct
bool PoseRiddle(const int CurrentGate)
{
	const int Difficulty = CurrentGate + 1;	// Offset index by 1
	const int CodeA = rand() % Difficulty + Difficulty;
	const int CodeB = rand() % Difficulty + Difficulty;
	const int CodeC = rand() % Difficulty + Difficulty;
	const int CodeSum = CodeA + CodeB + CodeC;
	const int CodeProduct = CodeA * CodeB * CodeC;
	std::cout <<	// Print riddle and prompt user for input
		"\"You have reached the Gate of " << Gates[CurrentGate] << ", " <<
		Ordinals[CurrentGate] << " barrier of twelve,\"\n"
		"says the sphinx. \"If you would venture deeper still, then rack\n"
		"your brains to solve this riddle:\n"
		"Added we are " << CodeSum << ".\n"
		"Multiplied we make " << CodeProduct << ".\n"
		"Speak to me these numbers three, and I will show the path you\n"
		"seek.\"\n";

	int GuessA = 0;
	int GuessB = 0;
	int GuessC = 0;
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin >> GuessA >> GuessB >> GuessC;	// Get user input
	if (!std::cin)	// Handle non-integer input
		throw std::exception("The sphinx takes only integers!\n");

	if (GuessA + GuessB + GuessC == CodeSum &&	// Check if answer is correct
		GuessA * GuessB * GuessC == CodeProduct)
		return true;
	return false;
}

// Inform user of success or failure
void PrintOutcome(const bool bIsCorrect, const int CurrentGate)
{
	if (bIsCorrect) {
		std::cout <<	// Print success message
			"\nThe sphinx lowers its head with a smile. There is a puff of\n"
			"smoke, sudden darkness, then silence. When the pitch-black cloud\n"
			"clears away, you find the path ahead open. Taking your torch in\n"
			"hand, you forge ahead into the depths of the crypt.\n";

		if (CurrentGate < Gates.size())	// Print lead-in to next riddle
			std::cout << "Before long, another sphinx bars the way.\n\n";
		return;
	}
	std::cout <<	// Print failure message
		"\nA frown appears on the sphinx's face. Pity fills its eyes.\n"
		"\"Alas,\" it speaks, \"These numbers three are not the ones I need.\n"
		"I am afraid your journey is at an end. You made it to the Gate of\n" <<
		Gates[CurrentGate] << ", " << Ordinals[CurrentGate] << 
		" of two and ten.\"\n\n"
		"Sorry! You didn't beat the game.\n";
}

// Inform user they have beaten game
void PrintVictory()
{
	std::cout <<	// Print victory message
		"Seconds pass, then minutes. For the better part of an hour you grope\n"
		"along the dry stone walls, delving further and further into the\n"
		"ancient ruin. Finally, turning into a wide passage you see a light\n"
		"at the end of the corridor. Torches burn in bronze braziers, lining\n"
		"the walls of a small, square room. On a raised platform at its\n"
		"center lies the tomb of Anakhaten, first nomarch of Khaset.\n\n"
		"Congratulations! You beat the game.\n";
}

int main()
{
	PrintIntroduction();

	srand(time(NULL));	// Ensure randomness in riddle generation
	int CurrentGate = 0;	// Start at first gate
	while (CurrentGate < Gates.size()) {
		try {
			if (!PoseRiddle(CurrentGate)) {
				PrintOutcome(false, CurrentGate);
				return 0;	// Wrong answer ends the game
			}
			PrintOutcome(true, CurrentGate);
			++CurrentGate;	// Proceed to next gate
		}
		catch (std::exception & e) {
			std::cerr << e.what() << '\n';
			std::cin.clear();
		}
		catch (...) {
			std::cerr << "Unexpected error.\n";
			return 1;
		}
	}

	PrintVictory();
}