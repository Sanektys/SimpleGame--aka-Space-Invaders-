#pragma once

#include <ctime>

#include "renderSystem.h"
#include "gameObject.h"


//////////////////////////
const int GAME_OBJECTS_COUNT_MAX = 512;

//////////////////////////
class Game
{
    public:
        Game();

	    void setupSystem();
	    void initialize();
	    bool frame();
	    bool shutdown();

    private:
        void render();
        void update(float dt);
		void canIncreaseLevel();
		void deleteAllObjects();

        GameObject* createObject(GameObjectType type, float x, float y,
			                     char symbol, ConsoleColor color,
			                     ConsoleColor bgColor);
        void destroyObject(GameObject* object);
		void hitObject(GameObject* object);

    private:
	    bool m_isGameActive;
		bool m_isLosing;
		bool m_isExitGame;
		int m_currentLevel;
	    
		clock_t m_clockLastFrame;
	    float m_frameTime;
	    int m_frameCount;
	    int m_fps;
		float m_gameTime;

	    RenderSystem m_renderSystem;

	    GameObject* m_objects[GAME_OBJECTS_COUNT_MAX];

	    float m_shipFireCooldownTime;
	    float m_alienAmplitudeTime;

		int m_gamePoints;
		int m_lastLevelPoints;
};