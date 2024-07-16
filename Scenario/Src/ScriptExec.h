#pragma once
// ScriptExec.h
#include "GameObject.h"
#include "CsvReader.h"
#include "Chara.h"
#include "Message.h"

class ScriptExec : public GameObject {
public:
	ScriptExec();
	~ScriptExec();
	void Update() override;
	void Draw() override;
private:
	CsvReader* csv;
	int readLine;

	Chara* charas[2];
	Message* message;

	float waitTimer;
	bool waitMessage;
};
