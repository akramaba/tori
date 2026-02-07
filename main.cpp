#include "inc/Window.hpp"
#include "inc/Input.hpp"
#include "inc/Camera.hpp"
#include "inc/Engine.hpp"
#include "inc/Audio.hpp"

#include "tests/BaseScene.hpp"
#include "tests/SceneA.hpp"
#include "tests/SceneB.hpp"

using namespace tori;

// Test scenes
BaseScene base_scene;
SceneA scene_a;
SceneB scene_b;

// SceneA's copy function to hand-off models to SceneB
// Doing it here to avoid circular dependencies
void SceneA::copy_models_to_b() {
    scene_b.models = this->models;
    scene_b.transforms = this->transforms;
}

int main() {
    if (!Window::init(1280, 720, "Tori")) {
        return 1;
    }

    Input::init();
    camera.init();
    if (!Engine::init()) {
        return 1;
    }
    Audio::init();

    scene_b.parent = &base_scene;

    // Start in Scene A
    Engine::set_scene(&scene_a);

    while (!Window::should_close()) {
        Window::poll();
        Engine::render();
        Window::swap();
        Input::update();
    }

    Audio::quit();
    Engine::quit();
    Window::close();

    return 0;
}