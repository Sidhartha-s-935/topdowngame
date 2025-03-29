#include "../includes/pausestate.hpp"
#include "../includes/gamestateman.hpp"
#include "../includes/mainmenustate.hpp"

#include <iostream>

using namespace std;

PauseState::PauseState(GameStateManager *stateManager, sf::RenderWindow &window)
    : stateManager(stateManager), window(window)
{
    auto sm = stateManager;
    // Load font
    if (!font.loadFromFile("assets/font.otf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Setup pause text
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::White);

    // Center the text
    sf::FloatRect textRect = pauseText.getLocalBounds();
    pauseText.setOrigin(textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f);
    pauseText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 4.0f);

    // Semi-transparent background
    background.setSize(sf::Vector2f(window.getSize()));
    background.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black

    // Create buttons
    float buttonWidth = 300.0f;
    float buttonHeight = 60.0f;
    float buttonY = window.getSize().y / 2.0f;
    float spacing = 20.0f;

    // Resume button
    buttons.emplace_back(
        window.getSize().x / 2.0f - buttonWidth / 2.0f,
        buttonY,
        buttonWidth, buttonHeight,
        &font, "Resume",
        sf::Color(100, 100, 100), // Idle color
        sf::Color(150, 150, 150), // Hover color
        sf::Color(70, 70, 70),    // Active color
        sf::Color::White,         // Text color
        [this]()
        {
            // Instead of immediately popping, set the transition flag.
            this->pendingTransition = Transition::Resume;
        });

    // Main Menu button lambda (for example, at line 68 in pausestate.cpp):
    buttons.emplace_back(
        window.getSize().x / 2.0f - buttonWidth / 2.0f,
        buttonY + buttonHeight + spacing,
        buttonWidth, buttonHeight,
        &font, "Main Menu",
        sf::Color(100, 100, 100), // Idle color
        sf::Color(150, 150, 150), // Hover color
        sf::Color(70, 70, 70),    // Active color
        sf::Color::White,         // Text color
        [this]()
        {
            // Set the flag to transition to main menu.
            this->pendingTransition = Transition::MainMenu;
        });

    // Exit button
    buttons.emplace_back(
        window.getSize().x / 2.0f - buttonWidth / 2.0f,
        buttonY + (buttonHeight + spacing) * 2,
        buttonWidth, buttonHeight,
        &font, "Exit Game",
        sf::Color(100, 100, 100), // Idle color
        sf::Color(150, 150, 150), // Hover color
        sf::Color(70, 70, 70),    // Active color
        sf::Color::White,         // Text color
        [&window]()
        {
            window.close();
        });
}

void PauseState::handleInput(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Return to game when Escape is pressed again
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            stateManager->popState();
            return;
        }

        // Handle button events
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (auto &button : buttons)
        {
            button.handleEvent(event, mousePos);
        }
    }
}

void PauseState::update(float deltaTime)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition();

    // Update buttons
    for (auto &button : buttons)
    {
        button.update(sf::Vector2f(mousePos.x, mousePos.y));
    }
}

void PauseState::render(sf::RenderWindow &window)
{
    // Draw semi-transparent background over the game
    window.setView(window.getDefaultView());
    window.draw(background);

    // Draw pause text
    window.draw(pauseText);

    // Draw buttons
    for (auto &button : buttons)
    {
        button.render(window);
    }
}
