#include "RunningManager.hpp"
#include "TextField.hpp"
#include "PageContainer.hpp"
#include "CellContainer.hpp"
#include <iostream>

static Timer timer;
static bool program_running = true;
static SDL_Event event;

static Button * quit_button = nullptr;
static TextField * tf1 = nullptr;
static TextField * tf2 = nullptr;
static TextField * tf3 = nullptr;

static Button * get_value_btn = nullptr;

static CellContainer * cc1 = nullptr;
static CellContainer * cc2 = nullptr;

void RunningManager::StartFrame()
{
    double_t elapsed_time = timer.getElapsedTime();
    if (elapsed_time < Assets::_60_FPS_FRAMETIME)
        SDL_Delay(int32_t((Assets::_60_FPS_FRAMETIME - elapsed_time) * 1000.0));
    
    timer.reset();
}

void RunningManager::FinishFrame()
{

}

void RunningManager::HandleUserInput()
{
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT)
            FinishProgramExecution();
        else if (event.type == SDL_KEYDOWN)
            InteractiveComponent::processKeyPressing(event.key.keysym.sym);
        else if (event.type == SDL_MOUSEBUTTONDOWN)
            InteractiveComponent::processMouseButtonDown({event.button.x, event.button.y});
        else if (event.type == SDL_MOUSEBUTTONUP)
            InteractiveComponent::processMouseButtonUp({event.button.x, event.button.y});
        // Not necessary
        else if (event.type == SDL_MOUSEMOTION)
            InteractiveComponent::processMouseMotion({event.motion.x, event.motion.y});
    }
}

void RunningManager::FinishProgramExecution()
{
    program_running = false;
}

bool RunningManager::ProgramIsRunning()
{
    return program_running;
}

void RunningManager::StartDependencies()
{
    Graphics::startUp();
    VisualComponent::startUp();
    InteractiveComponent::startUp();
}

void RunningManager::SetVerboseMode()
{
    AssetsManager::setVerboseMode();
    VisualComponent::setVerboseMode();
}

void RunningManager::ReleaseDependencies()
{
    VisualComponent::shutDown();
    AssetsManager::shutDown();
    Graphics::shutDown();
}

void RunningManager::RenderScreen()
{
    VisualComponent::drawComponents();
}

void changecc()
{
    if (cc2->getCellAlignment() != CellContainer::CellAlignment::TOP_MIDDLE)
        cc2->setCellAlignment(CellContainer::CellAlignment::TOP_MIDDLE);
    else
        cc2->setCellAlignment(CellContainer::CellAlignment::BOTTOM_MIDDLE);
}

void RunningManager::InitializeUIElments()
{
    quit_button = Button::newButton("Sair");
    quit_button->setClickReaction(RunningManager::FinishProgramExecution);
    quit_button->setGlobalX(Assets::WINDOW_WIDTH / 2 - quit_button->getWidth() / 2);
    quit_button->setGlobalY(Assets::WINDOW_HEIGHT - quit_button->getHeight() - 10);

    tf1 = TextField::newTextField();
    tf1->setRelativeX(10);
    tf1->setRelativeY(10);

    tf2 = TextField::newTextField();
    tf2->setRelativeX(0);
    tf2->setRelativeY(tf1->getHeight() + 10);
    tf2->setParent(tf1);
    tf2->setAlphaOnlyMode();

    tf3 = TextField::newTextField();
    tf3->setRelativeX(0);
    tf3->setRelativeY(tf2->getHeight() + 10);
    tf3->setParent(tf2);
    tf3->setNumericOnlyMode();

    get_value_btn = Button::newButton("Mudar container");
    get_value_btn->setParent(quit_button);
    get_value_btn->setRelativeX(quit_button->getWidth() + 10);
    get_value_btn->setRelativeY(0);
    get_value_btn->setClickReaction(changecc);

    cc1 = CellContainer::newCellContainer(100, 100, CellContainer::CellAlignment::BOTTOM_MIDDLE, quit_button);
    cc1->setParent(tf1);
    cc1->setRelativeX(tf1->getWidth() + 10);
    cc1->setRelativeY(0);

    cc2 = CellContainer::newCellContainer(100, 100, CellContainer::CellAlignment::TOP_MIDDLE, get_value_btn);
    cc2->setParent(cc1);
    cc2->setRelativeX(0);
    cc2->setRelativeY(cc1->getHeight());
}