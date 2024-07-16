#include "ScriptExec.h"

ScriptExec::ScriptExec()
{
	csv = new CsvReader("Assets/script.csv");
	if (csv->GetLines() < 1) {
		MessageBox(NULL, "script.csv‚ª“Ç‚ß‚Ü‚¹‚ñ", "ƒGƒ‰[", MB_OK);
	}
	readLine = 0;

	charas[0] = Instantiate<Chara>();
	charas[1] = Instantiate<Chara>();
	message = Instantiate<Message>();

	waitTimer = 0;
	waitMessage = false;
}

ScriptExec::~ScriptExec()
{
}

void ScriptExec::Update()
{
	//waitTimer>0‚È‚ç‚ÎˆÈ‰º‚ð‚â‚ç‚È‚¢
	if (waitTimer > 0) {
		waitTimer -= SceneManager::DeltaTime();
		return;
	}
	if (waitMessage) {
		if (message->IsFinish()) {
			waitMessage = false;
		}
		return;
	}
	std::string command = csv->GetString(readLine, 0);
	if (command.substr(0, 2) == "//") {
	} else if (command == "CHARA") {
		int id = csv->GetInt(readLine, 1);
		std::string filename = csv->GetString(readLine, 2);
		int x = csv->GetInt(readLine, 3);
		int y = csv->GetInt(readLine, 4);
		charas[id]->Set(filename, x, y);
	}
	else if (command == "CHARAMOVE") {
		int id = csv->GetInt(readLine, 1);
		int x = csv->GetInt(readLine, 3);
		int y = csv->GetInt(readLine, 4);
		float time = csv->GetFloat(readLine, 5);
		charas[id]->Move(x, y, time);
	}
	else if (command == "MESSAGE") {
		message->SetText(csv->GetString(readLine, 2));
		waitMessage = true;
	}
	else if (command == "WAIT") {
		waitTimer = csv->GetFloat(readLine, 5);
	}
	else if (command == "END") {
		return;
	}
	readLine++;
}

void ScriptExec::Draw()
{
}
