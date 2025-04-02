#ifndef TEXTBOX 
#define TEXTBOX

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>
#include <vector>
#include <iostream>

class TextBox {
private:
	std::vector<std::string> dialogues;
	std::vector<std::string> wrappedLines;
	int dindex;
	sf::Font font;
	sf::RectangleShape box;
	sf::Text dialogue;
	sf::Clock typingClock;
	float typingSpeed = 0.05f;
	bool isTyping = false;
	int charIndex = 0;
	std::string fullText;
	std::string currentText;

	sf::Clock dialogueClock;
	float dialogueDelay = 0.2f; 
	int totalPages , maxLines , currentPage;
public:
	TextBox();

	void loadDialogues(std::vector<std::string>& dialogues);
	void nextDialogue();

	void handleInput();

	void wrapText(std::string &text , float maxWidth);

	void draw(sf::RenderWindow& window);

};


#endif
