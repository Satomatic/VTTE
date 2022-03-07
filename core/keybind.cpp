#include <core/keybind.h>
#include <core/core.h>
#include <iostream>

std::vector <KeyBind> KeyBinds;

void CreateKeyBind(std::string key, int modifiers, callback function){
	KeyBind keybind;
	keybind.key = key;
	keybind.modifiers = modifiers;
	keybind.function = function;

	KeyBinds.push_back(keybind);
}

void InitKeybinds(){
	CreateKeyBind("N", CTRL, ActionNewFile);
	CreateKeyBind("O", CTRL, ActionOpenFile);
	CreateKeyBind("S", CTRL, ActionSaveFile);
	CreateKeyBind("X", CTRL, ActionCloseFile);
	CreateKeyBind(",", CTRL, ActionSwitchPrev);
	CreateKeyBind(".", CTRL, ActionSwitchNext);
}
