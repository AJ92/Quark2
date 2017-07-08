#ifndef COMPONENT_H
#define COMPONENT_H


class Component
{
public:

	enum Type {
		GameObject,
		Transfomation,
		Renderer,
		Material,
		Shader,
		Texture,
		Script,

		None
	};

	Component(Type type);
	~Component();

	Type getType();

public:
	virtual void start();
	virtual void update();
	virtual void end();

private:
	Type _type;
};

#endif // COMPONENT_H