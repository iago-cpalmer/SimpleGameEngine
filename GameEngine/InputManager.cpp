#include "InputManager.h"
#include <iostream>
std::map<InputManager::Key, KEY_STATE> InputManager::keys;

bool InputManager::isKeyPressed(Key key) {
    auto it = keys.find(key);
    if (it != keys.end()) {
        if (it->second == PRESSED) {
            return true;
        }
    }
    return false;
}

bool InputManager::isKeyDown(Key key) {
    auto it = keys.find(key);
    if (it != keys.end()) {
        if (it->second == DOWN) {
            return true;
        }
    }
    return false;
}
void InputManager::resetKeys() {
    for (const auto& pair : keys) {
        if (pair.second == UP) {
            keys[pair.first] = DOWN;
        }
    }
}

bool InputManager::isKeyUp(Key key) {
    auto it = keys.find(key);
    if (it != keys.end()) {
        if (it->second == UP) {
            return true;
        }
    }
    return false;
}



void InputManager::setKeyState(Key key, KEY_STATE state) {
    keys[key] = state;
}
bool InputManager::keyExists(Key key) {
    if (key == InputManager::Up || key == InputManager::Left
        || key == InputManager::Right || key == InputManager::Forward
        || key == InputManager::Backward || key == InputManager::Down
        || key == InputManager::Interact) {
        
        return true;
    }
    return false;
}