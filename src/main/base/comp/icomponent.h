#ifndef COMPONENT_H
#define COMPONENT_H


class IComponent
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

	explicit IComponent(Type type);
	~IComponent() = default;

	Type getType();

public:
	virtual void init() = 0;
	virtual void update() = 0;

private:
	Type mType;
};

#endif // COMPONENT_H