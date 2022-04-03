#ifndef COMPONENT_H
#define COMPONENT_H


class Component
{
public:

	enum class Type {
		GameObject,
		Transformation,
		Renderer,
		Geometry,
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
	virtual void init();
	virtual void update();

private:
	Type mType;
};

#endif // COMPONENT_H