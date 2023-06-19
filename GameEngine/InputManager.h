#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <map>
enum KEY_STATE {
    UP,
    DOWN,
    PRESSED
};
class InputManager {
public:
    enum Key {None=-1, Forward = 119, Backward = 115, Left = 97, Right = 100, Up = 32, Down = 120, Interact=101};
    
    static bool isKeyPressed(Key key);
    static bool isKeyUp(Key key);
    static bool isKeyDown(Key key);
    static void resetKeys();
    static void setKeyState(Key key, KEY_STATE state);
    static bool keyExists(Key key);
private:
    static std::map<Key, KEY_STATE> keys;
};
#endif
