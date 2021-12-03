#include "GameManager.h"

GameManager::GameManager()
{

}

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}


EntityID GameManager::PlayerRespawnerID()
{
    return playerRespawner;
}

void GameManager::SetPlayerRespawnerID(EntityID id)
{
    playerRespawner = id;
}

void GameManager::countGameFrame()
{
    gameFrameCount++;
}

int GameManager::getCurrGameFrame()
{
    return gameFrameCount;
}

void GameManager::handleGameOver() {
    vector<string> dates = {
        "2020/11/30 11:30",
        "2020/11/30 11:30",
        "2020/11/30 11:30",
        "2020/11/30 11:30",
        "2020/11/30 11:30",
    };

    vector<string> scores = {
        "15",
        "5",
        "15",
        "15",
        "5",
    };
    createGameOverOverlay(10, dates, scores);
}

/// <summary>
/// Create the game over overlay on top of the current scene.
/// Note that we are only displaying the top 5 highest scores.
/// </summary>
/// <param name="dates">The dates the scores were gotten.</param>
/// <param name="scores">The scores that were gotten.</param>
void GameManager::createGameOverOverlay(int playerScore, vector<string> dates, vector<string> scores) {
    auto& creator = GameEntityCreator::getInstance();
    float viewHeight = Renderer::getInstance()->getCamera()->getViewHeight();
    float viewWidth = Renderer::getInstance()->getCamera()->getViewWidth();
    float yPos = viewHeight / 2 - 2; // use this so we can change the starting yPos easily

    // create the panel
    creator.CreateUI(0, 0, viewHeight * 0.8, viewWidth * 0.6, 0, 0, 0, {Tag::GAME_OVER_UI});

    // create the title and user score
    creator.CreateText("GAME OVER", 0, yPos, 1, 0, 0, 1.5, { Tag::GAME_OVER_UI });
    yPos -= 0.75;
    creator.CreateText("SCORE: " + to_string(playerScore), 0, yPos, 1, 1, 1, 1, { Tag::GAME_OVER_UI });
    yPos -= 1;

    // create the highscore section
    creator.CreateText("HIGHSCORES", 0, yPos, 1, 1, 1, 1, { Tag::GAME_OVER_UI });
    yPos -= 1;
    int highscoresCount = 5;
    for (int i = 0; i < highscoresCount; i++) {
        string date;
        try {
            date = dates.at(i);
        }
        catch (out_of_range) {
            date = "-----";
        }

        string score;
        try {
            score = scores.at(i);
        }
        catch (out_of_range) {
            score = "0";
        }

        creator.CreateText(date, -3, yPos, 1, 1, 1, 1, TextAlign::LEFT, { Tag::GAME_OVER_UI });
        creator.CreateText(score, 3, yPos, 1, 1, 1, 1, TextAlign::RIGHT, { Tag::GAME_OVER_UI });
        yPos -= 0.75;
    }

    // replay instruction
    yPos -= 0.25;
    creator.CreateText("J to replay. Q to quit", 0, yPos, 1, 0, 0, 1, {Tag::GAME_OVER_UI});
}

void GameManager::removeGameOverOverlay() {
}


void GameManager::Receive(Event e, void* args) {
    if (e == Event::PLAYER_DIES) handleGameOver();
}
