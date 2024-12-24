#pragma once
#include <string>


class GameObject {
	static int m_next_id;
protected:
	class GameState* m_state;
	std::string m_name;
	bool m_active = true;
	int m_id = 0;
public:
	GameObject(const std::string& name = "");
	virtual void update(float dt) {}
	virtual void update(float dt, bool online) {}
	virtual void init() {}
	virtual void init(bool online) {}
	virtual void draw() {}
	virtual void draw(bool online) {}

	virtual  ~GameObject() {}
	bool isActive() { if (this != nullptr) { return m_active; } }
	void setActive( bool a) { m_active = a; }






};