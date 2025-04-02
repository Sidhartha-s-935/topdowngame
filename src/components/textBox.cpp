#include <SFML/Graphics/RenderWindow.hpp>
#include "../../includes/componentsH/textBox.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream>

TextBox::TextBox() {
	this->dindex = 0 , this->totalPages = 0 , this->currentPage = 0 , this->maxLines = 0;
	if(!this->font.loadFromFile("assets/font.otf")) {
		std::cerr << "cant load font file" << '\n';
	}
	else {
		std::cout << "font loaded" << '\n';
	}
	this->dialogue.setFont(this->font);
	this->dialogue.setCharacterSize(24);
	this->dialogue.setFillColor(sf::Color::White);
	this->dialogue.setPosition(50.f , 400.f);

	this->box.setSize(sf::Vector2f(700 , 100));
	this->box.setFillColor(sf::Color(0 , 0 , 0 , 150));
	this->box.setPosition(40.f, 390.f);
}


void TextBox::loadDialogues(std::vector<std::string>& newDialogues) {
	this->dialogues = newDialogues;
	this->dindex  = 0;
	if(!this->dialogues.empty()) {
		this->fullText = this->dialogues[this->dindex];
		this->currentText = "";
		this->charIndex = 0;
		this->isTyping = true;
	}
	return;	
}



void TextBox::nextDialogue() {
    if (this->dindex + 1 < this->dialogues.size()) {
        this->dindex++;
    } else {
        this->dindex = 0;
    }

    this->fullText = this->dialogues[this->dindex];
    this->currentText = "";
    this->charIndex = 0;
    this->isTyping = true;

    this->wrapText(this->fullText, this->box.getSize().x - 20.f);
    this->typingClock.restart();
}






void TextBox::draw(sf::RenderWindow& window) {
    sf::Vector2f viewPos = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();

    this->box.setPosition(viewPos.x - viewSize.x / 2 + 50, viewPos.y + viewSize.y / 2 - 120);
    this->dialogue.setPosition(viewPos.x - viewSize.x / 2 + 60, viewPos.y + viewSize.y / 2 - 110);

    window.draw(this->box);

    float lineHeight = dialogue.getCharacterSize() + 5;
    float yOffset = viewPos.y + viewSize.y / 2 - 110;

    int startLine = currentPage * maxLines;
    int endLine = std::min(startLine + maxLines, (int)wrappedLines.size());

    std::string displayedText = "";

    int typedCharacters = charIndex; 
    for (int i = startLine; i < endLine; ++i) {
        if (typedCharacters >= wrappedLines[i].size()) {
            displayedText += wrappedLines[i] + "\n";
            typedCharacters -= wrappedLines[i].size();
        } else {
            displayedText += wrappedLines[i].substr(0, typedCharacters);
            break;
        }
    }

    dialogue.setString(displayedText);
    window.draw(this->dialogue);

    if (currentPage < totalPages - 1) {
        sf::Text hint("Press E to continue...", *(this->dialogue.getFont()), 18);
        hint.setFillColor(sf::Color::White);
        hint.setPosition(viewPos.x - viewSize.x / 2 + 60, yOffset + maxLines * lineHeight);
        window.draw(hint);
    }
}




void TextBox::handleInput() { 
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && dialogueClock.getElapsedTime().asSeconds() > dialogueDelay) {
        if (isTyping) {
            charIndex = fullText.size(); 
            isTyping = false;
        } 
        else if (currentPage < totalPages - 1) {
            currentPage++; 
            charIndex = 0; 
            isTyping = true;
        } 
        else {
            nextDialogue();
        }
        dialogueClock.restart();
    }
}


void TextBox::wrapText(std::string& text, float maxWidth) {
    this->wrappedLines.clear();
    std::istringstream words(text);
    std::string word, line;

    while (words >> word) {
        std::string testLine = line.empty() ? word : line + " " + word;
        sf::Text testText(testLine, *(this->dialogue.getFont()), this->dialogue.getCharacterSize());

        if (testText.getLocalBounds().width > maxWidth) {
            this->wrappedLines.push_back(line);
            line = word;
        } else {
            line = testLine;
        }
    }

    if (!line.empty()) {
        wrappedLines.push_back(line);
    }

    this->maxLines = box.getSize().y / (dialogue.getCharacterSize() + 5);
    this->totalPages = (wrappedLines.size() + maxLines - 1) / maxLines;
    this->currentPage = 0; 

    this->charIndex = 0; 
    this->isTyping = true;
}



