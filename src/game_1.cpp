// This is the game entities for Srivathsan Govindarajan's game
#include "game.hpp"

std::string selfID;
ReplayEvent* replay_event;
PlaybackEvent* playback_event;
GameObjects* loadEntities(std::string clientID, Color color)
{
    global_time = new Timeline();
    physics = new Physics(10);
    selfID = clientID;
    GameObjects* game_objects = new GameObjects();
    game_objects->addEntity(clientID, new Rect(SCREEN_WIDTH * 0.05, SCREEN_HEIGHT * 0.7 - 50, 20, 30, true, color, true));
    input_event = new InputEvent(moveEntity, game_objects, global_time->getTime(), EventPriority::HIGH);
    input_event->setType("Input");
    event_manager.registerEvent(input_event);
    replay_event = new ReplayEvent(std::string("game_1"), global_time->getTime(), EventPriority::LOW);
    replay_event->setType("replay");
    event_manager.registerEvent(replay_event);
    playback_event = new PlaybackEvent(std::string("game_1"), global_time->getTime(), EventPriority::HIGH);
    playback_event->setType("playback");
    event_manager.registerEvent(playback_event);
    return game_objects;
}

void moveEntity(GameObjects* game_objects, Uint8 *keys)
{
    if (keys[SDL_SCANCODE_T])
    {
        isProportional = !isProportional;
        if(isProportional)
        {
            std::cout<<"Object sizes are set to proportional"<<std::endl;
        }
        else
        {
            std::cout<<"Object sizes are set to constant"<<std::endl;
        }
    }
    if (keys[SDL_SCANCODE_R])
    {
        std::cout<<"RECORDING"<<std::endl;
        event_manager.raiseEvent(replay_event);
        // recording = !recording;
    }
    if (keys[SDL_SCANCODE_Y])
    {
        std::cout<<"PLAYBACK"<<std::endl;
        event_manager.raiseEvent(playback_event);
        // playback = !playback;
    }
    if(keys[SDL_SCANCODE_KP_PLUS])
    {
        float tic = global_time->getTic();
        global_time->changeTic(tic + 0.1);
        std::cout<<"Tic set to "<<global_time->getTic()<<std::endl;
    }
    else if(keys[SDL_SCANCODE_KP_MINUS])
    {
        float tic = global_time->getTic();
        global_time->changeTic(tic - 0.1);
        std::cout<<"Tic set to "<<global_time->getTic()<<std::endl;
    }
    if(keys[SDL_SCANCODE_P])
    {
        if(global_time->isPaused())
        {
            global_time->unpause();
            std::cout<<"Game Unpaused"<<std::endl;
        }
        else{
            global_time->pause();
            std::cout<<"Game Paused"<<std::endl;
        }
    }
    float vel_x = 0.0, vel_y = 0.0;
    if(keys[SDL_SCANCODE_RIGHT])
    {
        if(keys[SDL_SCANCODE_LSHIFT])
        {
            vel_x = global_time->getTic()? 20.0 / global_time->getTic() : 0;
        }
        else
        {
            vel_x = global_time->getTic()? 10.0 / global_time->getTic() : 0;
        }
    }
    else if(keys[SDL_SCANCODE_LEFT])
    {
        if(keys[SDL_SCANCODE_LSHIFT])
        {
            vel_x = global_time->getTic()? -20.0 / global_time->getTic() : 0;
        }
        else
        {
            vel_x = global_time->getTic()? -10.0 / global_time->getTic() : 0;
        }
    }
    if (keys[SDL_SCANCODE_SPACE])
    {
        vel_y = global_time->getTic()? -10.0 / global_time->getTic() : 0;
    }
    game_objects->setEntityVelocityX(selfID, vel_x);
    if(vel_y)
    {
        game_objects->setEntityVelocityY(selfID, vel_y);
    }
}


void handleDzone(GameObjects* game_objs)
{
    Coord spawn_point = {SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.2};
    SDL_Rect dzone1 = { static_cast<int>(SCREEN_WIDTH * 0.25), static_cast<int>(SCREEN_HEIGHT * 0.7), static_cast<int>(SCREEN_WIDTH * 0.2), static_cast<int>(SCREEN_HEIGHT * 0.3)};
    SDL_Rect dzone2 = { static_cast<int>(SCREEN_WIDTH * 0.55), static_cast<int>(SCREEN_HEIGHT * 0.7), static_cast<int>(SCREEN_WIDTH * 0.4), static_cast<int>(SCREEN_HEIGHT * 0.3)};
    SDL_Rect dzone3 = { static_cast<int>(SCREEN_WIDTH * 0.95), static_cast<int>(SCREEN_HEIGHT * 0.7), static_cast<int>(SCREEN_WIDTH * 0.6), static_cast<int>(SCREEN_HEIGHT * 0.3)};
    Rect* playerE = dynamic_cast<Rect*>(game_objs->getEntity(selfID));

    DzoneHandler& dzHandler = DzoneHandler::getInstance();
    dzHandler.addDeathZone(dzone1, spawn_point);
    dzHandler.addDeathZone(dzone2, spawn_point);
    dzHandler.addDeathZone(dzone3, spawn_point);
    dzHandler.startCollisionChecking(playerE);
}