#include "GameManager.h"
#include "ScoreSystem.h"
#include <chrono>

std::string GameManager::menuScene = "menu.json";
std::string GameManager::gameScene = "scene.json";
std::string GameManager::prefabs = "prefab.json";

GameManager::GameManager()
{
    curScene = menuScene;
    sceneManager = new SceneManager();
    sceneManager->LoadPrefabs(prefabs);
    isGameOver = false;
}

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

void GameManager::identifyPlayerAndPlayerSpawner()
{
    auto& coordinator = EntityCoordinator::getInstance();
    for (auto const& e : sceneManager->entities)
    {
        if (coordinator.entityHasTag(Tag::PLAYERSPAWNER, e))
        {
            playerRespawner = e;
        }
    }
}

EntityID GameManager::PlayerRespawnerID()
{
    return playerRespawner;
}

void GameManager::countGameFrame()
{
    gameFrameCount++;
}

int GameManager::getCurrGameFrame()
{
    return gameFrameCount;
}

bool GameManager::GameIsPaused()
{
    return gamePaused;
}

void GameManager::PauseGame()
{
    gamePaused = true;
}

void GameManager::UnpauseGame()
{
    gamePaused = false;
}

void GameManager::handleGameOver() {
    isGameOver = true;
    json scoreJsonArray = json::parse(FileManager::readTextFile("scores.json"));

    json currentScore;
    currentScore["score"] = ScoreSystem::score;
    currentScore["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    scoreJsonArray.push_back(currentScore);

    std::sort(scoreJsonArray.begin(), scoreJsonArray.end(),
        [](const json& a, const json& b) -> bool {
            return a["score"] > b["score"];
        }
    );
    FileManager::writeTextFile("scores.json", scoreJsonArray.dump());

    PauseGame();
    vector<string> dates = {
        "----/--/-- --:--",
        "----/--/-- --:--",
        "----/--/-- --:--",
        "----/--/-- --:--",
        "----/--/-- --:--",
    };

    vector<string> scores = {
        "0","0","0","0","0"
    };

    for (int i = 0; i < 5; i++) {
        if (scoreJsonArray.size() > i) {
            scores[i] = std::to_string(scoreJsonArray[i]["score"].get<int>());
            std::time_t t = scoreJsonArray[i]["timestamp"].get<int>();
            std:tm tm;
            localtime_s(&tm, &t);
            stringstream s; 
            s << std::put_time(&tm, "%Y/%m/%d %H:%M");
            dates[i] = s.str();
        }
        
    }


    createGameOverOverlay(ScoreSystem::score, dates, scores);
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

void GameManager::replay() {
    isGameOver = false;
    hasActiveStar = false;
    ScoreSystem::score = 0;
    loadScene(gameScene);
    Notify(Event::PLAYER_REPLAY, {});
}


void GameManager::Receive(Event e, void* args) {
    if (e == Event::INPUT_SHOOT) {
        if (isGameOver) {
            replay();
        }
        else if (curScene == menuScene) {
            loadScene(gameScene);
        }
    }
    else if (e == Event::PLAYER_DIES) handleGameOver();
}

std::string GameManager::getCurScene() {
    return curScene;
}

void GameManager::loadScene(std::string scene) {
    if (scene != GameManager::menuScene && scene != GameManager::gameScene) return;
    curScene = scene;
    EntityCoordinator::getInstance().deactivateAllEntitiesAndPhysicsBodies();
    sceneManager->EmptyEntitiesList();
    sceneManager->LoadScene(curScene);
    sceneManager->CreateEntities();
    identifyPlayerAndPlayerSpawner();
    UnpauseGame();
}
